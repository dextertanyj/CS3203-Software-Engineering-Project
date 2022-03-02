#ifndef SPA_SRC_PKB_CALLSTATEMENTSTORE_H
#define SPA_SRC_PKB_CALLSTATEMENTSTORE_H

#include <memory>
#include <unordered_map>

#include "Common/TypeDefs.h"
#include "PKB/CallRelation.h"
#include "PKB/Types.h"
#include "PKB/PKB.h"
#include "PKB/TransitiveRelationStore.tpp"

class PKB::CallStatementStore {
public:
	CallStatementStore();
	void set(const shared_ptr<StmtInfo>& statement, ProcRef procedure);
	[[nodiscard]] ProcRef getProcedure(const shared_ptr<StmtInfo>& statement) const;
	void populate(const Types::ProcedureStore& procedures,
	              TransitiveRelationStore<ProcRef, ProcedureInfo, CallRelation>& store) const;

private:
	unordered_map<StmtRef, ProcRef> map;
};

#endif  // SPA_SRC_PKB_CALLSTATEMENTSTORE_H
