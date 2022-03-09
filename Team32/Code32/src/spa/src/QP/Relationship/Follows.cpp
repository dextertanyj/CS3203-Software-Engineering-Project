#include "QP/Relationship/Follows.h"

#include <utility>

QP::Relationship::Follows::Follows(ReferenceArgument left_stmt, ReferenceArgument right_stmt)
	: left_stmt(std::move(std::move(left_stmt))), right_stmt(std::move(std::move(right_stmt))) {}

ReferenceArgument QP::Relationship::Follows::getLeftStmt() { return left_stmt; }

ReferenceArgument QP::Relationship::Follows::getRightStmt() { return right_stmt; }

vector<string> QP::Relationship::Follows::getDeclarationSymbols() {
	vector<string> declaration_symbols;
	if (this->left_stmt.getType() == ReferenceType::Synonym) {
		declaration_symbols.push_back(this->left_stmt.getSynonym().symbol);
	}
	if (this->right_stmt.getType() == ReferenceType::Synonym) {
		declaration_symbols.push_back(this->right_stmt.getSynonym().symbol);
	}
	return declaration_symbols;
}

QP::QueryResult QP::Relationship::Follows::executeTrivial(PKB::StorageAccessInterface& pkb) {
	if (left_stmt.getType() == ReferenceType::StatementIndex && right_stmt.getType() == ReferenceType::StatementIndex) {
		return executeTrivialIndexIndex(pkb, left_stmt, right_stmt);
	}
	if (left_stmt.getType() == ReferenceType::StatementIndex && right_stmt.getType() == ReferenceType::Wildcard) {
		return executeTrivialIndexWildcard(pkb, left_stmt);
	}
	if (left_stmt.getType() == ReferenceType::StatementIndex && right_stmt.getType() == ReferenceType::Synonym) {
		return executeTrivialIndexSynonym(pkb, left_stmt, right_stmt);
	}
	if (left_stmt.getType() == ReferenceType::Wildcard && right_stmt.getType() == ReferenceType::StatementIndex) {
		return executeTrivialWildcardIndex(pkb, right_stmt);
	}
	if (left_stmt.getType() == ReferenceType::Wildcard && right_stmt.getType() == ReferenceType::Wildcard) {
		return executeTrivialWildcardWildcard(pkb);
	}
	if (left_stmt.getType() == ReferenceType::Wildcard && right_stmt.getType() == ReferenceType::Synonym) {
		return executeTrivialWildcardSynonym(pkb, right_stmt);
	}
	if (left_stmt.getType() == ReferenceType::Synonym && right_stmt.getType() == ReferenceType::StatementIndex) {
		return executeTrivialSynonymIndex(pkb, left_stmt, right_stmt);
	}
	if (left_stmt.getType() == ReferenceType::Synonym && right_stmt.getType() == ReferenceType::Wildcard) {
		return executeTrivialSynonymWildcard(pkb, left_stmt);
	}
	if (left_stmt.getType() == ReferenceType::Synonym && right_stmt.getType() == ReferenceType::Synonym) {
		return executeTrivialSynonymSynonym(pkb, left_stmt, right_stmt);
	}
	if (left_stmt.getType() == ReferenceType::Synonym && right_stmt.getType() == ReferenceType::StatementIndex) {
		return executeSynonymIndex(pkb, left_stmt, right_stmt);
	}
	if (left_stmt.getType() == ReferenceType::Synonym && right_stmt.getType() == ReferenceType::Wildcard) {
		return executeSynonymWildcard(pkb, left_stmt);
	}
	if (left_stmt.getType() == ReferenceType::Synonym && right_stmt.getType() == ReferenceType::Synonym) {
		return executeSynonymSynonym(pkb, left_stmt, right_stmt);
	}
	if (left_stmt.getType() == ReferenceType::Wildcard && right_stmt.getType() == ReferenceType::Synonym) {
		return executeWildcardSynonym(pkb, right_stmt);
	}
	if (left_stmt.getType() == ReferenceType::StatementIndex && right_stmt.getType() == ReferenceType::Synonym) {
		return executeIndexSynonym(pkb, left_stmt, right_stmt);
	}
	return {};
}

QP::QueryResult QP::Relationship::Follows::executeNonTrivial(PKB::StorageAccessInterface& pkb) {
	if (left_stmt.getType() == ReferenceType::Synonym && right_stmt.getType() == ReferenceType::StatementIndex) {
		return executeSynonymIndex(pkb, left_stmt, right_stmt);
	}
	if (left_stmt.getType() == ReferenceType::Synonym && right_stmt.getType() == ReferenceType::Wildcard) {
		return executeSynonymWildcard(pkb, left_stmt);
	}
	if (left_stmt.getType() == ReferenceType::Synonym && right_stmt.getType() == ReferenceType::Synonym) {
		return executeSynonymSynonym(pkb, left_stmt, right_stmt);
	}
	if (left_stmt.getType() == ReferenceType::Wildcard && right_stmt.getType() == ReferenceType::Synonym) {
		return executeWildcardSynonym(pkb, right_stmt);
	}
	if (left_stmt.getType() == ReferenceType::StatementIndex && right_stmt.getType() == ReferenceType::Synonym) {
		return executeIndexSynonym(pkb, left_stmt, right_stmt);
	}
	

	return {};
}

