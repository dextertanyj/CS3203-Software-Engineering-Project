#include "QP/Relationship/Parent.h"

#include <utility>

#include "QP/Relationship/ParentDispatcherTemplate.tpp"

QP::Relationship::Parent::Parent(ReferenceArgument parent_stmt, ReferenceArgument child_stmt)
	: parent_stmt(std::move(std::move(parent_stmt))), child_stmt(std::move(std::move(child_stmt))) {}

ReferenceArgument QP::Relationship::Parent::getParentStmt() { return parent_stmt; }

ReferenceArgument QP::Relationship::Parent::getChildStmt() { return child_stmt; }

vector<string> QP::Relationship::Parent::getDeclarationSymbols() {
	vector<string> declaration_symbols;
	if (this->parent_stmt.getType() == ReferenceType::Synonym) {
		declaration_symbols.push_back(this->parent_stmt.getSynonym().symbol);
	}
	if (this->child_stmt.getType() == ReferenceType::Synonym) {
		declaration_symbols.push_back(this->child_stmt.getSynonym().symbol);
	}
	return declaration_symbols;
}

QP::QueryResult QP::Relationship::Parent::executeTrivial(PKB::StorageAccessInterface& pkb) {
	if (parent_stmt.getType() == ReferenceType::StatementIndex && child_stmt.getType() == ReferenceType::StatementIndex) {
		return executeTrivialIndexIndex(pkb, parent_stmt, child_stmt);
	}
	if (parent_stmt.getType() == ReferenceType::StatementIndex && child_stmt.getType() == ReferenceType::Wildcard) {
		return executeTrivialIndexWildcard(pkb, parent_stmt);
	}
	if (parent_stmt.getType() == ReferenceType::StatementIndex && child_stmt.getType() == ReferenceType::Synonym) {
		return executeTrivialIndexSynonym(pkb, parent_stmt, child_stmt);
	}
	if (parent_stmt.getType() == ReferenceType::Wildcard && child_stmt.getType() == ReferenceType::StatementIndex) {
		return executeTrivialWildcardIndex(pkb, child_stmt);
	}
	if (parent_stmt.getType() == ReferenceType::Wildcard && child_stmt.getType() == ReferenceType::Wildcard) {
		return executeTrivialWildcardWildcard(pkb);
	}
	if (parent_stmt.getType() == ReferenceType::Wildcard && child_stmt.getType() == ReferenceType::Synonym) {
		return executeTrivialWildcardSynonym(pkb, child_stmt);
	}
	if (parent_stmt.getType() == ReferenceType::Synonym && child_stmt.getType() == ReferenceType::StatementIndex) {
		return executeTrivialSynonymIndex(pkb, parent_stmt, child_stmt);
	}
	if (parent_stmt.getType() == ReferenceType::Synonym && child_stmt.getType() == ReferenceType::Wildcard) {
		return executeTrivialSynonymWildcard(pkb, parent_stmt);
	}
	if (parent_stmt.getType() == ReferenceType::Synonym && child_stmt.getType() == ReferenceType::Synonym) {
		return executeTrivialSynonymSynonym(pkb, parent_stmt, child_stmt);
	}
	if (parent_stmt.getType() == ReferenceType::Synonym && child_stmt.getType() == ReferenceType::StatementIndex) {
		return executeSynonymIndex(pkb, parent_stmt, child_stmt);
	}
	if (parent_stmt.getType() == ReferenceType::Synonym && child_stmt.getType() == ReferenceType::Wildcard) {
		return executeSynonymWildcard(pkb, parent_stmt);
	}
	if (parent_stmt.getType() == ReferenceType::Synonym && child_stmt.getType() == ReferenceType::Synonym) {
		return executeSynonymSynonym(pkb, parent_stmt, child_stmt);
	}
	if (parent_stmt.getType() == ReferenceType::Wildcard && child_stmt.getType() == ReferenceType::Synonym) {
		return executeWildcardSynonym(pkb, child_stmt);
	}
	if (parent_stmt.getType() == ReferenceType::StatementIndex && child_stmt.getType() == ReferenceType::Synonym) {
		return executeIndexSynonym(pkb, parent_stmt, child_stmt);
	}

	return {};
}

QP::QueryResult QP::Relationship::Parent::executeNonTrivial(PKB::StorageAccessInterface& pkb) {
	if (parent_stmt.getType() == ReferenceType::Synonym && child_stmt.getType() == ReferenceType::StatementIndex) {
		return executeSynonymIndex(pkb, parent_stmt, child_stmt);
	}
	if (parent_stmt.getType() == ReferenceType::Synonym && child_stmt.getType() == ReferenceType::Wildcard) {
		return executeSynonymWildcard(pkb, parent_stmt);
	}
	if (parent_stmt.getType() == ReferenceType::Synonym && child_stmt.getType() == ReferenceType::Synonym) {
		return executeSynonymSynonym(pkb, parent_stmt, child_stmt);
	}
	if (parent_stmt.getType() == ReferenceType::Wildcard && child_stmt.getType() == ReferenceType::Synonym) {
		return executeWildcardSynonym(pkb, child_stmt);
	}
	if (parent_stmt.getType() == ReferenceType::StatementIndex && child_stmt.getType() == ReferenceType::Synonym) {
		return executeIndexSynonym(pkb, parent_stmt, child_stmt);
	}

	return {};
}

