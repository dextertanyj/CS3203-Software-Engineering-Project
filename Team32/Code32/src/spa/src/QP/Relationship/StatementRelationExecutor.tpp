#ifndef SPA_STATEMENTRELATIONEXECUTOR_TPP
#define SPA_STATEMENTRELATIONEXECUTOR_TPP

#include "QP/Relationship/StatementRelationExecutor.h"

#include "QP/QueryUtils.h"

// Trivial Executors
template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::StatementRelationExecutor<T>::executeTrivialIndexIndex(QP::StorageAdapter& pkb,
                                                                                        const Types::ReferenceArgument& lhs,
                                                                                        const Types::ReferenceArgument& rhs) {
	return QueryResult(pkb.checkStatementRelation<T>(lhs.getStatementIndex(), rhs.getStatementIndex()));
}

// TODO: Deduplicate the following cases
template <>
inline QP::QueryResult QP::Relationship::StatementRelationExecutor<QP::Types::ClauseType::ParentT>::executeTrivialIndexIndex(
	QP::StorageAdapter& pkb, const Types::ReferenceArgument& lhs, const Types::ReferenceArgument& rhs) {
	StmtInfoPtrSet rhs_set = pkb.getReverseStatements<QP::Types::ClauseType::ParentT>(lhs.getStatementIndex());
	StmtRef rhs_index = rhs.getStatementIndex();
	for (auto const& rhs_statement : rhs_set) {
		if (rhs_statement->getIdentifier() == rhs_index) {
			return QueryResult(true);
		}
	}
	return {};
}

