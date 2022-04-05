#include "OrderedStatementTransitiveRelation.h"

void PKB::OrderedStatementTransitiveRelation::appendForwardTransitive(const StmtInfoPtrSet& new_forward) {
	for (const auto& forward : new_forward) {
		assert(getSelf()->getIdentifier() > forward->getIdentifier());
	}
	PKB::TransitiveRelation<StmtInfo>::appendForwardTransitive(new_forward);
}

void PKB::OrderedStatementTransitiveRelation::appendReverseTransitive(const StmtInfoPtrSet& new_reverse) {
	for (const auto& reverse : new_reverse) {
		assert(getSelf()->getIdentifier() < reverse->getIdentifier());
	}
	PKB::TransitiveRelation<StmtInfo>::appendReverseTransitive(new_reverse);
}