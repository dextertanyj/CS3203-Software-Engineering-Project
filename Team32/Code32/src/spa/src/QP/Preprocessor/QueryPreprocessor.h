#ifndef SPA_SRC_QP_PREPROCESSOR_QUERYPREPROCESSOR_H
#define SPA_SRC_QP_PREPROCESSOR_QUERYPREPROCESSOR_H

#include <optional>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "QP/ClauseArgument.h"
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
	void parseClause(Types::ClauseType type, std::vector<ClauseArgument> prefixes);
	void createClause(Types::ClauseType type, std::vector<ClauseArgument> arguments);

	void parseSuchThat();
	void parseWith();
	void parsePattern();

	std::optional<Types::DesignEntity> parseDesignEntity();
	Types::Declaration parseClauseSynonym();
	std::vector<ClauseArgument> parseArgumentList(ClauseArgument (QueryPreprocessor::*parser)());
	ClauseArgument parseAnyArgument();
	ClauseArgument parseClauseArgument();
	ClauseArgument parseArgument(std::optional<ClauseArgument> (QueryPreprocessor::*parser)());
	std::optional<ClauseArgument> tryParseClauseArgument();
	std::optional<ClauseArgument> tryParseSelectArgument();
	std::optional<ClauseArgument> tryParseExpressionArgument();
	ClauseArgument parseAttribute();

	bool match(const std::string& token);
	void reset();
	void handleUnknownPattern();
	void logSemanticException(const std::string&& message);

	size_t token_index;
	std::vector<std::string> query_tokens;
	std::unordered_map<std::string, Types::Declaration> existing_declarations;
	Types::SelectList select_list;
	std::unordered_set<Evaluator::Clause> clause_set;

	std::optional<std::string> semantic_exception_message;

	static std::regex invalid_chars_regex;
	static std::regex query_token_regex;
};

#endif  // SPA_SRC_QP_PREPROCESSOR_QUERYPREPROCESSOR_H