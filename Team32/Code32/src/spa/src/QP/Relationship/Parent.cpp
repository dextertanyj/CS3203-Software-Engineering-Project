#include "QP/Relationship/Parent.h"

#include <utility>

Parent::Parent(QueryStmtRef parent_stmt, QueryStmtRef child_stmt)
	: parent_stmt(std::move(std::move(parent_stmt))), child_stmt(std::move(std::move(child_stmt))) {}

QueryStmtRef Parent::getParentStmt() { return parent_stmt; }

QueryStmtRef Parent::getChildStmt() { return child_stmt; }

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
		StmtInfoPtrSet stmt_set = pkb.getChildren(stoul(parent_stmt.stmt_ref));
		return QueryResult(!stmt_set.empty());
	}
	if (parent_stmt.type == StmtRefType::StmtNumber && child_stmt.type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getChildren(stoul(parent_stmt.stmt_ref));
		DesignEntity design_entity = map[child_stmt.stmt_ref];

		if (design_entity == DesignEntity::Stmt) {
			return QueryResult(!stmt_set.empty());
		}

		for (auto const& stmt : stmt_set) {
			if (QueryUtils::checkStmtTypeMatch(stmt, design_entity)) {
				return QueryResult(true);
			}
		}
	} else if (parent_stmt.type == StmtRefType::Underscore && child_stmt.type == StmtRefType::StmtNumber) {
		shared_ptr<StmtInfo> parent = pkb.getParent(stoul(child_stmt.stmt_ref));
		return QueryResult(parent != nullptr);
	} else if (parent_stmt.type == StmtRefType::Underscore && child_stmt.type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		for (auto const& stmt : stmt_set) {
			StmtInfoPtrSet children = pkb.getChildren(stmt->reference);
			if (!children.empty()) {
				return QueryResult(true);
			}
		}
	} else if (parent_stmt.type == StmtRefType::Underscore && child_stmt.type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[child_stmt.stmt_ref];
		for (auto const& stmt : stmt_set) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			shared_ptr<StmtInfo> parent = pkb.getParent(stmt->reference);
			if (parent != nullptr) {
				return QueryResult(true);
			}
		}
	} else if (parent_stmt.type == StmtRefType::Synonym && child_stmt.type == StmtRefType::StmtNumber) {
		shared_ptr<StmtInfo> parent = pkb.getParent(stoul(child_stmt.stmt_ref));
		DesignEntity design_entity = map[parent_stmt.stmt_ref];
		if (!QueryUtils::checkStmtTypeMatch(parent, design_entity)) {
			return {};
		}
		return QueryResult(true);
	} else if (parent_stmt.type == StmtRefType::Synonym && child_stmt.type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[parent_stmt.stmt_ref];
		for (auto const& stmt : stmt_set) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildren(stmt->reference);
			if (!children.empty()) {
				return QueryResult(true);
			}
		}
	} else if (parent_stmt.type == StmtRefType::Synonym && child_stmt.type == StmtRefType::Synonym) {
		if (parent_stmt.stmt_ref == child_stmt.stmt_ref) {
			return {};
		}

		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity parent_design_entity = map[parent_stmt.stmt_ref];
		DesignEntity child_design_entity = map[child_stmt.stmt_ref];
		for (auto const& stmt : stmt_set) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, parent_design_entity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildren(stmt->reference);
			for (auto const& child : children) {
				if (QueryUtils::checkStmtTypeMatch(child, child_design_entity)) {
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
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[parent_stmt.stmt_ref];
		vector<string> column;
		for (auto const& stmt : stmt_set) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildren(stmt->reference);
			if (!children.empty()) {
				column.push_back(to_string(stmt->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(parent_stmt.stmt_ref, column);
		return result;
	}
	if (parent_stmt.type == StmtRefType::Synonym && child_stmt.type == StmtRefType::Synonym) {
		if (parent_stmt.stmt_ref == child_stmt.stmt_ref) {
			return {};
		}

		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity parent_design_entity = map[parent_stmt.stmt_ref];
		DesignEntity child_design_entity = map[child_stmt.stmt_ref];
		vector<string> parent_column;
		vector<string> child_column;
		for (auto const& stmt : stmt_set) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, parent_design_entity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildren(stmt->reference);
			for (auto const& child : children) {
				if (QueryUtils::checkStmtTypeMatch(child, child_design_entity)) {
					parent_column.push_back(to_string(stmt->reference));
					child_column.push_back(to_string(child->reference));
				}
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(parent_stmt.stmt_ref, parent_column);
		result.addColumn(child_stmt.stmt_ref, child_column);
		return result;
	}
	if (parent_stmt.type == StmtRefType::Underscore && child_stmt.type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[child_stmt.stmt_ref];
		vector<string> column;
		for (auto const& stmt : stmt_set) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			shared_ptr<StmtInfo> parent = pkb.getParent(stmt->reference);
			if (parent != nullptr) {
				column.push_back(to_string(stmt->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(child_stmt.stmt_ref, column);
		return result;
	}
	if (parent_stmt.type == StmtRefType::StmtNumber && child_stmt.type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getChildren(stoul(parent_stmt.stmt_ref));
		DesignEntity design_entity = map[child_stmt.stmt_ref];
		vector<string> column;

		for (auto const& stmt : stmt_set) {
			if (QueryUtils::checkStmtTypeMatch(stmt, design_entity)) {
				column.push_back(to_string(stmt->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(child_stmt.stmt_ref, column);
		return result;
	}

	return {};
}
