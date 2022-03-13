#include "QP/ConnectedSynonyms.h"

QP::Types::ConnectedSynonyms::ConnectedSynonyms() {
	number_of_groups = 0;
}

QP::Types::ConnectedSynonyms::ConnectedSynonyms(size_t number_of_groups, unordered_map<string, size_t> synonyms_in_group,
                                                unordered_map<size_t, DeclarationList> group_to_selected_declarations)
	: number_of_groups(number_of_groups),
	  synonyms_in_group(synonyms_in_group),
	  group_to_selected_declarations(group_to_selected_declarations) {}

size_t QP::Types::ConnectedSynonyms::getNumberOfGroups() { return number_of_groups; }

unordered_map<string, size_t> QP::Types::ConnectedSynonyms::getSynonymsInGroup() { return synonyms_in_group; }

unordered_map<size_t, DeclarationList> QP::Types::ConnectedSynonyms::getGroupToSelectedDeclarations() {
	return group_to_selected_declarations;
}

void QP::Types::ConnectedSynonyms::setNumberOfGroups(size_t number_of_groups) {
	this->number_of_groups = number_of_groups;
}

void QP::Types::ConnectedSynonyms::insertSynonym(string synoym_symbol, size_t group_number) {
	synonyms_in_group.insert({synoym_symbol, group_number});
}

void QP::Types::ConnectedSynonyms::insertSelectedDeclarations(size_t group_number, DeclarationList select_list) {
	group_to_selected_declarations.insert({group_number, select_list});
}
