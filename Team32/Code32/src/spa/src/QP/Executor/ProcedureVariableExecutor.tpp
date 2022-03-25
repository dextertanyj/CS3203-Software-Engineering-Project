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
	VarRefSet var_set = storage.getVariableByProcedure<T>(procedure.getName());
	vector<string> column;

	for (auto const &var : var_set) {
		column.push_back(var);
	}

	QueryResult result = QueryResult();
	result.addColumn(variable.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureVariableExecutor::executeSynonymName(const QP::StorageAdapter &storage,
                                                                               const Types::ReferenceArgument &procedure,
                                                                               const Types::ReferenceArgument &variable) {
	ProcRefSet proc_set = storage.getProcedureByVariable<T>(variable.getName());
	vector<string> column;
	for (auto const &proc : proc_set) {
		column.push_back(proc);
	}
	QueryResult result = QueryResult();
	result.addColumn(procedure.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureVariableExecutor::executeSynonymWildcard(const QP::StorageAdapter &storage,
                                                                                   const Types::ReferenceArgument &procedure) {
	unordered_set<ProcRef> proc_set = storage.getProcedures();
	vector<string> column;
	for (auto const &proc : proc_set) {
		VarRefSet var_set = storage.getVariableByProcedure<T>(proc);
		if (!var_set.empty()) {
			column.push_back(proc);
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(procedure.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureVariableExecutor::executeSynonymSynonym(const QP::StorageAdapter &storage,
                                                                                  const Types::ReferenceArgument &procedure,
                                                                                  const Types::ReferenceArgument &variable) {
	unordered_set<ProcRef> proc_set = storage.getProcedures();
	vector<string> proc_column;
	vector<string> var_column;
	for (auto const &proc : proc_set) {
		VarRefSet var_set = storage.getVariableByProcedure<T>(proc);
		for (auto const &var : var_set) {
			proc_column.push_back(proc);
			var_column.push_back(var);
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(procedure.getSynonym().symbol, proc_column);
	result.addColumn(variable.getSynonym().symbol, var_column);
	return result;
}

#endif  // SPA_SRC_QP_EXECUTOR_PROCEDUREVARIABLEEXECUTOR_TPP