#include "FollowsT.h"

QP::QueryResult QP::Relationship::FollowsT::executeTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& /*map*/) {
	if (getLeftStmt().getType() == ReferenceType::StatementIndex && getRightStmt().getType() == ReferenceType::StatementIndex) {
		StmtInfoPtrSet followers_set = pkb.getFollowerStar(getLeftStmt().getStatementIndex());
		StmtRef right_stmt_no = getRightStmt().getStatementIndex();
		for (auto const& follower : followers_set) {
			if (follower->getIdentifier() == right_stmt_no) {
				return QueryResult(true);
			}
		}
	} else if (getLeftStmt().getType() == ReferenceType::StatementIndex && getRightStmt().getType() == ReferenceType::Wildcard) {
		StmtInfoPtrSet stmt_set = pkb.getFollowerStar(getLeftStmt().getStatementIndex());
		return QueryResult(!stmt_set.empty());
	} else if (getLeftStmt().getType() == ReferenceType::StatementIndex && getRightStmt().getType() == ReferenceType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getFollowerStar(getLeftStmt().getStatementIndex());
		DesignEntity design_entity = getRightStmt().getSynonym().type;
		for (auto const& stmt : stmt_set) {
			if (Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				return QueryResult(true);
			}
		}
	} else if (getLeftStmt().getType() == ReferenceType::Wildcard && getRightStmt().getType() == ReferenceType::StatementIndex) {
		StmtInfoPtrSet stmt_set = pkb.getPrecedingStar(getRightStmt().getStatementIndex());
		return QueryResult(!stmt_set.empty());
	} else if (getLeftStmt().getType() == ReferenceType::Wildcard && getRightStmt().getType() == ReferenceType::Wildcard) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		for (auto const& stmt : stmt_set) {
			StmtInfoPtrSet followers = pkb.getFollowerStar(stmt->getIdentifier());
			if (!followers.empty()) {
				return QueryResult(true);
			}
		}
	} else if (getLeftStmt().getType() == ReferenceType::Wildcard && getRightStmt().getType() == ReferenceType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = getRightStmt().getSynonym().type;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet preceding_set = pkb.getPrecedingStar(stmt->getIdentifier());
			if (!preceding_set.empty()) {
				return QueryResult(true);
			}
		}
	} else if (getLeftStmt().getType() == ReferenceType::Synonym && getRightStmt().getType() == ReferenceType::StatementIndex) {
		StmtInfoPtrSet preceding_set = pkb.getPrecedingStar(getRightStmt().getStatementIndex());
		DesignEntity design_entity = getLeftStmt().getSynonym().type;
		for (auto const& preceding : preceding_set) {
			if (Utilities::checkStmtTypeMatch(preceding, design_entity)) {
				return QueryResult(true);
			}
		}
	} else if (getLeftStmt().getType() == ReferenceType::Synonym && getRightStmt().getType() == ReferenceType::Wildcard) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = getLeftStmt().getSynonym().type;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet followers = pkb.getFollowerStar(stmt->getIdentifier());
			if (!followers.empty()) {
				return QueryResult(true);
			}
		}
	} else if (getLeftStmt().getType() == ReferenceType::Synonym && getRightStmt().getType() == ReferenceType::Synonym) {
		if (getLeftStmt().getSynonym().symbol == getRightStmt().getSynonym().symbol) {
			return {};
		}

		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity left_design_entity = getLeftStmt().getSynonym().type;
		DesignEntity right_design_entity = getRightStmt().getSynonym().type;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, left_design_entity)) {
				continue;
			}

			StmtInfoPtrSet follower_set = pkb.getFollowerStar(stmt->getIdentifier());
			for (auto const& follower : follower_set) {
				if (Utilities::checkStmtTypeMatch(follower, right_design_entity)) {
					return QueryResult(true);
				}
			}
		}
	}

	return {};
}

QP::QueryResult QP::Relationship::FollowsT::executeNonTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) {
	if (getLeftStmt().getType() == ReferenceType::Synonym && getRightStmt().getType() == ReferenceType::StatementIndex) {
		StmtInfoPtrSet preceding_set = pkb.getPrecedingStar(getRightStmt().getStatementIndex());
		DesignEntity design_entity = getLeftStmt().getSynonym().type;
		vector<string> column;
		for (auto const& preceding : preceding_set) {
			if (Utilities::checkStmtTypeMatch(preceding, design_entity)) {
				column.push_back(to_string(preceding->getIdentifier()));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(getLeftStmt().getSynonym().symbol, column);
		return result;
	}
	if (getLeftStmt().getType() == ReferenceType::Synonym && getRightStmt().getType() == ReferenceType::Wildcard) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = getLeftStmt().getSynonym().type;
		vector<string> column;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet follower_set = pkb.getFollowerStar(stmt->getIdentifier());
			if (!follower_set.empty()) {
				column.push_back(to_string(stmt->getIdentifier()));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(getLeftStmt().getSynonym().symbol, column);
		return result;
	}
	if (getLeftStmt().getType() == ReferenceType::Synonym && getRightStmt().getType() == ReferenceType::Synonym) {
		if (getLeftStmt().getSynonym().symbol == getRightStmt().getSynonym().symbol) {
			return {};
		}

		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity left_design_entity = getLeftStmt().getSynonym().type;
		DesignEntity right_design_entity = getRightStmt().getSynonym().type;
		vector<string> left_column;
		vector<string> right_column;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, left_design_entity)) {
				continue;
			}

			StmtInfoPtrSet follower_set = pkb.getFollowerStar(stmt->getIdentifier());
			for (auto const& follower : follower_set) {
				if (Utilities::checkStmtTypeMatch(follower, right_design_entity)) {
					left_column.push_back(to_string(stmt->getIdentifier()));
					right_column.push_back(to_string(follower->getIdentifier()));
				}
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(getLeftStmt().getSynonym().symbol, left_column);
		result.addColumn(getRightStmt().getSynonym().symbol, right_column);
		return result;
	}
	if (getLeftStmt().getType() == ReferenceType::Wildcard && getRightStmt().getType() == ReferenceType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = getRightStmt().getSynonym().type;
		vector<string> column;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet preceding_set = pkb.getPrecedingStar(stmt->getIdentifier());
			if (!preceding_set.empty()) {
				column.push_back(to_string(stmt->getIdentifier()));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(getRightStmt().getSynonym().symbol, column);
		return result;
	}
	if (getLeftStmt().getType() == ReferenceType::StatementIndex && getRightStmt().getType() == ReferenceType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getFollowerStar(getLeftStmt().getStatementIndex());
		DesignEntity design_entity = getRightStmt().getSynonym().type;
		vector<string> column;

		for (auto const& stmt : stmt_set) {
			if (Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				column.push_back(to_string(stmt->getIdentifier()));
			}
		}

		QueryResult result = QueryResult();
		result.addColumn(getRightStmt().getSynonym().symbol, column);
		return result;
	}

	return {};
}
