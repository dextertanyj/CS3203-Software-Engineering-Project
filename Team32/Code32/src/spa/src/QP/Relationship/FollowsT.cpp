#include "FollowsT.h"

QP::QueryResult QP::Relationship::FollowsT::executeTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) {
	if (getLeftStmt().type == StmtRefType::StmtNumber && getRightStmt().type == StmtRefType::StmtNumber) {
		StmtInfoPtrSet followers_set = pkb.getFollowerStar(stoul(getLeftStmt().stmt_ref));
		StmtRef right_stmt_no = stoul(getRightStmt().stmt_ref);
		for (auto const& follower : followers_set) {
			if (follower->getIdentifier() == right_stmt_no) {
				return QueryResult(true);
			}
		}
	} else if (getLeftStmt().type == StmtRefType::StmtNumber && getRightStmt().type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getFollowerStar(stoul(getLeftStmt().stmt_ref));
		return QueryResult(!stmt_set.empty());
	} else if (getLeftStmt().type == StmtRefType::StmtNumber && getRightStmt().type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getFollowerStar(stoul(getLeftStmt().stmt_ref));
		DesignEntity design_entity = map[getRightStmt().stmt_ref];
		for (auto const& stmt : stmt_set) {
			if (Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				return QueryResult(true);
			}
		}
	} else if (getLeftStmt().type == StmtRefType::Underscore && getRightStmt().type == StmtRefType::StmtNumber) {
		StmtInfoPtrSet stmt_set = pkb.getPrecedingStar(stoul(getRightStmt().stmt_ref));
		return QueryResult(!stmt_set.empty());
	} else if (getLeftStmt().type == StmtRefType::Underscore && getRightStmt().type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		for (auto const& stmt : stmt_set) {
			StmtInfoPtrSet followers = pkb.getFollowerStar(stmt->getIdentifier());
			if (!followers.empty()) {
				return QueryResult(true);
			}
		}
	} else if (getLeftStmt().type == StmtRefType::Underscore && getRightStmt().type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[getRightStmt().stmt_ref];
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet preceding_set = pkb.getPrecedingStar(stmt->getIdentifier());
			if (!preceding_set.empty()) {
				return QueryResult(true);
			}
		}
	} else if (getLeftStmt().type == StmtRefType::Synonym && getRightStmt().type == StmtRefType::StmtNumber) {
		StmtInfoPtrSet preceding_set = pkb.getPrecedingStar(stoul(getRightStmt().stmt_ref));
		DesignEntity design_entity = map[getLeftStmt().stmt_ref];
		for (auto const& preceding : preceding_set) {
			if (Utilities::checkStmtTypeMatch(preceding, design_entity)) {
				return QueryResult(true);
			}
		}
	} else if (getLeftStmt().type == StmtRefType::Synonym && getRightStmt().type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[getLeftStmt().stmt_ref];
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet followers = pkb.getFollowerStar(stmt->getIdentifier());
			if (!followers.empty()) {
				return QueryResult(true);
			}
		}
	} else if (getLeftStmt().type == StmtRefType::Synonym && getRightStmt().type == StmtRefType::Synonym) {
		if (getLeftStmt().stmt_ref == getRightStmt().stmt_ref) {
			return {};
		}

		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity left_design_entity = map[getLeftStmt().stmt_ref];
		DesignEntity right_design_entity = map[getRightStmt().stmt_ref];
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
	if (getLeftStmt().type == StmtRefType::Synonym && getRightStmt().type == StmtRefType::StmtNumber) {
		StmtInfoPtrSet preceding_set = pkb.getPrecedingStar(stoul(getRightStmt().stmt_ref));
		DesignEntity design_entity = map[getLeftStmt().stmt_ref];
		vector<string> column;
		for (auto const& preceding : preceding_set) {
			if (Utilities::checkStmtTypeMatch(preceding, design_entity)) {
				column.push_back(to_string(preceding->getIdentifier()));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(getLeftStmt().stmt_ref, column);
		return result;
	}
	if (getLeftStmt().type == StmtRefType::Synonym && getRightStmt().type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[getLeftStmt().stmt_ref];
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
		result.addColumn(getLeftStmt().stmt_ref, column);
		return result;
	}
	if (getLeftStmt().type == StmtRefType::Synonym && getRightStmt().type == StmtRefType::Synonym) {
		if (getLeftStmt().stmt_ref == getRightStmt().stmt_ref) {
			return {};
		}

		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity left_design_entity = map[getLeftStmt().stmt_ref];
		DesignEntity right_design_entity = map[getRightStmt().stmt_ref];
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
		result.addColumn(getLeftStmt().stmt_ref, left_column);
		result.addColumn(getRightStmt().stmt_ref, right_column);
		return result;
	}
	if (getLeftStmt().type == StmtRefType::Underscore && getRightStmt().type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[getRightStmt().stmt_ref];
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
		result.addColumn(getRightStmt().stmt_ref, column);
		return result;
	}
	if (getLeftStmt().type == StmtRefType::StmtNumber && getRightStmt().type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getFollowerStar(stoul(getLeftStmt().stmt_ref));
		DesignEntity design_entity = map[getRightStmt().stmt_ref];
		vector<string> column;

		for (auto const& stmt : stmt_set) {
			if (Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				column.push_back(to_string(stmt->getIdentifier()));
			}
		}

		QueryResult result = QueryResult();
		result.addColumn(getRightStmt().stmt_ref, column);
		return result;
	}

	return {};
}
