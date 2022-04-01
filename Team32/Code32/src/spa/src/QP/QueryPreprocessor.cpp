#include "QueryPreprocessor.h"

#include <utility>

#include "Common/ExpressionProcessor/ExpressionParser.h"
#include "Common/Validator.h"
#include "QP/Dispatcher/DispatchMap.h"
#include "QP/QueryExpressionLexer.h"
#include "QP/Relationship/Relation.h"
#include "QP/Types.h"

using namespace QP::Types;
using namespace QP::Dispatcher;

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

	return {declarations, this->select_list, this->clause_list};
}

// Declaration

void QP::QueryPreprocessor::parseDeclarations() {
	optional<DesignEntity> entity = parseDesignEntity();
	while (entity.has_value()) {
		parseDeclarationGroup(entity.value());
		entity = parseDesignEntity();
	}
}

void QP::QueryPreprocessor::parseDeclarationGroup(const DesignEntity& type) {
	if (!Common::Validator::validateName(this->query_tokens.at(token_index))) {
		throw QuerySyntaxException("Unexpected token found in declaration.");
	}
	do {
		parseDeclaration(type);
	} while (this->query_tokens.at(token_index) != ";" && match(","));
	match(";");
}

void QP::QueryPreprocessor::parseDeclaration(const DesignEntity& type) {
	string current_token = this->query_tokens.at(token_index++);
	if (existing_declarations.find(current_token) != existing_declarations.end()) {
		logSemanticException("Duplicate synonym.");
		return;
	}
	this->existing_declarations.insert({current_token, {type, current_token}});
}

// Select

void QP::QueryPreprocessor::parseSelect() {
	match("Select");
	string current_token = query_tokens.at(token_index);
	if (current_token == "BOOLEAN" && existing_declarations.find("BOOLEAN") == existing_declarations.end()) {
		token_index++;
	} else {
		parseSelectList();
	}
}

