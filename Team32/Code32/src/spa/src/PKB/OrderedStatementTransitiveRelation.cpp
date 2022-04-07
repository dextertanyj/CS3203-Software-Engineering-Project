#include "OrderedStatementTransitiveRelation.h"

void PKB::OrderedStatementTransitiveRelation::appendForwardTransitive(const StmtInfoPtrSet& new_forward) {
	assert(all_of(new_forward.begin(), new_forward.end(),
	              [&](const auto& forward) { return getSelf()->getIdentifier() > forward->getIdentifier(); }));
	PKB::TransitiveRelation<StmtInfo>::appendForwardTransitive(new_forward);
}

void PKB::OrderedStatementTransitiveRelation::appendReverseTransitive(const StmtInfoPtrSet& new_reverse) {
	assert(all_of(new_reverse.begin(), new_reverse.end(),
	              [&](const auto& reverse) { return getSelf()->getIdentifier() < reverse->getIdentifier(); }));
	PKB::TransitiveRelation<StmtInfo>::appendReverseTransitive(new_reverse);
}