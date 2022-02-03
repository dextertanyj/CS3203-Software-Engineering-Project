#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPREPROCESSOR_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPREPROCESSOR_H_

#include <stdexcept>
#include <string>
#include <vector>

#include "QueryEvaluator.h"
#include "QueryProperties.h"
#include "QueryResult.h"

using std::string;
using std::vector;
using std::runtime_error;

//struct QuerySyntaxException : public runtime_error {
//    using runtime_error::runtime_error;
//};

struct QueryException : public runtime_error {
	using runtime_error::runtime_error;
};

class QueryPreprocessor {
public:
	QueryProperties parseQuery(string query);

private:
    QueryProperties parseQuery();
	void tokenizeQuery(string query);
	void parseDeclaration(int& tokenIndex);
	void parseSelect(int& tokenIndex);
	void parseSuchThat(int& tokenIndex);
	void parsePattern(int& tokenIndex);
	DesignEntity parseDesignEntity(int& tokenIndex);
	Relation parseRelation(int& tokenIndex);
	QueryEntRef parseQueryEntRef(int& tokenIndex);
	QueryStmtRef parseQueryStmtRef(int& tokenIndex);
	string parseExpression(int& tokenIndex);
	bool isIdentOrName(string token);

	vector<string> queryTokens;
	DeclarationList declarationList;
	Declaration select;
	SuchThatClauseList suchThatClauseList;
	PatternClauseList patternClauseList;
	QueryResult queryResult;
};

#endif // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPREPROCESSOR_H_