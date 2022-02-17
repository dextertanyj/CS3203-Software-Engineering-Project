#include "QP/Relationship/Follows.h"

Follows::Follows(QueryStmtRef leftStmt, QueryStmtRef rightStmt)
	: leftStmt(leftStmt),
	  rightStmt(rightStmt) {}

QueryStmtRef Follows::getLeftStmt() {
	return leftStmt;
}

QueryStmtRef Follows::getRightStmt() {
	return rightStmt;
}

QueryResult Follows::execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map) {
	return isTrivial ? executeTrivial(pkb, map) : executeNonTrivial(pkb, map);
}

vector<string> Follows::getDeclarationSymbols() {
	vector<string> declarationSymbols;
	if (this->leftStmt.type == StmtRefType::synonym) {
		declarationSymbols.push_back(this->leftStmt.stmtRef);
	}
	if (this->rightStmt.type == StmtRefType::synonym) {
		declarationSymbols.push_back(this->rightStmt.stmtRef);
	}
	return declarationSymbols;
}

QueryResult Follows::executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	if (leftStmt.type == StmtRefType::stmtNumber && rightStmt.type == StmtRefType::stmtNumber) {
		return QueryResult(pkb.checkFollows(stoul(leftStmt.stmtRef), stoul(rightStmt.stmtRef)));
	}
	else if (leftStmt.type == StmtRefType::stmtNumber && rightStmt.type == StmtRefType::underscore) {
		shared_ptr<StmtInfo> stmt = pkb.getFollower(stoul(leftStmt.stmtRef));
		return QueryResult(stmt.get() != nullptr);
	}
	else if (leftStmt.type == StmtRefType::stmtNumber && rightStmt.type == StmtRefType::synonym) {
		shared_ptr<StmtInfo> stmt = pkb.getFollower(stoul(leftStmt.stmtRef));
		DesignEntity designEntity = map[rightStmt.stmtRef];

		if (QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
			return QueryResult(true);
		}
	}
	else if (leftStmt.type == StmtRefType::underscore && rightStmt.type == StmtRefType::stmtNumber) {
		shared_ptr<StmtInfo> stmt = pkb.getPreceding(stoul(rightStmt.stmtRef));
		return QueryResult(stmt.get() != nullptr);
	}
	else if (leftStmt.type == StmtRefType::underscore && rightStmt.type == StmtRefType::underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		for (auto const& stmt : stmtSet) {
			shared_ptr<StmtInfo> follower = pkb.getFollower(stmt.get()->reference);
			if (follower.get() != nullptr) {
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

			shared_ptr<StmtInfo> preceding = pkb.getPreceding(stmt.get()->reference);
			if (preceding != nullptr) {
				return QueryResult(true);
			}
		}
	}
	else if (leftStmt.type == StmtRefType::synonym && rightStmt.type == StmtRefType::stmtNumber) {
		shared_ptr<StmtInfo> preceding = pkb.getPreceding(stoul(rightStmt.stmtRef));
		DesignEntity designEntity = map[leftStmt.stmtRef];
		if (!QueryUtils::checkStmtTypeMatch(preceding, designEntity)) {
			return QueryResult();
		}
		return QueryResult(true);
	}
	else if (leftStmt.type == StmtRefType::synonym && rightStmt.type == StmtRefType::underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[leftStmt.stmtRef];
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			shared_ptr<StmtInfo> follower = pkb.getFollower(stmt.get()->reference);
			if (follower != nullptr) {
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

			shared_ptr<StmtInfo> follower = pkb.getFollower(stmt.get()->reference);
			if (QueryUtils::checkStmtTypeMatch(follower, rightDesignEntity)) {
				return QueryResult(true);
			}
		}
	}
	
	return QueryResult();
}

QueryResult Follows::executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	if (leftStmt.type == StmtRefType::synonym && rightStmt.type == StmtRefType::stmtNumber) {
		shared_ptr<StmtInfo> preceding = pkb.getPreceding(stoul(rightStmt.stmtRef));
		DesignEntity designEntity = map[leftStmt.stmtRef];
		vector<string> column;
		if (!QueryUtils::checkStmtTypeMatch(preceding, designEntity)) {
			return QueryResult();
		}
		column.push_back(to_string(preceding.get()->reference));
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

			shared_ptr<StmtInfo> follower = pkb.getFollower(stmt.get()->reference);
			if (follower.get() != nullptr) {
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

			shared_ptr<StmtInfo> follower = pkb.getFollower(stmt.get()->reference);
			if (QueryUtils::checkStmtTypeMatch(follower, rightDesignEntity)) {
				leftColumn.push_back(to_string(stmt.get()->reference));
				rightColumn.push_back(to_string(follower.get()->reference));
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

			shared_ptr<StmtInfo> preceding = pkb.getPreceding(stmt.get()->reference);
			if (preceding != nullptr) {
				column.push_back(to_string(stmt.get()->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(rightStmt.stmtRef, column);
		return result;
	}
	else if (leftStmt.type == StmtRefType::stmtNumber && rightStmt.type == StmtRefType::synonym) {
		shared_ptr<StmtInfo> stmt = pkb.getFollower(stoul(leftStmt.stmtRef));
		DesignEntity designEntity = map[rightStmt.stmtRef];
		vector<string> column;

		if (QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
			column.push_back(to_string(stmt.get()->reference));
		}
		QueryResult result = QueryResult();
		result.addColumn(rightStmt.stmtRef, column);
		return result;
	}

 	return QueryResult();
}
