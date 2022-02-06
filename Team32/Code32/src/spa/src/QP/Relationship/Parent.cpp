#include "QP/Parent.h"

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

bool Parent::execute(PKB& pkb, QueryResult& result) {
	return isTrivialCase() ? executeTrivial(pkb) : executeNonTrivial(pkb, result);
}

bool Parent::isTrivialCase() {
	return this->parentStmt.type != StmtRefType::synonym && this->childStmt.type != StmtRefType::synonym;
}

bool Parent::executeTrivial(PKB& pkb) {
	if (this->parentStmt.type == StmtRefType::stmtNumber) {
		int parent = stoi(this->parentStmt.stmtRef);

		switch (this->childStmt.type) {
		case StmtRefType::stmtNumber: {
			int child = stoi(this->childStmt.stmtRef);
			return pkb.checkParents(parent, child);
		}
		case StmtRefType::underscore: {
			unordered_set<StmtRef> stmtSet = pkb.getChildren(parent);
			return !stmtSet.empty();
		}
		default: {
			return false;
		}
		}
	}

	if (this->parentStmt.type == StmtRefType::underscore) {
		switch (this->childStmt.type) {
		case StmtRefType::stmtNumber: {
			int child = stoi(this->childStmt.stmtRef);
			StmtRef stmt = pkb.getParent(child);
			return stmt > 0;
		}
		case StmtRefType::underscore: {
			StmtRefList allStmts = pkb.getStatements();
			for (StmtRef stmt : allStmts) {
				if (!pkb.getChildren(stmt).empty()) {
					return true;
				}
			}
			return false;
		}
		default: {
			return false;
		}
		}
	}

	return false;
}

bool Parent::executeNonTrivial(PKB& pkb, QueryResult& result) {
	StmtRefList stmtList;
	
	if (this->parentStmt.type == StmtRefType::stmtNumber &&
			this->childStmt.type == StmtRefType::synonym) {
		int parent = stoi(this->parentStmt.stmtRef);
		unordered_set<StmtRef> set = pkb.getChildren(parent);
		stmtList.insert(stmtList.end(), set.begin(), set.end());
	}

	else if (this->parentStmt.type == StmtRefType::underscore &&
			this->childStmt.type == StmtRefType::synonym) {
		// Get all stmts with a parent
		StmtRefList allStmts = pkb.getStatements();
		for (StmtRef stmt : allStmts) {
			if (pkb.getParent(stmt) > 0) {
				stmtList.push_back(stmt);
			}
		}
	}
	
	else if (this->childStmt.type == StmtRefType::stmtNumber &&
			this->parentStmt.type == StmtRefType::synonym) {
		int child = stoi(this->childStmt.stmtRef);
		StmtRef parent = pkb.getParent(child);
		stmtList.push_back(parent);
	}

	else if (this->childStmt.type == StmtRefType::underscore &&
			this->parentStmt.type == StmtRefType::synonym) {
		// Get all stmts with a child
		StmtRefList allStmts = pkb.getStatements();
		for (StmtRef stmt : allStmts) {
			if (!pkb.getChildren(stmt).empty()) {
				stmtList.push_back(stmt);
			}
		}
	}
	
	// TODO: Implement the case where both are synonyms

	return result.updateStmtList(stmtList);
}
