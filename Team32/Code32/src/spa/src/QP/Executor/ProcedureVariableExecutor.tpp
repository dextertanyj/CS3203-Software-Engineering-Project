#ifndef SPA_SRC_QP_EXECUTOR_PROCEDUREVARIABLEEXECUTOR_TPP
#define SPA_SRC_QP_EXECUTOR_PROCEDUREVARIABLEEXECUTOR_TPP

#include "QP/Executor/ProcedureVariableExecutor.h"

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureVariableExecutor::executeTrivialNameName(const QP::StorageAdapter &storage,
                                                                                   const Types::ReferenceArgument &procedure,
                                                                                   const Types::ReferenceArgument &variable) {
	return QueryResult(storage.checkProcedureVariableRelation<T>(procedure.getName(), variable.getName()));
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureVariableExecutor::executeTrivialNameWildcardOrSynonym(const QP::StorageAdapter &storage,
                                                                                                const Types::ReferenceArgument &procedure) {
	return QueryResult(!storage.getVariableByProcedure<T>(procedure.getName()).empty());
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureVariableExecutor::executeTrivialSynonymName(const QP::StorageAdapter &storage,
                                                                                      const Types::ReferenceArgument &variable) {
	return QueryResult(!storage.getProcedureByVariable<T>(variable.getName()).empty());
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureVariableExecutor::executeTrivialSynonymWildcardOrSynonym(const QP::StorageAdapter &storage) {
	unordered_set<ProcRef> proc_set = storage.getProcedures();
	for (auto const &proc : proc_set) {
		VarRefSet var_set = storage.getVariableByProcedure<T>(proc);
		if (!var_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureVariableExecutor::executeNameSynonym(const QP::StorageAdapter &storage,
                                                                               const Types::ReferenceArgument &procedure,
                                                                               const Types::ReferenceArgument &variable) {
	QueryResult result = QueryResult({variable.getSynonym().symbol});
	VarRefSet var_set = storage.getVariableByProcedure<T>(procedure.getName());
	for (auto const &var : var_set) {
		result.addRow({var});
	}

	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureVariableExecutor::executeSynonymName(const QP::StorageAdapter &storage,
                                                                               const Types::ReferenceArgument &procedure,
                                                                               const Types::ReferenceArgument &variable) {
	QueryResult result = QueryResult({procedure.getSynonym().symbol});
	ProcRefSet proc_set = storage.getProcedureByVariable<T>(variable.getName());
	for (auto const &proc : proc_set) {
		result.addRow({proc});
	}

	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureVariableExecutor::executeSynonymWildcard(const QP::StorageAdapter &storage,
                                                                                   const Types::ReferenceArgument &procedure) {
	QueryResult result = QueryResult({procedure.getSynonym().symbol});
	unordered_set<ProcRef> proc_set = storage.getProcedures();
	for (auto const &proc : proc_set) {
		VarRefSet var_set = storage.getVariableByProcedure<T>(proc);
		if (!var_set.empty()) {
			result.addRow({proc});
		}
	}
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureVariableExecutor::executeSynonymSynonym(const QP::StorageAdapter &storage,
                                                                                  const Types::ReferenceArgument &procedure,
                                                                                  const Types::ReferenceArgument &variable) {
	QueryResult result = QueryResult({procedure.getSynonym().symbol, variable.getSynonym().symbol});
	unordered_set<ProcRef> proc_set = storage.getProcedures();
	vector<string> proc_column;
	vector<string> var_column;
	for (auto const &proc : proc_set) {
		VarRefSet var_set = storage.getVariableByProcedure<T>(proc);
		for (auto const &var : var_set) {
			result.addRow({proc, var});
		}
	}
	return result;
}

#endif  // SPA_SRC_QP_EXECUTOR_PROCEDUREVARIABLEEXECUTOR_TPP