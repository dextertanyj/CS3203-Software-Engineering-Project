#include "PKB/CallStatementStore.h"

PKB::CallStatementStore::CallStatementStore() = default;

void PKB::CallStatementStore::set(const shared_ptr<StmtInfo>& statement, ProcRef procedure) {
	if (statement->type != StmtType::Call) {
		throw logic_error("Invalid statement type for call store.");
	}
	StmtRef index = statement->reference;
	if (map.find(index) != map.end()) {
		throw logic_error("Call statement already set.");
	}
	map.insert({index, std::move(procedure)});
}

ProcRef PKB::CallStatementStore::getProcedure(const shared_ptr<StmtInfo>& statement) const {
	if (statement->type != StmtType::Call) {
		throw logic_error("Invalid statement type for call store.");
	}
	StmtRef index = statement->reference;
	auto iter = map.find(index);
	if (iter == map.end()) {
		throw logic_error("Call statement not set.");
	}
	return iter->second;
}

void PKB::CallStatementStore::populate(const ProcedureStore& procedures,
                                       PKB::TransitiveRelationStore<ProcRef, PKB::ProcedureInfo, PKB::CallRelation>& call_store) const {
	for (const shared_ptr<ProcedureInfo>& procedure : procedures.getAllInfo()) {
		for (const shared_ptr<StmtInfo>& statement : procedure->getStatements()) {
			if (statement->type != StmtType::Call) {
				continue;
			}
			ProcRef callee_name = getProcedure(statement);
			shared_ptr<ProcedureInfo> callee = procedures.get(callee_name);
			call_store.set(procedure, callee);
		}
	}
}
