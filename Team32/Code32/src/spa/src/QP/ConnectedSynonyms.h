#ifndef SPA_SRC_QP_CONNECTEDSYNONYMS_H
#define SPA_SRC_QP_CONNECTEDSYNONYMS_H

#include "QP/QP.h"
#include "QP/Types.h"

using QP::Types::DeclarationList;

class QP::Types::ConnectedSynonyms {
public:
	ConnectedSynonyms();
	ConnectedSynonyms(size_t number_of_groups, unordered_map<size_t, vector<string>> group_to_synonyms,
	                  unordered_map<size_t, DeclarationList> group_to_selected_declarations);

	[[nodiscard]] size_t getNumberOfGroups() const;
	[[nodiscard]] DeclarationList getGroupSelectedSynonyms(size_t group_number) const;
	[[nodiscard]] vector<string> getGroupSynonyms(size_t group_number) const;

	void setNumberOfGroups(size_t number_of_groups);
	void insertGroup(size_t group_number, const vector<string>& synonyms);
	void insertSelectedDeclarations(size_t group_number, const DeclarationList& select_list);

private:
	size_t number_of_groups;
	unordered_map<size_t, vector<string>> group_to_synonyms;
	unordered_map<size_t, DeclarationList> group_to_selected_declarations;
};

#endif  // SPA_SRC_QP_CONNECTEDSYNONYMS_H
