#include "ParentT.h"

QP::QueryResult QP::Relationship::ParentT::executeTrivial(PKB::StorageAccessInterface& pkb) {
	if (getParentStmt().getType() == ReferenceType::StatementIndex && getChildStmt().getType() == ReferenceType::StatementIndex) {
		return executeTrivialIndexIndex(pkb, getParentStmt(), getChildStmt());
	}
	if (getParentStmt().getType() == ReferenceType::StatementIndex && getChildStmt().getType() == ReferenceType::Wildcard) {
		return executeTrivialIndexWildcard(pkb, getParentStmt());
	}
	if (getParentStmt().getType() == ReferenceType::StatementIndex && getChildStmt().getType() == ReferenceType::Synonym) {
		return executeTrivialIndexSynonym(pkb, getParentStmt(), getChildStmt());
	}
	if (getParentStmt().getType() == ReferenceType::Wildcard && getChildStmt().getType() == ReferenceType::StatementIndex) {
		return executeTrivialWildcardIndex(pkb, getChildStmt());
	}
	if (getParentStmt().getType() == ReferenceType::Wildcard && getChildStmt().getType() == ReferenceType::Wildcard) {
		return executeTrivialWildcardWildcard(pkb);
	}
	if (getParentStmt().getType() == ReferenceType::Wildcard && getChildStmt().getType() == ReferenceType::Synonym) {
		return executeTrivialWildcardSynonym(pkb, getChildStmt());
	}
	if (getParentStmt().getType() == ReferenceType::Synonym && getChildStmt().getType() == ReferenceType::StatementIndex) {
		return executeTrivialSynonymIndex(pkb, getParentStmt(), getChildStmt());
	}
	if (getParentStmt().getType() == ReferenceType::Synonym && getChildStmt().getType() == ReferenceType::Wildcard) {
		return executeTrivialSynonymWildcard(pkb, getParentStmt());
	}
	if (getParentStmt().getType() == ReferenceType::Synonym && getChildStmt().getType() == ReferenceType::Synonym) {
		return executeTrivialSynonymSynonym(pkb, getParentStmt(), getChildStmt());
	}

	return {};
}

QP::QueryResult QP::Relationship::ParentT::executeNonTrivial(PKB::StorageAccessInterface& pkb) {
	if (getParentStmt().getType() == ReferenceType::Synonym && getChildStmt().getType() == ReferenceType::StatementIndex) {
		return executeSynonymIndex(pkb, getParentStmt(), getChildStmt());
	}
	if (getParentStmt().getType() == ReferenceType::Synonym && getChildStmt().getType() == ReferenceType::Wildcard) {
		return executeSynonymWildcard(pkb, getParentStmt());
	}
	if (getParentStmt().getType() == ReferenceType::Synonym && getChildStmt().getType() == ReferenceType::Synonym) {
		return executeSynonymSynonym(pkb, getParentStmt(), getChildStmt());
	}
	if (getParentStmt().getType() == ReferenceType::Wildcard && getChildStmt().getType() == ReferenceType::Synonym) {
		return executeWildcardSynonym(pkb, getChildStmt());
	}
	if (getParentStmt().getType() == ReferenceType::StatementIndex && getChildStmt().getType() == ReferenceType::Synonym) {
		return executeIndexSynonym(pkb, getParentStmt(), getChildStmt());
	}

	return {};
}

// Trivial Executors