QP::QueryResult QP::Relationship::Follows::executeTrivialIndexIndex(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front,
                                                                    const ReferenceArgument& rear) {
	return QueryResult(pkb.checkFollows(front.getStatementIndex(), rear.getStatementIndex()));
}

QP::QueryResult QP::Relationship::Follows::executeTrivialIndexWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front) {
	return QueryResult(pkb.getFollower(front.getStatementIndex()) != nullptr);
}

QP::QueryResult QP::Relationship::Follows::executeTrivialIndexSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front,
                                                                      const ReferenceArgument& rear) {
	shared_ptr<StmtInfo> statement = pkb.getFollower(front.getStatementIndex());
	return QueryResult(statement != nullptr && Utilities::checkStmtTypeMatch(statement, rear.getSynonym().type));
}

QP::QueryResult QP::Relationship::Follows::executeTrivialWildcardIndex(PKB::StorageAccessInterface& pkb, const ReferenceArgument& rear) {
	return QueryResult(pkb.getPreceding(rear.getStatementIndex()) != nullptr);
}

QP::QueryResult QP::Relationship::Follows::executeTrivialWildcardWildcard(PKB::StorageAccessInterface& pkb) {
	StmtInfoPtrSet statements = pkb.getStatements();
	for (auto const& statement : statements) {
		shared_ptr<StmtInfo> rear = pkb.getFollower(statement->getIdentifier());
		if (rear != nullptr) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::Follows::executeTrivialWildcardSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& rear) {
	StmtInfoPtrSet statements = pkb.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, rear.getSynonym().type)) {
			continue;
		}
		shared_ptr<StmtInfo> front = pkb.getPreceding(statement->getIdentifier());
		if (front != nullptr) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::Follows::executeTrivialSynonymIndex(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front,
                                                                      const ReferenceArgument& rear) {
	shared_ptr<StmtInfo> front_statement = pkb.getPreceding(rear.getStatementIndex());
	return QueryResult(Utilities::checkStmtTypeMatch(front_statement, front.getSynonym().type));
}

QP::QueryResult QP::Relationship::Follows::executeTrivialSynonymWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front) {
	StmtInfoPtrSet statements = pkb.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, front.getSynonym().type)) {
			continue;
		}

		shared_ptr<StmtInfo> rear = pkb.getFollower(statement->getIdentifier());
		if (rear != nullptr) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::Follows::executeTrivialSynonymSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front,
                                                                        const ReferenceArgument& rear) {
	if (front.getSynonym().symbol == rear.getSynonym().symbol) {
		return {};
	}

	StmtInfoPtrSet statements = pkb.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, front.getSynonym().type)) {
			continue;
		}

		shared_ptr<StmtInfo> rear_statement = pkb.getFollower(statement->getIdentifier());
		if (Utilities::checkStmtTypeMatch(rear_statement, rear.getSynonym().type)) {
			return QueryResult(true);
		}
	}
	return {};
}

// Executors

QP::QueryResult QP::Relationship::Follows::executeIndexSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front,
                                                               const ReferenceArgument& rear) {
	shared_ptr<StmtInfo> statement = pkb.getFollower(front.getStatementIndex());
	vector<string> column;

	if (!Utilities::checkStmtTypeMatch(statement, rear.getSynonym().type)) {
		return {};
	}
	column.push_back(to_string(statement->getIdentifier()));
	QueryResult result = QueryResult();
	result.addColumn(rear.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::Follows::executeWildcardSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& rear) {
	StmtInfoPtrSet statements = pkb.getStatements();
	vector<string> column;
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, rear.getSynonym().type)) {
			continue;
		}

		shared_ptr<StmtInfo> front = pkb.getPreceding(statement->getIdentifier());
		if (front != nullptr) {
			column.push_back(to_string(statement->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(rear.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::Follows::executeSynonymIndex(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front,
                                                               const ReferenceArgument& rear) {
	shared_ptr<StmtInfo> front_statement = pkb.getPreceding(rear.getStatementIndex());
	vector<string> column;
	if (!Utilities::checkStmtTypeMatch(front_statement, front.getSynonym().type)) {
		return {};
	}
	column.push_back(to_string(front_statement->getIdentifier()));
	QueryResult result = QueryResult();
	result.addColumn(front.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::Follows::executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front) {
	StmtInfoPtrSet statements = pkb.getStatements();
	vector<string> column;
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, front.getSynonym().type)) {
			continue;
		}

		shared_ptr<StmtInfo> rear = pkb.getFollower(statement->getIdentifier());
		if (rear != nullptr) {
			column.push_back(to_string(statement->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(front.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::Follows::executeSynonymSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& front,
                                                                 const ReferenceArgument& rear) {
	if (front.getSynonym().symbol == rear.getSynonym().symbol) {
		return {};
	}

	StmtInfoPtrSet statements = pkb.getStatements();
	vector<string> front_column;
	vector<string> rear_column;
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, front.getSynonym().type)) {
			continue;
		}

		shared_ptr<StmtInfo> rear_statement = pkb.getFollower(statement->getIdentifier());

		if (Utilities::checkStmtTypeMatch(rear_statement, rear.getSynonym().type)) {
			front_column.push_back(to_string(statement->getIdentifier()));
			rear_column.push_back(to_string(rear_statement->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(front.getSynonym().symbol, front_column);
	result.addColumn(rear.getSynonym().symbol, rear_column);
	return result;
}
