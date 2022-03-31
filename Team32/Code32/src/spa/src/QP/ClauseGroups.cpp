#include "QP/ClauseGroups.h"

#include <algorithm>
#include <cassert>

size_t QP::Types::ClauseGroups::getNumberOfGroups() const { return groups.size(); }

void QP::Types::ClauseGroups::insertGroup(unsigned long long group_cost, const vector<string>& synonyms,
                                          const DeclarationList& selected_synonyms) {
	groups.emplace_back(group_cost, synonyms, selected_synonyms);
}

void QP::Types::ClauseGroups::sort() { std::sort(groups.begin(), groups.end(), GroupComparator{}); }

DeclarationList QP::Types::ClauseGroups::getGroupSelectedSynonyms(size_t group_number) const {
	assert(group_number < groups.size());
	GroupInformation info = groups.at(group_number);
	return info.selected_synonyms;
}

vector<string> QP::Types::ClauseGroups::getGroupSynonyms(size_t group_number) const {
	assert(group_number < groups.size());
	GroupInformation info = groups.at(group_number);
	return info.synonyms;
}