void QP::QueryPreprocessor::parseSelectList() {
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

void QP::QueryPreprocessor::parseClauses() {
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
	throw QP::QuerySyntaxException("Unexpected token received.");
}

void QP::QueryPreprocessor::parseClauseLoop(void (QueryPreprocessor::*parser)()) {
	do {
		(this->*parser)();
	} while (token_index < this->query_tokens.size() && this->query_tokens.at(token_index) == "and" && match("and"));
}

void QP::QueryPreprocessor::parseClause(ClauseType type) { parseClause(type, {}); }

void QP::QueryPreprocessor::parseClause(ClauseType type, vector<ReferenceArgument> prefixes) {
	vector<ReferenceArgument> arguments = parseArgumentList(&QueryPreprocessor::parseReferenceArgument);
	prefixes.insert(prefixes.end(), arguments.begin(), arguments.end());
	createClause(type, move(prefixes));
}

void QP::QueryPreprocessor::createClause(ClauseType type, vector<ReferenceArgument> arguments) {
	validateSyntax(type, arguments);

	ArgumentDispatcher argument_dispatcher = DispatchMap::dispatch_map.at(type);
	try {
		auto info = argument_dispatcher(arguments);
		this->clause_list.push_back({make_unique<Relationship::Relation>(info.first, move(arguments), info.second)});
	} catch (const QueryDispatchException& e) {
		logSemanticException(e.what());
	}
}

// Clause - Such that

void QP::QueryPreprocessor::parseSuchThat() {
	auto clause_type_iter = DispatchMap::clause_map.find(this->query_tokens.at(token_index));
	if (clause_type_iter == DispatchMap::clause_map.end()) {
		throw QuerySyntaxException("Unknown relation type.");
	}
	token_index++;
	auto type = clause_type_iter->second;
	parseClause(type);
}

// Clause - With

void QP::QueryPreprocessor::parseWith() {
	vector<ReferenceArgument> arguments;
	arguments.push_back(parseReferenceArgument());
	match("=");
	arguments.push_back(parseReferenceArgument());
	createClause(ClauseType::With, move(arguments));
}

// Clause - Pattern

void QP::QueryPreprocessor::parsePattern() {
	Declaration synonym = parseClauseSynonym();
	ReferenceArgument synonym_argument = ReferenceArgument(synonym);
	// Assign Pattern requires custom parsing to always parse last argument as expression even if presented as name.
	if (synonym.type == DesignEntity::Assign) {
		parseAssignPattern(synonym_argument);
	} else {
		auto iter = DispatchMap::pattern_clause_map.find(synonym.type);
		if (iter == DispatchMap::pattern_clause_map.end()) {
			logSemanticException("Invalid pattern synonym type.");
			validateUnknownPatternSyntax();
			return;
		}
		parseClause(iter->second, {synonym_argument});
	}
}

void QP::QueryPreprocessor::parseAssignPattern(ReferenceArgument synonym) {
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

optional<DesignEntity> QP::QueryPreprocessor::parseDesignEntity() {
	auto iter = DispatchMap::design_entity_map.find(query_tokens.at(token_index));
	if (iter == DispatchMap::design_entity_map.end()) {
		return {};
	}
	++token_index;
	return iter->second;
}

vector<ReferenceArgument> QP::QueryPreprocessor::parseArgumentList(ReferenceArgument (QueryPreprocessor::*parser)()) {
	vector<ReferenceArgument> arguments;
	match("(");
	do {
		arguments.push_back((this->*parser)());
	} while (this->query_tokens.at(token_index) == "," && ((token_index++) != 0));
	match(")");
	return arguments;
}

ReferenceArgument QP::QueryPreprocessor::parseAnyArgument() {
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

ReferenceArgument QP::QueryPreprocessor::parseReferenceArgument() {
	auto result = tryParseReferenceArgument();
	if (!result.has_value()) {
		result = tryParseSelectArgument();
	}
	if (!result.has_value()) {
		throw QuerySyntaxException("Unexpected argument.");
	}
	return result.value();
}

ReferenceArgument QP::QueryPreprocessor::parseArgument(optional<ReferenceArgument> (QueryPreprocessor::*parser)()) {
	auto result = (this->*parser)();
	if (!result.has_value()) {
		throw QuerySyntaxException("Unexpected argument.");
	}
	return result.value();
}

optional<ReferenceArgument> QP::QueryPreprocessor::tryParseReferenceArgument() {
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

optional<ReferenceArgument> QP::QueryPreprocessor::tryParseSelectArgument() {
	if (token_index + 1 < query_tokens.size() && this->query_tokens.at(token_index + 1) == ".") {
		return parseAttribute();
	}
	if (Common::Validator::validateName(this->query_tokens.at(token_index))) {
		return ReferenceArgument(parseClauseSynonym());
	}
	return {};
}

ReferenceArgument QP::QueryPreprocessor::parseAttribute() {
	Declaration synonym = parseClauseSynonym();
	match(".");
	auto token_iter = DispatchMap::attribute_token_map.find(query_tokens.at(token_index));
	if (token_iter == DispatchMap::attribute_token_map.end()) {
		throw QP::QuerySyntaxException("Invalid attribute type.");
	}
	auto iter = DispatchMap::attribute_map.find({synonym.type, token_iter->second});
	token_index++;
	if (iter == DispatchMap::attribute_map.end()) {
		logSemanticException("Unknown synonym attribute combination.");
		return ReferenceArgument(Attribute{});
	}
	return ReferenceArgument(Attribute{iter->second, synonym});
}

optional<ReferenceArgument> QP::QueryPreprocessor::tryParseExpressionArgument() {
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

Declaration QP::QueryPreprocessor::parseClauseSynonym() {
	auto synonym_search_result = existing_declarations.find(query_tokens.at(token_index));
	token_index++;
	if (synonym_search_result == existing_declarations.end()) {
		logSemanticException("Undeclared synonym.");
		return {};
	}
	return synonym_search_result->second;
}

// Others

bool QP::QueryPreprocessor::match(const string& token) {
	if (this->query_tokens.at(token_index) != token) {
		throw QuerySyntaxException("Expected: " + token + ". Received: " + this->query_tokens.at(token_index) + ".");
	}
	token_index++;
	return true;
}

void QP::QueryPreprocessor::reset() {
	token_index = 0;
	semantic_exception_message = optional<string>();
	query_tokens.clear();
	existing_declarations.clear();
	select_list.clear();
	clause_list.clear();
}

void QP::QueryPreprocessor::validateSyntax(ClauseType type, vector<ReferenceArgument> arguments) {
	auto expected_types = DispatchMap::syntax_map.at(type);
	if (expected_types.size() != arguments.size()) {
		throw QuerySyntaxException("Invalid number of arguments.");
	}
	for (int i = 0; i < arguments.size(); i++) {
		auto expected = expected_types.at(i);
		if (expected.find(arguments.at(i).getType()) == expected.end()) {
			throw QuerySyntaxException("Invalid argument type.");
		}
	}
}

void QP::QueryPreprocessor::validateUnknownPatternSyntax() {
	vector<ReferenceArgument> arguments = parseArgumentList(&QueryPreprocessor::parseAnyArgument);
	auto iter = DispatchMap::pattern_syntax_map.find(arguments.size());
	if (iter == DispatchMap::pattern_syntax_map.end()) {
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
	if (!semantic_exception_message.has_value()) {
		semantic_exception_message = message;
	}
}
