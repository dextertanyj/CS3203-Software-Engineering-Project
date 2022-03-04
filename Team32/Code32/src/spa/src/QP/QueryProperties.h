#ifndef SPA_SRC_QP_QUERYPROPERTIES_H
#define SPA_SRC_QP_QUERYPROPERTIES_H

#include <memory>
#include <string>
#include <vector>

#include "Common/ExpressionProcessor/Expression.h"
#include "QP/QueryTypes.h"
#include "QP/Relationship/Pattern.h"
#include "QP/Relationship/Relation.h"

using namespace std;

class QP::QueryProperties {
public:
	QueryProperties(DeclarationList declaration_list, Declaration select, ClauseList clause_list);

	DeclarationList getDeclarationList();
	Declaration getSelect();
	ClauseList getClauseList();

private:
	DeclarationList declaration_list;
	Declaration select;
	ClauseList clause_list;
};

#endif  // SPA_SRC_QP_QUERYPROPERTIES_H
