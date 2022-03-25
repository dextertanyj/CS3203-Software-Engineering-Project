#include "QueryPreprocessor.h"

#include <optional>
#include <regex>
#include <utility>

#include "Common/ExpressionProcessor/ExpressionParser.h"
#include "Common/Validator.h"
#include "QP/QueryExpressionLexer.h"
#include "QP/Types.h"

regex QP::QueryPreprocessor::invalid_chars_regex = regex(R"([^a-zA-Z0-9\s,"_\(\);\+\-\*\/%<>#\.=])");
regex QP::QueryPreprocessor::query_token_regex =
	regex(R"(Follows\*|Calls\*|Parent\*|Next\*|stmt#|[a-zA-Z][a-zA-Z0-9]*|[0-9]+|\(|\)|;|\+|-|\*|\/|%|_|,|\"|<|>|\*|#|\.|=)");

QP::QueryProperties QP::QueryPreprocessor::parseQuery(string query) {
	reset();
	tokenizeQuery(std::move(query));
	try {
		return parseQuery();
	} catch (const std::out_of_range& e) {
		throw QueryException("Unexpected end of query.");
	}
}

void QP::QueryPreprocessor::tokenizeQuery(string query) {
	if (regex_search(query, invalid_chars_regex)) {
		throw QueryTokenizationException("Illegal token encountered.");
	}

	sregex_iterator iter = sregex_iterator(query.begin(), query.end(), query_token_regex);

	while (iter != sregex_iterator()) {
		this->query_tokens.push_back(iter->str());
		++iter;
	}
}

QP::QueryProperties QP::QueryPreprocessor::parseQuery() {
	parseDeclarations();
	parseSelect();
	while (token_index < this->query_tokens.size()) {
		parseClauses();
	}
	Types::DeclarationList declarations;
	declarations.reserve(existing_declarations.size());
	for (const auto& declaration_pair : existing_declarations) {
		declarations.push_back(declaration_pair.second);
	}
	if (this->is_semantically_invalid) {
		if (this->select_list.empty()) {
			throw QuerySemanticException({"FALSE"}, "");
		}
		else {
			throw QuerySemanticException({}, "");
		}
	}

	return {declarations, this->select_list, this->clause_list};
}

// Declaration

void QP::QueryPreprocessor::parseDeclarations() {
	optional<Types::DesignEntity> entity = parseDesignEntity();
	while (entity.has_value()) {
		parseDeclarationGroup(entity.value());
		entity = parseDesignEntity();
	}
}

void QP::QueryPreprocessor::parseDeclarationGroup(const Types::DesignEntity& type) {
	if (!Common::Validator::validateName(this->query_tokens.at(token_index))) {
		throw QueryException("Unexpected end of declaration.");
	}
	parseDeclaration(type);
	while (this->query_tokens.at(token_index) != ";") {
		matchTokenOrThrow(",");
		parseDeclaration(type);
	}
	matchTokenOrThrow(";");
}

void QP::QueryPreprocessor::parseDeclaration(const Types::DesignEntity& type) {
	string current_token = this->query_tokens.at(token_index);
	if (existing_declarations.find(current_token) != existing_declarations.end()) {
		this->is_semantically_invalid = true;
	}
	this->existing_declarations.insert({current_token, {type, current_token}});
	++token_index;
}

// Select

void QP::QueryPreprocessor::parseSelect() {
	matchTokenOrThrow("Select");
	string current_token = query_tokens.at(token_index);
	if (current_token == "BOOLEAN" && existing_declarations.find("BOOLEAN") == existing_declarations.end()) {
		token_index++;
	} else {
		parseSelectList();
	}
}

void QP::QueryPreprocessor::parseSelectList() {
	if (query_tokens.at(token_index) == "<") {
		matchTokenOrThrow("<");
		select_list.push_back(parseSelectArgument());
		while (query_tokens.at(token_index) == ",") {
			matchTokenOrThrow(",");
			select_list.push_back(parseSelectArgument());
		}
		matchTokenOrThrow(">");
	} else {
		select_list.push_back(parseSelectArgument());
	}
}

// Clause - General

void QP::QueryPreprocessor::parseClauses() {
	if (this->query_tokens.at(token_index) == "such") {
		++token_index;
		matchTokenOrThrow("that");
		parseClauseLoop(&QueryPreprocessor::parseSuchThat);
		return;
	}
	if (this->query_tokens.at(token_index) == "with") {
		++token_index;
		parseClauseLoop(&QueryPreprocessor::parseWith);
		return;
	}
	if (this->query_tokens.at(token_index) == "pattern") {
		++token_index;
		parseClauseLoop(&QueryPreprocessor::parsePattern);
		return;
	}
	throw QP::QueryException("Unexpected token received.");
}

void QP::QueryPreprocessor::parseClauseLoop(void (QueryPreprocessor::*parser)()) {
	(this->*parser)();
	while (token_index < this->query_tokens.size() && this->query_tokens.at(token_index) == "and") {
		token_index++;
		(this->*parser)();
	}
}

void QP::QueryPreprocessor::parseClause(QP::Types::ClauseType type) { parseClause(type, {}); }

void QP::QueryPreprocessor::parseClause(QP::Types::ClauseType type, vector<QP::Types::ReferenceArgument> prefixes) {
	vector<Types::ReferenceArgument> arguments = parseArguments();
	prefixes.insert(prefixes.end(), arguments.begin(), arguments.end());
	createClause(type, move(prefixes));
}

void QP::QueryPreprocessor::createClause(QP::Types::ClauseType type, vector<QP::Types::ReferenceArgument> arguments) {
	QP::Types::ArgumentDispatcher argument_dispatcher = dispatcher.dispatch_map.at(type);
	auto info = argument_dispatcher(arguments);
	this->clause_list.push_back({make_unique<Relationship::Relation>(info.first, move(arguments), info.second)});
}

// Clause - Such that

void QP::QueryPreprocessor::parseSuchThat() {
	auto clause_type_iter = dispatcher.clause_map.find(this->query_tokens.at(token_index));
	if (clause_type_iter == dispatcher.clause_map.end()) {
		throw QueryException("Unexpected relation type.");
	}
	token_index++;
	auto type = clause_type_iter->second;
	parseClause(type);
}

// Clause - With

void QP::QueryPreprocessor::parseWith() {
	vector<Types::ReferenceArgument> arguments;
	arguments.push_back(parseReferenceArgument());
	matchTokenOrThrow("=");
	arguments.push_back(parseReferenceArgument());
	createClause(Types::ClauseType::With, move(arguments));
}

// Clause - Pattern

void QP::QueryPreprocessor::parsePattern() {
	Declaration synonym = parseClauseSynonym();
	Types::ReferenceArgument synonym_argument = Types::ReferenceArgument(synonym);
	// Assign Pattern requires custom parsing to always parse last argument as expression even if presented as name.
	if (synonym.type == Types::DesignEntity::Assign) {
		parseAssignPattern(synonym_argument);
	} else {
		auto iter = dispatcher.pattern_clause_map.find(synonym.type);
		if (iter == dispatcher.pattern_clause_map.end()) {
			throw QP::QueryException("Incorrect synonym type.");
		}
		parseClause(iter->second, {synonym_argument});
	}
}

void QP::QueryPreprocessor::parseAssignPattern(Types::ReferenceArgument synonym) {
	QP::Types::ArgumentDispatcher argument_dispatcher = dispatcher.dispatch_map.at(Types::ClauseType::PatternAssign);
	matchTokenOrThrow("(");
	Types::ReferenceArgument variable = parseReferenceArgument();
	matchTokenOrThrow(",");
	Types::ReferenceArgument expression_argument;
	if (this->query_tokens.at(token_index) == "_") {
		token_index++;
		if (this->query_tokens.at(token_index) == "\"") {
			Common::ExpressionProcessor::Expression expression = parseExpression();
			matchTokenOrThrow("_");
			matchTokenOrThrow(")");
			expression_argument = Types::ReferenceArgument(expression, false);
		} else {
			// Default ReferenceArgument is wildcard.
			matchTokenOrThrow(")");
		}
	} else if (this->query_tokens.at(token_index) == "\"") {
		Common::ExpressionProcessor::Expression expression = parseExpression();
		matchTokenOrThrow(")");
		expression_argument = Types::ReferenceArgument(expression, true);
	} else {
		throw QueryException("Unexpected query expression: " + this->query_tokens.at(token_index) + ".");
	}

	vector<Types::ReferenceArgument> arguments = {move(synonym), move(variable), move(expression_argument)};

	auto info = argument_dispatcher(arguments);

	this->clause_list.push_back({make_unique<Relationship::Relation>(info.first, arguments, info.second)});
}

// Atomic entities

optional<QP::Types::DesignEntity> QP::QueryPreprocessor::parseDesignEntity() {
	Types::DesignEntity design_entity;
	if (this->query_tokens.at(token_index) == "stmt") {
		design_entity = Types::DesignEntity::Stmt;
	} else if (this->query_tokens.at(token_index) == "read") {
		design_entity = Types::DesignEntity::Read;
	} else if (this->query_tokens.at(token_index) == "print") {
		design_entity = Types::DesignEntity::Print;
	} else if (this->query_tokens.at(token_index) == "call") {
		design_entity = Types::DesignEntity::Call;
	} else if (this->query_tokens.at(token_index) == "while") {
		design_entity = Types::DesignEntity::While;
	} else if (this->query_tokens.at(token_index) == "if") {
		design_entity = Types::DesignEntity::If;
	} else if (this->query_tokens.at(token_index) == "assign") {
		design_entity = Types::DesignEntity::Assign;
	} else if (this->query_tokens.at(token_index) == "variable") {
		design_entity = Types::DesignEntity::Variable;
	} else if (this->query_tokens.at(token_index) == "constant") {
		design_entity = Types::DesignEntity::Constant;
	} else if (this->query_tokens.at(token_index) == "procedure") {
		design_entity = Types::DesignEntity::Procedure;
	} else {
		return {};
	}
	++token_index;
	return design_entity;
}

vector<QP::Types::ReferenceArgument> QP::QueryPreprocessor::parseArguments() {
	vector<QP::Types::ReferenceArgument> arguments;
	matchTokenOrThrow("(");
	do {
		arguments.push_back(parseReferenceArgument());
	} while (this->query_tokens.at(token_index) == "," && ((token_index++) != 0));
	matchTokenOrThrow(")");
	return arguments;
}

QP::Types::ReferenceArgument QP::QueryPreprocessor::parseReferenceArgument() {
	if (this->query_tokens.at(token_index) == "_") {
		token_index++;
		return Types::ReferenceArgument();
	}
	if (Common::Validator::validateInteger(this->query_tokens.at(token_index))) {
		token_index++;
		return Types::ReferenceArgument(stoul(this->query_tokens.at(token_index - 1)));
	}
	if (this->query_tokens.at(token_index) == "\"") {
		if (!Common::Validator::validateName(this->query_tokens.at(++token_index))) {
			throw QP::QueryException("Unknown clause argument.");
		}
		token_index += 2;
		return Types::ReferenceArgument(this->query_tokens.at(token_index - 2));
	}
	return parseSelectArgument();
}

QP::Types::ReferenceArgument QP::QueryPreprocessor::parseSelectArgument() {
	if (token_index + 1 < query_tokens.size() && this->query_tokens.at(token_index + 1) == ".") {
		Declaration synonym = parseClauseSynonym();
		matchTokenOrThrow(".");
		auto iter = dispatcher.attribute_map.find({synonym.type, query_tokens.at(token_index)});
		if (iter == dispatcher.attribute_map.end()) {
			throw QP::QueryException("Invalid attribute type.");
		}
		token_index++;
		return Types::ReferenceArgument(Types::Attribute{iter->second, synonym});
	}
	if (Common::Validator::validateName(this->query_tokens.at(token_index))) {
		return Types::ReferenceArgument(parseClauseSynonym());
	}
	throw QP::QueryException("Unknown argument.");
}

Common::ExpressionProcessor::Expression QP::QueryPreprocessor::parseExpression() {
	matchTokenOrThrow("\"");
	vector<string> expression;
	while (this->query_tokens.at(token_index) != "\"") {
		expression.push_back(this->query_tokens.at(token_index));
		token_index++;
	}
	QueryExpressionLexer lexer = QueryExpressionLexer(expression);
	Common::ExpressionProcessor::Expression arithmetic_expression =
		Common::ExpressionProcessor::ExpressionParser{lexer, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
	if (!lexer.readToken().empty()) {
		throw QueryException("Invalid expression.");
	}

	matchTokenOrThrow("\"");
	return arithmetic_expression;
}

QP::Types::Declaration QP::QueryPreprocessor::parseClauseSynonym() {
	auto synonym_search_result = existing_declarations.find(query_tokens.at(token_index));
	if (synonym_search_result == existing_declarations.end()) {
		throw QP::QueryException("Undeclared synonym detected.");
	}
	token_index++;
	return synonym_search_result->second;
}

// Others

void QP::QueryPreprocessor::matchTokenOrThrow(const string& token) {
	if (this->query_tokens.at(token_index) == token) {
		token_index++;  // Skip token
	} else {
		throw QueryException("Missing token: " + token + " VS " + this->query_tokens.at(token_index) + ".");
	}
}

void QP::QueryPreprocessor::reset() {
	token_index = 0;
	query_tokens.clear();
	existing_declarations.clear();
	select_list.clear();
	clause_list.clear();
}