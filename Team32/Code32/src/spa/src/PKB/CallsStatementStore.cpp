#include "PKB/CallsStatementStore.h"

#include <stdexcept>

using namespace std;

PKB::CallsStatementStore::CallsStatementStore() = default;

void PKB::CallsStatementStore::set(const shared_ptr<StmtInfo>& statement, ProcRef procedure) {
	if (statement->getType() != StmtType::Call) {
		throw logic_error("Invalid statement type for call store.");
	}
	StmtRef index = statement->getIdentifier();
	if (map.find(index) != map.end()) {
		throw logic_error("Call statement already set.");
	}
	map.emplace(index, move(procedure));
}

ProcRef PKB::CallsStatementStore::getProcedure(const shared_ptr<StmtInfo>& statement) const {
	if (statement->getType() != StmtType::Call) {
		throw logic_error("Invalid statement type for call store.");
	}
	StmtRef index = statement->getIdentifier();
	auto iter = map.find(index);
	if (iter == map.end()) {
		throw logic_error("Call statement not set.");
	}
	return iter->second;
}

ProcRef PKB::CallsStatementStore::getProcedure(StmtRef index) const {
	auto iter = map.find(index);
	if (iter == map.end()) {
		throw logic_error("Statement not found.");
	}
	return iter->second;
}

void PKB::CallsStatementStore::populateCallStore(
	const PKB::Types::ProcedureStore& procedures,
	PKB::TransitiveRelationStore<ProcRef, PKB::ProcedureInfo, PKB::CallsRelation>& call_store) const {
	for (const shared_ptr<ProcedureInfo>& procedure : procedures.getAll()) {
		for (const shared_ptr<StmtInfo>& info : procedure->getStatements()) {
			populate(info, procedures, procedure, call_store);
		}
	}
}

void PKB::CallsStatementStore::populate(const shared_ptr<StmtInfo>& stmt, const PKB::Types::ProcedureStore& procedures,
                                        const shared_ptr<ProcedureInfo>& procedure,
                                        PKB::TransitiveRelationStore<ProcRef, PKB::ProcedureInfo, PKB::CallsRelation>& store) const {
	if (stmt->getType() != StmtType::Call) {
		return;
	}
	ProcRef callee_name = getProcedure(stmt);
	shared_ptr<ProcedureInfo> callee = procedures.get(callee_name);
	if (callee == nullptr) {
		throw logic_error("Procedure does not exist.");
	}
	store.set(procedure, callee);
}
