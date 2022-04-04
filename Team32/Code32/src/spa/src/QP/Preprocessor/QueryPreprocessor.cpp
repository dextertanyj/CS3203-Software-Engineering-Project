#include "QP/Preprocessor/QueryPreprocessor.h"

#include <utility>

#include "Common/ExpressionProcessor/ExpressionParser.h"
#include "Common/Validator.h"
#include "QP/Preprocessor/Maps.h"
#include "QP/Preprocessor/QueryExpressionLexer.h"
#include "QP/Preprocessor/SyntaxValidator.h"
#include "QP/Types.h"

using namespace std;
using namespace QP::Types;

regex QP::Preprocessor::QueryPreprocessor::invalid_chars_regex = regex(R"([^a-zA-Z0-9\s,"_\(\);\+\-\*\/%<>#\.=])");
regex QP::Preprocessor::QueryPreprocessor::query_token_regex =
	regex(R"(Affects\*|Follows\*|Calls\*|Parent\*|Next\*|stmt#|[a-zA-Z][a-zA-Z0-9]*|[0-9]+|\(|\)|;|\+|-|\*|\/|%|_|,|\"|<|>|\*|#|\.|=)");

QP::QueryProperties QP::Preprocessor::QueryPreprocessor::parseQuery(string query) {
	reset();
	tokenizeQuery(move(query));
	try {
		return parseQuery();
	} catch (const out_of_range& e) {
		throw QuerySyntaxException("Unexpected end of query.");
	}
}

void QP::Preprocessor::QueryPreprocessor::tokenizeQuery(string query) {
	if (regex_search(query, invalid_chars_regex)) {
		throw QueryTokenizationException("Illegal token encountered.");
	}

	sregex_iterator iter = sregex_iterator(query.begin(), query.end(), query_token_regex);

	while (iter != sregex_iterator()) {
		this->query_tokens.push_back(iter->str());
		++iter;
	}
}

QP::QueryProperties QP::Preprocessor::QueryPreprocessor::parseQuery() {
	parseDeclarations();
	parseSelect();
	while (token_index < this->query_tokens.size()) {
		parseClauses();
	}
	DeclarationList declarations;
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

	ClauseList clauses;
	clauses.reserve(clause_set.size());
	transform(clause_set.begin(), clause_set.end(), back_inserter(clauses),
	          [](const auto& clause) { return make_shared<Evaluator::Clause>(clause); });

	return {declarations, this->select_list, clauses};
}

// Declaration

void QP::Preprocessor::QueryPreprocessor::parseDeclarations() {
	optional<DesignEntity> entity = parseDesignEntity();
	while (entity.has_value()) {
		parseDeclarationGroup(entity.value());
		entity = parseDesignEntity();
	}
}

void QP::Preprocessor::QueryPreprocessor::parseDeclarationGroup(const DesignEntity& type) {
	if (!Common::Validator::validateName(this->query_tokens.at(token_index))) {
		throw QuerySyntaxException("Unexpected token found in declaration.");
	}
	do {
		parseDeclaration(type);
	} while (this->query_tokens.at(token_index) != ";" && match(","));
	match(";");
}

void QP::Preprocessor::QueryPreprocessor::parseDeclaration(const DesignEntity& type) {
	string current_token = this->query_tokens.at(token_index++);
	if (existing_declarations.find(current_token) != existing_declarations.end()) {
		logSemanticException("Duplicate synonym.");
		return;
	}
	this->existing_declarations.insert({current_token, {type, current_token}});
}

// Select

void QP::Preprocessor::QueryPreprocessor::parseSelect() {
	match("Select");
	string current_token = query_tokens.at(token_index);
	if (current_token == "BOOLEAN" && existing_declarations.find("BOOLEAN") == existing_declarations.end()) {
		token_index++;
	} else {
		parseSelectList();
	}
}

void QP::Preprocessor::QueryPreprocessor::parseSelectList() {
	if (query_tokens.at(token_index) == "<") {
		match("<");
		do {
			select_list.push_back(parseArgument(&QueryPreprocessor::tryParseSelectArgument));
		} while (query_tokens.at(token_index) == "," && match(","));
		match(">");
	} else {
		select_list.push_back(parseArgument(&QueryPreprocessor::tryParseSelectArgument));
	}
}

// Clause - General

void QP::Preprocessor::QueryPreprocessor::parseClauses() {
	if (this->query_tokens.at(token_index) == "such") {
		++token_index;
		match("that");
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
	throw QuerySyntaxException("Unexpected token received.");
}

void QP::Preprocessor::QueryPreprocessor::parseClauseLoop(void (QueryPreprocessor::*parser)()) {
	do {
		(this->*parser)();
	} while (token_index < this->query_tokens.size() && this->query_tokens.at(token_index) == "and" && match("and"));
}

void QP::Preprocessor::QueryPreprocessor::parseClause(ClauseType type) { parseClause(type, {}); }

void QP::Preprocessor::QueryPreprocessor::parseClause(ClauseType type, vector<ReferenceArgument> prefixes) {
	vector<ReferenceArgument> arguments = parseArgumentList(&QueryPreprocessor::parseReferenceArgument);
	prefixes.insert(prefixes.end(), arguments.begin(), arguments.end());
	createClause(type, move(prefixes));
}

void QP::Preprocessor::QueryPreprocessor::createClause(ClauseType type, vector<ReferenceArgument> arguments) {
	if (!SyntaxValidator::validateArgumentsSyntax(type, arguments)) {
		throw QuerySyntaxException("Invalid arguments.");
	}

	ArgumentDispatcher argument_dispatcher = Maps::dispatch_map.at(type);
	try {
		auto info = argument_dispatcher(arguments);
		clause_set.emplace(info.first, move(arguments), info.second);
	} catch (const QueryDispatchException& e) {
		logSemanticException(e.what());
	}
}

// Clause - Such that

void QP::Preprocessor::QueryPreprocessor::parseSuchThat() {
	auto clause_type_iter = Maps::clause_map.find(this->query_tokens.at(token_index));
	if (clause_type_iter == Maps::clause_map.end()) {
		throw QuerySyntaxException("Unknown relation type.");
	}
	token_index++;
	auto type = clause_type_iter->second;
	parseClause(type);
}

// Clause - With

void QP::Preprocessor::QueryPreprocessor::parseWith() {
	vector<ReferenceArgument> arguments;
	arguments.push_back(parseReferenceArgument());
	match("=");
	arguments.push_back(parseReferenceArgument());
	createClause(ClauseType::With, move(arguments));
}

// Clause - Pattern

void QP::Preprocessor::QueryPreprocessor::parsePattern() {
	Declaration synonym = parseClauseSynonym();
	ReferenceArgument synonym_argument = ReferenceArgument(synonym);
	// Assign Pattern requires custom parsing to always parse last argument as expression even if presented as name.
	if (synonym.type == DesignEntity::Assign) {
		parseAssignPattern(synonym_argument);
	} else {
		auto iter = Maps::pattern_clause_map.find(synonym.type);
		if (iter == Maps::pattern_clause_map.end()) {
			logSemanticException("Invalid pattern synonym type.");
			handleUnknownPattern();
			return;
		}
		parseClause(iter->second, {synonym_argument});
	}
}

void QP::Preprocessor::QueryPreprocessor::parseAssignPattern(ReferenceArgument synonym) {
	match("(");
	ReferenceArgument variable = parseReferenceArgument();
	match(",");
	ReferenceArgument expression_argument;
	if (this->query_tokens.at(token_index) == "\"" || this->query_tokens.at(token_index + 1) == "\"") {
		expression_argument = parseArgument(&QueryPreprocessor::tryParseExpressionArgument);
	} else {
		match("_");
	}
	match(")");
	vector<ReferenceArgument> arguments = {move(synonym), move(variable), move(expression_argument)};
	createClause(ClauseType::PatternAssign, arguments);
}

// Atomic entities

optional<DesignEntity> QP::Preprocessor::QueryPreprocessor::parseDesignEntity() {
	auto iter = Maps::design_entity_map.find(query_tokens.at(token_index));
	if (iter == Maps::design_entity_map.end()) {
		return {};
	}
	++token_index;
	return iter->second;
}

vector<QP::ReferenceArgument> QP::Preprocessor::QueryPreprocessor::parseArgumentList(ReferenceArgument (QueryPreprocessor::*parser)()) {
	vector<ReferenceArgument> arguments;
	match("(");
	do {
		arguments.push_back((this->*parser)());
	} while (this->query_tokens.at(token_index) == "," && ((token_index++) != 0));
	match(")");
	return arguments;
}

QP::ReferenceArgument QP::Preprocessor::QueryPreprocessor::parseAnyArgument() {
	auto result = tryParseReferenceArgument();
	if (!result.has_value()) {
		result = tryParseSelectArgument();
	}
	if (!result.has_value()) {
		result = tryParseExpressionArgument();
	}
	if (!result.has_value()) {
		throw QuerySyntaxException("Unexpected argument.");
	}
	return result.value();
}

QP::ReferenceArgument QP::Preprocessor::QueryPreprocessor::parseReferenceArgument() {
	auto result = tryParseReferenceArgument();
	if (!result.has_value()) {
		result = tryParseSelectArgument();
	}
	if (!result.has_value()) {
		throw QuerySyntaxException("Unexpected argument.");
	}
	return result.value();
}

QP::ReferenceArgument QP::Preprocessor::QueryPreprocessor::parseArgument(optional<ReferenceArgument> (QueryPreprocessor::*parser)()) {
	auto result = (this->*parser)();
	if (!result.has_value()) {
		throw QuerySyntaxException("Unexpected argument.");
	}
	return result.value();
}

optional<QP::ReferenceArgument> QP::Preprocessor::QueryPreprocessor::tryParseReferenceArgument() {
	if (this->query_tokens.at(token_index) == "_") {
		token_index++;
		return ReferenceArgument();
	}
	if (Common::Validator::validateInteger(this->query_tokens.at(token_index))) {
		token_index++;
		return ReferenceArgument(stoul(this->query_tokens.at(token_index - 1)));
	}
	if (this->query_tokens.at(token_index) == "\"" && Common::Validator::validateName(this->query_tokens.at(token_index + 1))) {
		token_index += 2;
		match("\"");
		return ReferenceArgument(this->query_tokens.at(token_index - 2));
	}
	return {};
}

optional<QP::ReferenceArgument> QP::Preprocessor::QueryPreprocessor::tryParseSelectArgument() {
	if (token_index + 1 < query_tokens.size() && this->query_tokens.at(token_index + 1) == ".") {
		return parseAttribute();
	}
	if (Common::Validator::validateName(this->query_tokens.at(token_index))) {
		return ReferenceArgument(parseClauseSynonym());
	}
	return {};
}

QP::ReferenceArgument QP::Preprocessor::QueryPreprocessor::parseAttribute() {
	Declaration synonym = parseClauseSynonym();
	match(".");
	auto token_iter = Maps::attribute_token_map.find(query_tokens.at(token_index));
	if (token_iter == Maps::attribute_token_map.end()) {
		throw QuerySyntaxException("Invalid attribute type.");
	}
	auto iter = Maps::attribute_map.find({synonym.type, token_iter->second});
	token_index++;
	if (iter == Maps::attribute_map.end()) {
		logSemanticException("Unknown synonym attribute combination.");
		return ReferenceArgument(Attribute{});
	}
	return ReferenceArgument(Attribute{iter->second, synonym});
}

optional<QP::ReferenceArgument> QP::Preprocessor::QueryPreprocessor::tryParseExpressionArgument() {
	bool is_contains = false;
	if (query_tokens.at(token_index) == "_") {
		is_contains = true;
		token_index++;
	}
	match("\"");
	vector<string> expression_tokens;
	while (this->query_tokens.at(token_index) != "\"") {
		expression_tokens.push_back(this->query_tokens.at(token_index));
		token_index++;
	}
	match("\"");
	if (is_contains) {
		match("_");
	}
	QueryExpressionLexer lexer = QueryExpressionLexer(expression_tokens);
	Common::ExpressionProcessor::Expression expression =
		Common::ExpressionProcessor::ExpressionParser{lexer, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
	if (!lexer.readToken().empty()) {
		throw QuerySyntaxException("Invalid expression.");
	}
	return ReferenceArgument(expression, !is_contains);
}

Declaration QP::Preprocessor::QueryPreprocessor::parseClauseSynonym() {
	auto synonym_search_result = existing_declarations.find(query_tokens.at(token_index));
	token_index++;
	if (synonym_search_result == existing_declarations.end()) {
		logSemanticException("Undeclared synonym.");
		return {};
	}
	return synonym_search_result->second;
}

// Others

bool QP::Preprocessor::QueryPreprocessor::match(const string& token) {
	if (this->query_tokens.at(token_index) != token) {
		throw QuerySyntaxException("Expected: " + token + ". Received: " + this->query_tokens.at(token_index) + ".");
	}
	token_index++;
	return true;
}

void QP::Preprocessor::QueryPreprocessor::reset() {
	token_index = 0;
	semantic_exception_message = optional<string>();
	query_tokens.clear();
	existing_declarations.clear();
	select_list.clear();
	clause_set.clear();
}

void QP::Preprocessor::QueryPreprocessor::handleUnknownPattern() {
	vector<ReferenceArgument> arguments = vector<ReferenceArgument>({ReferenceArgument{Declaration{}}});
	auto parsed_arguments = parseArgumentList(&QueryPreprocessor::parseAnyArgument);
	arguments.insert(arguments.end(), parsed_arguments.begin(), parsed_arguments.end());
	if (!SyntaxValidator::validatePatternArgumentsSyntax(arguments)) {
		throw QuerySyntaxException("Invalid arguments.");
	}
}

void QP::Preprocessor::QueryPreprocessor::logSemanticException(const string&& message) {
	if (!semantic_exception_message.has_value()) {
		semantic_exception_message = message;
	}
}
