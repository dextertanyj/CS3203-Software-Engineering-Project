#include "QP/Relationship/Parent.h"

#include <utility>

QP::Relationship::Parent::Parent(ReferenceArgument parent_stmt, ReferenceArgument child_stmt)
	: parent_stmt(std::move(std::move(parent_stmt))), child_stmt(std::move(std::move(child_stmt))) {}

ReferenceArgument QP::Relationship::Parent::getParentStmt() { return parent_stmt; }

ReferenceArgument QP::Relationship::Parent::getChildStmt() { return child_stmt; }

vector<string> QP::Relationship::Parent::getDeclarationSymbols() {
	vector<string> declaration_symbols;
	if (this->parent_stmt.getType() == ReferenceType::Synonym) {
		declaration_symbols.push_back(this->parent_stmt.getSynonym().symbol);
	}
	if (this->parent_stmt.getType() == ReferenceType::Synonym) {
		declaration_symbols.push_back(this->child_stmt.getSynonym().symbol);
	}
	return declaration_symbols;
}

QP::QueryResult QP::Relationship::Parent::executeTrivial(PKB::StorageAccessInterface& pkb) {
	if (parent_stmt.getType() == ReferenceType::StatementIndex && child_stmt.getType() == ReferenceType::StatementIndex) {
		return QueryResult(pkb.checkParents(parent_stmt.getStatementIndex(), child_stmt.getStatementIndex()));
	}
	if (parent_stmt.getType() == ReferenceType::StatementIndex && child_stmt.getType() == ReferenceType::Wildcard) {
		StmtInfoPtrSet stmt_set = pkb.getChildren(parent_stmt.getStatementIndex());
		return QueryResult(!stmt_set.empty());
	}
	if (parent_stmt.getType() == ReferenceType::StatementIndex && child_stmt.getType() == ReferenceType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getChildren(parent_stmt.getStatementIndex());
		DesignEntity design_entity = child_stmt.getSynonym().type;

		if (design_entity == DesignEntity::Stmt) {
			return QueryResult(!stmt_set.empty());
		}

		for (auto const& stmt : stmt_set) {
			if (Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				return QueryResult(true);
			}
		}
	} else if (parent_stmt.getType() == ReferenceType::Wildcard && child_stmt.getType() == ReferenceType::StatementIndex) {
		shared_ptr<StmtInfo> parent = pkb.getParent(child_stmt.getStatementIndex());
		return QueryResult(parent != nullptr);
	} else if (parent_stmt.getType() == ReferenceType::Wildcard && child_stmt.getType() == ReferenceType::Wildcard) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		for (auto const& stmt : stmt_set) {
			StmtInfoPtrSet children = pkb.getChildren(stmt->getIdentifier());
			if (!children.empty()) {
				return QueryResult(true);
			}
		}
	} else if (parent_stmt.getType() == ReferenceType::Wildcard && child_stmt.getType() == ReferenceType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = child_stmt.getSynonym().type;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			shared_ptr<StmtInfo> parent = pkb.getParent(stmt->getIdentifier());
			if (parent != nullptr) {
				return QueryResult(true);
			}
		}
	} else if (parent_stmt.getType() == ReferenceType::Synonym && child_stmt.getType() == ReferenceType::StatementIndex) {
		shared_ptr<StmtInfo> parent = pkb.getParent(child_stmt.getStatementIndex());
		DesignEntity design_entity = parent_stmt.getSynonym().type;
		if (!Utilities::checkStmtTypeMatch(parent, design_entity)) {
			return {};
		}
		return QueryResult(true);
	} else if (parent_stmt.getType() == ReferenceType::Synonym && child_stmt.getType() == ReferenceType::Wildcard) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = parent_stmt.getSynonym().type;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildren(stmt->getIdentifier());
			if (!children.empty()) {
				return QueryResult(true);
			}
		}
	} else if (parent_stmt.getType() == ReferenceType::Synonym && child_stmt.getType() == ReferenceType::Synonym) {
		if (parent_stmt.getSynonym().symbol == child_stmt.getSynonym().symbol) {
			return {};
		}

		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity parent_design_entity = parent_stmt.getSynonym().type;
		DesignEntity child_design_entity = child_stmt.getSynonym().type;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, parent_design_entity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildren(stmt->getIdentifier());
			for (auto const& child : children) {
				if (Utilities::checkStmtTypeMatch(child, child_design_entity)) {
					return QueryResult(true);
				}
			}
		}
	}

	return {};
}

QP::QueryResult QP::Relationship::Parent::executeNonTrivial(PKB::StorageAccessInterface& pkb) {
	if (parent_stmt.getType() == ReferenceType::Synonym && child_stmt.getType() == ReferenceType::StatementIndex) {
		shared_ptr<StmtInfo> parent = pkb.getParent(child_stmt.getStatementIndex());
		DesignEntity design_entity = parent_stmt.getSynonym().type;
		vector<string> column;
		if (!Utilities::checkStmtTypeMatch(parent, design_entity)) {
			return {};
		}
		column.push_back(to_string(parent->getIdentifier()));
		QueryResult result = QueryResult();
		result.addColumn(parent_stmt.getSynonym().symbol, column);
		return result;
	}
	if (parent_stmt.getType() == ReferenceType::Synonym && child_stmt.getType() == ReferenceType::Wildcard) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = parent_stmt.getSynonym().type;
		vector<string> column;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildren(stmt->getIdentifier());
			if (!children.empty()) {
				column.push_back(to_string(stmt->getIdentifier()));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(parent_stmt.getSynonym().symbol, column);
		return result;
	}
	if (parent_stmt.getType() == ReferenceType::Synonym && child_stmt.getType() == ReferenceType::Synonym) {
		if (parent_stmt.getSynonym().symbol == child_stmt.getSynonym().symbol) {
			return {};
		}

		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity parent_design_entity = parent_stmt.getSynonym().type;
		DesignEntity child_design_entity = child_stmt.getSynonym().type;
		vector<string> parent_column;
		vector<string> child_column;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, parent_design_entity)) {
				continue;
			}

			StmtInfoPtrSet children = pkb.getChildren(stmt->getIdentifier());
			for (auto const& child : children) {
				if (Utilities::checkStmtTypeMatch(child, child_design_entity)) {
					parent_column.push_back(to_string(stmt->getIdentifier()));
					child_column.push_back(to_string(child->getIdentifier()));
				}
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(parent_stmt.getSynonym().symbol, parent_column);
		result.addColumn(child_stmt.getSynonym().symbol, child_column);
		return result;
	}
	if (parent_stmt.getType() == ReferenceType::Wildcard && child_stmt.getType() == ReferenceType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = child_stmt.getSynonym().type;
		vector<string> column;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			shared_ptr<StmtInfo> parent = pkb.getParent(stmt->getIdentifier());
			if (parent != nullptr) {
				column.push_back(to_string(stmt->getIdentifier()));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(child_stmt.getSynonym().symbol, column);
		return result;
	}
	if (parent_stmt.getType() == ReferenceType::StatementIndex && child_stmt.getType() == ReferenceType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getChildren(parent_stmt.getStatementIndex());
		DesignEntity design_entity = child_stmt.getSynonym().type;
		vector<string> column;

		for (auto const& stmt : stmt_set) {
			if (Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				column.push_back(to_string(stmt->getIdentifier()));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(child_stmt.getSynonym().symbol, column);
		return result;
	}

	return {};
}
