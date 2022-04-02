#include "QP/Optimizer/ClauseGroups.h"

#include <algorithm>
#include <cassert>

size_t QP::Optimizer::ClauseGroups::getNumberOfGroups() const { return groups.size(); }

void QP::Optimizer::ClauseGroups::insertGroup(unsigned long long group_cost, const vector<string>& synonyms,
                                          const DeclarationList& selected_synonyms) {
	groups.emplace_back(group_cost, synonyms, selected_synonyms);
}

void QP::Optimizer::ClauseGroups::sort() { std::sort(groups.begin(), groups.end(), GroupComparator{}); }

DeclarationList QP::Optimizer::ClauseGroups::getGroupSelectedSynonyms(size_t group_number) const {
	assert(group_number < groups.size());
	GroupInformation info = groups.at(group_number);
	return info.selected_synonyms;
}

vector<string> QP::Optimizer::ClauseGroups::getGroupSynonyms(size_t group_number) const {
	assert(group_number < groups.size());
	GroupInformation info = groups.at(group_number);
	return info.synonyms;
}
