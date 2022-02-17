#include "QP/Relationship/Parent.h"

Parent::Parent(QueryStmtRef parentStmt, QueryStmtRef childStmt)
	: parentStmt(parentStmt),
	  childStmt(childStmt) {}

QueryStmtRef Parent::getParentStmt() {
	return parentStmt;
}

QueryStmtRef Parent::getChildStmt() {
	return childStmt;
}

QueryResult Parent::execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map) {
	return isTrivial ? executeTrivial(pkb, map) : executeNonTrivial(pkb, map);
}

vector<string> Parent::getDeclarationSymbols() {
	vector<string> declarationSymbols;
	if (this->parentStmt.type == StmtRefType::synonym) {
		declarationSymbols.push_back(this->parentStmt.stmtRef);
	}
	if (this->childStmt.type == StmtRefType::synonym) {
		declarationSymbols.push_back(this->childStmt.stmtRef);
	}
	return declarationSymbols;
}

QueryResult Parent::executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	if (parentStmt.type == StmtRefType::stmtNumber && childStmt.type == StmtRefType::stmtNumber) {
		return QueryResult(pkb.checkParents(stoul(parentStmt.stmtRef), stoul(childStmt.stmtRef)));
	}
	else if (parentStmt.type == StmtRefType::stmtNumber && childStmt.type == StmtRefType::underscore) {
		StmtInfoPtrSet stmtSet = pkb.getChildren(stoul(parentStmt.stmtRef));
		return QueryResult(!stmtSet.empty());
	}
	else if (parentStmt.type == StmtRefType::stmtNumber && childStmt.type == StmtRefType::synonym) {
		StmtInfoPtrSet stmtSet = pkb.getChildren(stoul(parentStmt.stmtRef));
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
		shared_ptr<StmtInfo> parent = pkb.getParent(stoul(childStmt.stmtRef));
		return QueryResult(parent.get() != nullptr);
	}
	else if (parentStmt.type == StmtRefType::underscore && childStmt.type == StmtRefType::underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		for (auto const& stmt : stmtSet) {
			StmtInfoPtrSet children = pkb.getChildren(stmt.get()->reference);
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

			shared_ptr<StmtInfo> parent = pkb.getParent(stmt.get()->reference);
			if (parent != nullptr) {
				return QueryResult(true);
			}
		}
	}
	else if (parentStmt.type == StmtRefType::synonym && childStmt.type == StmtRefType::stmtNumber) {
		shared_ptr<StmtInfo> parent = pkb.getParent(stoul(childStmt.stmtRef));
		DesignEntity designEntity = map[parentStmt.stmtRef];
		if (!QueryUtils::checkStmtTypeMatch(parent, designEntity)) {
			return QueryResult();
		}
		return QueryResult(true);
	}
	else if (parentStmt.type == StmtRefType::synonym && childStmt.type == StmtRefType::underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[parentStmt.stmtRef];
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildren(stmt.get()->reference);
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

			StmtInfoPtrSet children = pkb.getChildren(stmt.get()->reference);
			for (auto const& child : children) {
				if (QueryUtils::checkStmtTypeMatch(child, childDesignEntity)) {
					return QueryResult(true);
				}
			}
		}
	}
	
	return QueryResult();
}

QueryResult Parent::executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	if (parentStmt.type == StmtRefType::synonym && childStmt.type == StmtRefType::stmtNumber) {
		shared_ptr<StmtInfo> parent = pkb.getParent(stoul(childStmt.stmtRef));
		DesignEntity designEntity = map[parentStmt.stmtRef];
		vector<string> column;
		if (!QueryUtils::checkStmtTypeMatch(parent, designEntity)) {
			return QueryResult();
		}
		column.push_back(to_string(parent.get()->reference));
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

			StmtInfoPtrSet children = pkb.getChildren(stmt.get()->reference);
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

			StmtInfoPtrSet children = pkb.getChildren(stmt.get()->reference);
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

			shared_ptr<StmtInfo> parent = pkb.getParent(stmt.get()->reference);
			if (parent != nullptr) {
				column.push_back(to_string(stmt.get()->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(childStmt.stmtRef, column);
		return result;
	}
	else if (parentStmt.type == StmtRefType::stmtNumber && childStmt.type == StmtRefType::synonym) {
		StmtInfoPtrSet stmtSet = pkb.getChildren(stoul(parentStmt.stmtRef));
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
