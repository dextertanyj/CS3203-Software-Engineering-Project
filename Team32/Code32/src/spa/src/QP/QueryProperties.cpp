#include "QP/QueryProperties.h"

#include <utility>

QP::QueryProperties::QueryProperties(DeclarationList declaration_list, DeclarationList select_list, ClauseList clause_list)
	: declaration_list(std::move(std::move(declaration_list))),
	  select_list(std::move(std::move(select_list))),
	  clause_list(std::move(std::move(clause_list))) {}

DeclarationList QP::QueryProperties::getDeclarationList() { return declaration_list; }

DeclarationList QP::QueryProperties::getSelectList() { return select_list; }

ClauseList QP::QueryProperties::getClauseList() { return clause_list; }
