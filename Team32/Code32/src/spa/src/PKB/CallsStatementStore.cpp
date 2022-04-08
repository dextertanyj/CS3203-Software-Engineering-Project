#include "PKB/CallsStatementStore.h"

#include <stdexcept>
#include <utility>

using namespace std;

PKB::CallsStatementStore::CallsStatementStore() = default;

void PKB::CallsStatementStore::set(const StmtInfoPtr& statement, ProcRef procedure) {
	assert(statement->getType() == StmtType::Call);
	assert(map.find(statement->getIdentifier()) == map.end());

	map.emplace(statement->getIdentifier(), move(procedure));
}

ProcRef PKB::CallsStatementStore::getProcedure(const StmtInfoPtr& statement) const {
	assert(statement->getType() == StmtType::Call);
	assert(map.find(statement->getIdentifier()) != map.end());

	return map.at(statement->getIdentifier());
}

ProcRef PKB::CallsStatementStore::getProcedure(StmtRef index) const {
	// Bounds check not required since procedure name attribute is only defined on Call statement synonyms.
	assert(map.find(index) != map.end());
	return map.at(index);
}

void PKB::CallsStatementStore::populateCallStore(const Types::ProcedureStore& procedures, Types::CallStore& call_store) const {
	for (const shared_ptr<ProcedureInfo>& procedure : procedures.getAll()) {
		for (const StmtInfoPtr& info : procedure->getStatements()) {
			populate(info, procedures, procedure, call_store);
		}
	}
}

void PKB::CallsStatementStore::populate(const StmtInfoPtr& stmt, const Types::ProcedureStore& procedures,
                                        const shared_ptr<ProcedureInfo>& procedure, Types::CallStore& store) const {
	if (stmt->getType() != StmtType::Call) {
		return;
	}
	ProcRef callee_name = getProcedure(stmt);
	shared_ptr<ProcedureInfo> callee = procedures.get(callee_name);

	assert(callee != nullptr);

	store.set(procedure, callee);
}
