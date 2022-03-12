#ifndef SPA_ORDEREDSTATEMENTTRANSITIVERELATION_H
#define SPA_ORDEREDSTATEMENTTRANSITIVERELATION_H

#include "PKB/PKB.h"
#include "PKB/TransitiveRelation.tpp"

class PKB::OrderedStatementTransitiveRelation : public PKB::TransitiveRelation<StmtInfo> {
public:
	using PKB::TransitiveRelation<StmtInfo>::TransitiveRelation;
	void appendForwardTransitive(const StmtInfoPtrSet& new_forward);
	void appendReverseTransitive(const StmtInfoPtrSet& new_reverse);
};

#endif  // SPA_ORDEREDSTATEMENTTRANSITIVERELATION_H
