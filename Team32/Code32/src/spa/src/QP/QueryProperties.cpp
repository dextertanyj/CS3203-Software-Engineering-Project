#include "QP/QueryProperties.h"

#include <utility>

QP::QueryProperties::QueryProperties(DeclarationList declaration_list, Declaration select, ClauseList clause_list)
	: declaration_list(std::move(std::move(declaration_list))),
	  select(std::move(std::move(select))),
	  clause_list(std::move(std::move(clause_list))) {}

DeclarationList QP::QueryProperties::getDeclarationList() { return declaration_list; }

Declaration QP::QueryProperties::getSelect() { return select; }

ClauseList QP::QueryProperties::getClauseList() { return clause_list; }
