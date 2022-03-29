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
		throw QuerySyntaxException("Unexpected end of query.");
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
	if (semantic_exception_message.has_value()) {
		if (this->select_list.empty()) {
			throw QuerySemanticException({"FALSE"}, semantic_exception_message.value());
		}
		throw QuerySemanticException({}, semantic_exception_message.value());
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
		throw QuerySyntaxException("Unexpected token found in declaration.");
	}
	parseDeclaration(type);
	while (this->query_tokens.at(token_index) != ";") {
		matchTokenOrThrow(",");
		parseDeclaration(type);
	}
	matchTokenOrThrow(";");
}

void QP::QueryPreprocessor::parseDeclaration(const Types::DesignEntity& type) {
	string current_token = this->query_tokens.at(token_index++);
	if (existing_declarations.find(current_token) != existing_declarations.end()) {
		logSemanticException("Duplicate synonym.");
		return;
	}
	this->existing_declarations.insert({current_token, {type, current_token}});
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
		select_list.push_back(parseArgument(&QueryPreprocessor::tryParseSelectArgument));
		while (query_tokens.at(token_index) == ",") {
			matchTokenOrThrow(",");
			select_list.push_back(parseArgument(&QueryPreprocessor::tryParseSelectArgument));
		}
		matchTokenOrThrow(">");
	} else {
		select_list.push_back(parseArgument(&QueryPreprocessor::tryParseSelectArgument));
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
	throw QP::QuerySyntaxException("Unexpected token received.");
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
	vector<Types::ReferenceArgument> arguments = parseArgumentList(&QueryPreprocessor::parseReferenceArgument);
	prefixes.insert(prefixes.end(), arguments.begin(), arguments.end());
	createClause(type, move(prefixes));
}

void QP::QueryPreprocessor::createClause(QP::Types::ClauseType type, vector<QP::Types::ReferenceArgument> arguments) {
	validateSyntax(type, arguments);

	QP::Types::ArgumentDispatcher argument_dispatcher = dispatcher.dispatch_map.at(type);
	try {
		auto info = argument_dispatcher(arguments);
		this->clause_list.push_back({make_unique<Relationship::Relation>(info.first, move(arguments), info.second)});
	} catch (const QueryDispatchException& e) {
		logSemanticException(e.what());
	}
}

// Clause - Such that

void QP::QueryPreprocessor::parseSuchThat() {
	auto clause_type_iter = dispatcher.clause_map.find(this->query_tokens.at(token_index));
	if (clause_type_iter == dispatcher.clause_map.end()) {
		throw QuerySyntaxException("Unknown relation type.");
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
			logSemanticException("Invalid pattern synonym type.");
			validateUnknownPatternSyntax();
			return;
		}
		parseClause(iter->second, {synonym_argument});
	}
}

void QP::QueryPreprocessor::parseAssignPattern(Types::ReferenceArgument synonym) {
	matchTokenOrThrow("(");
	Types::ReferenceArgument variable = parseReferenceArgument();
	matchTokenOrThrow(",");
	Types::ReferenceArgument expression_argument;
	if (this->query_tokens.at(token_index) == "\"" || this->query_tokens.at(token_index + 1) == "\"") {
		expression_argument = parseArgument(&QueryPreprocessor::tryParseExpressionArgument);
	} else {
		matchTokenOrThrow("_");
	}
	matchTokenOrThrow(")");
	vector<Types::ReferenceArgument> arguments = {move(synonym), move(variable), move(expression_argument)};
	createClause(Types::ClauseType::PatternAssign, arguments);
}

// Atomic entities

optional<QP::Types::DesignEntity> QP::QueryPreprocessor::parseDesignEntity() {
	auto iter = dispatcher.design_entity_map.find(query_tokens.at(token_index));
	if (iter == dispatcher.design_entity_map.end()) {
		return {};
	}
	++token_index;
	return iter->second;
}

vector<QP::Types::ReferenceArgument> QP::QueryPreprocessor::parseArgumentList(Types::ReferenceArgument (QueryPreprocessor::*parser)()) {
	vector<QP::Types::ReferenceArgument> arguments;
	matchTokenOrThrow("(");
	do {
		arguments.push_back((this->*parser)());
	} while (this->query_tokens.at(token_index) == "," && ((token_index++) != 0));
	matchTokenOrThrow(")");
	return arguments;
}

QP::Types::ReferenceArgument QP::QueryPreprocessor::parsePatternArgument() {
	auto result = tryParseReferenceArgument();
	if (!result.has_value()) {
		result = tryParseSelectArgument();
	}
	if (!result.has_value()) {
		result = tryParseExpressionArgument();
	}
	if (!result.has_value()) {
		throw QuerySyntaxException("");
	}
	return result.value();
}

QP::Types::ReferenceArgument QP::QueryPreprocessor::parseReferenceArgument() {
	auto result = tryParseReferenceArgument();
	if (!result.has_value()) {
		result = tryParseSelectArgument();
	}
	if (!result.has_value()) {
		throw QuerySyntaxException("");
	}
	return result.value();
}

