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
regex QP::Preprocessor::QueryPreprocessor::query_token_regex = regex(
	R"(Affects\*|Follows\*|Calls\*|Parent\*|Next\*|stmt#|[a-zA-Z][a-zA-Z0-9]*|0|[1-9][0-9]*|\(|\)|;|\+|\-|\*|\/|%|_|,|\"|<|>|\*|#|\.|=)");

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
	this->existing_declarations.emplace(current_token, Declaration{type, current_token});
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

void QP::Preprocessor::QueryPreprocessor::parseClause(ClauseType type, vector<ClauseArgument> prefixes) {
	vector<ClauseArgument> arguments = parseArgumentList(&QueryPreprocessor::parseAnyArgument);
	prefixes.insert(prefixes.end(), arguments.begin(), arguments.end());
	createClause(type, move(prefixes));
}

void QP::Preprocessor::QueryPreprocessor::createClause(ClauseType type, vector<ClauseArgument> arguments) {
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
	auto clause_type_iter = Maps::such_that_clause_map.find(this->query_tokens.at(token_index));
	if (clause_type_iter == Maps::such_that_clause_map.end()) {
		throw QuerySyntaxException("Unknown relation type.");
	}
	token_index++;
	auto type = clause_type_iter->second;
	parseClause(type);
}

// Clause - With

void QP::Preprocessor::QueryPreprocessor::parseWith() {
	vector<ClauseArgument> arguments;
	arguments.push_back(parseClauseArgument());
	match("=");
	arguments.push_back(parseClauseArgument());
	createClause(ClauseType::With, move(arguments));
}

// Clause - Pattern

void QP::Preprocessor::QueryPreprocessor::parsePattern() {
	Declaration synonym = parseClauseSynonym();
	ClauseArgument synonym_argument = ClauseArgument(synonym);
	auto iter = Maps::pattern_clause_map.find(synonym.type);
	if (iter == Maps::pattern_clause_map.end()) {
		logSemanticException("Invalid pattern synonym type.");
		handleUnknownPattern();
		return;
	}
	parseClause(iter->second, {synonym_argument});
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

vector<QP::ClauseArgument> QP::Preprocessor::QueryPreprocessor::parseArgumentList(ClauseArgument (QueryPreprocessor::*parser)()) {
	vector<ClauseArgument> arguments;
	match("(");
	do {
		arguments.push_back((this->*parser)());
	} while (this->query_tokens.at(token_index) == "," && ((token_index++) != 0));
	match(")");
	return arguments;
}

QP::ClauseArgument QP::Preprocessor::QueryPreprocessor::parseAnyArgument() {
	auto result = tryParseClauseArgument();
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

QP::ClauseArgument QP::Preprocessor::QueryPreprocessor::parseClauseArgument() {
	auto result = tryParseClauseArgument();
	if (!result.has_value()) {
		result = tryParseSelectArgument();
	}
	if (!result.has_value()) {
		throw QuerySyntaxException("Unexpected argument.");
	}
	return result.value();
}

QP::ClauseArgument QP::Preprocessor::QueryPreprocessor::parseArgument(optional<ClauseArgument> (QueryPreprocessor::*parser)()) {
	auto result = (this->*parser)();
	if (!result.has_value()) {
		throw QuerySyntaxException("Unexpected argument.");
	}
	return result.value();
}

optional<QP::ClauseArgument> QP::Preprocessor::QueryPreprocessor::tryParseClauseArgument() {
	if (this->query_tokens.at(token_index) == "_" && this->query_tokens.at(token_index + 1) != "\"") {
		token_index++;
		return ClauseArgument();
	}
	if (Common::Validator::validateInteger(this->query_tokens.at(token_index))) {
		token_index++;
		return ClauseArgument(stoul(this->query_tokens.at(token_index - 1)));
	}
	if (this->query_tokens.at(token_index) == "\"" && Common::Validator::validateName(this->query_tokens.at(token_index + 1)) &&
	    this->query_tokens.at(token_index + 2) == "\"") {
		token_index += 2;
		match("\"");
		return ClauseArgument(this->query_tokens.at(token_index - 2));
	}
	return {};
}

optional<QP::ClauseArgument> QP::Preprocessor::QueryPreprocessor::tryParseSelectArgument() {
	if (token_index + 1 < query_tokens.size() && this->query_tokens.at(token_index + 1) == ".") {
		return parseAttribute();
	}
	if (Common::Validator::validateName(this->query_tokens.at(token_index))) {
		return ClauseArgument(parseClauseSynonym());
	}
	return {};
}

QP::ClauseArgument QP::Preprocessor::QueryPreprocessor::parseAttribute() {
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
		return ClauseArgument(Attribute{});
	}
	return ClauseArgument(Attribute{iter->second, synonym});
}

optional<QP::ClauseArgument> QP::Preprocessor::QueryPreprocessor::tryParseExpressionArgument() {
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
	auto parser = Common::ExpressionProcessor::ExpressionParser{lexer, Common::ExpressionProcessor::ExpressionType::Arithmetic};
	try {
		auto expression = parser.parse();
		if (!lexer.readToken().empty()) {
			return {};
		}
		return ClauseArgument(expression, !is_contains);
	} catch (const Common::ExpressionProcessor::ExpressionProcessorException& e) {
		throw QuerySyntaxException(e.what());
	}
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
	vector<ClauseArgument> arguments = vector<ClauseArgument>({ClauseArgument{Declaration{}}});
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
