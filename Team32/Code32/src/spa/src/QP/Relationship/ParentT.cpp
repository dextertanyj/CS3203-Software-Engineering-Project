#include "ParentT.h"

QueryResult ParentT::execute(PKB& pkb, bool is_trivial, unordered_map<string, DesignEntity>& map) {
	return is_trivial ? executeTrivial(pkb, map) : executeNonTrivial(pkb, map);
}

QueryResult ParentT::executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	if (parent_stmt.type == StmtRefType::StmtNumber && child_stmt.type == StmtRefType::StmtNumber) {
		StmtInfoPtrSet children_set = pkb.getChildStar(stoul(parent_stmt.stmt_ref));
		StmtRef child_stmt_no = stoul(child_stmt.stmt_ref);
		for (auto const& child : children_set) {
			if (child->reference == child_stmt_no) {
				return QueryResult(true);
			}
		}
	}
	else if (parent_stmt.type == StmtRefType::StmtNumber && child_stmt.type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getChildStar(stoul(parent_stmt.stmt_ref));
		return QueryResult(!stmt_set.empty());
	}
	else if (parent_stmt.type == StmtRefType::StmtNumber && child_stmt.type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getChildStar(stoul(parent_stmt.stmt_ref));
		DesignEntity design_entity = map[child_stmt.stmt_ref];

		if (design_entity == DesignEntity::Stmt) {
			return QueryResult(!stmt_set.empty());
		}

		for (auto const& stmt : stmt_set) {
			if (QueryUtils::checkStmtTypeMatch(stmt, design_entity)) {
				return QueryResult(true);
			}
		}
	}
	else if (parent_stmt.type == StmtRefType::Underscore && child_stmt.type == StmtRefType::StmtNumber) {
		StmtInfoPtrSet parent_set = pkb.getParentStar(stoul(child_stmt.stmt_ref));
		return QueryResult(!parent_set.empty());
	}
	else if (parent_stmt.type == StmtRefType::Underscore && child_stmt.type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		for (auto const& stmt : stmt_set) {
			StmtInfoPtrSet children = pkb.getChildStar(stmt->reference);
			if (!children.empty()) {
				return QueryResult(true);
			}
		}
	}
	else if (parent_stmt.type == StmtRefType::Underscore && child_stmt.type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[child_stmt.stmt_ref];
		for (auto const& stmt : stmt_set) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet parent_set = pkb.getParentStar(stmt->reference);
			if (!parent_set.empty()) {
				return QueryResult(true);
			}
		}
	}
	else if (parent_stmt.type == StmtRefType::Synonym && child_stmt.type == StmtRefType::StmtNumber) {
		StmtInfoPtrSet parent_set = pkb.getParentStar(stoul(child_stmt.stmt_ref));
		DesignEntity design_entity = map[parent_stmt.stmt_ref];
		for (auto const& parent : parent_set) {
			if (QueryUtils::checkStmtTypeMatch(parent, design_entity)) {
				return QueryResult(true);
			}
		}
	}
	else if (parent_stmt.type == StmtRefType::Synonym && child_stmt.type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[parent_stmt.stmt_ref];
		for (auto const& stmt : stmt_set) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildStar(stmt->reference);
			if (!children.empty()) {
				return QueryResult(true);
			}
		}
	}
	else if (parent_stmt.type == StmtRefType::Synonym && child_stmt.type == StmtRefType::Synonym) {
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

			StmtInfoPtrSet children = pkb.getChildStar(stmt->reference);
			for (auto const& child : children) {
				if (QueryUtils::checkStmtTypeMatch(child, child_design_entity)) {
					return QueryResult(true);
				}
			}
		}
	}

	return {};
}

QueryResult ParentT::executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	if (parent_stmt.type == StmtRefType::Synonym && child_stmt.type == StmtRefType::StmtNumber) {
		StmtInfoPtrSet parent_set = pkb.getParentStar(stoul(child_stmt.stmt_ref));
		DesignEntity design_entity = map[parent_stmt.stmt_ref];
		vector<string> column;
		for (auto const& parent : parent_set) {
			if (QueryUtils::checkStmtTypeMatch(parent, design_entity)) {
				column.push_back(to_string(parent->reference));
			}
		}
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

			StmtInfoPtrSet children = pkb.getChildStar(stmt.get()->reference);
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

			StmtInfoPtrSet children = pkb.getChildStar(stmt.get()->reference);
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

			StmtInfoPtrSet parentSet = pkb.getParentStar(stmt.get()->reference);
			if (!parentSet.empty()) {
				column.push_back(to_string(stmt.get()->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(child_stmt.stmt_ref, column);
		return result;
	} else if (parent_stmt.type == StmtRefType::StmtNumber && child_stmt.type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmtSet = pkb.getChildStar(stoul(parent_stmt.stmt_ref));
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
