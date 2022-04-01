#ifndef SPA_SRC_QP_QUERYPREPROCESSOR_H
#define SPA_SRC_QP_QUERYPREPROCESSOR_H

#include <optional>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include "QP/QueryProperties.h"
#include "QP/Types.h"

class QP::QueryPreprocessor {
public:
	QueryProperties parseQuery(string query);

private:
	void tokenizeQuery(string query);

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
	void parseClause(Types::ClauseType type, vector<Types::ReferenceArgument> prefixes);
	void createClause(Types::ClauseType type, vector<Types::ReferenceArgument> arguments);

	void parseSuchThat();
	void parseWith();
	void parsePattern();
	void parseAssignPattern(Types::ReferenceArgument synonym);

	optional<Types::DesignEntity> parseDesignEntity();
	Types::Declaration parseClauseSynonym();
	vector<Types::ReferenceArgument> parseArgumentList(Types::ReferenceArgument (QueryPreprocessor::*parser)());
	Types::ReferenceArgument parseAnyArgument();
	Types::ReferenceArgument parseReferenceArgument();
	Types::ReferenceArgument parseArgument(optional<Types::ReferenceArgument> (QueryPreprocessor::*parser)());
	optional<Types::ReferenceArgument> tryParseReferenceArgument();
	optional<Types::ReferenceArgument> tryParseSelectArgument();
	optional<Types::ReferenceArgument> tryParseExpressionArgument();
	Types::ReferenceArgument parseAttribute();

	bool match(const string& token);
	void reset();
	void validateUnknownPatternSyntax();
	void logSemanticException(const string&& message);

	size_t token_index;
	vector<string> query_tokens;
	unordered_map<string, Types::Declaration> existing_declarations;
	Types::SelectList select_list;
	Types::ClauseList clause_list;

	optional<string> semantic_exception_message;

	static regex invalid_chars_regex;
	static regex query_token_regex;

	static void validateSyntax(Types::ClauseType type, vector<Types::ReferenceArgument> arguments);
};

#endif  // SPA_SRC_QP_QUERYPREPROCESSOR_H