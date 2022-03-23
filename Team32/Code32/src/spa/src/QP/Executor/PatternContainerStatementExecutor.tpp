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
	QueryResult result = QueryResult({stmt.getSynonym().symbol});
	StmtInfoPtrSet stmt_set = storage.getControlStmt<T>(var.getName());
	for (auto const& stmt_ref : stmt_set) {
		result.addRow({to_string(stmt_ref->getIdentifier())});
	}

	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::PatternContainerStatementExecutor<T>::executeWildcard(const QP::StorageAdapter& storage,
                                                                                    const Types::ReferenceArgument& stmt) {
	QueryResult result = QueryResult({stmt.getSynonym().symbol});
	StmtInfoPtrSet stmt_set = storage.getStatements();
	for (auto const& stmt_ref : stmt_set) {
		if (!Utilities::checkStmtTypeMatch(stmt_ref, stmt.getSynonym().type)) {
			continue;
		}

		VarRefSet var_set = storage.getControlVar<T>(stmt_ref->getIdentifier());
		if (!var_set.empty()) {
			result.addRow({to_string(stmt_ref->getIdentifier())});
		}
	}

	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::PatternContainerStatementExecutor<T>::executeSynonym(const QP::StorageAdapter& storage,
                                                                                   const Types::ReferenceArgument& stmt,
                                                                                   const Types::ReferenceArgument& var) {
	QueryResult result = QueryResult({stmt.getSynonym().symbol, var.getSynonym().symbol});
	VarRefSet var_set = storage.getVariables();
	for (auto const& var_ref : var_set) {
		StmtInfoPtrSet stmt_set = storage.getControlStmt<T>(var_ref);
		for (auto const& stmt_ref : stmt_set) {
			result.addRow({to_string(stmt_ref->getIdentifier()), var_ref});
		}
	}
	return result;
}

#endif  // SPA_SRC_QP_EXECUTOR_PATTERNCONTAINERSTATEMENTEXECUTOR_TPP
