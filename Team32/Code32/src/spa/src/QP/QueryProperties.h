#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPROPERTIES_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPROPERTIES_H_

#include <memory>
#include <string>
#include <vector>

#include "Common/ExpressionProcessor/Expression.h"
#include "QP/QueryTypeDefs.h"
#include "QP/Relationship/Pattern.h"
#include "QP/Relationship/Relation.h"

using namespace std;

typedef struct PatternClause {
	shared_ptr<Relation> relation;
} PatternClause;

typedef struct SuchThatClause {
	shared_ptr<Relation> relation;
} SuchThatClause;

typedef vector<Declaration> DeclarationList;
typedef vector<SuchThatClause> SuchThatClauseList;
typedef vector<PatternClause> PatternClauseList;

class QueryProperties {
public:
	QueryProperties(DeclarationList declaration_list, Declaration select, SuchThatClauseList such_that_clause_list,
	                PatternClauseList pattern_clause_list);

	DeclarationList getDeclarationList();
	Declaration getSelect();
	SuchThatClauseList getSuchThatClauseList();
	PatternClauseList getPatternClauseList();

private:
	DeclarationList declaration_list;
	Declaration select;
	SuchThatClauseList such_that_clause_list;
	PatternClauseList pattern_clause_list;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPROPERTIES_H_
