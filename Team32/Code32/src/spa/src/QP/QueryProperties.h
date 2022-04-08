#ifndef SPA_SRC_QP_QUERYPROPERTIES_H
#define SPA_SRC_QP_QUERYPROPERTIES_H

#include "QP/ClauseArgument.h"
#include "QP/QP.h"
#include "QP/Types.h"

class QP::QueryProperties {
public:
	QueryProperties(Types::DeclarationList declaration_list, Types::SelectList select_list, Types::ClauseList clause_list);

	[[nodiscard]] Types::DeclarationList getDeclarationList() const;
	[[nodiscard]] Types::DeclarationList getSelectSynonymList() const;
	[[nodiscard]] Types::SelectList getSelectList() const;
	[[nodiscard]] Types::ClauseList getClauseList() const;

private:
	Types::DeclarationList declaration_list;
	Types::SelectList select_list;
	Types::ClauseList clause_list;
};

#endif  // SPA_SRC_QP_QUERYPROPERTIES_H
