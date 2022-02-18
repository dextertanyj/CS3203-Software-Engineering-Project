#include "QP/QueryProperties.h"

#include <utility>

QueryProperties::QueryProperties(DeclarationList declaration_list, Declaration select, SuchThatClauseList such_that_clause_list,
                                 PatternClauseList pattern_clause_list)
	: declaration_list(std::move(std::move(declaration_list))),
	  select(std::move(std::move(select))),
	  such_that_clause_list(std::move(std::move(such_that_clause_list))),
	  pattern_clause_list(std::move(std::move(pattern_clause_list))) {}

DeclarationList QueryProperties::getDeclarationList() { return declaration_list; }

Declaration QueryProperties::getSelect() { return select; }

SuchThatClauseList QueryProperties::getSuchThatClauseList() { return such_that_clause_list; }

PatternClauseList QueryProperties::getPatternClauseList() { return pattern_clause_list; }
