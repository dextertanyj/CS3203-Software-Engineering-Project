#include "QP/ConnectedSynonyms.h"

QP::Types::ConnectedSynonyms::ConnectedSynonyms() : number_of_groups(0) {}

QP::Types::ConnectedSynonyms::ConnectedSynonyms(size_t number_of_groups, unordered_map<size_t, vector<string>> group_to_synonyms,
                                                unordered_map<size_t, DeclarationList> group_to_selected_declarations)
	: number_of_groups(number_of_groups),
	  group_to_synonyms(move(group_to_synonyms)),
	  group_to_selected_declarations(move(group_to_selected_declarations)) {}

size_t QP::Types::ConnectedSynonyms::getNumberOfGroups() const { return number_of_groups; }

void QP::Types::ConnectedSynonyms::setNumberOfGroups(size_t number_of_groups) { this->number_of_groups = number_of_groups; }

void QP::Types::ConnectedSynonyms::insertGroup(size_t group_number, vector<string> synonyms) {
	group_to_synonyms.insert({group_number, synonyms});
}

void QP::Types::ConnectedSynonyms::insertSelectedDeclarations(size_t group_number, const DeclarationList& select_list) {
	group_to_selected_declarations.insert({group_number, select_list});
}

DeclarationList QP::Types::ConnectedSynonyms::getGroupSelectedSynonyms(size_t group_number) const {
	return group_to_selected_declarations.at(group_number);
}

vector<string> QP::Types::ConnectedSynonyms::getGroupSynonyms(size_t group_number) const { return group_to_synonyms.at(group_number); }
