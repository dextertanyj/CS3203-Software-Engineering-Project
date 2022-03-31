#ifndef SPA_SRC_QP_CONNECTEDSYNONYMS_H
#define SPA_SRC_QP_CONNECTEDSYNONYMS_H

#include <vector>

#include "QP/QP.h"
#include "QP/Types.h"

using QP::Types::DeclarationList;

class QP::Types::ConnectedSynonyms {
public:
	ConnectedSynonyms() = default;

	[[nodiscard]] size_t getNumberOfGroups() const;
	[[nodiscard]] DeclarationList getGroupSelectedSynonyms(size_t group_number) const;
	[[nodiscard]] vector<string> getGroupSynonyms(size_t group_number) const;

	void insertGroup(unsigned long long group_cost, const vector<string>& synonyms, const DeclarationList& selected_synonyms);
	void sort();

private:
	struct GroupInformation {
		unsigned long long group_cost;      // NOLINT
		vector<string> synonyms;            // NOLINT
		DeclarationList selected_synonyms;  // NOLINT
		GroupInformation(unsigned long long group_cost, vector<string> synonyms, DeclarationList selected_synonyms)
			: group_cost(group_cost), synonyms(move(synonyms)), selected_synonyms(move(selected_synonyms)) {}
	};

	struct GroupComparator {
		bool operator()(const GroupInformation& lhs, const GroupInformation& rhs) const { return lhs.group_cost < rhs.group_cost; }
	};

	vector<GroupInformation> groups;
};

#endif  // SPA_SRC_QP_CONNECTEDSYNONYMS_H
