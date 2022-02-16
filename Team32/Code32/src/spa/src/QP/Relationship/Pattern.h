#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPATTERN_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPATTERN_H_

#include <string>
#include <vector>

#include "Common/ExpressionProcessor/Expression.h"
#include "QP/QueryExpressionLexer.h"
#include "QP/QueryTypeDefs.h"
#include "QP/Relationship/Relation.h"

class Pattern : public Relation {
public:
	Pattern(Declaration synonym, QueryEntRef entRef, ExpressionType expressionType, 
				Common::ExpressionProcessor::Expression expression);

	Declaration getSynAssign();
	QueryEntRef getEntRef();
	ExpressionType getExpressionType();
	Common::ExpressionProcessor::Expression getExpression();

	QueryResult execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map);
	vector<string> getDeclarationSymbols();

protected:
	Declaration synAssign;
	QueryEntRef entRef;
	ExpressionType expressionType;
	Common::ExpressionProcessor::Expression expression;

private:
	QueryResult executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map);
	QueryResult executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map);
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPATTERN_H_
