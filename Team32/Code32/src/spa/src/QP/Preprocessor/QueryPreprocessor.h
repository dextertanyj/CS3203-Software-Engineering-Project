#ifndef SPA_SRC_QP_QUERYPREPROCESSOR_H
#define SPA_SRC_QP_QUERYPREPROCESSOR_H

#include <optional>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include "QP/Evaluator/Clause.h"
#include "QP/Hash.h"
#include "QP/Preprocessor/Preprocessor.h"
#include "QP/QueryProperties.h"
#include "QP/Types.h"

class QP::Preprocessor::QueryPreprocessor {
public:
	QueryProperties parseQuery(std::string query);

private:
	void tokenizeQuery(std::string query);

	// Starting point for recursive descent parser
	QueryProperties parseQuery();

	// Parse constructs
	void parseDeclarations();
	void parseDeclarationGroup(const Types::DesignEntity& type);
	void parseDeclaration(const Types::DesignEntity& type);

	void parseSelect();
	void parseSelectList();

	void parseClauses();
	void parseClauseLoop(void (QueryPreprocessor::*parser)());
	void parseClause(Types::ClauseType type);
	void parseClause(Types::ClauseType type, std::vector<ReferenceArgument> prefixes);
	void createClause(Types::ClauseType type, std::vector<ReferenceArgument> arguments);

	void parseSuchThat();
	void parseWith();
	void parsePattern();
	void parseAssignPattern(ReferenceArgument synonym);

	std::optional<Types::DesignEntity> parseDesignEntity();
	Types::Declaration parseClauseSynonym();
	std::vector<ReferenceArgument> parseArgumentList(ReferenceArgument (QueryPreprocessor::*parser)());
	ReferenceArgument parseAnyArgument();
	ReferenceArgument parseReferenceArgument();
	ReferenceArgument parseArgument(std::optional<ReferenceArgument> (QueryPreprocessor::*parser)());
	std::optional<ReferenceArgument> tryParseReferenceArgument();
	std::optional<ReferenceArgument> tryParseSelectArgument();
	std::optional<ReferenceArgument> tryParseExpressionArgument();
	ReferenceArgument parseAttribute();

	bool match(const std::string& token);
	void reset();
	void validateUnknownPatternSyntax();
	void logSemanticException(const std::string&& message);

	size_t token_index;
	std::vector<std::string> query_tokens;
	std::unordered_map<std::string, Types::Declaration> existing_declarations;
	Types::SelectList select_list;
	std::unordered_set<Evaluator::Clause> clause_set;

	std::optional<std::string> semantic_exception_message;

	static std::regex invalid_chars_regex;
	static std::regex query_token_regex;

	static void validateSyntax(Types::ClauseType type, std::vector<ReferenceArgument> arguments);
};

#endif  // SPA_SRC_QP_QUERYPREPROCESSOR_H