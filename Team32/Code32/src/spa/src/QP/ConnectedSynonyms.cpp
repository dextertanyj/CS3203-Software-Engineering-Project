#include "QP/ConnectedSynonyms.h"

#include <cassert>

#include <algorithm>

size_t QP::Types::ConnectedSynonyms::getNumberOfGroups() const { return groups.size(); }

void QP::Types::ConnectedSynonyms::insertGroup(unsigned long long group_cost, const vector<string>& synonyms,
                                               const DeclarationList& selected_synonyms) {
	groups.emplace_back(group_cost, synonyms, selected_synonyms);
}

void QP::Types::ConnectedSynonyms::sort() {
	std::sort(groups.begin(), groups.end(), GroupComparator{});
}

DeclarationList QP::Types::ConnectedSynonyms::getGroupSelectedSynonyms(size_t group_number) const {
	assert(group_number < groups.size());
	GroupInformation info = groups.at(group_number);
	return info.selected_synonyms;
}

vector<string> QP::Types::ConnectedSynonyms::getGroupSynonyms(size_t group_number) const {
	assert(group_number < groups.size());
	GroupInformation info = groups.at(group_number);
	return info.synonyms;
}
