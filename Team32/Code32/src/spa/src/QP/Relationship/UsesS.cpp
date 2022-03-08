#include "QP/Relationship/UsesS.h"

#include <utility>

QP::Relationship::UsesS::UsesS(ReferenceArgument stmt, ReferenceArgument ent) {
	if (stmt.getType() == ReferenceType::Wildcard) {
		throw QueryException("Ambiguous wildcard _.");
	}
	this->stmt = std::move(std::move(stmt));
	this->ent = std::move(std::move(ent));
}

ReferenceArgument QP::Relationship::UsesS::getStmt() { return stmt; }

ReferenceArgument QP::Relationship::UsesS::getEnt() { return ent; }

vector<string> QP::Relationship::UsesS::getDeclarationSymbols() {
	vector<string> declaration_symbols;
	if (this->stmt.getType() == ReferenceType::Synonym) {
		declaration_symbols.push_back(this->stmt.getSynonym().symbol);
	}
	if (this->ent.getType() == ReferenceType::Synonym) {
		declaration_symbols.push_back(this->ent.getSynonym().symbol);
	}
	return declaration_symbols;
}

QP::QueryResult QP::Relationship::UsesS::executeTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& /*map*/) {
	if (stmt.getType() == ReferenceType::StatementIndex && ent.getType() == ReferenceType::Name) {
		return QueryResult(pkb.checkUses(stmt.getStatementIndex(), ent.getName()));
	}
	if ((stmt.getType() == ReferenceType::StatementIndex && ent.getType() == ReferenceType::Wildcard) ||
	    (stmt.getType() == ReferenceType::StatementIndex && ent.getType() == ReferenceType::Synonym)) {
		VarRefSet var_set = pkb.getUsesByStmt(stmt.getStatementIndex());
		return QueryResult(!var_set.empty());
	}
	if (stmt.getType() == ReferenceType::Synonym && ent.getType() == ReferenceType::Name) {
		return executeTrivialEntVarName(pkb);
	}
	if (stmt.getType() == ReferenceType::Synonym && ent.getType() == ReferenceType::Wildcard) {
		return executeTrivialEntUnknown(pkb);
	}
	if (stmt.getType() == ReferenceType::Synonym && ent.getType() == ReferenceType::Synonym) {
		return executeTrivialEntUnknown(pkb);
	}

	return {};
}

QP::QueryResult QP::Relationship::UsesS::executeNonTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& /*map*/) {
	if (stmt.getType() == ReferenceType::Synonym && ent.getType() == ReferenceType::Name) {
		return executeNonTrivialEntVarName(pkb);
	}
	if (stmt.getType() == ReferenceType::Synonym && ent.getType() == ReferenceType::Wildcard) {
		return executeNonTrivialEntUnderscore(pkb);
	}
	if (stmt.getType() == ReferenceType::Synonym && ent.getType() == ReferenceType::Synonym) {
		return executeNonTrivialEntSynonym(pkb);
	}
	if (stmt.getType() == ReferenceType::StatementIndex && ent.getType() == ReferenceType::Synonym) {
		return executeNonTrivialStmtStmtNo(pkb);
	}
	return {};
}

QP::QueryResult QP::Relationship::UsesS::executeTrivialEntVarName(PKB::StorageAccessInterface& pkb) {
	StmtInfoPtrSet stmt_set = pkb.getStmtUsesByVar(ent.getName());
	DesignEntity design_entity = stmt.getSynonym().type;
	for (auto const& stmt : stmt_set) {
		if (Utilities::checkStmtTypeMatch(stmt, design_entity)) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::UsesS::executeTrivialEntUnknown(PKB::StorageAccessInterface& pkb) {
	StmtInfoPtrSet stmt_set = pkb.getStatements();
	DesignEntity design_entity = stmt.getSynonym().type;
	for (auto const& stmt : stmt_set) {
		if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
			continue;
		}

		VarRefSet var_set = pkb.getUsesByStmt(stmt->getIdentifier());
		if (!var_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::UsesS::executeNonTrivialEntVarName(PKB::StorageAccessInterface& pkb) {
	StmtInfoPtrSet stmt_set = pkb.getStmtUsesByVar(ent.getName());
	DesignEntity design_entity = stmt.getSynonym().type;
	vector<string> column;
	for (auto const& stmt : stmt_set) {
		if (Utilities::checkStmtTypeMatch(stmt, design_entity)) {
			column.push_back(to_string(stmt->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(stmt.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::UsesS::executeNonTrivialEntUnderscore(PKB::StorageAccessInterface& pkb) {
	StmtInfoPtrSet stmt_set = pkb.getStatements();
	DesignEntity design_entity = stmt.getSynonym().type;
	vector<string> column;
	for (auto const& stmt : stmt_set) {
		if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
			continue;
		}

		VarRefSet var_set = pkb.getUsesByStmt(stmt->getIdentifier());
		if (!var_set.empty()) {
			column.push_back(to_string(stmt->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(stmt.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::UsesS::executeNonTrivialEntSynonym(PKB::StorageAccessInterface& pkb) {
	StmtInfoPtrSet stmt_set = pkb.getStatements();
	DesignEntity design_entity = stmt.getSynonym().type;
	vector<string> stmt_column;
	vector<string> var_column;
	for (auto const& stmt : stmt_set) {
		if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
			continue;
		}

		VarRefSet var_set = pkb.getUsesByStmt(stmt->getIdentifier());
		for (auto const& var : var_set) {
			stmt_column.push_back(to_string(stmt->getIdentifier()));
			var_column.push_back(var);
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(stmt.getSynonym().symbol, stmt_column);
	result.addColumn(ent.getSynonym().symbol, var_column);
	return result;
}

QP::QueryResult QP::Relationship::UsesS::executeNonTrivialStmtStmtNo(PKB::StorageAccessInterface& pkb) {
	VarRefSet var_set = pkb.getUsesByStmt(stmt.getStatementIndex());
	vector<string> column;

	for (auto const& var : var_set) {
		column.push_back(var);
	}

	QueryResult result = QueryResult();
	result.addColumn(ent.getSynonym().symbol, column);
	return result;
}
