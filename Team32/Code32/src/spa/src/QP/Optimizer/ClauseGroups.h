#ifndef SPA_SRC_QP_CLAUSEGROUPS_H
#define SPA_SRC_QP_CLAUSEGROUPS_H

#include <vector>

#include "QP/Optimizer/Optimizer.h"
#include "QP/QP.h"
#include "QP/Types.h"

class QP::Optimizer::ClauseGroups {
public:
	ClauseGroups() = default;

	[[nodiscard]] size_t getNumberOfGroups() const;
	[[nodiscard]] Types::DeclarationList getGroupSelectedSynonyms(size_t group_number) const;
	[[nodiscard]] std::vector<std::string> getGroupSynonyms(size_t group_number) const;

	void insertGroup(unsigned long long group_cost, const std::vector<std::string>& synonyms,
	                 const Types::DeclarationList& selected_synonyms);
	void sort();

private:
	struct GroupInformation {
		unsigned long long group_cost;             // NOLINT(misc-non-private-member-variables-in-classes)
		std::vector<std::string> synonyms;         // NOLINT(misc-non-private-member-variables-in-classes)
		Types::DeclarationList selected_synonyms;  // NOLINT(misc-non-private-member-variables-in-classes)
		GroupInformation(unsigned long long group_cost, std::vector<std::string> synonyms, Types::DeclarationList selected_synonyms)
			: group_cost(group_cost), synonyms(move(synonyms)), selected_synonyms(move(selected_synonyms)) {}
	};

	struct GroupComparator {
		bool operator()(const GroupInformation& lhs, const GroupInformation& rhs) const { return lhs.group_cost < rhs.group_cost; }
	};

	std::vector<GroupInformation> groups;
};

#endif  // SPA_SRC_QP_CONNECTEDSYNONYMS_H
