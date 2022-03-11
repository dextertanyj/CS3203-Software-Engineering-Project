#include "QP/Relationship/FollowsT.h"

#include <utility>
#include <vector>

#include "QP/QueryUtils.h"
#include "QP/Relationship/FollowsDispatcherTemplate.tpp"

// Trivial Executors

QP::QueryResult QP::Relationship::FollowsT::executeTrivialIndexIndex(PKB::StorageAccessInterface& pkb,
                                                                     const Types::ReferenceArgument& front,
                                                                     const Types::ReferenceArgument& rear) {
	StmtInfoPtrSet rears = pkb.getFollowerStar(front.getStatementIndex());
	StmtRef rear_index = rear.getStatementIndex();
	for (auto const& rear_statement : rears) {
		if (rear_statement->getIdentifier() == rear_index) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::FollowsT::executeTrivialIndexWildcard(PKB::StorageAccessInterface& pkb,
                                                                        const Types::ReferenceArgument& front) {
	return QueryResult(!pkb.getFollowerStar(front.getStatementIndex()).empty());
}

QP::QueryResult QP::Relationship::FollowsT::executeTrivialIndexSynonym(PKB::StorageAccessInterface& pkb,
                                                                       const Types::ReferenceArgument& front,
                                                                       const Types::ReferenceArgument& rear) {
	StmtInfoPtrSet rears = pkb.getFollowerStar(front.getStatementIndex());
	if (rear.getSynonym().type == Types::DesignEntity::Stmt) {
		return QueryResult(!rears.empty());
	}

	for (auto const& rear_statement : rears) {
		if (Utilities::checkStmtTypeMatch(rear_statement, rear.getSynonym().type)) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::FollowsT::executeTrivialWildcardIndex(PKB::StorageAccessInterface& pkb,
                                                                        const Types::ReferenceArgument& rear) {
	return QueryResult(!pkb.getPrecedingStar(rear.getStatementIndex()).empty());
}

QP::QueryResult QP::Relationship::FollowsT::executeTrivialWildcardWildcard(PKB::StorageAccessInterface& pkb) {
	StmtInfoPtrSet statements = pkb.getStatements();
	for (auto const& statement : statements) {
		StmtInfoPtrSet rears = pkb.getFollowerStar(statement->getIdentifier());
		if (!rears.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::FollowsT::executeTrivialWildcardSynonym(PKB::StorageAccessInterface& pkb,
                                                                          const Types::ReferenceArgument& rear) {
	StmtInfoPtrSet statements = pkb.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, rear.getSynonym().type)) {
			continue;
		}
		StmtInfoPtrSet fronts = pkb.getPrecedingStar(statement->getIdentifier());
		if (!fronts.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::FollowsT::executeTrivialSynonymIndex(PKB::StorageAccessInterface& pkb,
                                                                       const Types::ReferenceArgument& front,
                                                                       const Types::ReferenceArgument& rear) {
	StmtInfoPtrSet fronts = pkb.getPrecedingStar(rear.getStatementIndex());
	for (auto const& front_statement : fronts) {
		if (Utilities::checkStmtTypeMatch(front_statement, front.getSynonym().type)) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::FollowsT::executeTrivialSynonymWildcard(PKB::StorageAccessInterface& pkb,
                                                                          const Types::ReferenceArgument& front) {
	StmtInfoPtrSet statements = pkb.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, front.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet rears = pkb.getFollowerStar(statement->getIdentifier());
		if (!rears.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::FollowsT::executeTrivialSynonymSynonym(PKB::StorageAccessInterface& pkb,
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

		StmtInfoPtrSet rears = pkb.getFollowerStar(statement->getIdentifier());
		for (auto const& rear_statement : rears) {
			if (Utilities::checkStmtTypeMatch(rear_statement, rear.getSynonym().type)) {
				return QueryResult(true);
			}
		}
	}

	return {};
}

// Executors

QP::QueryResult QP::Relationship::FollowsT::executeIndexSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& front,
                                                                const Types::ReferenceArgument& rear) {
	StmtInfoPtrSet statements = pkb.getFollowerStar(front.getStatementIndex());
	vector<string> column;

	for (auto const& statement : statements) {
		if (Utilities::checkStmtTypeMatch(statement, rear.getSynonym().type)) {
			column.push_back(to_string(statement->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(rear.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::FollowsT::executeWildcardSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& rear) {
	StmtInfoPtrSet statements = pkb.getStatements();
	vector<string> column;
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, rear.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet fronts = pkb.getPrecedingStar(statement->getIdentifier());
		if (!fronts.empty()) {
			column.push_back(to_string(statement->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(rear.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::FollowsT::executeSynonymIndex(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& front,
                                                                const Types::ReferenceArgument& rear) {
	StmtInfoPtrSet fronts = pkb.getPrecedingStar(rear.getStatementIndex());
	vector<string> column;
	for (auto const& front_statement : fronts) {
		if (Utilities::checkStmtTypeMatch(front_statement, front.getSynonym().type)) {
			column.push_back(to_string(front_statement->getIdentifier()));
		}
	}

	QueryResult result = QueryResult();
	result.addColumn(front.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::FollowsT::executeSynonymWildcard(PKB::StorageAccessInterface& pkb,
                                                                   const Types::ReferenceArgument& front) {
	StmtInfoPtrSet statements = pkb.getStatements();
	vector<string> column;
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, front.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet rearren = pkb.getFollowerStar(statement->getIdentifier());
		if (!rearren.empty()) {
			column.push_back(to_string(statement->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(front.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::FollowsT::executeSynonymSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& front,
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

		StmtInfoPtrSet rear_statements = pkb.getFollowerStar(statement->getIdentifier());
		for (auto const& rear_statement : rear_statements) {
			if (Utilities::checkStmtTypeMatch(rear_statement, rear.getSynonym().type)) {
				front_column.push_back(to_string(statement->getIdentifier()));
				rear_column.push_back(to_string(rear_statement->getIdentifier()));
			}
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(front.getSynonym().symbol, front_column);
	result.addColumn(rear.getSynonym().symbol, rear_column);
	return result;
}

QP::Types::ArgumentDispatcher QP::Relationship::FollowsT::dispatcher = [](vector<Types::ReferenceArgument> args) {
	return FollowsDispatcherTemplate<FollowsT>::argumentDispatcher(Types::ClauseType::FollowsT, move(args));
};