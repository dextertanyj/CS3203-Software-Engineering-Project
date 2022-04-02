#include "QP/QueryProperties.h"

#include <algorithm>
#include <utility>

using namespace std;

QP::QueryProperties::QueryProperties(Types::DeclarationList declaration_list, Types::SelectList select_list, Types::ClauseList clause_list)
	: declaration_list(move(declaration_list)), select_list(move(select_list)), clause_list(move(clause_list)) {}

QP::Types::DeclarationList QP::QueryProperties::getDeclarationList() { return declaration_list; }

QP::Types::SelectList QP::QueryProperties::getSelectList() { return select_list; }

QP::Types::DeclarationList QP::QueryProperties::getSelectSynonymList() {
	unordered_set<Types::Declaration, Types::DeclarationHash> set;
	for_each(select_list.begin(), select_list.end(), [&](const ReferenceArgument& arg) { set.insert(arg.getSynonym()); });
	return {set.begin(), set.end()};
}

QP::Types::ClauseList QP::QueryProperties::getClauseList() { return clause_list; }