QP::QueryResult QP::Relationship::ParentT::executeTrivialIndexIndex(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent,
                                                                    const ReferenceArgument& child) {
	StmtInfoPtrSet children = pkb.getChildStar(parent.getStatementIndex());
	StmtRef child_index = child.getStatementIndex();
	for (auto const& child_statement : children) {
		if (child_statement->getIdentifier() == child_index) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::ParentT::executeTrivialIndexWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent) {
	return QueryResult(!pkb.getChildStar(parent.getStatementIndex()).empty());
}

QP::QueryResult QP::Relationship::ParentT::executeTrivialIndexSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent,
                                                                      const ReferenceArgument& child) {
	StmtInfoPtrSet children = pkb.getChildStar(parent.getStatementIndex());
	if (child.getSynonym().type == DesignEntity::Stmt) {
		return QueryResult(!children.empty());
	}

	for (auto const& child_statement : children) {
		if (Utilities::checkStmtTypeMatch(child_statement, child.getSynonym().type)) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::ParentT::executeTrivialWildcardIndex(PKB::StorageAccessInterface& pkb, const ReferenceArgument& child) {
	return QueryResult(!pkb.getParentStar(child.getStatementIndex()).empty());
}

QP::QueryResult QP::Relationship::ParentT::executeTrivialWildcardWildcard(PKB::StorageAccessInterface& pkb) {
	StmtInfoPtrSet statements = pkb.getStatements();
	for (auto const& statement : statements) {
		// Avoid call to PKB unless statement can contain others.
		if (statement->getType() != StmtType::IfStmt && statement->getType() != StmtType::WhileStmt) {
			continue;
		}
		StmtInfoPtrSet children = pkb.getChildStar(statement->getIdentifier());
		if (!children.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::ParentT::executeTrivialWildcardSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& child) {
	StmtInfoPtrSet statements = pkb.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, child.getSynonym().type)) {
			continue;
		}
		StmtInfoPtrSet parents = pkb.getParentStar(statement->getIdentifier());
		if (!parents.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::ParentT::executeTrivialSynonymIndex(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent,
                                                                      const ReferenceArgument& child) {
	StmtInfoPtrSet parents = pkb.getParentStar(child.getStatementIndex());
	for (auto const& parent_statement : parents) {
		if (Utilities::checkStmtTypeMatch(parent_statement, parent.getSynonym().type)) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::ParentT::executeTrivialSynonymWildcard(PKB::StorageAccessInterface& pkb,
                                                                         const ReferenceArgument& parent) {
	StmtInfoPtrSet statements = pkb.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, parent.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet children = pkb.getChildStar(statement->getIdentifier());
		if (!children.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::ParentT::executeTrivialSynonymSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent,
                                                                        const ReferenceArgument& child) {
	if (parent.getSynonym().symbol == child.getSynonym().symbol) {
		return {};
	}

	StmtInfoPtrSet statements = pkb.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, parent.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet children = pkb.getChildStar(statement->getIdentifier());
		for (auto const& child_statement : children) {
			if (Utilities::checkStmtTypeMatch(child_statement, child.getSynonym().type)) {
				return QueryResult(true);
			}
		}
	}

	return {};
}

// Executors

QP::QueryResult QP::Relationship::ParentT::executeIndexSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent,
                                                               const ReferenceArgument& child) {
	StmtInfoPtrSet statements = pkb.getChildStar(parent.getStatementIndex());
	vector<string> column;

	for (auto const& statement : statements) {
		if (Utilities::checkStmtTypeMatch(statement, child.getSynonym().type)) {
			column.push_back(to_string(statement->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(child.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::ParentT::executeWildcardSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& child) {
	StmtInfoPtrSet statements = pkb.getStatements();
	vector<string> column;
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, child.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet parents = pkb.getParentStar(statement->getIdentifier());
		if (!parents.empty()) {
			column.push_back(to_string(statement->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(child.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::ParentT::executeSynonymIndex(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent,
                                                               const ReferenceArgument& child) {
	StmtInfoPtrSet parents = pkb.getParentStar(child.getStatementIndex());
	vector<string> column;
	for (auto const& parent_statement : parents) {
		if (Utilities::checkStmtTypeMatch(parent_statement, parent.getSynonym().type)) {
			column.push_back(to_string(parent_statement->getIdentifier()));
		}
	}

	QueryResult result = QueryResult();
	result.addColumn(parent.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::ParentT::executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent) {
	StmtInfoPtrSet statements = pkb.getStatements();
	vector<string> column;
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, parent.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet children = pkb.getChildStar(statement->getIdentifier());
		if (!children.empty()) {
			column.push_back(to_string(statement->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(parent.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::ParentT::executeSynonymSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent,
                                                                 const ReferenceArgument& child) {
	if (parent.getSynonym().symbol == child.getSynonym().symbol) {
		return {};
	}

	StmtInfoPtrSet statements = pkb.getStatements();
	vector<string> parent_column;
	vector<string> child_column;
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, parent.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet children_statements = pkb.getChildStar(statement->getIdentifier());
		for (auto const& child_statement : children_statements) {
			if (Utilities::checkStmtTypeMatch(child_statement, child.getSynonym().type)) {
				parent_column.push_back(to_string(statement->getIdentifier()));
				child_column.push_back(to_string(child_statement->getIdentifier()));
			}
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(parent.getSynonym().symbol, parent_column);
	result.addColumn(child.getSynonym().symbol, child_column);
	return result;
}
