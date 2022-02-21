#include "FollowsT.h"

QueryResult FollowsT::execute(PKB::Storage& pkb, bool is_trivial, unordered_map<string, DesignEntity>& map) {
	return is_trivial ? executeTrivial(pkb, map) : executeNonTrivial(pkb, map);
}

QueryResult FollowsT::executeTrivial(PKB::Storage& pkb, unordered_map<string, DesignEntity>& map) {
	if (getLeftStmt().type == StmtRefType::StmtNumber && getRightStmt().type == StmtRefType::StmtNumber) {
		StmtInfoPtrSet followers_set = pkb.getFollowerStar(stoul(getLeftStmt().stmt_ref));
		StmtRef right_stmt_no = stoul(getRightStmt().stmt_ref);
		for (auto const& follower : followers_set) {
			if (follower->reference == right_stmt_no) {
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
			if (QueryUtils::checkStmtTypeMatch(stmt, design_entity)) {
				return QueryResult(true);
			}
		}
	} else if (getLeftStmt().type == StmtRefType::Underscore && getRightStmt().type == StmtRefType::StmtNumber) {
		StmtInfoPtrSet stmt_set = pkb.getPrecedingStar(stoul(getRightStmt().stmt_ref));
		return QueryResult(!stmt_set.empty());
	} else if (getLeftStmt().type == StmtRefType::Underscore && getRightStmt().type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		for (auto const& stmt : stmt_set) {
			StmtInfoPtrSet followers = pkb.getFollowerStar(stmt->reference);
			if (!followers.empty()) {
				return QueryResult(true);
			}
		}
	} else if (getLeftStmt().type == StmtRefType::Underscore && getRightStmt().type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[getRightStmt().stmt_ref];
		for (auto const& stmt : stmt_set) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet preceding_set = pkb.getPrecedingStar(stmt->reference);
			if (!preceding_set.empty()) {
				return QueryResult(true);
			}
		}
	} else if (getLeftStmt().type == StmtRefType::Synonym && getRightStmt().type == StmtRefType::StmtNumber) {
		StmtInfoPtrSet preceding_set = pkb.getPrecedingStar(stoul(getRightStmt().stmt_ref));
		DesignEntity design_entity = map[getLeftStmt().stmt_ref];
		for (auto const& preceding : preceding_set) {
			if (QueryUtils::checkStmtTypeMatch(preceding, design_entity)) {
				return QueryResult(true);
			}
		}
	} else if (getLeftStmt().type == StmtRefType::Synonym && getRightStmt().type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[getLeftStmt().stmt_ref];
		for (auto const& stmt : stmt_set) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet followers = pkb.getFollowerStar(stmt->reference);
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
			if (!QueryUtils::checkStmtTypeMatch(stmt, left_design_entity)) {
				continue;
			}

			StmtInfoPtrSet follower_set = pkb.getFollowerStar(stmt->reference);
			for (auto const& follower : follower_set) {
				if (QueryUtils::checkStmtTypeMatch(follower, right_design_entity)) {
					return QueryResult(true);
				}
			}
		}
	}

	return {};
}

QueryResult FollowsT::executeNonTrivial(PKB::Storage& pkb, unordered_map<string, DesignEntity>& map) {
	if (getLeftStmt().type == StmtRefType::Synonym && getRightStmt().type == StmtRefType::StmtNumber) {
		StmtInfoPtrSet preceding_set = pkb.getPrecedingStar(stoul(getRightStmt().stmt_ref));
		DesignEntity design_entity = map[getLeftStmt().stmt_ref];
		vector<string> column;
		for (auto const& preceding : preceding_set) {
			if (QueryUtils::checkStmtTypeMatch(preceding, design_entity)) {
				column.push_back(to_string(preceding->reference));
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
			if (!QueryUtils::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet follower_set = pkb.getFollowerStar(stmt->reference);
			if (!follower_set.empty()) {
				column.push_back(to_string(stmt->reference));
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
			if (!QueryUtils::checkStmtTypeMatch(stmt, left_design_entity)) {
				continue;
			}

			StmtInfoPtrSet follower_set = pkb.getFollowerStar(stmt->reference);
			for (auto const& follower : follower_set) {
				if (QueryUtils::checkStmtTypeMatch(follower, right_design_entity)) {
					left_column.push_back(to_string(stmt->reference));
					right_column.push_back(to_string(follower->reference));
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
			if (!QueryUtils::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet preceding_set = pkb.getPrecedingStar(stmt->reference);
			if (!preceding_set.empty()) {
				column.push_back(to_string(stmt->reference));
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
			if (QueryUtils::checkStmtTypeMatch(stmt, design_entity)) {
				column.push_back(to_string(stmt->reference));
			}
		}

		QueryResult result = QueryResult();
		result.addColumn(getRightStmt().stmt_ref, column);
		return result;
	}

	return {};
}
