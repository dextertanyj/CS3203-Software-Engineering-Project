#ifndef SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_TPP
#define SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_TPP

#include "QP/Executor/StatementVariableExecutor.h"

#include "QP/QueryUtils.h"

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementVariableExecutor<T>::executeTrivialIndexName(const QP::StorageAdapter& storage,
                                                                                    const Types::ReferenceArgument& index,
                                                                                    const Types::ReferenceArgument& variable) {
	return QueryResult(storage.checkStatementVariableRelation<T>(index.getStatementIndex(), variable.getName()));
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementVariableExecutor<T>::executeTrivialIndexWildcardOrSynonym(const QP::StorageAdapter& storage,
                                                                                                 const Types::ReferenceArgument& index) {
	return QueryResult(!storage.getVariableByStatement<T>(index.getStatementIndex()).empty());
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementVariableExecutor<T>::executeTrivialSynonymName(const QP::StorageAdapter& storage,
                                                                                      const Types::ReferenceArgument& index,
                                                                                      const Types::ReferenceArgument& variable) {
	StmtInfoPtrSet index_set = storage.getStatementByVariable<T>(variable.getName());
	for (auto const& res_index : index_set) {
		if (Utilities::checkStmtTypeMatch(res_index, index.getSynonym().type)) {
			return QueryResult(true);
		}
	}
	return {};
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementVariableExecutor<T>::executeTrivialSynonymWildcardOrSynonym(const QP::StorageAdapter& storage,
                                                                                                   const Types::ReferenceArgument& index) {
	StmtInfoPtrSet index_set = storage.getStatements();
	for (auto const& res_index : index_set) {
		if (!Utilities::checkStmtTypeMatch(res_index, index.getSynonym().type)) {
			continue;
		}

		VarRefSet var_set = storage.getVariableByStatement<T>(res_index->getIdentifier());
		if (!var_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementVariableExecutor<T>::executeSynonymName(const QP::StorageAdapter& storage,
                                                                               const Types::ReferenceArgument& index,
                                                                               const Types::ReferenceArgument& variable) {
	StmtInfoPtrSet index_set = storage.getStatementByVariable<T>(variable.getName());
	vector<string> column;
	for (auto const& res_index : index_set) {
		if (Utilities::checkStmtTypeMatch(res_index, index.getSynonym().type)) {
			column.push_back(to_string(res_index->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(index.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementVariableExecutor<T>::executeSynonymWildcard(const QP::StorageAdapter& storage,
                                                                                   const Types::ReferenceArgument& index) {
	StmtInfoPtrSet index_set = storage.getStatements();
	vector<string> column;
	for (auto const& res_index : index_set) {
		if (!Utilities::checkStmtTypeMatch(res_index, index.getSynonym().type)) {
			continue;
		}

		VarRefSet var_set = storage.getVariableByStatement<T>(res_index->getIdentifier());
		if (!var_set.empty()) {
			column.push_back(to_string(res_index->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(index.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementVariableExecutor<T>::executeSynonymSynonym(const QP::StorageAdapter& storage,
                                                                                  const Types::ReferenceArgument& index,
                                                                                  const Types::ReferenceArgument& variable) {
	StmtInfoPtrSet index_set = storage.getStatements();
	Types::DesignEntity design_variableity = index.getSynonym().type;
	vector<string> index_column;
	vector<string> var_column;
	for (auto const& res_index : index_set) {
		if (!Utilities::checkStmtTypeMatch(res_index, design_variableity)) {
			continue;
		}

		VarRefSet var_set = storage.getVariableByStatement<T>(res_index->getIdentifier());
		for (auto const& var : var_set) {
			index_column.push_back(to_string(res_index->getIdentifier()));
			var_column.push_back(var);
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(index.getSynonym().symbol, index_column);
	result.addColumn(variable.getSynonym().symbol, var_column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementVariableExecutor<T>::executeIndexSynonym(const QP::StorageAdapter& storage,
                                                                                const Types::ReferenceArgument& index,
                                                                                const Types::ReferenceArgument& variable) {
	VarRefSet var_set = storage.getVariableByStatement<T>(index.getStatementIndex());
	vector<string> column;

	for (auto const& var : var_set) {
		column.push_back(var);
	}

	QueryResult result = QueryResult();
	result.addColumn(variable.getSynonym().symbol, column);
	return result;
}

#endif  // SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_TPP