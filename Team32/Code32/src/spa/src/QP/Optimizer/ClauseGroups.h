#ifndef SPA_SRC_QP_CLAUSEGROUPS_H
#define SPA_SRC_QP_CLAUSEGROUPS_H

#include <vector>

#include "QP/Optimizer/Optimizer.h"
#include "QP/QP.h"
#include "QP/Types.h"

using QP::Types::DeclarationList;

class QP::Optimizer::ClauseGroups {
public:
	ClauseGroups() = default;

	[[nodiscard]] size_t getNumberOfGroups() const;
	[[nodiscard]] DeclarationList getGroupSelectedSynonyms(size_t group_number) const;
	[[nodiscard]] vector<string> getGroupSynonyms(size_t group_number) const;

	void insertGroup(unsigned long long group_cost, const vector<string>& synonyms, const DeclarationList& selected_synonyms);
	void sort();

private:
	struct GroupInformation {
		unsigned long long group_cost;      // NOLINT(misc-non-private-member-variables-in-classes)
		vector<string> synonyms;            // NOLINT(misc-non-private-member-variables-in-classes)
		DeclarationList selected_synonyms;  // NOLINT(misc-non-private-member-variables-in-classes)
		GroupInformation(unsigned long long group_cost, vector<string> synonyms, DeclarationList selected_synonyms)
			: group_cost(group_cost), synonyms(move(synonyms)), selected_synonyms(move(selected_synonyms)) {}
	};

	struct GroupComparator {
		bool operator()(const GroupInformation& lhs, const GroupInformation& rhs) const { return lhs.group_cost < rhs.group_cost; }
	};

	vector<GroupInformation> groups;
};

#endif  // SPA_SRC_QP_CONNECTEDSYNONYMS_H
