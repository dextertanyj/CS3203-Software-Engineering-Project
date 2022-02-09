#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPREPROCESSOR_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPREPROCESSOR_H_

#include <stdexcept>
#include <string>
#include <vector>

#include "QueryEvaluator.h"
#include "QueryProperties.h"
#include "QueryResult.h"
#include "Relationship/Follows.h"
#include "Relationship/ModifiesP.h"
#include "Relationship/ModifiesS.h"
#include "Relationship/Parent.h"
#include "Relationship/UsesP.h"
#include "Relationship/UsesS.h"

using std::string;
using std::vector;
using std::runtime_error;

//struct QuerySyntaxException : public runtime_error {
//    using runtime_error::runtime_error;
//};

struct QueryException : public runtime_error {
  using runtime_error::runtime_error;
};

struct TokenizationException : public runtime_error {
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
	void parseDeclaration(int& tokenIndex);
	void parseSelect(int& tokenIndex);
	void parseSuchThat(int& tokenIndex);
	void parsePattern(int& tokenIndex);

	// Parsing Rules
	DesignEntity parseDesignEntity(int& tokenIndex);
	Relation* parseRelation(int& tokenIndex);
	Follows* parseFollows(int& tokenIndex);
	Parent* parseParent(int& tokenIndex);
	UsesP* parseUsesP(int& tokenIndex);
	UsesS* parseUsesS(int& tokenIndex);
	ModifiesP* parseModifiesP(int& tokenIndex);
	ModifiesS* parseModifiesS(int& tokenIndex);
	QueryEntRef parseQueryEntRef(int& tokenIndex);
	QueryStmtRef parseQueryStmtRef(int& tokenIndex);
	string parseExpression(int& tokenIndex);

	// Helper methods
	bool isIdentOrName(string token);
	void matchTokenOrThrow(string token, int& tokenIndex);

	vector<string> queryTokens;
	DeclarationList declarationList;
	Declaration select;
	SuchThatClauseList suchThatClauseList;
	PatternClauseList patternClauseList;
	QueryResult queryResult;
};

#endif // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPREPROCESSOR_H_