#include "Calls.h"

#include <utility>

#include "QP/Relationship/CallDispatcherTemplate.tpp"

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

QP::QueryResult QP::Relationship::Calls::executeTrivial(PKB::StorageAccessInterface& pkb) {
	if (caller_ent.getType() == ReferenceType::Name && callee_ent.getType() == ReferenceType::Name) {
		return executeTrivialNameName(pkb, caller_ent, callee_ent);
	}
	if (caller_ent.getType() == ReferenceType::Name) {
		return executeTrivialNameWildcardOrSynonym(pkb, caller_ent);
	}
	if (callee_ent.getType() == ReferenceType::Name) {
		return executeTrivialWildcardOrSynonymName(pkb, callee_ent);
	}
	if (caller_ent.getType() == ReferenceType::Synonym && callee_ent.getType() == ReferenceType::Synonym) {
		return executeTrivialSynonymSynonym(pkb, caller_ent, callee_ent);
	}
	return executeTrivialWildcardOrSynonymWildcardOrSynonym(pkb);
}

QP::QueryResult QP::Relationship::Calls::executeNonTrivial(PKB::StorageAccessInterface& pkb) {
	if (caller_ent.getType() == ReferenceType::Name && callee_ent.getType() == ReferenceType::Synonym) {
		return executeNameSynonym(pkb, caller_ent, callee_ent);
	}
	if (caller_ent.getType() == ReferenceType::Wildcard && callee_ent.getType() == ReferenceType::Synonym) {
		return executeWildcardSynonym(pkb, callee_ent);
	}
	if (caller_ent.getType() == ReferenceType::Synonym && callee_ent.getType() == ReferenceType::Name) {
		return executeSynonymName(pkb, caller_ent, callee_ent);
	}
	if (caller_ent.getType() == ReferenceType::Synonym && callee_ent.getType() == ReferenceType::Wildcard) {
		return executeSynonymWildcard(pkb, caller_ent);
	}
	if (caller_ent.getType() == ReferenceType::Synonym && callee_ent.getType() == ReferenceType::Synonym) {
		return executeSynonymSynonym(pkb, caller_ent, callee_ent);
	}
	return {};
}

// Trivial Executors

QP::QueryResult QP::Relationship::Calls::executeTrivialNameName(PKB::StorageAccessInterface& pkb, const ReferenceArgument& caller,
                                                                const ReferenceArgument& callee) {
	return QueryResult(pkb.checkCall(caller.getName(), callee.getName()));
}

QP::QueryResult QP::Relationship::Calls::executeTrivialNameWildcardOrSynonym(PKB::StorageAccessInterface& pkb,
                                                                             const ReferenceArgument& caller) {
	return QueryResult(!pkb.getCallee(caller.getName()).empty());
}

QP::QueryResult QP::Relationship::Calls::executeTrivialWildcardOrSynonymName(PKB::StorageAccessInterface& pkb,
                                                                             const ReferenceArgument& callee) {
	return QueryResult(!pkb.getCaller(callee.getName()).empty());
}

QP::QueryResult QP::Relationship::Calls::executeTrivialWildcardOrSynonymWildcardOrSynonym(PKB::StorageAccessInterface& pkb) {
	ProcRefSet proc_set = pkb.getProcedures();
	for (auto const& proc : proc_set) {
		ProcRefSet caller_set = pkb.getCaller(proc);
		if (!caller_set.empty()) {
			return QueryResult(true);
		}
	}

	return {};
}

QP::QueryResult QP::Relationship::Calls::executeTrivialSynonymSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& caller,
                                                                      const ReferenceArgument& callee) {
	if (caller.getSynonym().symbol == callee.getSynonym().symbol) {
		return {};
	}
	return executeTrivialWildcardOrSynonymWildcardOrSynonym(pkb);
}

// Executors

QP::QueryResult QP::Relationship::Calls::executeNameSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& caller,
                                                            const ReferenceArgument& callee) {
	QueryResult result = QueryResult();
	vector<string> column;
	ProcRefSet callees = pkb.getCallee(caller.getName());
	for (auto const& callee_reference : callees) {
		column.push_back(callee_reference);
	}

	result.addColumn(callee.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::Calls::executeWildcardSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& callee) {
	QueryResult result = QueryResult();
	vector<string> column;
	ProcRefSet procedures = pkb.getProcedures();
	for (auto const& procedure : procedures) {
		ProcRefSet callers = pkb.getCaller(procedure);
		if (!callers.empty()) {
			column.push_back(procedure);
		}
	}

	result.addColumn(callee.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::Calls::executeSynonymName(PKB::StorageAccessInterface& pkb, const ReferenceArgument& caller,
                                                            const ReferenceArgument& callee) {
	QueryResult result = QueryResult();
	vector<string> caller_column;
	ProcRefSet callers = pkb.getCaller(callee.getName());
	for (auto const& caller_reference : callers) {
		caller_column.push_back(caller_reference);
	}
	result.addColumn(caller.getSynonym().symbol, caller_column);
	return result;
}
QP::QueryResult QP::Relationship::Calls::executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& caller) {
	QueryResult result = QueryResult();
	vector<string> caller_column;
	ProcRefSet procedures = pkb.getProcedures();
	for (auto const& procedure : procedures) {
		ProcRefSet callees = pkb.getCallee(procedure);
		if (!callees.empty()) {
			caller_column.push_back(procedure);
		}
	}
	result.addColumn(caller.getSynonym().symbol, caller_column);
	return result;
}

QP::QueryResult QP::Relationship::Calls::executeSynonymSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& caller,
                                                               const ReferenceArgument& callee) {
	if (caller.getSynonym().symbol == callee.getSynonym().symbol) {
		return {};
	}

	QueryResult result = QueryResult();
	vector<string> caller_column;
	vector<string> callee_column;

	ProcRefSet procedures = pkb.getProcedures();
	for (auto const& procedure : procedures) {
		ProcRefSet callees = pkb.getCallee(procedure);
		for (auto const& callee_reference : callees) {
			caller_column.push_back(procedure);
			callee_column.push_back(callee_reference);
		}
	}

	result.addColumn(caller.getSynonym().symbol, caller_column);
	result.addColumn(callee.getSynonym().symbol, callee_column);
	return result;
}

QP::Types::ArgumentDispatcher QP::Relationship::Calls::dispatcher = [](vector<ReferenceArgument> args) {
	return CallDispatcherTemplate<Calls>::argumentDispatcher(Types::ClauseType::Call, std::move(args));
};