// Trivial Executors

QP::QueryResult QP::Relationship::Parent::executeTrivialIndexIndex(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent,
                                                                   const ReferenceArgument& child) {
	return QueryResult(pkb.checkParents(parent.getStatementIndex(), child.getStatementIndex()));
}

QP::QueryResult QP::Relationship::Parent::executeTrivialIndexWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent) {
	return QueryResult(!pkb.getChildren(parent.getStatementIndex()).empty());
}

QP::QueryResult QP::Relationship::Parent::executeTrivialIndexSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent,
                                                                     const ReferenceArgument& child) {
	StmtInfoPtrSet statements = pkb.getChildren(parent.getStatementIndex());
	if (child.getSynonym().type == DesignEntity::Stmt) {
		return QueryResult(!statements.empty());
	}
	for (auto const& stmt : statements) {
		if (Utilities::checkStmtTypeMatch(stmt, child.getSynonym().type)) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::Parent::executeTrivialWildcardIndex(PKB::StorageAccessInterface& pkb, const ReferenceArgument& child) {
	return QueryResult(pkb.getParent(child.getStatementIndex()) != nullptr);
}

QP::QueryResult QP::Relationship::Parent::executeTrivialWildcardWildcard(PKB::StorageAccessInterface& pkb) {
	StmtInfoPtrSet statements = pkb.getStatements();
	for (auto const& statement : statements) {
		// Avoid call to PKB unless statement can contain others.
		if (statement->getType() != StmtType::IfStmt && statement->getType() != StmtType::WhileStmt) {
			continue;
		}
		StmtInfoPtrSet children = pkb.getChildren(statement->getIdentifier());
		if (!children.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::Parent::executeTrivialWildcardSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& child) {
	StmtInfoPtrSet statements = pkb.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, child.getSynonym().type)) {
			continue;
		}
		shared_ptr<StmtInfo> parent = pkb.getParent(statement->getIdentifier());
		if (parent != nullptr) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::Parent::executeTrivialSynonymIndex(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent,
                                                                     const ReferenceArgument& child) {
	shared_ptr<StmtInfo> parent_statement = pkb.getParent(child.getStatementIndex());
	return QueryResult(Utilities::checkStmtTypeMatch(parent_statement, parent.getSynonym().type));
}

QP::QueryResult QP::Relationship::Parent::executeTrivialSynonymWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent) {
	StmtInfoPtrSet statements = pkb.getStatements();
	for (auto const& stmt : statements) {
		if (!Utilities::checkStmtTypeMatch(stmt, parent.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet children = pkb.getChildren(stmt->getIdentifier());
		if (!children.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::Parent::executeTrivialSynonymSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent,
                                                                       const ReferenceArgument& child) {
	if (parent.getSynonym().symbol == child.getSynonym().symbol) {
		return {};
	}

	StmtInfoPtrSet statements = pkb.getStatements();
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, parent.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet children_statements = pkb.getChildren(statement->getIdentifier());
		for (auto const& child_statement : children_statements) {
			if (Utilities::checkStmtTypeMatch(child_statement, child.getSynonym().type)) {
				return QueryResult(true);
			}
		}
	}
	return {};
}

// Executors

QP::QueryResult QP::Relationship::Parent::executeIndexSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent,
                                                              const ReferenceArgument& child) {
	StmtInfoPtrSet statements = pkb.getChildren(parent.getStatementIndex());
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

QP::QueryResult QP::Relationship::Parent::executeWildcardSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& child) {
	StmtInfoPtrSet statements = pkb.getStatements();
	vector<string> column;
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, child.getSynonym().type)) {
			continue;
		}

		shared_ptr<StmtInfo> parent = pkb.getParent(statement->getIdentifier());
		if (parent != nullptr) {
			column.push_back(to_string(statement->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(child.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::Parent::executeSynonymIndex(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent,
                                                              const ReferenceArgument& child) {
	shared_ptr<StmtInfo> parent_statement = pkb.getParent(child.getStatementIndex());
	vector<string> column;
	if (!Utilities::checkStmtTypeMatch(parent_statement, parent.getSynonym().type)) {
		return {};
	}
	column.push_back(to_string(parent_statement->getIdentifier()));
	QueryResult result = QueryResult();
	result.addColumn(parent.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::Parent::executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent) {
	StmtInfoPtrSet statements = pkb.getStatements();
	vector<string> column;
	for (auto const& statement : statements) {
		if (!Utilities::checkStmtTypeMatch(statement, parent.getSynonym().type)) {
			continue;
		}

		StmtInfoPtrSet children = pkb.getChildren(statement->getIdentifier());
		if (!children.empty()) {
			column.push_back(to_string(statement->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(parent.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::Parent::executeSynonymSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& parent,
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

		StmtInfoPtrSet children_statements = pkb.getChildren(statement->getIdentifier());
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

QP::Types::ArgumentDispatcher QP::Relationship::Parent::dispatcher = [](vector<ReferenceArgument> args) {
	return ParentDispatcherTemplate<Parent>::argumentDispatcher(Types::ClauseType::Parent, move(args));
};