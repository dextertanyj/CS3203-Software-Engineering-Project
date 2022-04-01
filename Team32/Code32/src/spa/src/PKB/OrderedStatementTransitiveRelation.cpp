#include "OrderedStatementTransitiveRelation.h"

void PKB::OrderedStatementTransitiveRelation::appendForwardTransitive(const StmtInfoPtrSet &new_forward) {
    for (const auto &forward: new_forward) {
        if (getSelf()->getIdentifier() <= forward->getIdentifier()) {
            throw invalid_argument("Statement out of order");
        }
    }
    PKB::TransitiveRelation<StmtInfo>::appendForwardTransitive(new_forward);
}

void PKB::OrderedStatementTransitiveRelation::appendReverseTransitive(const StmtInfoPtrSet &new_reverse) {
    for (const auto &reverse: new_reverse) {
        if (getSelf()->getIdentifier() >= reverse->getIdentifier()) {
            throw invalid_argument("Statement out of order");
        }
    }
    PKB::TransitiveRelation<StmtInfo>::appendReverseTransitive(new_reverse);
}