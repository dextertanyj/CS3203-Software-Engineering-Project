#include "PKB/CallStatementStore.h"

#include <stdexcept>

PKB::CallStatementStore::CallStatementStore() = default;

void PKB::CallStatementStore::set(const shared_ptr<StmtInfo>& statement, ProcRef procedure) {
	if (statement->getType() != StmtType::Call) {
		throw logic_error("Invalid statement type for call store.");
	}
	StmtRef index = statement->getIdentifier();
	if (map.find(index) != map.end()) {
		throw logic_error("Call statement already set.");
	}
	map.insert({index, std::move(procedure)});
}

ProcRef PKB::CallStatementStore::getProcedure(const shared_ptr<StmtInfo>& statement) const {
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

ProcRef PKB::CallStatementStore::getProcedure(StmtRef index) const {
	auto iter = map.find(index);
	if (iter == map.end()) {
		throw logic_error("Statement not found.");
	}
	return iter->second;
}

void PKB::CallStatementStore::populate(const PKB::Types::ProcedureStore& procedures,
                                       PKB::TransitiveRelationStore<ProcRef, PKB::ProcedureInfo, PKB::CallRelation>& call_store) const {
	for (const shared_ptr<ProcedureInfo>& procedure : procedures.getAll()) {
		for (const shared_ptr<StmtInfo>& statement : procedure->getStatements()) {
			if (statement->getType() != StmtType::Call) {
				continue;
			}
			ProcRef callee_name = getProcedure(statement);
			shared_ptr<ProcedureInfo> callee = procedures.get(callee_name);
			if (callee == nullptr) {
				throw logic_error("Procedure does not exist.");
			}
			call_store.set(procedure, callee);
		}
	}
}
