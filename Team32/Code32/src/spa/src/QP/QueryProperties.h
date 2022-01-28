#ifndef QUERY_PROPERTIES_H_
#define QUERY_PROPERTIES_H_

#include <string>
#include <vector>

#include "QueryTypeDefs.h"
#include "Relationship/Relation.h"

using namespace std;

typedef struct PatternClause {
	Declaration synonym;
	QueryEntRef entRef;
	string expression;
} PatternClause;

typedef struct SuchThatClause {
	Relation relation;
} SuchThatClause;

typedef vector<Declaration> DeclarationList;
typedef vector<SuchThatClause> SuchThatClauseList;
typedef vector<PatternClause> PatternClauseList;

class QueryProperties {
public:
	QueryProperties(DeclarationList declarationList, Declaration select,
		SuchThatClauseList suchThatClauseList, PatternClauseList patternClauseList);

private:
	DeclarationList declarationList;
	Declaration select;
	SuchThatClauseList suchThatClauseList;
	PatternClauseList patternClauseList;
};

#endif // QUERY_PROPERTIES_H_
