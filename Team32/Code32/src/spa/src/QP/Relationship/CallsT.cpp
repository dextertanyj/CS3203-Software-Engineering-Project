#include "CallsT.h"

QP::QueryResult QP::Relationship::CallsT::executeTrivial(PKB::StorageAccessInterface& pkb) {
	if (caller_ent.getType() == ReferenceType::Name) {
		return executeTrivialCallerVarName(pkb);
	} else {
		return executeTrivialCallerUnderscoreSynonym(pkb);
	}
}

QP::QueryResult QP::Relationship::CallsT::executeNonTrivial(PKB::StorageAccessInterface& pkb) {
	if (caller_ent.getType() == ReferenceType::Name) {
		return executeNonTrivialCallerVarName(pkb);
	} else if (caller_ent.getType() == ReferenceType::Wildcard) {
		return executeNonTrivialCallerUnderscore(pkb);
	} else {
		return executeNonTrivialCallerSynonym(pkb);
	}
}

QP::QueryResult QP::Relationship::CallsT::executeTrivialCallerVarName(PKB::StorageAccessInterface& pkb) {
	if (callee_ent.getType() == ReferenceType::Name) {
		ProcRefSet callee_set = pkb.getCalleeStar(caller_ent.getName());
		for (auto const& callee : callee_set) {
			if (callee == callee_ent.getName()) {
				return QueryResult(true);
			}
		}
		return QueryResult();
	} else {
		ProcRefSet proc_set = pkb.getCalleeStar(caller_ent.getName());
		return QueryResult(!proc_set.empty());
	}
}

QP::QueryResult QP::Relationship::CallsT::executeTrivialCallerUnderscoreSynonym(PKB::StorageAccessInterface& pkb) {
	if (callee_ent.getType() == ReferenceType::Synonym && caller_ent.getType() == ReferenceType::Synonym) {
		if (callee_ent.getSynonym().symbol == caller_ent.getSynonym().symbol) {
			return {};
		}
	}

	if (callee_ent.getType() == ReferenceType::Name) {
		ProcRefSet proc_set = pkb.getCallerStar(callee_ent.getName());
		return QueryResult(!proc_set.empty());
	} else {
		ProcRefSet proc_set = pkb.getProcedures();
		for (auto const& proc : proc_set) {
			ProcRefSet caller_set = pkb.getCallerStar(proc);
			if (!caller_set.empty()) {
				return QueryResult(true);
			}
		}

		return {};
	}
}

QP::QueryResult QP::Relationship::CallsT::executeNonTrivialCallerVarName(PKB::StorageAccessInterface& pkb) {
	if (callee_ent.getType() == ReferenceType::Synonym) {
		vector<string> column;
		ProcRefSet callee_set = pkb.getCalleeStar(caller_ent.getName());
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

QP::QueryResult QP::Relationship::CallsT::executeNonTrivialCallerUnderscore(PKB::StorageAccessInterface& pkb) {
	if (callee_ent.getType() == ReferenceType::Synonym) {
		vector<string> column;
		ProcRefSet proc_set = pkb.getProcedures();
		for (auto const& proc : proc_set) {
			ProcRefSet caller_set = pkb.getCallerStar(proc);
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

QP::QueryResult QP::Relationship::CallsT::executeNonTrivialCallerSynonym(PKB::StorageAccessInterface& pkb) {
	QueryResult result = QueryResult();
	vector<string> caller_column;

	if (callee_ent.getType() == ReferenceType::Name) {
		ProcRefSet caller_set = pkb.getCallerStar(callee_ent.getName());
		for (auto const& caller : caller_set) {
			caller_column.push_back(caller);
		}
	} else if (callee_ent.getType() == ReferenceType::Wildcard) {
		ProcRefSet proc_set = pkb.getProcedures();
		for (auto const& proc : proc_set) {
			ProcRefSet callee_set = pkb.getCalleeStar(proc);
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
			ProcRefSet callee_set = pkb.getCalleeStar(proc);
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
