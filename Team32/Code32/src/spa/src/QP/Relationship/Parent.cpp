#include "QP/Relationship/Parent.h"

#include <utility>

Parent::Parent(QueryStmtRef parent_stmt, QueryStmtRef child_stmt)
	: parent_stmt(std::move(std::move(parent_stmt))), child_stmt(std::move(std::move(child_stmt))) {}

QueryStmtRef Parent::getParentStmt() {
	return parent_stmt;
}

QueryStmtRef Parent::getChildStmt() {
	return child_stmt;
}

QueryResult Parent::execute(PKB& pkb, bool is_trivial, unordered_map<string, DesignEntity>& map) {
	return is_trivial ? executeTrivial(pkb, map) : executeNonTrivial(pkb, map);
}

vector<string> Parent::getDeclarationSymbols() {
	vector<string> declaration_symbols;
	if (this->parent_stmt.type == StmtRefType::Synonym) {
		declaration_symbols.push_back(this->parent_stmt.stmt_ref);
	}
	if (this->child_stmt.type == StmtRefType::Synonym) {
		declaration_symbols.push_back(this->child_stmt.stmt_ref);
	}
	return declaration_symbols;
}

QueryResult Parent::executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	if (parent_stmt.type == StmtRefType::StmtNumber && child_stmt.type == StmtRefType::StmtNumber) {
		return QueryResult(pkb.checkParents(stoul(parent_stmt.stmt_ref), stoul(child_stmt.stmt_ref)));
	}
	if (parent_stmt.type == StmtRefType::StmtNumber && child_stmt.type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmtSet = pkb.getChildren(stoul(parent_stmt.stmt_ref));
		return QueryResult(!stmtSet.empty());
	} else if (parent_stmt.type == StmtRefType::StmtNumber && child_stmt.type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmtSet = pkb.getChildren(stoul(parent_stmt.stmt_ref));
		DesignEntity designEntity = map[child_stmt.stmt_ref];

		if (designEntity == DesignEntity::Stmt) {
			return QueryResult(!stmtSet.empty());
		}

		for (auto const& stmt : stmtSet) {
			if (QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				return QueryResult(true);
			}
		}
	} else if (parent_stmt.type == StmtRefType::Underscore && child_stmt.type == StmtRefType::StmtNumber) {
		shared_ptr<StmtInfo> parent = pkb.getParent(stoul(child_stmt.stmt_ref));
		return QueryResult(parent.get() != nullptr);
	} else if (parent_stmt.type == StmtRefType::Underscore && child_stmt.type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		for (auto const& stmt : stmtSet) {
			StmtInfoPtrSet children = pkb.getChildren(stmt.get()->reference);
			if (!children.empty()) {
				return QueryResult(true);
			}
		}
	} else if (parent_stmt.type == StmtRefType::Underscore && child_stmt.type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[child_stmt.stmt_ref];
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			shared_ptr<StmtInfo> parent = pkb.getParent(stmt.get()->reference);
			if (parent != nullptr) {
				return QueryResult(true);
			}
		}
	} else if (parent_stmt.type == StmtRefType::Synonym && child_stmt.type == StmtRefType::StmtNumber) {
		shared_ptr<StmtInfo> parent = pkb.getParent(stoul(child_stmt.stmt_ref));
		DesignEntity designEntity = map[parent_stmt.stmt_ref];
		if (!QueryUtils::checkStmtTypeMatch(parent, designEntity)) {
			return QueryResult();
		}
		return QueryResult(true);
	} else if (parent_stmt.type == StmtRefType::Synonym && child_stmt.type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[parent_stmt.stmt_ref];
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildren(stmt.get()->reference);
			if (!children.empty()) {
				return QueryResult(true);
			}
		}
	} else if (parent_stmt.type == StmtRefType::Synonym && child_stmt.type == StmtRefType::Synonym) {
		if (parent_stmt.stmt_ref == child_stmt.stmt_ref) {
			return QueryResult();
		}

		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity parentDesignEntity = map[parent_stmt.stmt_ref];
		DesignEntity childDesignEntity = map[child_stmt.stmt_ref];
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

	return {};
}

QueryResult Parent::executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	if (parent_stmt.type == StmtRefType::Synonym && child_stmt.type == StmtRefType::StmtNumber) {
		shared_ptr<StmtInfo> parent = pkb.getParent(stoul(child_stmt.stmt_ref));
		DesignEntity design_entity = map[parent_stmt.stmt_ref];
		vector<string> column;
		if (!QueryUtils::checkStmtTypeMatch(parent, design_entity)) {
			return {};
		}
		column.push_back(to_string(parent->reference));
		QueryResult result = QueryResult();
		result.addColumn(parent_stmt.stmt_ref, column);
		return result;
	}
	if (parent_stmt.type == StmtRefType::Synonym && child_stmt.type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[parent_stmt.stmt_ref];
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
		result.addColumn(parent_stmt.stmt_ref, column);
		return result;
	} else if (parent_stmt.type == StmtRefType::Synonym && child_stmt.type == StmtRefType::Synonym) {
		if (parent_stmt.stmt_ref == child_stmt.stmt_ref) {
			return QueryResult();
		}

		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity parentDesignEntity = map[parent_stmt.stmt_ref];
		DesignEntity childDesignEntity = map[child_stmt.stmt_ref];
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
		result.addColumn(parent_stmt.stmt_ref, parentColumn);
		result.addColumn(child_stmt.stmt_ref, childColumn);
		return result;
	} else if (parent_stmt.type == StmtRefType::Underscore && child_stmt.type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[child_stmt.stmt_ref];
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
		result.addColumn(child_stmt.stmt_ref, column);
		return result;
	} else if (parent_stmt.type == StmtRefType::StmtNumber && child_stmt.type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmtSet = pkb.getChildren(stoul(parent_stmt.stmt_ref));
		DesignEntity designEntity = map[child_stmt.stmt_ref];
		vector<string> column;

		for (auto const& stmt : stmtSet) {
			if (QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				column.push_back(to_string(stmt.get()->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(child_stmt.stmt_ref, column);
		return result;
	}

	return {};
}
