#include "QP/Relationship/Calls.h"

#include <utility>
#include <vector>

#include "QP/Relationship/CallDispatcherTemplate.tpp"

// Trivial Executors
QP::QueryResult QP::Relationship::Calls::executeTrivialNameName(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& caller,
                                                                const Types::ReferenceArgument& callee) {
	return QueryResult(pkb.checkCall(caller.getName(), callee.getName()));
}

QP::QueryResult QP::Relationship::Calls::executeTrivialNameWildcardOrSynonym(PKB::StorageAccessInterface& pkb,
                                                                             const Types::ReferenceArgument& caller) {
	return QueryResult(!pkb.getCallee(caller.getName()).empty());
}

QP::QueryResult QP::Relationship::Calls::executeTrivialWildcardOrSynonymName(PKB::StorageAccessInterface& pkb,
                                                                             const Types::ReferenceArgument& callee) {
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

QP::QueryResult QP::Relationship::Calls::executeTrivialSynonymSynonym(PKB::StorageAccessInterface& pkb,
                                                                      const Types::ReferenceArgument& caller,
                                                                      const Types::ReferenceArgument& callee) {
	if (caller.getSynonym().symbol == callee.getSynonym().symbol) {
		return {};
	}
	return executeTrivialWildcardOrSynonymWildcardOrSynonym(pkb);
}

// Executors
QP::QueryResult QP::Relationship::Calls::executeNameSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& caller,
                                                            const Types::ReferenceArgument& callee) {
	QueryResult result = QueryResult();
	vector<string> column;
	ProcRefSet callees = pkb.getCallee(caller.getName());
	for (auto const& callee_reference : callees) {
		column.push_back(callee_reference);
	}

	result.addColumn(callee.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::Calls::executeWildcardSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& callee) {
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

QP::QueryResult QP::Relationship::Calls::executeSynonymName(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& caller,
                                                            const Types::ReferenceArgument& callee) {
	QueryResult result = QueryResult();
	vector<string> caller_column;
	ProcRefSet callers = pkb.getCaller(callee.getName());
	for (auto const& caller_reference : callers) {
		caller_column.push_back(caller_reference);
	}
	result.addColumn(caller.getSynonym().symbol, caller_column);
	return result;
}
QP::QueryResult QP::Relationship::Calls::executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& caller) {
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

QP::QueryResult QP::Relationship::Calls::executeSynonymSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& caller,
                                                               const Types::ReferenceArgument& callee) {
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

QP::Types::ArgumentDispatcher QP::Relationship::Calls::dispatcher = [](vector<Types::ReferenceArgument> args) {
	return CallDispatcherTemplate<Calls>::argumentDispatcher(Types::ClauseType::Call, move(args));
};