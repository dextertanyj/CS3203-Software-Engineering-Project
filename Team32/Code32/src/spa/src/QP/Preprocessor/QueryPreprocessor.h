#ifndef SPA_SRC_QP_QUERYPREPROCESSOR_H
#define SPA_SRC_QP_QUERYPREPROCESSOR_H

#include <optional>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include "QP/Preprocessor/Preprocessor.h"
#include "QP/QueryProperties.h"
#include "QP/Types.h"

class QP::Preprocessor::QueryPreprocessor {
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
	void parseClause(Types::ClauseType type, vector<ReferenceArgument> prefixes);
	void createClause(Types::ClauseType type, vector<ReferenceArgument> arguments);

	void parseSuchThat();
	void parseWith();
	void parsePattern();
	void parseAssignPattern(ReferenceArgument synonym);

	optional<Types::DesignEntity> parseDesignEntity();
	Types::Declaration parseClauseSynonym();
	vector<ReferenceArgument> parseArgumentList(ReferenceArgument (QueryPreprocessor::*parser)());
	ReferenceArgument parseAnyArgument();
	ReferenceArgument parseReferenceArgument();
	ReferenceArgument parseArgument(optional<ReferenceArgument> (QueryPreprocessor::*parser)());
	optional<ReferenceArgument> tryParseReferenceArgument();
	optional<ReferenceArgument> tryParseSelectArgument();
	optional<ReferenceArgument> tryParseExpressionArgument();
	ReferenceArgument parseAttribute();

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

	static void validateSyntax(Types::ClauseType type, vector<ReferenceArgument> arguments);
};

#endif  // SPA_SRC_QP_QUERYPREPROCESSOR_H