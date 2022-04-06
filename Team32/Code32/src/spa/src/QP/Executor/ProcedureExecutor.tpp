#ifndef SPA_SRC_QP_EXECUTOR_PROCEDUREEXECUTOR_TPP
#define SPA_SRC_QP_EXECUTOR_PROCEDUREEXECUTOR_TPP

#include "QP/Executor/ProcedureExecutor.h"

namespace QP::Executor::ProcedureExecutor {

template <ClauseType T>
QueryResult executeTrivialNameName(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs) {
	return QueryResult(store.checkProcedureRelation<T>(lhs.getName(), rhs.getName()));
}

template <ClauseType T>
QueryResult executeTrivialNameWildcardOrSynonym(const StorageAdapter& store, const ReferenceArgument& lhs) {
	return QueryResult(!store.getReverseProcedures<T>(lhs.getName()).empty());
}

template <ClauseType T>
QueryResult executeTrivialWildcardOrSynonymName(const StorageAdapter& store, const ReferenceArgument& rhs) {
	return QueryResult(!store.template getForwardProcedures<T>(rhs.getName()).empty());
}

template <ClauseType T>
QueryResult executeTrivialWildcardOrSynonymWildcardOrSynonym(const StorageAdapter& store) {
	ProcRefSet proc_set = store.getProcedures();
	for (auto const& proc : proc_set) {
		ProcRefSet lhs_set = store.getForwardProcedures<T>(proc);
		if (!lhs_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <ClauseType T>
QueryResult executeTrivialSynonymSynonym(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs) {
	if (lhs.getSynonymSymbol() == rhs.getSynonymSymbol()) {
		return {};
	}
	return executeTrivialWildcardOrSynonymWildcardOrSynonym<T>(store);
}

// Executors
template <ClauseType T>
QueryResult executeNameSynonym(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs) {
	QueryResult result = QueryResult({rhs.getSynonymSymbol()});
	ProcRefSet rhs_set = store.getReverseProcedures<T>(lhs.getName());
	for (auto const& rhs_reference : rhs_set) {
		result.addRow({rhs_reference});
	}

	return result;
}

template <ClauseType T>
QueryResult executeWildcardSynonym(const StorageAdapter& store, const ReferenceArgument& rhs) {
	QueryResult result = QueryResult({rhs.getSynonymSymbol()});
	ProcRefSet procedures = store.getProcedures();
	for (auto const& procedure : procedures) {
		ProcRefSet lhss = store.getForwardProcedures<T>(procedure);
		if (!lhss.empty()) {
			result.addRow({procedure});
		}
	}

	return result;
}

template <ClauseType T>
QueryResult executeSynonymName(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs) {
	QueryResult result = QueryResult({lhs.getSynonymSymbol()});
	ProcRefSet lhs_set = store.getForwardProcedures<T>(rhs.getName());
	for (auto const& lhs_reference : lhs_set) {
		result.addRow({lhs_reference});
	}
	return result;
}

template <ClauseType T>
QueryResult executeSynonymWildcard(const StorageAdapter& store, const ReferenceArgument& lhs) {
	QueryResult result = QueryResult({lhs.getSynonymSymbol()});
	ProcRefSet procedures = store.getProcedures();
	for (auto const& procedure : procedures) {
		ProcRefSet rhs_set = store.getReverseProcedures<T>(procedure);
		if (!rhs_set.empty()) {
			result.addRow({procedure});
		}
	}
	return result;
}

template <ClauseType T>
QueryResult executeSynonymSynonym(const StorageAdapter& store, const ReferenceArgument& lhs, const ReferenceArgument& rhs) {
	if (lhs.getSynonymSymbol() == rhs.getSynonymSymbol()) {
		return {};
	}

	QueryResult result = QueryResult({lhs.getSynonymSymbol(), rhs.getSynonymSymbol()});

	ProcRefSet procedures = store.getProcedures();
	for (auto const& procedure : procedures) {
		ProcRefSet rhs_set = store.getReverseProcedures<T>(procedure);
		for (auto const& rhs_reference : rhs_set) {
			result.addRow({procedure, rhs_reference});
		}
	}
	return result;
}

// Executor Set Factories

template <ClauseType T>
ExecutorSet executorFactoryNameName(const vector<ReferenceArgument>& args) {
	return [caller = args.at(0), callee = args.at(1)](const StorageAdapter& pkb) { return executeTrivialNameName<T>(pkb, caller, callee); };
}

template <ClauseType T>
ExecutorSet executorFactoryNameWildcard(const vector<ReferenceArgument>& args) {
	return [caller = args.at(0)](const StorageAdapter& pkb) { return executeTrivialNameWildcardOrSynonym<T>(pkb, caller); };
}

template <ClauseType T>
ExecutorSet executorFactoryNameSynonym(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [caller = args.at(0)](const StorageAdapter& pkb) {
		return executeTrivialNameWildcardOrSynonym<T>(pkb, caller);
	};
	Types::Executor executor = [caller = args.at(0), callee = args.at(1)](const StorageAdapter& pkb) {
		return executeNameSynonym<T>(pkb, caller, callee);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactoryWildcardName(const vector<ReferenceArgument>& args) {
	return [callee = args.at(1)](const StorageAdapter& pkb) { return executeTrivialWildcardOrSynonymName<T>(pkb, callee); };
}

template <ClauseType T>
ExecutorSet executorFactoryWildcardWildcard(const vector<ReferenceArgument>& /*args*/) {
	return [](const StorageAdapter& pkb) { return executeTrivialWildcardOrSynonymWildcardOrSynonym<T>(pkb); };
}

template <ClauseType T>
ExecutorSet executorFactoryWildcardSynonym(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [](const StorageAdapter& pkb) { return executeTrivialWildcardOrSynonymWildcardOrSynonym<T>(pkb); };
	Types::Executor executor = [caller = args.at(0), callee = args.at(1)](const StorageAdapter& pkb) {
		return executeWildcardSynonym<T>(pkb, callee);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymName(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [callee = args.at(1)](const StorageAdapter& pkb) {
		return executeTrivialWildcardOrSynonymName<T>(pkb, callee);
	};
	Types::Executor executor = [caller = args.at(0), callee = args.at(1)](const StorageAdapter& pkb) {
		return executeSynonymName<T>(pkb, caller, callee);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymWildcard(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [](const StorageAdapter& pkb) { return executeTrivialWildcardOrSynonymWildcardOrSynonym<T>(pkb); };
	Types::Executor executor = [caller = args.at(0)](const StorageAdapter& pkb) { return executeSynonymWildcard<T>(pkb, caller); };
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymSynonym(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [caller = args.at(0), callee = args.at(1)](const StorageAdapter& pkb) {
		return executeTrivialSynonymSynonym<T>(pkb, caller, callee);
	};
	Types::Executor executor = [caller = args.at(0), callee = args.at(1)](const StorageAdapter& pkb) {
		return executeSynonymSynonym<T>(pkb, caller, callee);
	};
	return pair{trivial_executor, executor};
}

}

#endif  // SPA_SRC_QP_EXECUTOR_PROCEDUREEXECUTOR_TPP