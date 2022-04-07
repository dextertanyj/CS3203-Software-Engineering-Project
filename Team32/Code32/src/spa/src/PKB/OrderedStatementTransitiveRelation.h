#ifndef SPA_SRC_PKB_ORDEREDSTATEMENTTRANSITIVERELATION_H
#define SPA_SRC_PKB_ORDEREDSTATEMENTTRANSITIVERELATION_H

#include <cassert>

#include "PKB/PKB.h"
#include "PKB/TransitiveRelation.tpp"

class PKB::OrderedStatementTransitiveRelation : public PKB::TransitiveRelation<StmtInfo> {
public:
	using PKB::TransitiveRelation<StmtInfo>::TransitiveRelation;
	void appendForwardTransitive(const StmtInfoPtrSet& new_forward) override;
	void appendReverseTransitive(const StmtInfoPtrSet& new_reverse) override;
};

#endif  // SPA_SRC_PKB_ORDEREDSTATEMENTTRANSITIVERELATION_H