template <>
inline QP::QueryResult QP::Relationship::StatementRelationExecutor<QP::Types::ClauseType::FollowsT>::executeTrivialIndexIndex(
	QP::StorageAdapter& pkb, const Types::ReferenceArgument& lhs, const Types::ReferenceArgument& rhs) {
	StmtInfoPtrSet rhs_set = pkb.getReverseStatements<QP::Types::ClauseType::FollowsT>(lhs.getStatementIndex());
	StmtRef rhs_index = rhs.getStatementIndex();
	for (auto const& rhs_statement : rhs_set) {
		if (rhs_statement->getIdentifier() == rhs_index) {
			return QueryResult(true);
		}
	}
	return {};
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::StatementRelationExecutor<T>::executeTrivialIndexWildcard(QP::StorageAdapter& pkb,
                                                                                           const Types::ReferenceArgument& lhs) {
	return QueryResult(!pkb.getReverseStatements<T>(lhs.getStatementIndex()).empty());
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::StatementRelationExecutor<T>::executeTrivialIndexSynonym(QP::StorageAdapter& pkb,
                                                                                          const Types::ReferenceArgument& lhs,
                                                                                          const Types::ReferenceArgument& rhs) {
	StmtInfoPtrSet rhs_set = pkb.getReverseStatements<T>(lhs.getStatementIndex());
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
QP::QueryResult QP::Relationship::StatementRelationExecutor<T>::executeTrivialWildcardIndex(QP::StorageAdapter& pkb,
                                                                                           const Types::ReferenceArgument& rhs) {
	return QueryResult(!pkb.getForwardStatements<T>(rhs.getStatementIndex()).empty());
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::StatementRelationExecutor<T>::executeTrivialWildcardWildcard(QP::StorageAdapter& pkb) {
	StmtInfoPtrSet statements = pkb.getStatements();
	for (auto const& statement : statements) {
		StmtInfoPtrSet rhs_set = pkb.getReverseStatements<T>(statement->getIdentifier());
		if (!rhs_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <>
inline QP::QueryResult QP::Relationship::StatementRelationExecutor<QP::Types::ClauseType::Parent>::executeTrivialWildcardWildcard(
	QP::StorageAdapter& pkb) {
	StmtInfoPtrSet statements = pkb.getStatements();
	for (auto const& statement : statements) {
		// Avoid call to PKB unless statement can contain others.
		if (statement->getType() != StmtType::IfStmt && statement->getType() != StmtType::WhileStmt) {
			continue;
		}
		StmtInfoPtrSet rhs_set = pkb.getReverseStatements<QP::Types::ClauseType::Parent>(statement->getIdentifier());
		if (!rhs_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <>
inline QP::QueryResult QP::Relationship::StatementRelationExecutor<QP::Types::ClauseType::ParentT>::executeTrivialWildcardWildcard(
	QP::StorageAdapter& pkb) {
	return QP::Relationship::StatementRelationExecutor<QP::Types::ClauseType::Parent>::executeTrivialWildcardWildcard(pkb);
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::StatementRelationExecutor<T>::executeTrivialWildcardSynonym(QP::StorageAdapter& pkb,
                                                                                             const Types::ReferenceArgument& rhs) {
	StmtInfoPtrSet statements = pkb.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, rhs.getSynonym().type)) {
			continue;
		}
		StmtInfoPtrSet lhs_set = pkb.getForwardStatements<T>(statement->getIdentifier());
		if (!lhs_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::StatementRelationExecutor<T>::executeTrivialSynonymIndex(QP::StorageAdapter& pkb,
                                                                                          const Types::ReferenceArgument& lhs,
                                                                                          const Types::ReferenceArgument& rhs) {
	StmtInfoPtrSet lhs_set = pkb.getForwardStatements<T>(rhs.getStatementIndex());
	for (auto const& lhs_statement : lhs_set) {
		if (Utilities::checkStmtTypeMatch(lhs_statement, lhs.getSynonym().type)) {
			return QueryResult(true);
		}
	}
	return {};
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::StatementRelationExecutor<T>::executeTrivialSynonymWildcard(QP::StorageAdapter& pkb,
                                                                                             const Types::ReferenceArgument& lhs) {
	StmtInfoPtrSet statements = pkb.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, lhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet rhs_set = pkb.getReverseStatements<T>(statement->getIdentifier());
		if (!rhs_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::StatementRelationExecutor<T>::executeTrivialSynonymSynonym(QP::StorageAdapter& pkb,
                                                                                            const Types::ReferenceArgument& lhs,
                                                                                            const Types::ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol == rhs.getSynonym().symbol) {
		return {};
	}

	StmtInfoPtrSet statements = pkb.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, lhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet rhs_set = pkb.getReverseStatements<T>(statement->getIdentifier());
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
QP::QueryResult QP::Relationship::StatementRelationExecutor<T>::executeIndexSynonym(QP::StorageAdapter& pkb,
                                                                                   const Types::ReferenceArgument& lhs,
                                                                                   const Types::ReferenceArgument& rhs) {
	StmtInfoPtrSet statements = pkb.getReverseStatements<T>(lhs.getStatementIndex());
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
QP::QueryResult QP::Relationship::StatementRelationExecutor<T>::executeWildcardSynonym(QP::StorageAdapter& pkb,
                                                                                      const Types::ReferenceArgument& rhs) {
	StmtInfoPtrSet statements = pkb.getStatements();
	vector<string> column;
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, rhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet lhs_set = pkb.getForwardStatements<T>(statement->getIdentifier());
		if (!lhs_set.empty()) {
			column.push_back(to_string(statement->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(rhs.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::StatementRelationExecutor<T>::executeSynonymIndex(QP::StorageAdapter& pkb,
                                                                                   const Types::ReferenceArgument& lhs,
                                                                                   const Types::ReferenceArgument& rhs) {
	StmtInfoPtrSet lhs_set = pkb.getForwardStatements<T>(rhs.getStatementIndex());
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
QP::QueryResult QP::Relationship::StatementRelationExecutor<T>::executeSynonymWildcard(QP::StorageAdapter& pkb,
                                                                                      const Types::ReferenceArgument& lhs) {
	StmtInfoPtrSet statements = pkb.getStatements();
	vector<string> column;
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, lhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet rhs_set = pkb.getReverseStatements<T>(statement->getIdentifier());
		if (!rhs_set.empty()) {
			column.push_back(to_string(statement->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(lhs.getSynonym().symbol, column);
	return result;
}

template <QP::Types::ClauseType T>
QP::QueryResult QP::Relationship::StatementRelationExecutor<T>::executeSynonymSynonym(QP::StorageAdapter& pkb,
                                                                                     const Types::ReferenceArgument& lhs,
                                                                                     const Types::ReferenceArgument& rhs) {
	if (lhs.getSynonym().symbol == rhs.getSynonym().symbol) {
		return {};
	}

	StmtInfoPtrSet statements = pkb.getStatements();
	vector<string> lhs_column;
	vector<string> rhs_column;
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, lhs.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet rhs_set = pkb.getReverseStatements<T>(statement->getIdentifier());
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