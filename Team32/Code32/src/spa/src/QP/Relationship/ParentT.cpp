#include "ParentT.h"

QP::QueryResult QP::Relationship::ParentT::execute(PKB::StorageAccessInterface& pkb, bool is_trivial,
                                                   unordered_map<string, DesignEntity>& map) {
	return is_trivial ? executeTrivial(pkb, map) : executeNonTrivial(pkb, map);
}

QP::QueryResult QP::Relationship::ParentT::executeTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) {
	if (getParentStmt().type == StmtRefType::StmtNumber && getChildStmt().type == StmtRefType::StmtNumber) {
		StmtInfoPtrSet children_set = pkb.getChildStar(stoul(getParentStmt().stmt_ref));
		StmtRef child_stmt_no = stoul(getChildStmt().stmt_ref);
		for (auto const& child : children_set) {
			if (child->reference == child_stmt_no) {
				return QueryResult(true);
			}
		}
	} else if (getParentStmt().type == StmtRefType::StmtNumber && getChildStmt().type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getChildStar(stoul(getParentStmt().stmt_ref));
		return QueryResult(!stmt_set.empty());
	} else if (getParentStmt().type == StmtRefType::StmtNumber && getChildStmt().type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getChildStar(stoul(getParentStmt().stmt_ref));
		DesignEntity design_entity = map[getChildStmt().stmt_ref];

		if (design_entity == DesignEntity::Stmt) {
			return QueryResult(!stmt_set.empty());
		}

		for (auto const& stmt : stmt_set) {
			if (Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				return QueryResult(true);
			}
		}
	} else if (getParentStmt().type == StmtRefType::Underscore && getChildStmt().type == StmtRefType::StmtNumber) {
		StmtInfoPtrSet parent_set = pkb.getParentStar(stoul(getChildStmt().stmt_ref));
		return QueryResult(!parent_set.empty());
	} else if (getParentStmt().type == StmtRefType::Underscore && getChildStmt().type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		for (auto const& stmt : stmt_set) {
			StmtInfoPtrSet children = pkb.getChildStar(stmt->reference);
			if (!children.empty()) {
				return QueryResult(true);
			}
		}
	} else if (getParentStmt().type == StmtRefType::Underscore && getChildStmt().type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[getChildStmt().stmt_ref];
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet parent_set = pkb.getParentStar(stmt->reference);
			if (!parent_set.empty()) {
				return QueryResult(true);
			}
		}
	} else if (getParentStmt().type == StmtRefType::Synonym && getChildStmt().type == StmtRefType::StmtNumber) {
		StmtInfoPtrSet parent_set = pkb.getParentStar(stoul(getChildStmt().stmt_ref));
		DesignEntity design_entity = map[getParentStmt().stmt_ref];
		for (auto const& parent : parent_set) {
			if (Utilities::checkStmtTypeMatch(parent, design_entity)) {
				return QueryResult(true);
			}
		}
	} else if (getParentStmt().type == StmtRefType::Synonym && getChildStmt().type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[getParentStmt().stmt_ref];
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildStar(stmt->reference);
			if (!children.empty()) {
				return QueryResult(true);
			}
		}
	} else if (getParentStmt().type == StmtRefType::Synonym && getChildStmt().type == StmtRefType::Synonym) {
		if (getParentStmt().stmt_ref == getChildStmt().stmt_ref) {
			return {};
		}

		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity parent_design_entity = map[getParentStmt().stmt_ref];
		DesignEntity child_design_entity = map[getChildStmt().stmt_ref];
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, parent_design_entity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildStar(stmt->reference);
			for (auto const& child : children) {
				if (Utilities::checkStmtTypeMatch(child, child_design_entity)) {
					return QueryResult(true);
				}
			}
		}
	}

	return {};
}

QP::QueryResult QP::Relationship::ParentT::executeNonTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) {
	if (getParentStmt().type == StmtRefType::Synonym && getChildStmt().type == StmtRefType::StmtNumber) {
		StmtInfoPtrSet parent_set = pkb.getParentStar(stoul(getChildStmt().stmt_ref));
		DesignEntity design_entity = map[getParentStmt().stmt_ref];
		vector<string> column;
		for (auto const& parent : parent_set) {
			if (Utilities::checkStmtTypeMatch(parent, design_entity)) {
				column.push_back(to_string(parent->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(getParentStmt().stmt_ref, column);
		return result;
	}
	if (getParentStmt().type == StmtRefType::Synonym && getChildStmt().type == StmtRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[getParentStmt().stmt_ref];
		vector<string> column;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildStar(stmt->reference);
			if (!children.empty()) {
				column.push_back(to_string(stmt->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(getParentStmt().stmt_ref, column);
		return result;
	}
	if (getParentStmt().type == StmtRefType::Synonym && getChildStmt().type == StmtRefType::Synonym) {
		if (getParentStmt().stmt_ref == getChildStmt().stmt_ref) {
			return {};
		}

		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity parent_design_entity = map[getParentStmt().stmt_ref];
		DesignEntity child_design_entity = map[getChildStmt().stmt_ref];
		vector<string> parent_column;
		vector<string> child_column;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, parent_design_entity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildStar(stmt->reference);
			for (auto const& child : children) {
				if (Utilities::checkStmtTypeMatch(child, child_design_entity)) {
					parent_column.push_back(to_string(stmt->reference));
					child_column.push_back(to_string(child->reference));
				}
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(getParentStmt().stmt_ref, parent_column);
		result.addColumn(getChildStmt().stmt_ref, child_column);
		return result;
	}
	if (getParentStmt().type == StmtRefType::Underscore && getChildStmt().type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[getChildStmt().stmt_ref];
		vector<string> column;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet parent_set = pkb.getParentStar(stmt->reference);
			if (!parent_set.empty()) {
				column.push_back(to_string(stmt->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(getChildStmt().stmt_ref, column);
		return result;
	}
	if (getParentStmt().type == StmtRefType::StmtNumber && getChildStmt().type == StmtRefType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getChildStar(stoul(getParentStmt().stmt_ref));
		DesignEntity design_entity = map[getChildStmt().stmt_ref];
		vector<string> column;

		for (auto const& stmt : stmt_set) {
			if (Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				column.push_back(to_string(stmt->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(getChildStmt().stmt_ref, column);
		return result;
	}

	return {};
}
