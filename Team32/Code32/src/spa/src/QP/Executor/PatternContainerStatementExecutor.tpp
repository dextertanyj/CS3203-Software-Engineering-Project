#ifndef SPA_SRC_QP_EXECUTOR_PATTERNCONTAINERSTATEMENTEXECUTOR_TPP
#define SPA_SRC_QP_EXECUTOR_PATTERNCONTAINERSTATEMENTEXECUTOR_TPP

#include "QP/Executor/PatternContainerStatementExecutor.h"

#include "QP/QueryUtils.h"

// Trivial Executors
template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::PatternContainerStatementExecutor<T>::executeTrivialName(const QP::StorageAdapter& storage,
                                                                                       const QP::Types::ReferenceArgument& var) {
	
	return QP::QueryResult(!storage.getControlStmt<T>(var.getName()).empty());
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::PatternContainerStatementExecutor<T>::executeTrivialWildcardOrSynonym(const QP::StorageAdapter& storage) {
	VarRefSet var_set = storage.getVariables();
	for (auto const& var : var_set) {
		StmtInfoPtrSet stmt_set = storage.getControlStmt<T>(var);
		if (!stmt_set.empty()) {
			return QueryResult(true);
		}
	}
	
	return {};
}

// Executors
template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::PatternContainerStatementExecutor<T>::executeName(const QP::StorageAdapter& storage,
                                                                                const Types::ReferenceArgument& stmt,
                                                                                const Types::ReferenceArgument& var) {
	QueryResult result = QueryResult();
	vector<string> column;
	StmtInfoPtrSet stmt_set = storage.getControlStmt<T>(var.getName());
	for (auto const& stmt_ref : stmt_set) {
		column.push_back(to_string(stmt_ref->getIdentifier()));
	}

	result.addColumn(stmt.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::PatternContainerStatementExecutor<T>::executeWildcard(const QP::StorageAdapter& storage,
                                                                                    const Types::ReferenceArgument& stmt) {
	QueryResult result = QueryResult();
	vector<string> column;
	StmtInfoPtrSet stmt_set = storage.getStatements();
	for (auto const& stmt_ref : stmt_set) {
		if (!Utilities::checkStmtTypeMatch(stmt_ref, stmt.getSynonym().type)) {
			continue;
		}
		
		VarRefSet var_set = storage.getControlVar<T>(stmt_ref->getIdentifier());
		if (!var_set.empty()) {
			column.push_back(to_string(stmt_ref->getIdentifier()));
		}
	}

	result.addColumn(stmt.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::PatternContainerStatementExecutor<T>::executeSynonym(const QP::StorageAdapter& storage,
                                                                                   const Types::ReferenceArgument& stmt,
                                                                                   const Types::ReferenceArgument& var) {
	QueryResult result = QueryResult();
	vector<string> stmt_column;
	vector<string> var_column;
	VarRefSet var_set = storage.getVariables();
	for (auto const& var_ref : var_set) {
		StmtInfoPtrSet stmt_set = storage.getControlStmt<T>(var_ref);
		for (auto const& stmt_ref : stmt_set) {
			var_column.push_back(var_ref);
			stmt_column.push_back(to_string(stmt_ref->getIdentifier()));
		}
	}
	result.addColumn(stmt.getSynonym().symbol, stmt_column);
	result.addColumn(var.getSynonym().symbol, var_column);
	return result;
}

#endif  // SPA_SRC_QP_EXECUTOR_PATTERNCONTAINERSTATEMENTEXECUTOR_TPP
