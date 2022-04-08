#ifndef SPA_SRC_PKB_CALLSSTATEMENTSTORE_H
#define SPA_SRC_PKB_CALLSSTATEMENTSTORE_H

#include <memory>
#include <unordered_map>

#include "Common/TypeDefs.h"
#include "PKB/CallsRelation.h"
#include "PKB/PKB.h"
#include "PKB/ProcedureInfo.h"
#include "PKB/TransitiveRelationStore.tpp"
#include "PKB/Types.h"

class PKB::CallsStatementStore {
public:
	CallsStatementStore();
	void set(const StmtInfoPtr& statement, ProcRef procedure);
	[[nodiscard]] ProcRef getProcedure(const StmtInfoPtr& statement) const;
	[[nodiscard]] ProcRef getProcedure(StmtRef index) const;
	void populateCallStore(const Types::ProcedureStore& procedures, Types::CallStore& call_store) const;

private:
	std::unordered_map<StmtRef, ProcRef> map;

	void populate(const StmtInfoPtr& stmt, const PKB::Types::ProcedureStore& procedures, const std::shared_ptr<ProcedureInfo>& procedure,
	              Types::CallStore& store) const;
};

#endif  // SPA_SRC_PKB_CALLSSTATEMENTSTORE_H
