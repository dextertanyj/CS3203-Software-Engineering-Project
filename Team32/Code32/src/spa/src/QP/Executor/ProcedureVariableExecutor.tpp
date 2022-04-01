#ifndef SPA_SRC_QP_EXECUTOR_PROCEDUREVARIABLEEXECUTOR_TPP
#define SPA_SRC_QP_EXECUTOR_PROCEDUREVARIABLEEXECUTOR_TPP

#include "QP/Executor/ProcedureVariableExecutor.h"

namespace QP::Executor::ProcedureVariableExecutor {

template <ClauseType T>
QueryResult executeTrivialNameName(const StorageAdapter& storage, const ReferenceArgument& procedure, const ReferenceArgument& variable) {
	return QueryResult(storage.checkProcedureVariableRelation<T>(procedure.getName(), variable.getName()));
}

template <ClauseType T>
QueryResult executeTrivialNameWildcardOrSynonym(const StorageAdapter& storage, const ReferenceArgument& procedure) {
	return QueryResult(!storage.getVariableByProcedure<T>(procedure.getName()).empty());
}

template <ClauseType T>
QueryResult executeTrivialSynonymName(const StorageAdapter& storage, const ReferenceArgument& variable) {
	return QueryResult(!storage.getProcedureByVariable<T>(variable.getName()).empty());
}

template <ClauseType T>
QueryResult executeTrivialSynonymWildcardOrSynonym(const StorageAdapter& storage) {
	unordered_set<ProcRef> proc_set = storage.getProcedures();
	for (auto const& proc : proc_set) {
		VarRefSet var_set = storage.getVariableByProcedure<T>(proc);
		if (!var_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <ClauseType T>
QueryResult executeNameSynonym(const StorageAdapter& storage, const ReferenceArgument& procedure, const ReferenceArgument& variable) {
	QueryResult result = QueryResult({variable.getSynonym().symbol});
	VarRefSet var_set = storage.getVariableByProcedure<T>(procedure.getName());
	for (auto const& var : var_set) {
		result.addRow({var});
	}

	return result;
}

template <ClauseType T>
QueryResult executeSynonymName(const StorageAdapter& storage, const ReferenceArgument& procedure, const ReferenceArgument& variable) {
	QueryResult result = QueryResult({procedure.getSynonym().symbol});
	ProcRefSet proc_set = storage.getProcedureByVariable<T>(variable.getName());
	for (auto const& proc : proc_set) {
		result.addRow({proc});
	}

	return result;
}

template <ClauseType T>
QueryResult executeSynonymWildcard(const StorageAdapter& storage, const ReferenceArgument& procedure) {
	QueryResult result = QueryResult({procedure.getSynonym().symbol});
	unordered_set<ProcRef> proc_set = storage.getProcedures();
	for (auto const& proc : proc_set) {
		VarRefSet var_set = storage.getVariableByProcedure<T>(proc);
		if (!var_set.empty()) {
			result.addRow({proc});
		}
	}
	return result;
}

template <ClauseType T>
QueryResult executeSynonymSynonym(const StorageAdapter& storage, const ReferenceArgument& procedure, const ReferenceArgument& variable) {
	QueryResult result = QueryResult({procedure.getSynonym().symbol, variable.getSynonym().symbol});
	unordered_set<ProcRef> proc_set = storage.getProcedures();
	vector<string> proc_column;
	vector<string> var_column;
	for (auto const& proc : proc_set) {
		VarRefSet var_set = storage.getVariableByProcedure<T>(proc);
		for (auto const& var : var_set) {
			result.addRow({proc, var});
		}
	}
	return result;
}

template <ClauseType T>
ExecutorSet executorFactoryNameName(const vector<ReferenceArgument>& args) {
	return [procedure = args.at(0), variable = args.at(1)](const StorageAdapter& storage) {
		return executeTrivialNameName<T>(storage, procedure, variable);
	};
}

template <ClauseType T>
ExecutorSet executorFactoryNameWildcard(const vector<ReferenceArgument>& args) {
	return [procedure = args.at(0)](const StorageAdapter& storage) { return executeTrivialNameWildcardOrSynonym<T>(storage, procedure); };
}

template <ClauseType T>
ExecutorSet executorFactoryNameSynonym(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [procedure = args.at(0)](const StorageAdapter& storage) {
		return executeTrivialNameWildcardOrSynonym<T>(storage, procedure);
	};
	Types::Executor executor = [procedure = args.at(0), variable = args.at(1)](const StorageAdapter& storage) {
		return executeNameSynonym<T>(storage, procedure, variable);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymName(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [variable = args.at(1)](const StorageAdapter& storage) {
		return executeTrivialSynonymName<T>(storage, variable);
	};
	Types::Executor executor = [procedure = args.at(0), variable = args.at(1)](const StorageAdapter& storage) {
		return executeSynonymName<T>(storage, procedure, variable);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymWildcard(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [](const StorageAdapter& storage) { return executeTrivialSynonymWildcardOrSynonym<T>(storage); };
	Types::Executor executor = [procedure = args.at(0)](const StorageAdapter& storage) {
		return executeSynonymWildcard<T>(storage, procedure);
	};
	return pair{trivial_executor, executor};
}

template <ClauseType T>
ExecutorSet executorFactorySynonymSynonym(const vector<ReferenceArgument>& args) {
	Types::Executor trivial_executor = [](const StorageAdapter& storage) { return executeTrivialSynonymWildcardOrSynonym<T>(storage); };
	Types::Executor executor = [procedure = args.at(0), variable = args.at(1)](const StorageAdapter& storage) {
		return executeSynonymSynonym<T>(storage, procedure, variable);
	};
	return pair{trivial_executor, executor};
}

}

#endif  // SPA_SRC_QP_EXECUTOR_PROCEDUREVARIABLEEXECUTOR_TPP