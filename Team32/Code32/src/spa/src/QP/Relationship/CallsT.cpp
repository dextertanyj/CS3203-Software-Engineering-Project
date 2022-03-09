#include "CallsT.h"

QP::QueryResult QP::Relationship::CallsT::executeTrivial(PKB::StorageAccessInterface& pkb) {
	if (getCallerEnt().getType() == ReferenceType::Name && getCalleeEnt().getType() == ReferenceType::Name) {
		return executeTrivialNameName(pkb, getCallerEnt(), getCalleeEnt());
	}
	if (getCallerEnt().getType() == ReferenceType::Name) {
		return executeTrivialNameWildcardOrSynonym(pkb, getCallerEnt());
	}
	if (getCalleeEnt().getType() == ReferenceType::Name) {
		return executeTrivialWildcardOrSynonymName(pkb, getCalleeEnt());
	}
	if (getCallerEnt().getType() == ReferenceType::Synonym && getCalleeEnt().getType() == ReferenceType::Synonym) {
		return executeTrivialSynonymSynonym(pkb, getCallerEnt(), getCalleeEnt());
	}
	return executeTrivialWildcardOrSynonymWildcardOrSynonym(pkb);
}

QP::QueryResult QP::Relationship::CallsT::executeNonTrivial(PKB::StorageAccessInterface& pkb) {
	if (getCallerEnt().getType() == ReferenceType::Name && getCalleeEnt().getType() == ReferenceType::Synonym) {
		return executeNameSynonym(pkb, getCallerEnt(), getCalleeEnt());
	}
	if (getCallerEnt().getType() == ReferenceType::Wildcard && getCalleeEnt().getType() == ReferenceType::Synonym) {
		return executeWildcardSynonym(pkb, getCalleeEnt());
	}
	if (getCallerEnt().getType() == ReferenceType::Synonym && getCalleeEnt().getType() == ReferenceType::Name) {
		return executeSynonymName(pkb, getCallerEnt(), getCalleeEnt());
	}
	if (getCallerEnt().getType() == ReferenceType::Synonym && getCalleeEnt().getType() == ReferenceType::Wildcard) {
		return executeSynonymWildcard(pkb, getCallerEnt());
	}
	if (getCallerEnt().getType() == ReferenceType::Synonym && getCalleeEnt().getType() == ReferenceType::Synonym) {
		return executeSynonymSynonym(pkb, getCallerEnt(), getCalleeEnt());
	}
	return {};
}


// Trivial Executors

QP::QueryResult QP::Relationship::CallsT::executeTrivialNameName(PKB::StorageAccessInterface& pkb, const ReferenceArgument& caller,
                                                                const ReferenceArgument& callee) {
	ProcRefSet callee_set = pkb.getCalleeStar(caller.getName());
	for (auto const& callee_reference : callee_set) {
		if (callee_reference == callee.getName()) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::CallsT::executeTrivialNameWildcardOrSynonym(PKB::StorageAccessInterface& pkb,
                                                                             const ReferenceArgument& caller) {
	return QueryResult(!pkb.getCalleeStar(caller.getName()).empty());
}

QP::QueryResult QP::Relationship::CallsT::executeTrivialWildcardOrSynonymName(PKB::StorageAccessInterface& pkb,
                                                                             const ReferenceArgument& callee) {
	return QueryResult(!pkb.getCallerStar(callee.getName()).empty());
}

QP::QueryResult QP::Relationship::CallsT::executeTrivialWildcardOrSynonymWildcardOrSynonym(PKB::StorageAccessInterface& pkb) {
	ProcRefSet proc_set = pkb.getProcedures();
	for (auto const& proc : proc_set) {
		ProcRefSet caller_set = pkb.getCallerStar(proc);
		if (!caller_set.empty()) {
			return QueryResult(true);
		}
	}

	return {};
}

QP::QueryResult QP::Relationship::CallsT::executeTrivialSynonymSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& caller,
                                                                      const ReferenceArgument& callee) {
	if (caller.getSynonym().symbol == callee.getSynonym().symbol) {
		return {};
	}
	return executeTrivialWildcardOrSynonymWildcardOrSynonym(pkb);
}

// Executors

QP::QueryResult QP::Relationship::CallsT::executeNameSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& caller,
                                                            const ReferenceArgument& callee) {
	QueryResult result = QueryResult();
	vector<string> column;
	ProcRefSet callees = pkb.getCalleeStar(caller.getName());
	for (auto const& callee_reference : callees) {
		column.push_back(callee_reference);
	}

	result.addColumn(callee.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::CallsT::executeWildcardSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& callee) {
	QueryResult result = QueryResult();
	vector<string> column;
	ProcRefSet procedures = pkb.getProcedures();
	for (auto const& procedure : procedures) {
		ProcRefSet callers = pkb.getCallerStar(procedure);
		if (!callers.empty()) {
			column.push_back(procedure);
		}
	}

	result.addColumn(callee.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::CallsT::executeSynonymName(PKB::StorageAccessInterface& pkb, const ReferenceArgument& caller,
                                                            const ReferenceArgument& callee) {
	QueryResult result = QueryResult();
	vector<string> caller_column;
	ProcRefSet callers = pkb.getCallerStar(callee.getName());
	for (auto const& caller_reference : callers) {
		caller_column.push_back(caller_reference);
	}
	result.addColumn(caller.getSynonym().symbol, caller_column);
	return result;
}
QP::QueryResult QP::Relationship::CallsT::executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& caller) {
	QueryResult result = QueryResult();
	vector<string> caller_column;
	ProcRefSet procedures = pkb.getProcedures();
	for (auto const& procedure : procedures) {
		ProcRefSet callees = pkb.getCalleeStar(procedure);
		if (!callees.empty()) {
			caller_column.push_back(procedure);
		}
	}
	result.addColumn(caller.getSynonym().symbol, caller_column);
	return result;
}

QP::QueryResult QP::Relationship::CallsT::executeSynonymSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& caller,
                                                               const ReferenceArgument& callee) {
	if (caller.getSynonym().symbol == callee.getSynonym().symbol) {
		return {};
	}

	QueryResult result = QueryResult();
	vector<string> caller_column;
	vector<string> callee_column;

	ProcRefSet procedures = pkb.getProcedures();
	for (auto const& procedure : procedures) {
		ProcRefSet callees = pkb.getCalleeStar(procedure);
		for (auto const& callee_reference : callees) {
			caller_column.push_back(procedure);
			callee_column.push_back(callee_reference);
		}
	}

	result.addColumn(caller.getSynonym().symbol, caller_column);
	result.addColumn(callee.getSynonym().symbol, callee_column);
	return result;
}
