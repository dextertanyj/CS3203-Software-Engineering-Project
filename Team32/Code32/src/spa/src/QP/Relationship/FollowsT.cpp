#include "FollowsT.h"

QueryResult FollowsT::execute(PKB& pkb, bool is_trivial, unordered_map<string, DesignEntity>& map) {
	return is_trivial ? executeTrivial(pkb, map) : executeNonTrivial(pkb, map);
}

QueryResult FollowsT::executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	if (left_stmt.type == StmtRefType::StmtNumber && right_stmt.type == StmtRefType::StmtNumber) {
		StmtInfoPtrSet followers_set = pkb.getFollowerStar(stoul(left_stmt.stmt_ref));
		StmtRef right_stmt_no = stoul(right_stmt.stmt_ref);
		for (auto const& follower : followers_set) {
			if (follower->reference == right_stmt_no) {
				return QueryResult(true);
			}
		}
	}
	else if (left_stmt.type == StmtRefType::StmtNumber && right_stmt.type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getFollowerStar(stoul(left_stmt.stmt_ref));
		return QueryResult(!stmt_set.empty());
	}
	else if (left_stmt.type == StmtRefType::StmtNumber && right_stmt.type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getFollowerStar(stoul(left_stmt.stmt_ref));
		DesignEntity design_entity = map[right_stmt.stmt_ref];
		for (auto const& stmt : stmt_set) {
			if (QueryUtils::checkStmtTypeMatch(stmt, design_entity)) {
				return QueryResult(true);
			}
		}
	}
	else if (left_stmt.type == StmtRefType::Underscore && right_stmt.type == StmtRefType::StmtNumber) {
		StmtInfoPtrSet stmt_set = pkb.getPrecedingStar(stoul(right_stmt.stmt_ref));
		return QueryResult(!stmt_set.empty());
	}
	else if (left_stmt.type == StmtRefType::Underscore && right_stmt.type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		for (auto const& stmt : stmt_set) {
			StmtInfoPtrSet followers = pkb.getFollowerStar(stmt->reference);
			if (!followers.empty()) {
				return QueryResult(true);
			}
		}
	}
	else if (left_stmt.type == StmtRefType::Underscore && right_stmt.type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[right_stmt.stmt_ref];
		for (auto const& stmt : stmt_set) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet preceding_set = pkb.getPrecedingStar(stmt->reference);
			if (!preceding_set.empty()) {
				return QueryResult(true);
			}
		}
	}
	else if (left_stmt.type == StmtRefType::Synonym && right_stmt.type == StmtRefType::StmtNumber) {
		StmtInfoPtrSet preceding_set = pkb.getPrecedingStar(stoul(right_stmt.stmt_ref));
		DesignEntity design_entity = map[left_stmt.stmt_ref];
		for (auto const& preceding : preceding_set) {
			if (QueryUtils::checkStmtTypeMatch(preceding, design_entity)) {
				return QueryResult(true);
			}
		}
	}
	else if (left_stmt.type == StmtRefType::Synonym && right_stmt.type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[left_stmt.stmt_ref];
		for (auto const& stmt : stmt_set) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet followers = pkb.getFollowerStar(stmt->reference);
			if (!followers.empty()) {
				return QueryResult(true);
			}
		}
	}
	else if (left_stmt.type == StmtRefType::Synonym && right_stmt.type == StmtRefType::Synonym) {
		if (left_stmt.stmt_ref == right_stmt.stmt_ref) {
			return {};
		}

		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity left_design_entity = map[left_stmt.stmt_ref];
		DesignEntity right_design_entity = map[right_stmt.stmt_ref];
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

QueryResult FollowsT::executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	if (left_stmt.type == StmtRefType::Synonym && right_stmt.type == StmtRefType::StmtNumber) {
		StmtInfoPtrSet preceding_set = pkb.getPrecedingStar(stoul(right_stmt.stmt_ref));
		DesignEntity design_entity = map[left_stmt.stmt_ref];
		vector<string> column;
		for (auto const& preceding : preceding_set) {
			if (QueryUtils::checkStmtTypeMatch(preceding, design_entity)) {
				column.push_back(to_string(preceding->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(left_stmt.stmt_ref, column);
		return result;
	}
	if (left_stmt.type == StmtRefType::Synonym && right_stmt.type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[left_stmt.stmt_ref];
		vector<string> column;
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			StmtInfoPtrSet followerSet = pkb.getFollowerStar(stmt.get()->reference);
			if (!followerSet.empty()) {
				column.push_back(to_string(stmt.get()->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(left_stmt.stmt_ref, column);
		return result;
	} else if (left_stmt.type == StmtRefType::Synonym && right_stmt.type == StmtRefType::Synonym) {
		if (left_stmt.stmt_ref == right_stmt.stmt_ref) {
			return QueryResult();
		}

		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity leftDesignEntity = map[left_stmt.stmt_ref];
		DesignEntity rightDesignEntity = map[right_stmt.stmt_ref];
		vector<string> leftColumn;
		vector<string> rightColumn;
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, leftDesignEntity)) {
				continue;
			}

			StmtInfoPtrSet followerSet = pkb.getFollowerStar(stmt.get()->reference);
			for (auto const& follower : followerSet) {
				if (QueryUtils::checkStmtTypeMatch(follower, rightDesignEntity)) {
					leftColumn.push_back(to_string(stmt.get()->reference));
					rightColumn.push_back(to_string(follower.get()->reference));
				}
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(left_stmt.stmt_ref, leftColumn);
		result.addColumn(right_stmt.stmt_ref, rightColumn);
		return result;
	} else if (left_stmt.type == StmtRefType::Underscore && right_stmt.type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[right_stmt.stmt_ref];
		vector<string> column;
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			StmtInfoPtrSet precedingSet = pkb.getPrecedingStar(stmt.get()->reference);
			if (!precedingSet.empty()) {
				column.push_back(to_string(stmt.get()->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(right_stmt.stmt_ref, column);
		return result;
	} else if (left_stmt.type == StmtRefType::StmtNumber && right_stmt.type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmtSet = pkb.getFollowerStar(stoul(left_stmt.stmt_ref));
		DesignEntity designEntity = map[right_stmt.stmt_ref];
		vector<string> column;

		for (auto const& stmt : stmtSet) {
			if (QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				column.push_back(to_string(stmt.get()->reference));
			}
		}

		QueryResult result = QueryResult();
		result.addColumn(right_stmt.stmt_ref, column);
		return result;
	}

	return {};
}
