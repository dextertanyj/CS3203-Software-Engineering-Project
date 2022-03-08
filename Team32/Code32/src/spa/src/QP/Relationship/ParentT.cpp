#include "ParentT.h"

QP::QueryResult QP::Relationship::ParentT::executeTrivial(PKB::StorageAccessInterface& pkb) {
	if (getParentStmt().getType() == ReferenceType::StatementIndex && getChildStmt().getType() == ReferenceType::StatementIndex) {
		StmtInfoPtrSet children_set = pkb.getChildStar(getParentStmt().getStatementIndex());
		StmtRef child_stmt_no = getChildStmt().getStatementIndex();
		for (auto const& child : children_set) {
			if (child->getIdentifier() == child_stmt_no) {
				return QueryResult(true);
			}
		}
	} else if (getParentStmt().getType() == ReferenceType::StatementIndex && getChildStmt().getType() == ReferenceType::Wildcard) {
		StmtInfoPtrSet stmt_set = pkb.getChildStar(getParentStmt().getStatementIndex());
		return QueryResult(!stmt_set.empty());
	} else if (getParentStmt().getType() == ReferenceType::StatementIndex && getChildStmt().getType() == ReferenceType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getChildStar(getParentStmt().getStatementIndex());
		DesignEntity design_entity = getChildStmt().getSynonym().type;

		if (design_entity == DesignEntity::Stmt) {
			return QueryResult(!stmt_set.empty());
		}

		for (auto const& stmt : stmt_set) {
			if (Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				return QueryResult(true);
			}
		}
	} else if (getParentStmt().getType() == ReferenceType::Wildcard && getChildStmt().getType() == ReferenceType::StatementIndex) {
		StmtInfoPtrSet parent_set = pkb.getParentStar(getChildStmt().getStatementIndex());
		return QueryResult(!parent_set.empty());
	} else if (getParentStmt().getType() == ReferenceType::Wildcard && getChildStmt().getType() == ReferenceType::Wildcard) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		for (auto const& stmt : stmt_set) {
			StmtInfoPtrSet children = pkb.getChildStar(stmt->getIdentifier());
			if (!children.empty()) {
				return QueryResult(true);
			}
		}
	} else if (getParentStmt().getType() == ReferenceType::Wildcard && getChildStmt().getType() == ReferenceType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = getChildStmt().getSynonym().type;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet parent_set = pkb.getParentStar(stmt->getIdentifier());
			if (!parent_set.empty()) {
				return QueryResult(true);
			}
		}
	} else if (getParentStmt().getType() == ReferenceType::Synonym && getChildStmt().getType() == ReferenceType::StatementIndex) {
		StmtInfoPtrSet parent_set = pkb.getParentStar(getChildStmt().getStatementIndex());
		DesignEntity design_entity = getParentStmt().getSynonym().type;
		for (auto const& parent : parent_set) {
			if (Utilities::checkStmtTypeMatch(parent, design_entity)) {
				return QueryResult(true);
			}
		}
	} else if (getParentStmt().getType() == ReferenceType::Synonym && getChildStmt().getType() == ReferenceType::Wildcard) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = getParentStmt().getSynonym().type;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildStar(stmt->getIdentifier());
			if (!children.empty()) {
				return QueryResult(true);
			}
		}
	} else if (getParentStmt().getType() == ReferenceType::Synonym && getChildStmt().getType() == ReferenceType::Synonym) {
		if (getParentStmt().getSynonym().symbol == getParentStmt().getSynonym().symbol) {
			return {};
		}

		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity parent_design_entity = getParentStmt().getSynonym().type;
		DesignEntity child_design_entity = getChildStmt().getSynonym().type;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, parent_design_entity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildStar(stmt->getIdentifier());
			for (auto const& child : children) {
				if (Utilities::checkStmtTypeMatch(child, child_design_entity)) {
					return QueryResult(true);
				}
			}
		}
	}

	return {};
}

QP::QueryResult QP::Relationship::ParentT::executeNonTrivial(PKB::StorageAccessInterface& pkb) {
	if (getParentStmt().getType() == ReferenceType::Synonym && getChildStmt().getType() == ReferenceType::StatementIndex) {
		StmtInfoPtrSet parent_set = pkb.getParentStar(getChildStmt().getStatementIndex());
		DesignEntity design_entity = getParentStmt().getSynonym().type;
		vector<string> column;
		for (auto const& parent : parent_set) {
			if (Utilities::checkStmtTypeMatch(parent, design_entity)) {
				column.push_back(to_string(parent->getIdentifier()));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(getParentStmt().getSynonym().symbol, column);
		return result;
	}
	if (getParentStmt().getType() == ReferenceType::Synonym && getChildStmt().getType() == ReferenceType::Wildcard) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = getParentStmt().getSynonym().type;
		vector<string> column;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildStar(stmt->getIdentifier());
			if (!children.empty()) {
				column.push_back(to_string(stmt->getIdentifier()));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(getParentStmt().getSynonym().symbol, column);
		return result;
	}
	if (getParentStmt().getType() == ReferenceType::Synonym && getChildStmt().getType() == ReferenceType::Synonym) {
		if (getParentStmt().getSynonym().symbol == getChildStmt().getSynonym().symbol) {
			return {};
		}

		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity parent_design_entity = getParentStmt().getSynonym().type;
		DesignEntity child_design_entity = getChildStmt().getSynonym().type;
		vector<string> parent_column;
		vector<string> child_column;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, parent_design_entity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildStar(stmt->getIdentifier());
			for (auto const& child : children) {
				if (Utilities::checkStmtTypeMatch(child, child_design_entity)) {
					parent_column.push_back(to_string(stmt->getIdentifier()));
					child_column.push_back(to_string(child->getIdentifier()));
				}
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(getParentStmt().getSynonym().symbol, parent_column);
		result.addColumn(getChildStmt().getSynonym().symbol, child_column);
		return result;
	}
	if (getParentStmt().getType() == ReferenceType::Wildcard && getChildStmt().getType() == ReferenceType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = getChildStmt().getSynonym().type;
		vector<string> column;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet parent_set = pkb.getParentStar(stmt->getIdentifier());
			if (!parent_set.empty()) {
				column.push_back(to_string(stmt->getIdentifier()));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(getChildStmt().getSynonym().symbol, column);
		return result;
	}
	if (getParentStmt().getType() == ReferenceType::StatementIndex && getChildStmt().getType() == ReferenceType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getChildStar(getParentStmt().getStatementIndex());
		DesignEntity design_entity = getChildStmt().getSynonym().type;
		vector<string> column;

		for (auto const& stmt : stmt_set) {
			if (Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				column.push_back(to_string(stmt->getIdentifier()));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(getChildStmt().getSynonym().symbol, column);
		return result;
	}

	return {};
}
