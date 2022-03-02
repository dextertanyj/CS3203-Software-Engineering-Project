#ifndef SPA_SRC_QP_QUERYPREPROCESSOR_H
#define SPA_SRC_QP_QUERYPREPROCESSOR_H

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
#include "QP/Relationship/Calls.h"
#include "QP/Relationship/CallsT.h"
#include "QP/Relationship/Follows.h"
#include "QP/Relationship/FollowsT.h"
#include "QP/Relationship/ModifiesP.h"
#include "QP/Relationship/ModifiesS.h"
#include "QP/Relationship/Parent.h"
#include "QP/Relationship/ParentT.h"
#include "QP/Relationship/UsesP.h"
#include "QP/Relationship/UsesS.h"

using std::set;
using std::string;
using std::vector;

class QP::QueryPreprocessor {
public:
	QueryProperties parseQuery(string query);

private:
	void tokenizeQuery(string query);

	// Starting point for recursive descent parser
	QueryProperties parseQuery();

	// Parsing clauses/2nd level nodes
	void parseDeclaration(size_t& token_index);
	void parseSelect(size_t& token_index);
	void parseSuchThat(size_t& token_index);
	void parsePattern(size_t& token_index);

	// Parsing Rules
	DesignEntity parseDesignEntity(size_t& token_index);
	unique_ptr<Relationship::Relation> parseRelation(size_t& token_index);
	unique_ptr<Relationship::Follows> parseFollows(size_t& token_index);
	unique_ptr<Relationship::Parent> parseParent(size_t& token_index);
	unique_ptr<Relationship::UsesP> parseUsesP(size_t& token_index);
	unique_ptr<Relationship::UsesS> parseUsesS(size_t& token_index);
	unique_ptr<Relationship::ModifiesP> parseModifiesP(size_t& token_index);
	unique_ptr<Relationship::ModifiesS> parseModifiesS(size_t& token_index);
	unique_ptr<Relationship::Calls> parseCalls(size_t& token_index);
	QueryEntRef parseQueryEntRef(size_t& token_index, const set<DesignEntity>& accepted_design_entities);
	QueryStmtRef parseQueryStmtRef(size_t& token_index, const set<DesignEntity>& accepted_design_entities);
	Common::ExpressionProcessor::Expression parseExpression(size_t& token_index);

	// Helper methods
	static bool isIdentOrName(const string& token);
	void matchTokenOrThrow(const string& token, size_t& token_index);

	vector<string> query_tokens;
	DeclarationList declaration_list;
	Declaration select;
	ClauseList clause_list;
	QueryResult query_result;
};

#endif  // SPA_SRC_QP_QUERYPREPROCESSOR_H