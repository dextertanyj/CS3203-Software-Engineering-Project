#ifndef SPA_SRC_PKB_CALLRELATION_H
#define SPA_SRC_PKB_CALLRELATION_H

#include <memory>
#include <unordered_set>

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "PKB/ProcedureInfo.h"
#include "PKB/TransitiveRelation.tpp"
#include "PKB/TransitiveRelationStore.tpp"

class PKB::CallsRelation : public PKB::TransitiveRelation<ProcedureInfo> {
public:
	using TransitiveRelation<ProcedureInfo>::TransitiveRelation;
	void insertForward(const std::shared_ptr<ProcedureInfo>& caller);
	void insertReverse(const std::shared_ptr<ProcedureInfo>& callee);
	void appendForwardTransitive(const std::unordered_set<std::shared_ptr<ProcedureInfo>>& new_callers) override;
	void appendReverseTransitive(const std::unordered_set<std::shared_ptr<ProcedureInfo>>& new_callees) override;

	[[nodiscard]] std::unordered_set<std::shared_ptr<ProcedureInfo>> getForward() const;
	[[nodiscard]] std::unordered_set<std::shared_ptr<ProcedureInfo>> getReverse() const;

private:
	std::unordered_set<std::shared_ptr<ProcedureInfo>> callers;
	std::unordered_set<std::shared_ptr<ProcedureInfo>> callees;
};

// Template specializations for Call relationship.

template <>
void PKB::TransitiveRelationStore<ProcRef, PKB::ProcedureInfo, PKB::CallsRelation>::optimize();

template <>
std::unordered_set<std::shared_ptr<PKB::ProcedureInfo>>
PKB::TransitiveRelationStore<ProcRef, PKB::ProcedureInfo, PKB::CallsRelation>::populateTransitive(
	CallsRelation& current, std::unordered_set<std::shared_ptr<ProcedureInfo>> previous);

#endif  // SPA_SRC_PKB_CALLRELATION_H
