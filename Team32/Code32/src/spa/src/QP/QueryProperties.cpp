#include "QP/QueryProperties.h"

#include <utility>

QP::QueryProperties::QueryProperties(DeclarationList declaration_list, Declaration select, SuchThatClauseList such_that_clause_list,
                                 PatternClauseList pattern_clause_list)
	: declaration_list(std::move(std::move(declaration_list))),
	  select(std::move(std::move(select))),
	  such_that_clause_list(std::move(std::move(such_that_clause_list))),
	  pattern_clause_list(std::move(std::move(pattern_clause_list))) {}

DeclarationList QP::QueryProperties::getDeclarationList() { return declaration_list; }

Declaration QP::QueryProperties::getSelect() { return select; }

SuchThatClauseList QP::QueryProperties::getSuchThatClauseList() { return such_that_clause_list; }

PatternClauseList QP::QueryProperties::getPatternClauseList() { return pattern_clause_list; }