QP::Types::ReferenceArgument QP::QueryPreprocessor::parseArgument(optional<Types::ReferenceArgument> (QueryPreprocessor::*parser)()) {
	auto result = (this->*parser)();
	if (!result.has_value()) {
		throw QuerySyntaxException("");
	}
	return result.value();
}

optional<QP::Types::ReferenceArgument> QP::QueryPreprocessor::tryParseReferenceArgument() {
	if (this->query_tokens.at(token_index) == "_") {
		token_index++;
		return Types::ReferenceArgument();
	}
	if (Common::Validator::validateInteger(this->query_tokens.at(token_index))) {
		token_index++;
		return Types::ReferenceArgument(stoul(this->query_tokens.at(token_index - 1)));
	}
	if (this->query_tokens.at(token_index) == "\"" && Common::Validator::validateName(this->query_tokens.at(token_index + 1)) ) {
		token_index += 2;
		matchTokenOrThrow("\"");
		return Types::ReferenceArgument(this->query_tokens.at(token_index - 2));
	}
	return {};
}

optional<QP::Types::ReferenceArgument> QP::QueryPreprocessor::tryParseSelectArgument() {
	if (token_index + 1 < query_tokens.size() && this->query_tokens.at(token_index + 1) == ".") {
		Declaration synonym = parseClauseSynonym();
		matchTokenOrThrow(".");
		if (!dispatcher.attribute_string_set.count(query_tokens.at(token_index))) {
			throw QP::QueryException("Invalid attribute type.");
		}

		auto iter = dispatcher.attribute_map.find({synonym.type, query_tokens.at(token_index)});
		token_index++;
		if (iter == dispatcher.attribute_map.end()) {
			logSemanticException("Unknown synonym attribute combination.");
			return Types::ReferenceArgument(Types::Attribute{});
		}
		return Types::ReferenceArgument(Types::Attribute{iter->second, synonym});
	}
	if (Common::Validator::validateName(this->query_tokens.at(token_index))) {
		return Types::ReferenceArgument(parseClauseSynonym());
	}
	return {};
}

optional<QP::Types::ReferenceArgument> QP::QueryPreprocessor::tryParseExpressionArgument() {
	bool is_contains = false;
	if (query_tokens.at(token_index) == "_") {
		is_contains = true;
		token_index++;
	}
	matchTokenOrThrow("\"");
	vector<string> expression_tokens;
	while (this->query_tokens.at(token_index) != "\"") {
		expression_tokens.push_back(this->query_tokens.at(token_index));
		token_index++;
	}
	matchTokenOrThrow("\"");
	if (is_contains) {
		matchTokenOrThrow("_");
	}
	QueryExpressionLexer lexer = QueryExpressionLexer(expression_tokens);
	Common::ExpressionProcessor::Expression expression =
		Common::ExpressionProcessor::ExpressionParser{lexer, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
	if (!lexer.readToken().empty()) {
		throw QuerySyntaxException("Invalid expression.");
	}
	return Types::ReferenceArgument(expression, !is_contains);
}

QP::Types::Declaration QP::QueryPreprocessor::parseClauseSynonym() {
	auto synonym_search_result = existing_declarations.find(query_tokens.at(token_index));
	token_index++;
	if (synonym_search_result == existing_declarations.end()) {
		logSemanticException("Undeclared synonym.");
		return {};
	}
	return synonym_search_result->second;
}

// Others

void QP::QueryPreprocessor::matchTokenOrThrow(const string& token) {
	if (this->query_tokens.at(token_index) == token) {
		token_index++;  // Skip token
	} else {
		throw QuerySyntaxException("Missing token: " + token + " VS " + this->query_tokens.at(token_index) + ".");
	}
}

void QP::QueryPreprocessor::reset() {
	token_index = 0;
	semantic_exception_message = optional<string>();
	query_tokens.clear();
	existing_declarations.clear();
	select_list.clear();
	clause_list.clear();
}

void QP::QueryPreprocessor::validateSyntax(QP::Types::ClauseType type, vector<QP::Types::ReferenceArgument> arguments) {
	auto expected_types = dispatcher.syntax_map.at(type);
	for (int i = 0; i < arguments.size(); i++) {
		auto expected = expected_types.at(i);
		if (expected.find(arguments.at(i).getType()) == expected.end()) {
			throw QuerySyntaxException("Invalid argument type.");
		}
	}
}

void QP::QueryPreprocessor::validateUnknownPatternSyntax() {
	vector<QP::Types::ReferenceArgument> arguments = parseArgumentList(&QueryPreprocessor::parsePatternArgument);
	auto iter = dispatcher.pattern_syntax_map.find(arguments.size());
	if (iter == dispatcher.pattern_syntax_map.end()) {
		throw QuerySyntaxException("Invalid number of arguments.");
	}
	auto expected_types = iter->second;
	for (int i = 0; i < arguments.size(); i++) {
		auto expected = expected_types.at(i);
		if (expected.find(arguments.at(i).getType()) == expected.end()) {
			throw QuerySyntaxException("Invalid argument type.");
		}
	}
}

void QP::QueryPreprocessor::logSemanticException(const string&& message) {
	if (semantic_exception_message->empty()) {
		semantic_exception_message = message;
	}
}
