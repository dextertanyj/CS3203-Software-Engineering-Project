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

	[[nodiscard]] size_t getNumberOfGroups() const;
	[[nodiscard]] size_t getGroupNumber(const string& synonym) const;
	[[nodiscard]] DeclarationList getGroupSynonyms(size_t group_number) const;

	void setNumberOfGroups(size_t number_of_groups);
	void insertSynonym(const string& synoym_symbol, size_t group_number);
	void insertSelectedDeclarations(size_t group_number, const DeclarationList& select_list);

private:
	size_t number_of_groups;
	unordered_map<string, size_t> synonyms_in_group;
	unordered_map<size_t, DeclarationList> group_to_selected_declarations;
};

#endif  // SPA_SRC_QP_CONNECTEDSYNONYMS_H
