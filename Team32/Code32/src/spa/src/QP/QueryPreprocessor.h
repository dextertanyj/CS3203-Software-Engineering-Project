#ifndef SPA_SRC_QP_QUERYPREPROCESSOR_H
#define SPA_SRC_QP_QUERYPREPROCESSOR_H

#include <functional>
#include <memory>
#include <regex>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "Common/ExpressionProcessor/Expression.h"
#include "QP/Dispatcher.h"
#include "QP/QueryEvaluator.h"
#include "QP/QueryExpressionLexer.h"
#include "QP/QueryProperties.h"
#include "QP/QueryResult.h"
#include "QP/Relationship/Follows.h"
#include "QP/Relationship/FollowsT.h"
#include "QP/Relationship/Modifies.h"
#include "QP/Relationship/ModifiesP.h"
#include "QP/Relationship/ModifiesS.h"
#include "QP/Relationship/Parent.h"
#include "QP/Relationship/ParentT.h"
#include "QP/Relationship/UsesP.h"
#include "QP/Relationship/UsesS.h"

class QP::QueryPreprocessor {
public:
	QueryProperties parseQuery(string query);

private:
	void tokenizeQuery(string query);

	// Starting point for recursive descent parser
	QueryProperties parseQuery();

	// Parsing clauses/2nd level nodes
	void parseDeclarations();
	void parseDeclaration(const Types::DesignEntity& type);
	void parseSelect();
	void parseClauses();
	void parseSuchThat();
	void parsePattern();

	// Parsing Rules
	optional<Types::DesignEntity> parseDesignEntity();
	unique_ptr<Relationship::Relation> parseRelation();
	unique_ptr<Relationship::Relation> parseAssignPattern(Types::Declaration synonym);
	Types::ReferenceArgument parseReferenceArgument();
	Common::ExpressionProcessor::Expression parseExpression();

	// Helper methods
	void matchTokenOrThrow(const string& token);

	size_t token_index;
	vector<string> query_tokens;
	unordered_map<string, Types::Declaration> existing_declarations;
	Types::DeclarationList select_list;
	Types::ClauseList clause_list;

	static regex invalid_chars_regex;
	static regex query_token_regex;
	QP::Dispatcher dispatcher;
};

#endif  // SPA_SRC_QP_QUERYPREPROCESSOR_H