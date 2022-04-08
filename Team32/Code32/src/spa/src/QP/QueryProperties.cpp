#include "QP/QueryProperties.h"

#include <algorithm>
#include <iterator>
#include <unordered_set>
#include <utility>

#include "QP/Hash.h"

using namespace std;

QP::QueryProperties::QueryProperties(Types::DeclarationList declaration_list, Types::SelectList select_list, Types::ClauseList clause_list)
	: declaration_list(move(declaration_list)), select_list(move(select_list)), clause_list(move(clause_list)) {}

QP::Types::DeclarationList QP::QueryProperties::getDeclarationList() { return declaration_list; }

QP::Types::SelectList QP::QueryProperties::getSelectList() { return select_list; }

QP::Types::DeclarationList QP::QueryProperties::getSelectSynonymList() {
	unordered_set<Types::Declaration> set;
	set.reserve(select_list.size());
	transform(select_list.begin(), select_list.end(), inserter(set, set.end()),
	          [](const ClauseArgument& argument) { return argument.getSynonym(); });
	return {set.begin(), set.end()};
}

QP::Types::ClauseList QP::QueryProperties::getClauseList() { return clause_list; }
