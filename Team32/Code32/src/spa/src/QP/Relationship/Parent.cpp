#include "QP/Relationship/Parent.h"

#include <memory>
#include <utility>
#include <vector>

#include "QP/QueryUtils.h"
#include "QP/Relationship/ParentDispatcherTemplate.tpp"

// Trivial Executors

QP::QueryResult QP::Relationship::Parent::executeTrivialIndexIndex(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& parent,
                                                                   const Types::ReferenceArgument& child) {
	return QueryResult(pkb.checkParents(parent.getStatementIndex(), child.getStatementIndex()));
}

QP::QueryResult QP::Relationship::Parent::executeTrivialIndexWildcard(PKB::StorageAccessInterface& pkb,
                                                                      const Types::ReferenceArgument& parent) {
	return QueryResult(!pkb.getChildren(parent.getStatementIndex()).empty());
}

QP::QueryResult QP::Relationship::Parent::executeTrivialIndexSynonym(PKB::StorageAccessInterface& pkb,
                                                                     const Types::ReferenceArgument& parent,
                                                                     const Types::ReferenceArgument& child) {
	StmtInfoPtrSet statements = pkb.getChildren(parent.getStatementIndex());
	if (child.getSynonym().type == Types::DesignEntity::Stmt) {
		return QueryResult(!statements.empty());
	}
	for (auto const& stmt : statements) {
		if (Utilities::checkStmtTypeMatch(stmt, child.getSynonym().type)) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::Parent::executeTrivialWildcardIndex(PKB::StorageAccessInterface& pkb,
                                                                      const Types::ReferenceArgument& child) {
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

QP::QueryResult QP::Relationship::Parent::executeTrivialWildcardSynonym(PKB::StorageAccessInterface& pkb,
                                                                        const Types::ReferenceArgument& child) {
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

QP::QueryResult QP::Relationship::Parent::executeTrivialSynonymIndex(PKB::StorageAccessInterface& pkb,
                                                                     const Types::ReferenceArgument& parent,
                                                                     const Types::ReferenceArgument& child) {
	shared_ptr<StmtInfo> parent_statement = pkb.getParent(child.getStatementIndex());
	return QueryResult(Utilities::checkStmtTypeMatch(parent_statement, parent.getSynonym().type));
}

QP::QueryResult QP::Relationship::Parent::executeTrivialSynonymWildcard(PKB::StorageAccessInterface& pkb,
                                                                        const Types::ReferenceArgument& parent) {
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

QP::QueryResult QP::Relationship::Parent::executeTrivialSynonymSynonym(PKB::StorageAccessInterface& pkb,
                                                                       const Types::ReferenceArgument& parent,
                                                                       const Types::ReferenceArgument& child) {
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

QP::QueryResult QP::Relationship::Parent::executeIndexSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& parent,
                                                              const Types::ReferenceArgument& child) {
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

QP::QueryResult QP::Relationship::Parent::executeWildcardSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& child) {
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

QP::QueryResult QP::Relationship::Parent::executeSynonymIndex(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& parent,
                                                              const Types::ReferenceArgument& child) {
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

QP::QueryResult QP::Relationship::Parent::executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& parent) {
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

QP::QueryResult QP::Relationship::Parent::executeSynonymSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& parent,
                                                                const Types::ReferenceArgument& child) {
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

QP::Types::ArgumentDispatcher QP::Relationship::Parent::dispatcher = [](vector<Types::ReferenceArgument> args) {
	return ParentDispatcherTemplate<Parent>::argumentDispatcher(Types::ClauseType::Parent, move(args));
};