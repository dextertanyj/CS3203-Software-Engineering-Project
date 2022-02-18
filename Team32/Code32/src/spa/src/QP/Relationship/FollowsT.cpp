#include "FollowsT.h"

QueryResult FollowsT::execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map) {
	return isTrivial ? executeTrivial(pkb, map) : executeNonTrivial(pkb, map);
}

QueryResult FollowsT::executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	if (leftStmt.type == StmtRefType::stmtNumber && rightStmt.type == StmtRefType::stmtNumber) {
		StmtInfoPtrSet followersSet = pkb.getFollowerStar(stoul(leftStmt.stmtRef));
		StmtRef rightStmtNo = stoul(rightStmt.stmtRef);
		for (auto const& follower : followersSet) {
			if (follower.get()->reference == rightStmtNo) {
				return QueryResult(true);
			}
		}
	}
	else if (leftStmt.type == StmtRefType::stmtNumber && rightStmt.type == StmtRefType::underscore) {
		StmtInfoPtrSet stmtSet = pkb.getFollowerStar(stoul(leftStmt.stmtRef));
		return QueryResult(!stmtSet.empty());
	}
	else if (leftStmt.type == StmtRefType::stmtNumber && rightStmt.type == StmtRefType::synonym) {
		StmtInfoPtrSet stmtSet = pkb.getFollowerStar(stoul(leftStmt.stmtRef));
		DesignEntity designEntity = map[rightStmt.stmtRef];
		for (auto const& stmt : stmtSet) {
			if (QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				return QueryResult(true);
			}
		}
	}
	else if (leftStmt.type == StmtRefType::underscore && rightStmt.type == StmtRefType::stmtNumber) {
		StmtInfoPtrSet stmtSet = pkb.getPrecedingStar(stoul(rightStmt.stmtRef));
		return QueryResult(!stmtSet.empty());
	}
	else if (leftStmt.type == StmtRefType::underscore && rightStmt.type == StmtRefType::underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		for (auto const& stmt : stmtSet) {
			StmtInfoPtrSet followers = pkb.getFollowerStar(stmt.get()->reference);
			if (!followers.empty()) {
				return QueryResult(true);
			}
		}
	}
	else if (leftStmt.type == StmtRefType::underscore && rightStmt.type == StmtRefType::synonym) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[rightStmt.stmtRef];
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			StmtInfoPtrSet precedingSet = pkb.getPrecedingStar(stmt.get()->reference);
			if (!precedingSet.empty()) {
				return QueryResult(true);
			}
		}
	}
	else if (leftStmt.type == StmtRefType::synonym && rightStmt.type == StmtRefType::stmtNumber) {
		StmtInfoPtrSet precedingSet = pkb.getPrecedingStar(stoul(rightStmt.stmtRef));
		DesignEntity designEntity = map[leftStmt.stmtRef];
		for (auto const& preceding : precedingSet) {
			if (QueryUtils::checkStmtTypeMatch(preceding, designEntity)) {
				return QueryResult(true);
			}
		}
	}
	else if (leftStmt.type == StmtRefType::synonym && rightStmt.type == StmtRefType::underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[leftStmt.stmtRef];
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			StmtInfoPtrSet followers = pkb.getFollowerStar(stmt.get()->reference);
			if (!followers.empty()) {
				return QueryResult(true);
			}
		}
	}
	else if (leftStmt.type == StmtRefType::synonym && rightStmt.type == StmtRefType::synonym) {
		if (leftStmt.stmtRef == rightStmt.stmtRef) {
			return QueryResult();
		}
		
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity leftDesignEntity = map[leftStmt.stmtRef];
		DesignEntity rightDesignEntity = map[rightStmt.stmtRef];
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, leftDesignEntity)) {
				continue;
			}

			StmtInfoPtrSet followerSet = pkb.getFollowerStar(stmt.get()->reference);
			for (auto const& follower : followerSet) {
				if (QueryUtils::checkStmtTypeMatch(follower, rightDesignEntity)) {
					return QueryResult(true);
				}
			}
		}
	}
	
	return QueryResult();
}

QueryResult FollowsT::executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	if (leftStmt.type == StmtRefType::synonym && rightStmt.type == StmtRefType::stmtNumber) {
		StmtInfoPtrSet precedingSet = pkb.getPrecedingStar(stoul(rightStmt.stmtRef));
		DesignEntity designEntity = map[leftStmt.stmtRef];
		vector<string> column;
		for (auto const& preceding : precedingSet) {
			if (QueryUtils::checkStmtTypeMatch(preceding, designEntity)) {
				column.push_back(to_string(preceding.get()->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(leftStmt.stmtRef, column);
		return result;
	}
	else if (leftStmt.type == StmtRefType::synonym && rightStmt.type == StmtRefType::underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[leftStmt.stmtRef];
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
		result.addColumn(leftStmt.stmtRef, column);
		return result;
	}
	else if (leftStmt.type == StmtRefType::synonym && rightStmt.type == StmtRefType::synonym) {
		if (leftStmt.stmtRef == rightStmt.stmtRef) {
			return QueryResult();
		}
		
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity leftDesignEntity = map[leftStmt.stmtRef];
		DesignEntity rightDesignEntity = map[rightStmt.stmtRef];
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
		result.addColumn(leftStmt.stmtRef, leftColumn);
		result.addColumn(rightStmt.stmtRef, rightColumn);
		return result;
	}
	else if (leftStmt.type == StmtRefType::underscore && rightStmt.type == StmtRefType::synonym) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[rightStmt.stmtRef];
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
		result.addColumn(rightStmt.stmtRef, column);
		return result;
	}
	else if (leftStmt.type == StmtRefType::stmtNumber && rightStmt.type == StmtRefType::synonym) {
		StmtInfoPtrSet stmtSet = pkb.getFollowerStar(stoul(leftStmt.stmtRef));
		DesignEntity designEntity = map[rightStmt.stmtRef];
		vector<string> column;

		for (auto const& stmt : stmtSet) {
			if (QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				column.push_back(to_string(stmt.get()->reference));
			}
		}

		QueryResult result = QueryResult();
		result.addColumn(rightStmt.stmtRef, column);
		return result;
	}

 	return QueryResult();
}