#ifndef SPA_STATEMENTEXECUTOR_TPP
#define SPA_STATEMENTEXECUTOR_TPP

#include "QP/Executor/StatementExecutor.h"

#include "QP/QueryUtils.h"

// Trivial Executors
template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementExecutor<T>::executeTrivialIndexIndex(QP::StorageAdapter& storage,
                                                                                     const Types::ReferenceArgument& lhs,
                                                                                     const Types::ReferenceArgument& rhs) {
	return QueryResult(storage.checkStatementRelation<T>(lhs.getStatementIndex(), rhs.getStatementIndex()));
}

// TODO: Deduplicate the following cases
template <>
inline QP::QueryResult QP::Executor::StatementExecutor<QP::Types::ClauseType::ParentT>::executeTrivialIndexIndex(
	QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs, const Types::ReferenceArgument& rhs) {
	StmtInfoPtrSet rhs_set = storage.getReverseStatements<QP::Types::ClauseType::ParentT>(lhs.getStatementIndex());
	StmtRef rhs_index = rhs.getStatementIndex();
	for (auto const& rhs_statement : rhs_set) {
		if (rhs_statement->getIdentifier() == rhs_index) {
			return QueryResult(true);
		}
	}
	return {};
}

template <>
inline QP::QueryResult QP::Executor::StatementExecutor<QP::Types::ClauseType::FollowsT>::executeTrivialIndexIndex(
	QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs, const Types::ReferenceArgument& rhs) {
	StmtInfoPtrSet rhs_set = storage.getReverseStatements<QP::Types::ClauseType::FollowsT>(lhs.getStatementIndex());
	StmtRef rhs_index = rhs.getStatementIndex();
	for (auto const& rhs_statement : rhs_set) {
		if (rhs_statement->getIdentifier() == rhs_index) {
			return QueryResult(true);
		}
	}
	return {};
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementExecutor<T>::executeTrivialIndexWildcard(QP::StorageAdapter& storage,
                                                                                        const Types::ReferenceArgument& lhs) {
	return QueryResult(!storage.getReverseStatements<T>(lhs.getStatementIndex()).empty());
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementExecutor<T>::executeTrivialIndexSynonym(QP::StorageAdapter& storage,
                                                                                       const Types::ReferenceArgument& lhs,
                                                                                       const Types::ReferenceArgument& rhs) {
	StmtInfoPtrSet rhs_set = storage.getReverseStatements<T>(lhs.getStatementIndex());
	if (rhs.getSynonym().type == Types::DesignEntity::Stmt) {
		return QueryResult(!rhs_set.empty());
	}

	for (auto const& rhs_statement : rhs_set) {
		if (Utilities::checkStmtTypeMatch(rhs_statement, rhs.getSynonym().type)) {
			return QueryResult(true);
		}
	}
	return {};
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementExecutor<T>::executeTrivialWildcardIndex(QP::StorageAdapter& storage,
                                                                                        const Types::ReferenceArgument& rhs) {
	return QueryResult(!storage.getForwardStatements<T>(rhs.getStatementIndex()).empty());
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementExecutor<T>::executeTrivialWildcardWildcard(QP::StorageAdapter& storage) {
	StmtInfoPtrSet statements = storage.getStatements();
	for (auto const& statement : statements) {
		StmtInfoPtrSet rhs_set = storage.getReverseStatements<T>(statement->getIdentifier());
		if (!rhs_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <>
inline QP::QueryResult QP::Executor::StatementExecutor<QP::Types::ClauseType::Parent>::executeTrivialWildcardWildcard(
	QP::StorageAdapter& storage) {
	StmtInfoPtrSet statements = storage.getStatements();
	for (auto const& statement : statements) {
		// Avoid call to PKB unless statement can contain others.
		if (statement->getType() != StmtType::IfStmt && statement->getType() != StmtType::WhileStmt) {
			continue;
		}
		StmtInfoPtrSet rhs_set = storage.getReverseStatements<QP::Types::ClauseType::Parent>(statement->getIdentifier());
		if (!rhs_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <>
inline QP::QueryResult QP::Executor::StatementExecutor<QP::Types::ClauseType::ParentT>::executeTrivialWildcardWildcard(
	QP::StorageAdapter& storage) {
	return QP::Executor::StatementExecutor<QP::Types::ClauseType::Parent>::executeTrivialWildcardWildcard(storage);
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementExecutor<T>::executeTrivialWildcardSynonym(QP::StorageAdapter& storage,
                                                                                          const Types::ReferenceArgument& rhs) {
	StmtInfoPtrSet statements = storage.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, rhs.getSynonym().type)) {
			continue;
		}
		StmtInfoPtrSet lhs_set = storage.getForwardStatements<T>(statement->getIdentifier());
		if (!lhs_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementExecutor<T>::executeTrivialSynonymIndex(QP::StorageAdapter& storage,
                                                                                       const Types::ReferenceArgument& lhs,
                                                                                       const Types::ReferenceArgument& rhs) {
	StmtInfoPtrSet lhs_set = storage.getForwardStatements<T>(rhs.getStatementIndex());
	for (auto const& lhs_statement : lhs_set) {
		if (Utilities::checkStmtTypeMatch(lhs_statement, lhs.getSynonym().type)) {
			return QueryResult(true);
		}
	}
	return {};
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementExecutor<T>::executeTrivialSynonymWildcard(QP::StorageAdapter& storage,
                                                                                          const Types::ReferenceArgument& lhs) {
	StmtInfoPtrSet statements = storage.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, lhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet rhs_set = storage.getReverseStatements<T>(statement->getIdentifier());
		if (!rhs_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementExecutor<T>::executeTrivialSynonymSynonym(QP::StorageAdapter& storage,
                                                                                         const Types::ReferenceArgument& lhs,
                                                                                         const Types::ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol == rhs.getSynonym().symbol) {
		return {};
	}

	StmtInfoPtrSet statements = storage.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, lhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet rhs_set = storage.getReverseStatements<T>(statement->getIdentifier());
		for (auto const& rhs_statement : rhs_set) {
			if (Utilities::checkStmtTypeMatch(rhs_statement, rhs.getSynonym().type)) {
				return QueryResult(true);
			}
		}
	}

	return {};
}

// Executors
template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementExecutor<T>::executeIndexSynonym(QP::StorageAdapter& storage,
                                                                                const Types::ReferenceArgument& lhs,
                                                                                const Types::ReferenceArgument& rhs) {
	StmtInfoPtrSet statements = storage.getReverseStatements<T>(lhs.getStatementIndex());
	vector<string> column;

	for (auto const& statement : statements) {
		if (Utilities::checkStmtTypeMatch(statement, rhs.getSynonym().type)) {
			column.push_back(to_string(statement->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(rhs.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementExecutor<T>::executeWildcardSynonym(QP::StorageAdapter& storage,
                                                                                   const Types::ReferenceArgument& rhs) {
	StmtInfoPtrSet statements = storage.getStatements();
	vector<string> column;
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, rhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet lhs_set = storage.getForwardStatements<T>(statement->getIdentifier());
		if (!lhs_set.empty()) {
			column.push_back(to_string(statement->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(rhs.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementExecutor<T>::executeSynonymIndex(QP::StorageAdapter& storage,
                                                                                const Types::ReferenceArgument& lhs,
                                                                                const Types::ReferenceArgument& rhs) {
	StmtInfoPtrSet lhs_set = storage.getForwardStatements<T>(rhs.getStatementIndex());
	vector<string> column;
	for (auto const& lhs_statement : lhs_set) {
		if (Utilities::checkStmtTypeMatch(lhs_statement, lhs.getSynonym().type)) {
			column.push_back(to_string(lhs_statement->getIdentifier()));
		}
	}

	QueryResult result = QueryResult();
	result.addColumn(lhs.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementExecutor<T>::executeSynonymWildcard(QP::StorageAdapter& storage,
                                                                                   const Types::ReferenceArgument& lhs) {
	StmtInfoPtrSet statements = storage.getStatements();
	vector<string> column;
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, lhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet rhs_set = storage.getReverseStatements<T>(statement->getIdentifier());
		if (!rhs_set.empty()) {
			column.push_back(to_string(statement->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(lhs.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementExecutor<T>::executeSynonymSynonym(QP::StorageAdapter& storage,
                                                                                  const Types::ReferenceArgument& lhs,
                                                                                  const Types::ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol == rhs.getSynonym().symbol) {
		return {};
	}

	StmtInfoPtrSet statements = storage.getStatements();
	vector<string> lhs_column;
	vector<string> rhs_column;
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, lhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet rhs_set = storage.getReverseStatements<T>(statement->getIdentifier());
		for (auto const& rhs_statement : rhs_set) {
			if (Utilities::checkStmtTypeMatch(rhs_statement, rhs.getSynonym().type)) {
				lhs_column.push_back(to_string(statement->getIdentifier()));
				rhs_column.push_back(to_string(rhs_statement->getIdentifier()));
			}
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(lhs.getSynonym().symbol, lhs_column);
	result.addColumn(rhs.getSynonym().symbol, rhs_column);
	return result;
}

#endif