#ifndef SPA_STATEMENTEXECUTOR_TPP
#define SPA_STATEMENTEXECUTOR_TPP

#include "QP/Executor/StatementExecutor.h"

#include "QP/QueryUtils.h"

// Trivial Executors
template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementExecutor::executeTrivialIndexIndex(const QP::StorageAdapter& storage,
                                                                          const Types::ReferenceArgument& lhs,
                                                                          const Types::ReferenceArgument& rhs) {
	return QueryResult(storage.checkStatementRelation<T>(lhs.getStatementIndex(), rhs.getStatementIndex()));
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementExecutor::executeTrivialIndexWildcard(const QP::StorageAdapter& storage,
                                                                             const Types::ReferenceArgument& lhs) {
	return QueryResult(!storage.getReverseStatements<T>(lhs.getStatementIndex()).empty());
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementExecutor::executeTrivialIndexSynonym(const QP::StorageAdapter& storage,
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
QP::QueryResult QP::Executor::StatementExecutor::executeTrivialWildcardIndex(const QP::StorageAdapter& storage,
                                                                             const Types::ReferenceArgument& rhs) {
	return QueryResult(!storage.getForwardStatements<T>(rhs.getStatementIndex()).empty());
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementExecutor::executeTrivialWildcardWildcard(const QP::StorageAdapter& storage) {
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
inline QP::QueryResult QP::Executor::StatementExecutor::executeTrivialWildcardWildcard<QP::Types::ClauseType::Parent>(
	const QP::StorageAdapter& storage) {
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
inline QP::QueryResult QP::Executor::StatementExecutor::executeTrivialWildcardWildcard<QP::Types::ClauseType::ParentT>(
	const QP::StorageAdapter& storage) {
	return QP::Executor::StatementExecutor::executeTrivialWildcardWildcard<QP::Types::ClauseType::Parent>(storage);
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementExecutor::executeTrivialWildcardSynonym(const QP::StorageAdapter& storage,
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
QP::QueryResult QP::Executor::StatementExecutor::executeTrivialSynonymIndex(const QP::StorageAdapter& storage,
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
QP::QueryResult QP::Executor::StatementExecutor::executeTrivialSynonymWildcard(const QP::StorageAdapter& storage,
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
static QP::QueryResult executeTrivialStatementSynonymSynonym(const QP::StorageAdapter& storage, const QP::Types::ReferenceArgument& lhs,
                                                             const QP::Types::ReferenceArgument& rhs) {
	StmtInfoPtrSet statements = storage.getStatements();
	for (auto const& statement : statements) {
		if (!QP::Utilities::checkStmtTypeMatch(statement, lhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet rhs_set = storage.getReverseStatements<T>(statement->getIdentifier());
		bool match = any_of(rhs_set.begin(), rhs_set.end(), [&rhs](const auto& rhs_statement) {
			return QP::Utilities::checkStmtTypeMatch(rhs_statement, rhs.getSynonym().type);
		});
		if (match) {
			return QP::QueryResult(true);
		}
	}

	return {};
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementExecutor::executeTrivialSynonymSynonym(const QP::StorageAdapter& storage,
                                                                              const Types::ReferenceArgument& lhs,
                                                                              const Types::ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol == rhs.getSynonym().symbol) {
		return {};
	}

	return executeTrivialStatementSynonymSynonym<T>(storage, lhs, rhs);
}

// Next* specialization to account for semantically correct same synonym case.
template <>
inline QP::QueryResult QP::Executor::StatementExecutor::executeTrivialSynonymSynonym<QP::Types::ClauseType::NextT>(
	const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs, const Types::ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol != rhs.getSynonym().symbol) {
		return executeTrivialStatementSynonymSynonym<QP::Types::ClauseType::NextT>(storage, lhs, rhs);
	}
	StmtInfoPtrSet statements = storage.getStatements();
	for (auto const& statement : statements) {
		if (!QP::Utilities::checkStmtTypeMatch(statement, lhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet others = storage.getReverseStatements<QP::Types::ClauseType::NextT>(statement->getIdentifier());
		if (others.find(statement) != others.end()) {
			return QueryResult(true);
		}
	}
	return {};
}

// Executors
template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementExecutor::executeIndexSynonym(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
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
QP::QueryResult QP::Executor::StatementExecutor::executeWildcardSynonym(const QP::StorageAdapter& storage,
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
QP::QueryResult QP::Executor::StatementExecutor::executeSynonymIndex(const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs,
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
QP::QueryResult QP::Executor::StatementExecutor::executeSynonymWildcard(const QP::StorageAdapter& storage,
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
static QP::QueryResult executeStatementSynonymSynonym(const QP::StorageAdapter& storage, const QP::Types::ReferenceArgument& lhs,
                                                      const QP::Types::ReferenceArgument& rhs) {
	StmtInfoPtrSet statements = storage.getStatements();
	vector<string> lhs_column;
	vector<string> rhs_column;
	for (auto const& statement : statements) {
		if (!QP::Utilities::checkStmtTypeMatch(statement, lhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet rhs_set = storage.getReverseStatements<T>(statement->getIdentifier());
		for (auto const& rhs_statement : rhs_set) {
			if (!QP::Utilities::checkStmtTypeMatch(rhs_statement, rhs.getSynonym().type)) {
				continue;
			}
			lhs_column.push_back(to_string(statement->getIdentifier()));
			rhs_column.push_back(to_string(rhs_statement->getIdentifier()));
		}
	}
	QP::QueryResult result = QP::QueryResult();
	result.addColumn(lhs.getSynonym().symbol, lhs_column);
	result.addColumn(rhs.getSynonym().symbol, rhs_column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Executor::StatementExecutor::executeSynonymSynonym(const QP::StorageAdapter& storage,
                                                                       const Types::ReferenceArgument& lhs,
                                                                       const Types::ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol == rhs.getSynonym().symbol) {
		return {};
	}
	return executeStatementSynonymSynonym<T>(storage, lhs, rhs);
}

// Next* specialization to account for semantically correct same synonym case.
template <>
inline QP::QueryResult QP::Executor::StatementExecutor::executeSynonymSynonym<QP::Types::ClauseType::NextT>(
	const QP::StorageAdapter& storage, const Types::ReferenceArgument& lhs, const Types::ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol != rhs.getSynonym().symbol) {
		return executeStatementSynonymSynonym<QP::Types::ClauseType::NextT>(storage, lhs, rhs);
	}
	StmtInfoPtrSet statements = storage.getStatements();
	vector<string> column;
	for (auto const& statement : statements) {
		if (!QP::Utilities::checkStmtTypeMatch(statement, lhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet others = storage.getReverseStatements<QP::Types::ClauseType::NextT>(statement->getIdentifier());
		if (others.find(statement) == others.end()) {
			continue;
		}
		column.push_back(to_string(statement->getIdentifier()));
	}
	QP::QueryResult result = QP::QueryResult();
	result.addColumn(lhs.getSynonym().symbol, column);
	return result;
}

#endif  // SPA_STATEMENTEXECUTOR_TPP
