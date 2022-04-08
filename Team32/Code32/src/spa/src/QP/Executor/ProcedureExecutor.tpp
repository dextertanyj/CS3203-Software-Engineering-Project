#ifndef SPA_SRC_QP_EXECUTOR_PROCEDUREEXECUTOR_TPP
#define SPA_SRC_QP_EXECUTOR_PROCEDUREEXECUTOR_TPP

#include "QP/Executor/ProcedureExecutor.h"

#include <utility>

#include "Common/TypeDefs.h"

namespace QP::Executor::ProcedureExecutor {

template <ClauseType T>
QueryResult executeTrivialNameName(const StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs) {
	bool result = store.checkProcedureRelation<T>(lhs.getName(), rhs.getName());
	return QueryResult(result);
}

template <ClauseType T>
QueryResult executeTrivialNameWildcardOrSynonym(const StorageAdapter& store, const ClauseArgument& lhs) {
	unordered_set result = store.getReverseProcedures<T>(lhs.getName());
	return QueryResult(!result.empty());
}

template <ClauseType T>
QueryResult executeTrivialWildcardOrSynonymName(const StorageAdapter& store, const ClauseArgument& rhs) {
	unordered_set result = store.template getForwardProcedures<T>(rhs.getName());
	return QueryResult(!result.empty());
}

template <ClauseType T>
QueryResult executeTrivialWildcardOrSynonymWildcardOrSynonym(const StorageAdapter& store) {
	ProcRefSet proc_set = store.getProcedures();
	for (const auto& proc : proc_set) {
		ProcRefSet lhs_set = store.getForwardProcedures<T>(proc);
		if (!lhs_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <ClauseType T>
QueryResult executeTrivialSynonymSynonym(const StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs) {
	if (lhs.getSynonymSymbol() == rhs.getSynonymSymbol()) {
		return {};
	}
	return executeTrivialWildcardOrSynonymWildcardOrSynonym<T>(store);
}

// Executors
template <ClauseType T>
QueryResult executeNameSynonym(const StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs) {
	QueryResult result = QueryResult({rhs.getSynonymSymbol()});
	ProcRefSet rhs_set = store.getReverseProcedures<T>(lhs.getName());
	for (const auto& rhs_reference : rhs_set) {
		result.addRow({rhs_reference});
	}

	return result;
}

template <ClauseType T>
QueryResult executeWildcardSynonym(const StorageAdapter& store, const ClauseArgument& rhs) {
	QueryResult result = QueryResult({rhs.getSynonymSymbol()});
	ProcRefSet procedures = store.getProcedures();
	for (const auto& procedure : procedures) {
		ProcRefSet lhss = store.getForwardProcedures<T>(procedure);
		if (!lhss.empty()) {
			result.addRow({procedure});
		}
	}

	return result;
}

template <ClauseType T>
QueryResult executeSynonymName(const StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs) {
	QueryResult result = QueryResult({lhs.getSynonymSymbol()});
	ProcRefSet lhs_set = store.getForwardProcedures<T>(rhs.getName());
	for (const auto& lhs_reference : lhs_set) {
		result.addRow({lhs_reference});
	}
	return result;
}

template <ClauseType T>
QueryResult executeSynonymWildcard(const StorageAdapter& store, const ClauseArgument& lhs) {
	QueryResult result = QueryResult({lhs.getSynonymSymbol()});
	ProcRefSet procedures = store.getProcedures();
	for (const auto& procedure : procedures) {
		ProcRefSet rhs_set = store.getReverseProcedures<T>(procedure);
		if (!rhs_set.empty()) {
			result.addRow({procedure});
		}
	}
	return result;
}

template <ClauseType T>
QueryResult executeSynonymSynonym(const StorageAdapter& store, const ClauseArgument& lhs, const ClauseArgument& rhs) {
	if (lhs.getSynonymSymbol() == rhs.getSynonymSymbol()) {
		return {};
	}

	QueryResult result = QueryResult({lhs.getSynonymSymbol(), rhs.getSynonymSymbol()});

	ProcRefSet procedures = store.getProcedures();
	for (const auto& procedure : procedures) {
		ProcRefSet rhs_set = store.getReverseProcedures<T>(procedure);
		for (const auto& rhs_reference : rhs_set) {
			result.addRow({procedure, rhs_reference});
		}
	}
	return result;
}

// Executor Set Factories

template <ClauseType T>
ExecutorSet executorFactoryNameName(const vector<ClauseArgument>& args) {
	return [caller = args[0], callee = args[1]](const StorageAdapter& pkb) { return executeTrivialNameName<T>(pkb, caller, callee); };
}

template <ClauseType T>
ExecutorSet executorFactoryNameWildcard(const vector<ClauseArgument>& args) {
	return [caller = args[0]](const StorageAdapter& pkb) { return executeTrivialNameWildcardOrSynonym<T>(pkb, caller); };
}

template <ClauseType T>
ExecutorSet executorFactoryNameSynonym(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [caller = args[0]](const StorageAdapter& pkb) {
		return executeTrivialNameWildcardOrSynonym<T>(pkb, caller);
	};
	Types::Executor executor = [caller = args[0], callee = args[1]](const StorageAdapter& pkb) {
		return executeNameSynonym<T>(pkb, caller, callee);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactoryWildcardName(const vector<ClauseArgument>& args) {
	return [callee = args[1]](const StorageAdapter& pkb) { return executeTrivialWildcardOrSynonymName<T>(pkb, callee); };
}

template <ClauseType T>
ExecutorSet executorFactoryWildcardWildcard(const vector<ClauseArgument>& /*args*/) {
	return [](const StorageAdapter& pkb) { return executeTrivialWildcardOrSynonymWildcardOrSynonym<T>(pkb); };
}

template <ClauseType T>
ExecutorSet executorFactoryWildcardSynonym(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [](const StorageAdapter& pkb) { return executeTrivialWildcardOrSynonymWildcardOrSynonym<T>(pkb); };
	Types::Executor executor = [caller = args[0], callee = args[1]](const StorageAdapter& pkb) {
		return executeWildcardSynonym<T>(pkb, callee);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymName(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [callee = args[1]](const StorageAdapter& pkb) {
		return executeTrivialWildcardOrSynonymName<T>(pkb, callee);
	};
	Types::Executor executor = [caller = args[0], callee = args[1]](const StorageAdapter& pkb) {
		return executeSynonymName<T>(pkb, caller, callee);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymWildcard(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [](const StorageAdapter& pkb) { return executeTrivialWildcardOrSynonymWildcardOrSynonym<T>(pkb); };
	Types::Executor executor = [caller = args[0]](const StorageAdapter& pkb) { return executeSynonymWildcard<T>(pkb, caller); };
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymSynonym(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [caller = args[0], callee = args[1]](const StorageAdapter& pkb) {
		return executeTrivialSynonymSynonym<T>(pkb, caller, callee);
	};
	Types::Executor executor = [caller = args[0], callee = args[1]](const StorageAdapter& pkb) {
		return executeSynonymSynonym<T>(pkb, caller, callee);
	};
	return pair{trivial_executor, executor};
}

}

#endif  // SPA_SRC_QP_EXECUTOR_PROCEDUREEXECUTOR_TPP