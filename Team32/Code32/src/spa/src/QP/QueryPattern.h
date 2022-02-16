#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPATTERN_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPATTERN_H_

#include <string>
#include <vector>

#include "Common/ExpressionProcessor/Expression.h"
#include "QP/QueryExpressionLexer.h"
#include "QP/QueryTypeDefs.h"
#include "QP/Relationship/Relation.h"

class QueryPattern {
public:
	QueryPattern(Declaration synonym, QueryEntRef entRef, ExpressionType expressionType, 
				Common::ExpressionProcessor::Expression expression);

	Declaration getSynonym();
	QueryEntRef getEntRef();
	ExpressionType getExpressionType();
	Common::ExpressionProcessor::Expression getExpression();

	QueryResult execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map);
	vector<string> getDeclarationSymbols();

private:
	QueryResult executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map);
	QueryResult executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map);

	Declaration synonym;
	QueryEntRef entRef;
	ExpressionType expressionType;
	Common::ExpressionProcessor::Expression expression;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPATTERN_H_
