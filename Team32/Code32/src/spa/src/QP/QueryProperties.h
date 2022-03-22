#ifndef SPA_SRC_QP_QUERYPROPERTIES_H
#define SPA_SRC_QP_QUERYPROPERTIES_H

#include "QP/Types.h"

using namespace std;

class QP::QueryProperties {
public:
	QueryProperties(Types::DeclarationList declaration_list, Types::DeclarationList select_list, Types::ClauseList clause_list);

	Types::DeclarationList getDeclarationList();
	Types::DeclarationList getSelectList();
	Types::ClauseList getClauseList();

private:
	Types::DeclarationList declaration_list;
	Types::DeclarationList select_list;
	Types::ClauseList clause_list;
};

#endif  // SPA_SRC_QP_QUERYPROPERTIES_H
