#ifndef SPA_SRC_QP_QUERYPREPROCESSOR_H
#define SPA_SRC_QP_QUERYPREPROCESSOR_H

#include <functional>
#include <memory>
#include <optional>
#include <regex>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "Common/ExpressionProcessor/Expression.h"
#include "QP/Dispatcher/DispatchMap.h"
#include "QP/QueryEvaluator.h"
#include "QP/QueryExpressionLexer.h"
#include "QP/QueryProperties.h"
#include "QP/QueryResult.h"

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
	Declaration parseClauseSynonym();
	vector<Types::ReferenceArgument> parseArguments();
	Types::ReferenceArgument parseReferenceArgument();
	Types::ReferenceArgument parseSelectArgument();
	Common::ExpressionProcessor::Expression parseExpression();

	void matchTokenOrThrow(const string& token);
	void reset();

	size_t token_index;
	vector<string> query_tokens;
	unordered_map<string, Types::Declaration> existing_declarations;
	Types::SelectList select_list;
	Types::ClauseList clause_list;

	QP::Dispatcher::DispatchMap dispatcher;
	bool is_semantically_invalid;

	static regex invalid_chars_regex;
	static regex query_token_regex;
};

#endif  // SPA_SRC_QP_QUERYPREPROCESSOR_H