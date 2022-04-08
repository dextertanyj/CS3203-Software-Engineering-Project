#ifndef SPA_SRC_QP_QUERYPROPERTIES_H
#define SPA_SRC_QP_QUERYPROPERTIES_H

#include "QP/ClauseArgument.h"
#include "QP/QP.h"
#include "QP/Types.h"

class QP::QueryProperties {
public:
	QueryProperties(Types::DeclarationList declaration_list, Types::SelectList select_list, Types::ClauseList clause_list);

	Types::DeclarationList getDeclarationList();
	Types::DeclarationList getSelectSynonymList();
	Types::SelectList getSelectList();
	Types::ClauseList getClauseList();

private:
	Types::DeclarationList declaration_list;
	Types::SelectList select_list;
	Types::ClauseList clause_list;
};

#endif  // SPA_SRC_QP_QUERYPROPERTIES_H
