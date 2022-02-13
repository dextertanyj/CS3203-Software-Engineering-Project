#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPROPERTIES_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPROPERTIES_H_

#include <string>
#include <vector>

#include "Common/ArithmeticProcessor/ArithmeticExpression.h"
#include "QP/QueryTypeDefs.h"
#include "QP/Relationship/Relation.h"

using std::string;
using std::vector;

typedef struct PatternClause {
	Declaration synonym;
	QueryEntRef entRef;
	ExpressionType expressionType;
	Common::ArithmeticProcessor::ArithmeticExpression* expression;
} PatternClause;

typedef struct SuchThatClause {
	Relation* relation;
} SuchThatClause;

typedef vector<Declaration> DeclarationList;
typedef vector<SuchThatClause> SuchThatClauseList;
typedef vector<PatternClause> PatternClauseList;

class QueryProperties {
public:
	QueryProperties(DeclarationList declarationList,
  	              Declaration select,
  	              SuchThatClauseList suchThatClauseList,
  	              PatternClauseList patternClauseList);

	DeclarationList getDeclarationList();
	Declaration getSelect();
	SuchThatClauseList getSuchThatClauseList();
	PatternClauseList getPatternClauseList();

private:
	DeclarationList declarationList;
	Declaration select;
	SuchThatClauseList suchThatClauseList;
	PatternClauseList patternClauseList;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPROPERTIES_H_
