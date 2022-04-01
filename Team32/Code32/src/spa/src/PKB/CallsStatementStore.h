#ifndef SPA_SRC_PKB_CALLSSTATEMENTSTORE_H
#define SPA_SRC_PKB_CALLSSTATEMENTSTORE_H

#include <memory>
#include <unordered_map>

#include "Common/TypeDefs.h"
#include "PKB/CallsRelation.h"
#include "PKB/PKB.h"
#include "PKB/TransitiveRelationStore.tpp"
#include "PKB/Types.h"

class PKB::CallsStatementStore {
public:
	CallsStatementStore();
	void set(const shared_ptr<StmtInfo>& statement, ProcRef procedure);
	[[nodiscard]] ProcRef getProcedure(const shared_ptr<StmtInfo>& statement) const;
	[[nodiscard]] ProcRef getProcedure(StmtRef index) const;
	void populateCallStore(const Types::ProcedureStore& procedures,
	                       TransitiveRelationStore<ProcRef, ProcedureInfo, CallsRelation>& call_store) const;

private:
	unordered_map<StmtRef, ProcRef> map;
	void populate(const shared_ptr<StmtInfo>& stmt, const PKB::Types::ProcedureStore& procedures,
	              const shared_ptr<ProcedureInfo>& procedure,
	              TransitiveRelationStore<ProcRef, PKB::ProcedureInfo, PKB::CallsRelation>& store) const;
};

#endif  // SPA_SRC_PKB_CALLSSTATEMENTSTORE_H
