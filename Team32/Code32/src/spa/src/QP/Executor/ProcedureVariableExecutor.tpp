#ifndef SPA_SRC_QP_RELATIONSHIP_PROCEDUREVARIABLEEXECUTOR_TPP
#define SPA_SRC_QP_RELATIONSHIP_PROCEDUREVARIABLEEXECUTOR_TPP

#include "QP/Executor/ProcedureVariableExecutor.h"

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureVariableExecutor<T>::executeTrivialNameName(QP::StorageAdapter &storage,
                                                                                   const Types::ReferenceArgument &left_ent,
                                                                                   const Types::ReferenceArgument &right_ent) {
	return QueryResult(storage.checkProcedureVariableRelation<T>(left_ent.getName(), right_ent.getName()));
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureVariableExecutor<T>::executeTrivialNameWildcardOrSynonym(QP::StorageAdapter &storage,
                                                                                                const Types::ReferenceArgument &left_ent) {
	return QueryResult(!storage.getVariableByProcedure<T>(left_ent.getName()).empty());
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureVariableExecutor<T>::executeTrivialSynonymName(QP::StorageAdapter &storage,
                                                                                      const Types::ReferenceArgument &right_ent) {
	return QueryResult(!storage.getProcedureByVariable<T>(right_ent.getName()).empty());
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureVariableExecutor<T>::executeTrivialSynonymWildcardOrSynonym(QP::StorageAdapter &storage) {
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
QP::QueryResult QP::Executor::ProcedureVariableExecutor<T>::executeNameSynonym(QP::StorageAdapter &storage,
                                                                               const Types::ReferenceArgument &left_ent,
                                                                               const Types::ReferenceArgument &right_ent) {
	VarRefSet var_set = storage.getVariableByProcedure<T>(left_ent.getName());
	vector<string> column;

	for (auto const &var : var_set) {
		column.push_back(var);
	}

	QueryResult result = QueryResult();
	result.addColumn(right_ent.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureVariableExecutor<T>::executeSynonymName(QP::StorageAdapter &storage,
                                                                               const Types::ReferenceArgument &left_ent,
                                                                               const Types::ReferenceArgument &right_ent) {
	ProcRefSet proc_set = storage.getProcedureByVariable<T>(right_ent.getName());
	vector<string> column;
	for (auto const &proc : proc_set) {
		column.push_back(proc);
	}
	QueryResult result = QueryResult();
	result.addColumn(left_ent.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureVariableExecutor<T>::executeSynonymWildcard(QP::StorageAdapter &storage,
                                                                                   const Types::ReferenceArgument &left_ent) {
	unordered_set<ProcRef> proc_set = storage.getProcedures();
	vector<string> column;
	for (auto const &proc : proc_set) {
		VarRefSet var_set = storage.getVariableByProcedure<T>(proc);
		if (!var_set.empty()) {
			column.push_back(proc);
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(left_ent.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::ProcedureVariableExecutor<T>::executeSynonymSynonym(QP::StorageAdapter &storage,
                                                                                  const Types::ReferenceArgument &left_ent,
                                                                                  const Types::ReferenceArgument &right_ent) {
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
	result.addColumn(left_ent.getSynonym().symbol, proc_column);
	result.addColumn(right_ent.getSynonym().symbol, var_column);
	return result;
}

#endif