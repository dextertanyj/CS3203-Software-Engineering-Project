#include "QP/Relationship/Follows.h"

#include <memory>
#include <utility>
#include <vector>

#include "QP/QueryUtils.h"
#include "QP/Relationship/FollowsDispatcherTemplate.tpp"

QP::QueryResult QP::Relationship::Follows::executeTrivialIndexIndex(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& front,
                                                                    const Types::ReferenceArgument& rear) {
	return QueryResult(pkb.checkFollows(front.getStatementIndex(), rear.getStatementIndex()));
}

QP::QueryResult QP::Relationship::Follows::executeTrivialIndexWildcard(PKB::StorageAccessInterface& pkb,
                                                                       const Types::ReferenceArgument& front) {
	return QueryResult(pkb.getFollower(front.getStatementIndex()) != nullptr);
}

QP::QueryResult QP::Relationship::Follows::executeTrivialIndexSynonym(PKB::StorageAccessInterface& pkb,
                                                                      const Types::ReferenceArgument& front,
                                                                      const Types::ReferenceArgument& rear) {
	shared_ptr<StmtInfo> statement = pkb.getFollower(front.getStatementIndex());
	return QueryResult(statement != nullptr && Utilities::checkStmtTypeMatch(statement, rear.getSynonym().type));
}

QP::QueryResult QP::Relationship::Follows::executeTrivialWildcardIndex(PKB::StorageAccessInterface& pkb,
                                                                       const Types::ReferenceArgument& rear) {
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

QP::QueryResult QP::Relationship::Follows::executeTrivialWildcardSynonym(PKB::StorageAccessInterface& pkb,
                                                                         const Types::ReferenceArgument& rear) {
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

QP::QueryResult QP::Relationship::Follows::executeTrivialSynonymIndex(PKB::StorageAccessInterface& pkb,
                                                                      const Types::ReferenceArgument& front,
                                                                      const Types::ReferenceArgument& rear) {
	shared_ptr<StmtInfo> front_statement = pkb.getPreceding(rear.getStatementIndex());
	return QueryResult(Utilities::checkStmtTypeMatch(front_statement, front.getSynonym().type));
}

QP::QueryResult QP::Relationship::Follows::executeTrivialSynonymWildcard(PKB::StorageAccessInterface& pkb,
                                                                         const Types::ReferenceArgument& front) {
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

QP::QueryResult QP::Relationship::Follows::executeTrivialSynonymSynonym(PKB::StorageAccessInterface& pkb,
                                                                        const Types::ReferenceArgument& front,
                                                                        const Types::ReferenceArgument& rear) {
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

QP::QueryResult QP::Relationship::Follows::executeIndexSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& front,
                                                               const Types::ReferenceArgument& rear) {
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

QP::QueryResult QP::Relationship::Follows::executeWildcardSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& rear) {
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

QP::QueryResult QP::Relationship::Follows::executeSynonymIndex(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& front,
                                                               const Types::ReferenceArgument& rear) {
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

QP::QueryResult QP::Relationship::Follows::executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& front) {
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

QP::QueryResult QP::Relationship::Follows::executeSynonymSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& front,
                                                                 const Types::ReferenceArgument& rear) {
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

QP::Types::ArgumentDispatcher QP::Relationship::Follows::dispatcher = [](vector<Types::ReferenceArgument> args) {
	return FollowsDispatcherTemplate<Follows>::argumentDispatcher(Types::ClauseType::Follows, move(args));
};