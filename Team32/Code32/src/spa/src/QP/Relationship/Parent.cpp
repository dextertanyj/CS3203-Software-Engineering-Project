#include "QP/Relationship/Parent.h"

Parent::Parent(bool isStar, QueryStmtRef parentStmt, QueryStmtRef childStmt)
	: isStar(isStar),
	  parentStmt(parentStmt),
	  childStmt(childStmt) {}

QueryStmtRef Parent::getParentStmt() {
	return parentStmt;
}

QueryStmtRef Parent::getChildStmt() {
	return childStmt;
}

bool Parent::getIsStar() {
	return isStar;
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
		return QueryResult(pkb.checkParents(stoi(parentStmt.stmtRef), stoi(childStmt.stmtRef)));
	}
	else if (parentStmt.type == StmtRefType::stmtNumber && childStmt.type == StmtRefType::underscore) {
		StmtInfoPtrSet stmtSet = pkb.getChildren(stoi(parentStmt.stmtRef));
		return QueryResult(!stmtSet.empty());
	}
	else if (parentStmt.type == StmtRefType::stmtNumber && childStmt.type == StmtRefType::synonym) {
		StmtInfoPtrSet stmtSet = pkb.getChildren(stoi(parentStmt.stmtRef));
		DesignEntity designEntity = map[childStmt.stmtRef];
		
		if (designEntity == DesignEntity::stmt) {
			return QueryResult(!stmtSet.empty());
		}

		for (auto const &stmt : stmtSet) {
			if (stmt.get()->type == QueryUtils::designEntToStmtType[designEntity]) {
				return QueryResult(true);
			}
		}
		return QueryResult();
	}
	else if (parentStmt.type == StmtRefType::underscore && childStmt.type == StmtRefType::stmtNumber) {
		shared_ptr<StmtInfo> parentStmt = pkb.getParent(stoi(childStmt.stmtRef));
		return QueryResult(parentStmt.get() != nullptr);
	}
	else if (parentStmt.type == StmtRefType::underscore && childStmt.type == StmtRefType::underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		for (auto const& stmt : stmtSet) {
			StmtInfoPtrSet children = pkb.getChildren(stmt.get()->reference);
			if (!children.empty()) {
				return QueryResult(true);
			}
		}
		return QueryResult();
	}
	else if (parentStmt.type == StmtRefType::underscore && childStmt.type == StmtRefType::synonym) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[childStmt.stmtRef];
		for (auto const& stmt : stmtSet) {
			if (designEntity != DesignEntity::stmt &&
			    stmt.get()->type != QueryUtils::designEntToStmtType[designEntity]) {
				continue;
			}

			shared_ptr<StmtInfo> parent = pkb.getParent(stmt.get()->reference);
			if (parent != nullptr) {
				return QueryResult(true);
			}
		}
		return QueryResult();
	}
	else if (parentStmt.type == StmtRefType::synonym && childStmt.type == StmtRefType::stmtNumber) {
		shared_ptr<StmtInfo> parent = pkb.getParent(stoi(childStmt.stmtRef));
		DesignEntity designEntity = map[parentStmt.stmtRef];
		if (parent.get() == nullptr || parent.get()->type != QueryUtils::designEntToStmtType[designEntity]) {
			return QueryResult();
		}
		return QueryResult(true);
	}
	else if (parentStmt.type == StmtRefType::synonym && childStmt.type == StmtRefType::underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[parentStmt.stmtRef];
		for (auto const& stmt : stmtSet) {
			if (designEntity != DesignEntity::stmt &&
			    stmt.get()->type != QueryUtils::designEntToStmtType[designEntity]) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildren(stmt.get()->reference);
			if (!children.empty()) {
				return QueryResult(true);
			}
		}
		return QueryResult();
	}
	
	return QueryResult();
}

QueryResult Parent::executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	if (parentStmt.type == StmtRefType::synonym && childStmt.type == StmtRefType::stmtNumber) {
		shared_ptr<StmtInfo> parent = pkb.getParent(stoi(childStmt.stmtRef));
		DesignEntity designEntity = map[parentStmt.stmtRef];
		vector<string> column;
		if (parent.get() == nullptr || parent.get()->type != QueryUtils::designEntToStmtType[designEntity]) {
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
			if (designEntity != DesignEntity::stmt &&
			    stmt.get()->type != QueryUtils::designEntToStmtType[designEntity]) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildren(stmt.get()->reference);
			for (auto const& child : children) {
				column.push_back(to_string(child.get()->reference));
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
			if (parentDesignEntity != DesignEntity::stmt &&
			    stmt.get()->type != QueryUtils::designEntToStmtType[parentDesignEntity]) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildren(stmt.get()->reference);
			for (auto const& child : children) {
				if (childDesignEntity == DesignEntity::stmt ||
					  stmt.get()->type == QueryUtils::designEntToStmtType[parentDesignEntity]) {
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
			if (designEntity != DesignEntity::stmt &&
			    stmt.get()->type != QueryUtils::designEntToStmtType[designEntity]) {
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
		StmtInfoPtrSet stmtSet = pkb.getChildren(stoi(parentStmt.stmtRef));
		DesignEntity designEntity = map[childStmt.stmtRef];
		vector<string> column;

		for (auto const &stmt : stmtSet) {
			if (designEntity == DesignEntity::stmt ||
				  stmt.get()->type == QueryUtils::designEntToStmtType[designEntity]) {
				column.push_back(to_string(stmt.get()->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(childStmt.stmtRef, column);
		return result;
	}

 	return QueryResult();
}
