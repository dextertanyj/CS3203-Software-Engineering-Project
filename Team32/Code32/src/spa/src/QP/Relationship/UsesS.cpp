#include "QP/Relationship/UsesS.h"

#include <utility>

#include "QP/QueryUtils.h"

QP::QueryResult QP::Relationship::UsesS::executeTrivialIndexName(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& stmt,
                                                                 const Types::ReferenceArgument& ent) {
	return QueryResult(pkb.checkUses(stmt.getStatementIndex(), ent.getName()));
}

QP::QueryResult QP::Relationship::UsesS::executeTrivialIndexWildcardOrSynonym(PKB::StorageAccessInterface& pkb,
                                                                              const Types::ReferenceArgument& stmt) {
	VarRefSet var_set = pkb.getUsesByStmt(stmt.getStatementIndex());
	return QueryResult(!var_set.empty());
}

QP::QueryResult QP::Relationship::UsesS::executeTrivialSynonymName(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& stmt,
                                                                   const Types::ReferenceArgument& ent) {
	StmtInfoPtrSet stmt_set = pkb.getStmtUsesByVar(ent.getName());
	Types::DesignEntity design_entity = stmt.getSynonym().type;
	for (auto const& res_stmt : stmt_set) {
		if (Utilities::checkStmtTypeMatch(res_stmt, design_entity)) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::UsesS::executeTrivialSynonymWildcardOrSynonym(PKB::StorageAccessInterface& pkb,
                                                                                const Types::ReferenceArgument& stmt) {
	StmtInfoPtrSet stmt_set = pkb.getStatements();
	Types::DesignEntity design_entity = stmt.getSynonym().type;
	for (auto const& res_stmt : stmt_set) {
		if (!Utilities::checkStmtTypeMatch(res_stmt, design_entity)) {
			continue;
		}

		VarRefSet var_set = pkb.getUsesByStmt(res_stmt->getIdentifier());
		if (!var_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::UsesS::executeSynonymName(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& stmt,
                                                            const Types::ReferenceArgument& ent) {
	StmtInfoPtrSet stmt_set = pkb.getStmtUsesByVar(ent.getName());
	Types::DesignEntity design_entity = stmt.getSynonym().type;
	vector<string> column;
	for (auto const& res_stmt : stmt_set) {
		if (Utilities::checkStmtTypeMatch(res_stmt, design_entity)) {
			column.push_back(to_string(res_stmt->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(stmt.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::UsesS::executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& stmt) {
	StmtInfoPtrSet stmt_set = pkb.getStatements();
	Types::DesignEntity design_entity = stmt.getSynonym().type;
	vector<string> column;
	for (auto const& res_stmt : stmt_set) {
		if (!Utilities::checkStmtTypeMatch(res_stmt, design_entity)) {
			continue;
		}

		VarRefSet var_set = pkb.getUsesByStmt(res_stmt->getIdentifier());
		if (!var_set.empty()) {
			column.push_back(to_string(res_stmt->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(stmt.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::UsesS::executeSynonymSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& stmt,
                                                               const Types::ReferenceArgument& ent) {
	StmtInfoPtrSet stmt_set = pkb.getStatements();
	Types::DesignEntity design_entity = stmt.getSynonym().type;
	vector<string> stmt_column;
	vector<string> var_column;
	for (auto const& res_stmt : stmt_set) {
		if (!Utilities::checkStmtTypeMatch(res_stmt, design_entity)) {
			continue;
		}

		VarRefSet var_set = pkb.getUsesByStmt(res_stmt->getIdentifier());
		for (auto const& var : var_set) {
			stmt_column.push_back(to_string(res_stmt->getIdentifier()));
			var_column.push_back(var);
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(stmt.getSynonym().symbol, stmt_column);
	result.addColumn(ent.getSynonym().symbol, var_column);
	return result;
}

QP::QueryResult QP::Relationship::UsesS::executeIndexSynonym(PKB::StorageAccessInterface& pkb, const Types::ReferenceArgument& stmt,
                                                             const Types::ReferenceArgument& ent) {
	VarRefSet var_set = pkb.getUsesByStmt(stmt.getStatementIndex());
	vector<string> column;

	for (auto const& var : var_set) {
		column.push_back(var);
	}

	QueryResult result = QueryResult();
	result.addColumn(ent.getSynonym().symbol, column);
	return result;
}
