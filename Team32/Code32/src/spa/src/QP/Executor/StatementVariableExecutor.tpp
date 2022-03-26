#ifndef SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_TPP
#define SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_TPP

#include "QP/Executor/StatementVariableExecutor.h"

#include "QP/QueryUtils.h"

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementVariableExecutor::executeTrivialIndexName(const QP::StorageAdapter& storage,
                                                                                    const Types::ReferenceArgument& index,
                                                                                    const Types::ReferenceArgument& variable) {
	return QueryResult(storage.checkStatementVariableRelation<T>(index.getStatementIndex(), variable.getName()));
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementVariableExecutor::executeTrivialIndexWildcardOrSynonym(const QP::StorageAdapter& storage,
                                                                                                 const Types::ReferenceArgument& index) {
	return QueryResult(!storage.getVariableByStatement<T>(index.getStatementIndex()).empty());
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementVariableExecutor::executeTrivialSynonymName(const QP::StorageAdapter& storage,
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
QP::QueryResult QP::Executor::StatementVariableExecutor::executeTrivialSynonymWildcardOrSynonym(const QP::StorageAdapter& storage,
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
QP::QueryResult QP::Executor::StatementVariableExecutor::executeSynonymName(const QP::StorageAdapter& storage,
                                                                               const Types::ReferenceArgument& index,
                                                                               const Types::ReferenceArgument& variable) {
	QueryResult result = QueryResult({index.getSynonym().symbol});
	StmtInfoPtrSet index_set = storage.getStatementByVariable<T>(variable.getName());
	vector<string> column;
	for (auto const& res_index : index_set) {
		if (Utilities::checkStmtTypeMatch(res_index, index.getSynonym().type)) {
			result.addRow({to_string(res_index->getIdentifier())});
		}
	}
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementVariableExecutor::executeSynonymWildcard(const QP::StorageAdapter& storage,
                                                                                   const Types::ReferenceArgument& index) {
	QueryResult result = QueryResult({index.getSynonym().symbol});
	StmtInfoPtrSet index_set = storage.getStatements();
	for (auto const& res_index : index_set) {
		if (!Utilities::checkStmtTypeMatch(res_index, index.getSynonym().type)) {
			continue;
		}

		VarRefSet var_set = storage.getVariableByStatement<T>(res_index->getIdentifier());
		if (!var_set.empty()) {
			result.addRow({to_string(res_index->getIdentifier())});
		}
	}
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementVariableExecutor::executeSynonymSynonym(const QP::StorageAdapter& storage,
                                                                                  const Types::ReferenceArgument& index,
                                                                                  const Types::ReferenceArgument& variable) {
	QueryResult result = QueryResult({index.getSynonym().symbol, variable.getSynonym().symbol});
	StmtInfoPtrSet index_set = storage.getStatements();
	Types::DesignEntity design_entity = index.getSynonym().type;
	for (auto const& res_index : index_set) {
		if (!Utilities::checkStmtTypeMatch(res_index, design_entity)) {
			continue;
		}

		VarRefSet var_set = storage.getVariableByStatement<T>(res_index->getIdentifier());
		for (auto const& var : var_set) {
			result.addRow({to_string(res_index->getIdentifier()), var});
		}
	}
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementVariableExecutor::executeIndexSynonym(const QP::StorageAdapter& storage,
                                                                                const Types::ReferenceArgument& index,
                                                                                const Types::ReferenceArgument& variable) {
	QueryResult result = QueryResult({variable.getSynonym().symbol});
	VarRefSet var_set = storage.getVariableByStatement<T>(index.getStatementIndex());

	for (auto const& var : var_set) {
		result.addRow({var});
	}

	return result;
}

#endif  // SPA_SRC_QP_EXECUTOR_STATEMENTVARIABLEEXECUTOR_TPP