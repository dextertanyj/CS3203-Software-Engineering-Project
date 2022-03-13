#ifndef SPA_SRC_QP_CONNECTEDSYNONYMS_H
#define SPA_SRC_QP_CONNECTEDSYNONYMS_H

#include "QP/QP.h"
#include "QP/QueryTypes.h"

using QP::Types::DeclarationList;

class QP::Types::ConnectedSynonyms {
public:
	ConnectedSynonyms();
	ConnectedSynonyms(size_t number_of_groups, unordered_map<string, size_t> synonyms_in_group,
	                  unordered_map<size_t, DeclarationList> group_to_selected_declarations);

	size_t getNumberOfGroups();
	unordered_map<string, size_t> getSynonymsInGroup();
	unordered_map<size_t, DeclarationList> getGroupToSelectedDeclarations();

	void setNumberOfGroups(size_t number_of_groups);
	void insertSynonym(string synoym_symbol, size_t group_number);
	void insertSelectedDeclarations(size_t group_number, DeclarationList select_list);

private:
	size_t number_of_groups;
	unordered_map<string, size_t> synonyms_in_group;
	unordered_map<size_t, DeclarationList> group_to_selected_declarations;
};

#endif  // SPA_SRC_QP_CONNECTEDSYNONYMS_H
