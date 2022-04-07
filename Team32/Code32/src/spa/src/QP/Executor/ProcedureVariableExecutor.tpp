#ifndef SPA_SRC_QP_EXECUTOR_PROCEDUREVARIABLEEXECUTOR_TPP
#define SPA_SRC_QP_EXECUTOR_PROCEDUREVARIABLEEXECUTOR_TPP

#include "QP/Executor/ProcedureVariableExecutor.h"

namespace QP::Executor::ProcedureVariableExecutor {

using namespace std;

template <ClauseType T>
QueryResult executeTrivialNameName(const StorageAdapter& store, const ClauseArgument& procedure, const ClauseArgument& variable) {
	return QueryResult(store.checkProcedureVariableRelation<T>(procedure.getName(), variable.getName()));
}

template <ClauseType T>
QueryResult executeTrivialNameWildcardOrSynonym(const StorageAdapter& store, const ClauseArgument& procedure) {
	return QueryResult(!store.getVariableByProcedure<T>(procedure.getName()).empty());
}

template <ClauseType T>
QueryResult executeTrivialSynonymName(const StorageAdapter& store, const ClauseArgument& variable) {
	return QueryResult(!store.getProcedureByVariable<T>(variable.getName()).empty());
}

template <ClauseType T>
QueryResult executeTrivialSynonymWildcardOrSynonym(const StorageAdapter& store) {
	unordered_set<ProcRef> proc_set = store.getProcedures();
	for (const auto& proc : proc_set) {
		VarRefSet var_set = store.getVariableByProcedure<T>(proc);
		if (!var_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <ClauseType T>
QueryResult executeNameSynonym(const StorageAdapter& store, const ClauseArgument& procedure, const ClauseArgument& variable) {
	QueryResult result = QueryResult({variable.getSynonymSymbol()});
	VarRefSet var_set = store.getVariableByProcedure<T>(procedure.getName());
	for (const auto& var : var_set) {
		result.addRow({var});
	}

	return result;
}

template <ClauseType T>
QueryResult executeSynonymName(const StorageAdapter& store, const ClauseArgument& procedure, const ClauseArgument& variable) {
	QueryResult result = QueryResult({procedure.getSynonymSymbol()});
	ProcRefSet proc_set = store.getProcedureByVariable<T>(variable.getName());
	for (const auto& proc : proc_set) {
		result.addRow({proc});
	}

	return result;
}

template <ClauseType T>
QueryResult executeSynonymWildcard(const StorageAdapter& store, const ClauseArgument& procedure) {
	QueryResult result = QueryResult({procedure.getSynonymSymbol()});
	unordered_set<ProcRef> proc_set = store.getProcedures();
	for (const auto& proc : proc_set) {
		VarRefSet var_set = store.getVariableByProcedure<T>(proc);
		if (!var_set.empty()) {
			result.addRow({proc});
		}
	}
	return result;
}

template <ClauseType T>
QueryResult executeSynonymSynonym(const StorageAdapter& store, const ClauseArgument& procedure, const ClauseArgument& variable) {
	QueryResult result = QueryResult({procedure.getSynonymSymbol(), variable.getSynonymSymbol()});
	unordered_set<ProcRef> proc_set = store.getProcedures();
	vector<string> proc_column;
	vector<string> var_column;
	for (const auto& proc : proc_set) {
		VarRefSet var_set = store.getVariableByProcedure<T>(proc);
		for (const auto& var : var_set) {
			result.addRow({proc, var});
		}
	}
	return result;
}

template <ClauseType T>
ExecutorSet executorFactoryNameName(const vector<ClauseArgument>& args) {
	return [procedure = args.at(0), variable = args.at(1)](const StorageAdapter& store) {
		return executeTrivialNameName<T>(store, procedure, variable);
	};
}

template <ClauseType T>
ExecutorSet executorFactoryNameWildcard(const vector<ClauseArgument>& args) {
	return [procedure = args.at(0)](const StorageAdapter& store) { return executeTrivialNameWildcardOrSynonym<T>(store, procedure); };
}

template <ClauseType T>
ExecutorSet executorFactoryNameSynonym(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [procedure = args.at(0)](const StorageAdapter& store) {
		return executeTrivialNameWildcardOrSynonym<T>(store, procedure);
	};
	Types::Executor executor = [procedure = args.at(0), variable = args.at(1)](const StorageAdapter& store) {
		return executeNameSynonym<T>(store, procedure, variable);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymName(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [variable = args.at(1)](const StorageAdapter& store) {
		return executeTrivialSynonymName<T>(store, variable);
	};
	Types::Executor executor = [procedure = args.at(0), variable = args.at(1)](const StorageAdapter& store) {
		return executeSynonymName<T>(store, procedure, variable);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymWildcard(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [](const StorageAdapter& store) { return executeTrivialSynonymWildcardOrSynonym<T>(store); };
	Types::Executor executor = [procedure = args.at(0)](const StorageAdapter& store) {
		return executeSynonymWildcard<T>(store, procedure);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymSynonym(const vector<ClauseArgument>& args) {
	Types::Executor trivial_executor = [](const StorageAdapter& store) { return executeTrivialSynonymWildcardOrSynonym<T>(store); };
	Types::Executor executor = [procedure = args.at(0), variable = args.at(1)](const StorageAdapter& store) {
		return executeSynonymSynonym<T>(store, procedure, variable);
	};
	return pair{trivial_executor, executor};
}

}

#endif  // SPA_SRC_QP_EXECUTOR_PROCEDUREVARIABLEEXECUTOR_TPP