#include "QP/QueryProperties.h"

#include <utility>

QP::QueryProperties::QueryProperties(Types::DeclarationList declaration_list, Types::DeclarationList select_list,
                                     Types::ClauseList clause_list)
	: declaration_list(move(declaration_list)), select_list(move(select_list)), clause_list(move(clause_list)) {}

QP::Types::DeclarationList QP::QueryProperties::getDeclarationList() { return declaration_list; }

QP::Types::DeclarationList QP::QueryProperties::getSelectList() { return select_list; }

QP::Types::ClauseList QP::QueryProperties::getClauseList() { return clause_list; }
