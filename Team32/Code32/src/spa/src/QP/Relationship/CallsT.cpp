#include "CallsT.h"

QP::QueryResult QP::Relationship::CallsT::executeTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& /*map*/) {
	if (caller_ent.type == EntRefType::VarName) {
		return executeTrivialCallerVarName(pkb);
	} else {
		return executeTrivialCallerUnderscoreSynonym(pkb);
	}
}

QP::QueryResult QP::Relationship::CallsT::executeNonTrivial(PKB::StorageAccessInterface& pkb,
                                                            unordered_map<string, DesignEntity>& /*map*/) {
	if (caller_ent.type == EntRefType::VarName) {
		return executeNonTrivialCallerVarName(pkb);
	} else if (caller_ent.type == EntRefType::Underscore) {
		return executeNonTrivialCallerUnderscore(pkb);
	} else {
		return executeNonTrivialCallerSynonym(pkb);
	}
}

QP::QueryResult QP::Relationship::CallsT::executeTrivialCallerVarName(PKB::StorageAccessInterface& pkb) {
	if (callee_ent.type == EntRefType::VarName) {
		ProcRefSet callee_set = pkb.getCalleeStar(caller_ent.ent_ref);
		for (auto const& callee : callee_set) {
			if (callee == callee_ent.ent_ref) {
				return QueryResult(true);
			}
		}
		return QueryResult();
	} else {
		ProcRefSet proc_set = pkb.getCalleeStar(caller_ent.ent_ref);
		return QueryResult(!proc_set.empty());
	}
}

QP::QueryResult QP::Relationship::CallsT::executeTrivialCallerUnderscoreSynonym(PKB::StorageAccessInterface& pkb) {
	if (callee_ent.type == EntRefType::VarName) {
		ProcRefSet proc_set = pkb.getCallerStar(callee_ent.ent_ref);
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
	if (callee_ent.type == EntRefType::Synonym) {
		vector<string> column;
		ProcRefSet callee_set = pkb.getCalleeStar(caller_ent.ent_ref);
		for (auto const& callee : callee_set) {
			column.push_back(callee);
		}

		QueryResult result = QueryResult();
		result.addColumn(callee_ent.ent_ref, column);
		return result;
	}

	return {};
}

QP::QueryResult QP::Relationship::CallsT::executeNonTrivialCallerUnderscore(PKB::StorageAccessInterface& pkb) {
	if (callee_ent.type == EntRefType::Synonym) {
		vector<string> column;
		ProcRefSet proc_set = pkb.getProcedures();
		for (auto const& proc : proc_set) {
			ProcRefSet caller_set = pkb.getCallerStar(callee_ent.ent_ref);
			if (!caller_set.empty()) {
				column.push_back(proc);
			}
		}

		QueryResult result = QueryResult();
		result.addColumn(callee_ent.ent_ref, column);
		return result;
	}

	return {};
}

QP::QueryResult QP::Relationship::CallsT::executeNonTrivialCallerSynonym(PKB::StorageAccessInterface& pkb) {
	QueryResult result = QueryResult();
	vector<string> caller_column;

	if (callee_ent.type == EntRefType::VarName) {
		ProcRefSet caller_set = pkb.getCallerStar(callee_ent.ent_ref);
		for (auto const& caller : caller_set) {
			caller_column.push_back(caller);
		}
	} else if (callee_ent.type == EntRefType::Underscore) {
		ProcRefSet proc_set = pkb.getProcedures();
		for (auto const& proc : proc_set) {
			ProcRefSet caller_set = pkb.getCallerStar(proc);
			if (!caller_set.empty()) {
				caller_column.push_back(proc);
			}
		}
	} else if (callee_ent.type == EntRefType::Synonym) {
		vector<string> callee_column;
		ProcRefSet proc_set = pkb.getProcedures();
		for (auto const& proc : proc_set) {
			ProcRefSet callee_set = pkb.getCalleeStar(caller_ent.ent_ref);
			for (auto const& callee : callee_set) {
				caller_column.push_back(proc);
				callee_column.push_back(callee);
			}
		}
		result.addColumn(callee_ent.ent_ref, callee_column);
	}

	result.addColumn(caller_ent.ent_ref, caller_column);
	return result;
}
