#include "Calls.h"

QP::Relationship::Calls::Calls(ReferenceArgument caller_ent, ReferenceArgument callee_ent)
	: caller_ent(std::move(std::move(caller_ent))), callee_ent(std::move(std::move(callee_ent))) {}

ReferenceArgument QP::Relationship::Calls::getCallerEnt() { return caller_ent; }

ReferenceArgument QP::Relationship::Calls::getCalleeEnt() { return callee_ent; }

vector<string> QP::Relationship::Calls::getDeclarationSymbols() {
	vector<string> declaration_symbols;
	if (this->caller_ent.getType() == ReferenceType::Synonym) {
		declaration_symbols.push_back(this->caller_ent.getSynonym().symbol);
	}
	if (this->callee_ent.getType() == ReferenceType::Synonym) {
		declaration_symbols.push_back(this->callee_ent.getSynonym().symbol);
	}
	return declaration_symbols;
}

QP::QueryResult QP::Relationship::Calls::executeTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& /*map*/) {
	if (caller_ent.getType() == ReferenceType::Name) {
		return executeTrivialCallerVarName(pkb);
	} else {
		return executeTrivialCallerUnderscoreSynonym(pkb);
	}
}

QP::QueryResult QP::Relationship::Calls::executeNonTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& /*map*/) {
	if (caller_ent.getType() == ReferenceType::Name) {
		return executeNonTrivialCallerVarName(pkb);
	} else if (caller_ent.getType() == ReferenceType::Wildcard) {
		return executeNonTrivialCallerUnderscore(pkb);
	} else {
		return executeNonTrivialCallerSynonym(pkb);
	}
}

QP::QueryResult QP::Relationship::Calls::executeTrivialCallerVarName(PKB::StorageAccessInterface& pkb) {
	if (callee_ent.getType() == ReferenceType::Name) {
		return QueryResult(pkb.checkCall(caller_ent.getName(), callee_ent.getName()));
	} else {
		ProcRefSet proc_set = pkb.getCallee(caller_ent.getName());
		return QueryResult(!proc_set.empty());
	}
}

QP::QueryResult QP::Relationship::Calls::executeTrivialCallerUnderscoreSynonym(PKB::StorageAccessInterface& pkb) {
	if (callee_ent.getType() == ReferenceType::Synonym && caller_ent.getType() == ReferenceType::Synonym) {
		if (callee_ent.getSynonym().symbol == caller_ent.getSynonym().symbol) {
			return {};
		}
	}

	if (callee_ent.getType() == ReferenceType::Name) {
		ProcRefSet proc_set = pkb.getCaller(callee_ent.getName());
		return QueryResult(!proc_set.empty());
	} else {
		ProcRefSet proc_set = pkb.getProcedures();
		for (auto const& proc : proc_set) {
			ProcRefSet caller_set = pkb.getCaller(proc);
			if (!caller_set.empty()) {
				return QueryResult(true);
			}
		}

		return {};
	}
}

QP::QueryResult QP::Relationship::Calls::executeNonTrivialCallerVarName(PKB::StorageAccessInterface& pkb) {
	if (callee_ent.getType() == ReferenceType::Synonym) {
		vector<string> column;
		ProcRefSet callee_set = pkb.getCallee(caller_ent.getName());
		for (auto const& callee : callee_set) {
			column.push_back(callee);
		}

		QueryResult result = QueryResult();
		result.addColumn(callee_ent.getSynonym().symbol, column);
		return result;
	} else {
		throw QueryException("Invalid non trivial case.");
	}
}

QP::QueryResult QP::Relationship::Calls::executeNonTrivialCallerUnderscore(PKB::StorageAccessInterface& pkb) {
	if (callee_ent.getType() == ReferenceType::Synonym) {
		vector<string> column;
		ProcRefSet proc_set = pkb.getProcedures();
		for (auto const& proc : proc_set) {
			ProcRefSet caller_set = pkb.getCaller(proc);
			if (!caller_set.empty()) {
				column.push_back(proc);
			}
		}

		QueryResult result = QueryResult();
		result.addColumn(callee_ent.getSynonym().symbol, column);
		return result;
	} else {
		throw QueryException("Invalid non trivial case.");
	}
}

QP::QueryResult QP::Relationship::Calls::executeNonTrivialCallerSynonym(PKB::StorageAccessInterface& pkb) {
	QueryResult result = QueryResult();
	vector<string> caller_column;

	if (callee_ent.getType() == ReferenceType::Name) {
		ProcRefSet caller_set = pkb.getCaller(callee_ent.getName());
		for (auto const& caller : caller_set) {
			caller_column.push_back(caller);
		}
	} else if (callee_ent.getType() == ReferenceType::Wildcard) {
		ProcRefSet proc_set = pkb.getProcedures();
		for (auto const& proc : proc_set) {
			ProcRefSet callee_set = pkb.getCallee(proc);
			if (!callee_set.empty()) {
				caller_column.push_back(proc);
			}
		}
	} else if (callee_ent.getType() == ReferenceType::Synonym) {
		if (callee_ent.getSynonym().symbol == caller_ent.getSynonym().symbol) {
			return {};
		}

		vector<string> callee_column;
		ProcRefSet proc_set = pkb.getProcedures();
		for (auto const& proc : proc_set) {
			ProcRefSet callee_set = pkb.getCallee(proc);
			for (auto const& callee : callee_set) {
				caller_column.push_back(proc);
				callee_column.push_back(callee);
			}
		}
		result.addColumn(callee_ent.getSynonym().symbol, callee_column);
	}

	result.addColumn(caller_ent.getSynonym().symbol, caller_column);
	return result;
}
