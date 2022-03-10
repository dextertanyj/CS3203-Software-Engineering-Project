#include "QP/Relationship/ModifiesS.h"

#include <utility>

QP::Relationship::ModifiesS::ModifiesS(ReferenceArgument stmt, ReferenceArgument ent) {
	if (stmt.getType() == ReferenceType::Wildcard) {
		throw QueryException("Ambiguous wildcard _.");
	}
	this->stmt = std::move(std::move(stmt));
	this->ent = std::move(std::move(ent));
}

ReferenceArgument QP::Relationship::ModifiesS::getStmt() { return stmt; }

ReferenceArgument QP::Relationship::ModifiesS::getEnt() { return ent; }

vector<string> QP::Relationship::ModifiesS::getDeclarationSymbols() {
	vector<string> declaration_symbols;
	if (this->stmt.getType() == ReferenceType::Synonym) {
		declaration_symbols.push_back(this->stmt.getSynonym().symbol);
	}
	if (this->ent.getType() == ReferenceType::Synonym) {
		declaration_symbols.push_back(this->ent.getSynonym().symbol);
	}
	return declaration_symbols;
}

QP::QueryResult QP::Relationship::ModifiesS::executeTrivial(PKB::StorageAccessInterface& pkb) {
	if (stmt.getType() == ReferenceType::StatementIndex && ent.getType() == ReferenceType::Name) {
		return executeTrivialIndexName(pkb, stmt, ent);
	}
	if ((stmt.getType() == ReferenceType::StatementIndex && ent.getType() == ReferenceType::Wildcard) ||
	    (stmt.getType() == ReferenceType::StatementIndex && ent.getType() == ReferenceType::Synonym)) {
		return executeTrivialIndexWildcardOrSynonym(pkb, stmt);
	}
	if (stmt.getType() == ReferenceType::Synonym && ent.getType() == ReferenceType::Name) {
		return executeTrivialSynonymName(pkb, stmt, ent);
	}
	if ((stmt.getType() == ReferenceType::Synonym && ent.getType() == ReferenceType::Wildcard) ||
	    (stmt.getType() == ReferenceType::Synonym && ent.getType() == ReferenceType::Synonym)) {
		return executeTrivialSynonymWildcardOrSynonym(pkb, stmt);
	}
	return {};
}

QP::QueryResult QP::Relationship::ModifiesS::executeNonTrivial(PKB::StorageAccessInterface& pkb) {
	if (stmt.getType() == ReferenceType::Synonym && ent.getType() == ReferenceType::Name) {
		return executeSynonymName(pkb, stmt, ent);
	}
	if (stmt.getType() == ReferenceType::Synonym && ent.getType() == ReferenceType::Wildcard) {
		return executeSynonymWildcard(pkb, stmt);
	}
	if (stmt.getType() == ReferenceType::Synonym && ent.getType() == ReferenceType::Synonym) {
		return executeSynonymSynonym(pkb, stmt, ent);
	}
	if (stmt.getType() == ReferenceType::StatementIndex && ent.getType() == ReferenceType::Synonym) {
		return executeIndexSynonym(pkb, stmt, ent);
	}

	return {};
}

QP::QueryResult QP::Relationship::ModifiesS::executeTrivialIndexName(PKB::StorageAccessInterface& pkb, const ReferenceArgument& stmt,
                                                                     const ReferenceArgument& ent) {
	return QueryResult(pkb.checkModifies(stmt.getStatementIndex(), ent.getName()));
}

QP::QueryResult QP::Relationship::ModifiesS::executeTrivialIndexWildcardOrSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& stmt) {
	VarRefSet var_set = pkb.getModifiesByStmt(stmt.getStatementIndex());
	return QueryResult(!var_set.empty());
}

QP::QueryResult QP::Relationship::ModifiesS::executeTrivialSynonymName(PKB::StorageAccessInterface& pkb, const ReferenceArgument& stmt,
                                                                       const ReferenceArgument& ent) {
	StmtInfoPtrSet stmt_set = pkb.getStmtModifiesByVar(ent.getName());
	DesignEntity design_entity = stmt.getSynonym().type;
	for (auto const& res_stmt : stmt_set) {
		if (Utilities::checkStmtTypeMatch(res_stmt, design_entity)) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::ModifiesS::executeTrivialSynonymWildcardOrSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& stmt) {
	StmtInfoPtrSet stmt_set = pkb.getStatements();
	DesignEntity design_entity = stmt.getSynonym().type;
	for (auto const& res_stmt : stmt_set) {
		if (!Utilities::checkStmtTypeMatch(res_stmt, design_entity)) {
			continue;
		}

		VarRefSet var_set = pkb.getModifiesByStmt(res_stmt->getIdentifier());
		if (!var_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::ModifiesS::executeSynonymName(PKB::StorageAccessInterface& pkb, const ReferenceArgument& stmt,
                                                                const ReferenceArgument& ent) {
	StmtInfoPtrSet stmt_set = pkb.getStmtModifiesByVar(ent.getName());
	DesignEntity design_entity = stmt.getSynonym().type;
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

QP::QueryResult QP::Relationship::ModifiesS::executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& stmt) {
	StmtInfoPtrSet stmt_set = pkb.getStatements();
	DesignEntity design_entity = stmt.getSynonym().type;
	vector<string> column;
	for (auto const& res_stmt : stmt_set) {
		if (!Utilities::checkStmtTypeMatch(res_stmt, design_entity)) {
			continue;
		}

		VarRefSet var_set = pkb.getModifiesByStmt(res_stmt->getIdentifier());
		if (!var_set.empty()) {
			column.push_back(to_string(res_stmt->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(stmt.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::ModifiesS::executeSynonymSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& stmt,
                                                                   const ReferenceArgument& ent) {
	StmtInfoPtrSet stmt_set = pkb.getStatements();
	DesignEntity design_entity = stmt.getSynonym().type;
	vector<string> stmt_column;
	vector<string> var_column;
	for (auto const& res_stmt : stmt_set) {
		if (!Utilities::checkStmtTypeMatch(res_stmt, design_entity)) {
			continue;
		}

		VarRefSet var_set = pkb.getModifiesByStmt(res_stmt->getIdentifier());
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

QP::QueryResult QP::Relationship::ModifiesS::executeIndexSynonym(PKB::StorageAccessInterface& pkb, const ReferenceArgument& stmt,
                                                                 const ReferenceArgument& ent) {
	VarRefSet var_set = pkb.getModifiesByStmt(stmt.getStatementIndex());
	vector<string> column;

	for (auto const& var : var_set) {
		column.push_back(var);
	}

	QueryResult result = QueryResult();
	result.addColumn(ent.getSynonym().symbol, column);
	return result;
}
