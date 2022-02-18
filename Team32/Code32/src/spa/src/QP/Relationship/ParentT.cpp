#include "ParentT.h"

QueryResult ParentT::execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map) {
	return isTrivial ? executeTrivial(pkb, map) : executeNonTrivial(pkb, map);
}

QueryResult ParentT::executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	if (parentStmt.type == StmtRefType::stmtNumber && childStmt.type == StmtRefType::stmtNumber) {
		StmtInfoPtrSet childrenSet = pkb.getChildStar(stoul(parentStmt.stmtRef));
		StmtRef childStmtNo = stoul(childStmt.stmtRef);
		for (auto const& child : childrenSet) {
			if (child.get()->reference == childStmtNo) {
				return QueryResult(true);
			}
		}
	}
	else if (parentStmt.type == StmtRefType::stmtNumber && childStmt.type == StmtRefType::underscore) {
		StmtInfoPtrSet stmtSet = pkb.getChildStar(stoul(parentStmt.stmtRef));
		return QueryResult(!stmtSet.empty());
	}
	else if (parentStmt.type == StmtRefType::stmtNumber && childStmt.type == StmtRefType::synonym) {
		StmtInfoPtrSet stmtSet = pkb.getChildStar(stoul(parentStmt.stmtRef));
		DesignEntity designEntity = map[childStmt.stmtRef];
		
		if (designEntity == DesignEntity::stmt) {
			return QueryResult(!stmtSet.empty());
		}

		for (auto const &stmt : stmtSet) {
			if (QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				return QueryResult(true);
			}
		}
	}
	else if (parentStmt.type == StmtRefType::underscore && childStmt.type == StmtRefType::stmtNumber) {
		StmtInfoPtrSet parentSet = pkb.getParentStar(stoul(childStmt.stmtRef));
		return QueryResult(!parentSet.empty());
	}
	else if (parentStmt.type == StmtRefType::underscore && childStmt.type == StmtRefType::underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		for (auto const& stmt : stmtSet) {
			StmtInfoPtrSet children = pkb.getChildStar(stmt.get()->reference);
			if (!children.empty()) {
				return QueryResult(true);
			}
		}
	}
	else if (parentStmt.type == StmtRefType::underscore && childStmt.type == StmtRefType::synonym) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[childStmt.stmtRef];
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			StmtInfoPtrSet parentSet = pkb.getParentStar(stmt.get()->reference);
			if (!parentSet.empty()) {
				return QueryResult(true);
			}
		}
	}
	else if (parentStmt.type == StmtRefType::synonym && childStmt.type == StmtRefType::stmtNumber) {
		StmtInfoPtrSet parentSet = pkb.getParentStar(stoul(childStmt.stmtRef));
		DesignEntity designEntity = map[parentStmt.stmtRef];
		for (auto const& parent : parentSet) {
			if (QueryUtils::checkStmtTypeMatch(parent, designEntity)) {
				return QueryResult(true);
			}
		}
	}
	else if (parentStmt.type == StmtRefType::synonym && childStmt.type == StmtRefType::underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[parentStmt.stmtRef];
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildStar(stmt.get()->reference);
			if (!children.empty()) {
				return QueryResult(true);
			}
		}
	}
	else if (parentStmt.type == StmtRefType::synonym && childStmt.type == StmtRefType::synonym) {
		if (parentStmt.stmtRef == childStmt.stmtRef) {
			return QueryResult();
		}
		
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity parentDesignEntity = map[parentStmt.stmtRef];
		DesignEntity childDesignEntity = map[childStmt.stmtRef];
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, parentDesignEntity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildStar(stmt.get()->reference);
			for (auto const& child : children) {
				if (QueryUtils::checkStmtTypeMatch(child, childDesignEntity)) {
					return QueryResult(true);
				}
			}
		}
	}
	
	return QueryResult();
}

QueryResult ParentT::executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	if (parentStmt.type == StmtRefType::synonym && childStmt.type == StmtRefType::stmtNumber) {
		StmtInfoPtrSet parentSet = pkb.getParentStar(stoul(childStmt.stmtRef));
		DesignEntity designEntity = map[parentStmt.stmtRef];
		vector<string> column;
		for (auto const& parent : parentSet) {
			if (QueryUtils::checkStmtTypeMatch(parent, designEntity)) {
				column.push_back(to_string(parent.get()->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(parentStmt.stmtRef, column);
		return result;
	}
	else if (parentStmt.type == StmtRefType::synonym && childStmt.type == StmtRefType::underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[parentStmt.stmtRef];
		vector<string> column;
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildStar(stmt.get()->reference);
			if (!children.empty()) {
				column.push_back(to_string(stmt.get()->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(parentStmt.stmtRef, column);
		return result;
	}
	else if (parentStmt.type == StmtRefType::synonym && childStmt.type == StmtRefType::synonym) {
		if (parentStmt.stmtRef == childStmt.stmtRef) {
			return QueryResult();
		}
		
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity parentDesignEntity = map[parentStmt.stmtRef];
		DesignEntity childDesignEntity = map[childStmt.stmtRef];
		vector<string> parentColumn;
		vector<string> childColumn;
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, parentDesignEntity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildStar(stmt.get()->reference);
			for (auto const& child : children) {
				if (QueryUtils::checkStmtTypeMatch(child, childDesignEntity)) {
					parentColumn.push_back(to_string(stmt.get()->reference));
					childColumn.push_back(to_string(child.get()->reference));
				}
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(parentStmt.stmtRef, parentColumn);
		result.addColumn(childStmt.stmtRef, childColumn);
		return result;
	}
	else if (parentStmt.type == StmtRefType::underscore && childStmt.type == StmtRefType::synonym) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[childStmt.stmtRef];
		vector<string> column;
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			StmtInfoPtrSet parentSet = pkb.getParentStar(stmt.get()->reference);
			if (!parentSet.empty()) {
				column.push_back(to_string(stmt.get()->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(childStmt.stmtRef, column);
		return result;
	}
	else if (parentStmt.type == StmtRefType::stmtNumber && childStmt.type == StmtRefType::synonym) {
		StmtInfoPtrSet stmtSet = pkb.getChildStar(stoul(parentStmt.stmtRef));
		DesignEntity designEntity = map[childStmt.stmtRef];
		vector<string> column;

		for (auto const &stmt : stmtSet) {
			if (QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				column.push_back(to_string(stmt.get()->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(childStmt.stmtRef, column);
		return result;
	}

 	return QueryResult();
}
