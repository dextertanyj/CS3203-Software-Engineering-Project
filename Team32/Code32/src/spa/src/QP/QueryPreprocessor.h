#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPREPROCESSOR_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPREPROCESSOR_H_

#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include "Common/ExpressionProcessor/Expression.h"
#include "QP/QueryEvaluator.h"
#include "QP/QueryExpressionLexer.h"
#include "QP/QueryProperties.h"
#include "QP/QueryResult.h"
#include "QP/Relationship/Follows.h"
#include "QP/Relationship/FollowsT.h"
#include "QP/Relationship/ModifiesP.h"
#include "QP/Relationship/ModifiesS.h"
#include "QP/Relationship/Parent.h"
#include "QP/Relationship/ParentT.h"
#include "QP/Relationship/UsesP.h"
#include "QP/Relationship/UsesS.h"

using std::runtime_error;
using std::set;
using std::string;
using std::vector;

//struct QuerySyntaxException : public runtime_error {
//    using runtime_error::runtime_error;
//};

struct QueryException : public runtime_error {
  using runtime_error::runtime_error;
};

struct QueryTokenizationException : public runtime_error {
	using runtime_error::runtime_error;
};

class QueryPreprocessor {
public:
	QueryProperties parseQuery(string query);

private:
	void tokenizeQuery(string query);

	// Starting point for recursive descent parser
    QueryProperties parseQuery();

	// Parsing clauses/2nd level nodes
	void parseDeclaration(int& token_index);
	void parseSelect(int& token_index);
	void parseSuchThat(int& token_index);
	void parsePattern(int& token_index);

	// Parsing Rules
	DesignEntity parseDesignEntity(int& token_index);
	unique_ptr<Relation> parseRelation(int& token_index);
	unique_ptr<Follows> parseFollows(int& token_index);
	unique_ptr<Parent> parseParent(int& token_index);
	unique_ptr<UsesP> parseUsesP(int& token_index);
	unique_ptr<UsesS> parseUsesS(int& token_index);
	unique_ptr<ModifiesP> parseModifiesP(int& token_index);
	unique_ptr<ModifiesS> parseModifiesS(int& token_index);
	QueryEntRef parseQueryEntRef(int& token_index, const set<DesignEntity>& accepted_design_entities);
	QueryStmtRef parseQueryStmtRef(int& token_index, const set<DesignEntity>& accepted_design_entities);
	Common::ExpressionProcessor::Expression parseExpression(int& token_index);

	// Helper methods
	static bool isIdentOrName(const string& token);
	void matchTokenOrThrow(const string& token, int& token_index);

	vector<string> query_tokens;
	DeclarationList declaration_list;
	Declaration select;
	SuchThatClauseList such_that_clause_list;
	PatternClauseList pattern_clause_list;
	QueryResult query_result;
};

#endif // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPREPROCESSOR_H_