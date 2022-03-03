#include "QP/Relationship/ModifiesS.h"

#include <utility>

QP::Relationship::ModifiesS::ModifiesS(QueryStmtRef stmt, QueryEntRef ent) {
	if (stmt.type == StmtRefType::Underscore) {
		throw QueryException("Ambiguous wildcard _.");
	}
	this->stmt = std::move(std::move(stmt));
	this->ent = std::move(std::move(ent));
}

QueryStmtRef QP::Relationship::ModifiesS::getStmt() { return stmt; }

QueryEntRef QP::Relationship::ModifiesS::getEnt() { return ent; }

vector<string> QP::Relationship::ModifiesS::getDeclarationSymbols() {
	vector<string> declaration_symbols;
	if (this->stmt.type == StmtRefType::Synonym) {
		declaration_symbols.push_back(this->stmt.stmt_ref);
	}
	if (this->ent.type == EntRefType::Synonym) {
		declaration_symbols.push_back(this->ent.ent_ref);
	}
	return declaration_symbols;
}

QP::QueryResult QP::Relationship::ModifiesS::executeTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) {
	if (stmt.type == StmtRefType::StmtNumber && ent.type == EntRefType::VarName) {
		return QueryResult(pkb.checkModifies(stoul(stmt.stmt_ref), ent.ent_ref));
	}
	if ((stmt.type == StmtRefType::StmtNumber && ent.type == EntRefType::Underscore) ||
	    (stmt.type == StmtRefType::StmtNumber && ent.type == EntRefType::Synonym)) {
		VarRefSet var_set = pkb.getModifiesByStmt(stoul(stmt.stmt_ref));
		return QueryResult(!var_set.empty());
	}
	if (stmt.type == StmtRefType::Synonym && ent.type == EntRefType::VarName) {
		return executeTrivialEntVarName(pkb, map);
	} else if (stmt.type == StmtRefType::Synonym && ent.type == EntRefType::Underscore) {
		return executeTrivialEntUnknown(pkb, map);
	} else if (stmt.type == StmtRefType::Synonym && ent.type == EntRefType::Synonym) {
		return executeTrivialEntUnknown(pkb, map);
	}

	return {};
}

QP::QueryResult QP::Relationship::ModifiesS::executeNonTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) {
	if (stmt.type == StmtRefType::Synonym && ent.type == EntRefType::VarName) {
		return executeNonTrivialEntVarName(pkb, map);
	}
	if (stmt.type == StmtRefType::Synonym && ent.type == EntRefType::Underscore) {
		return executeNonTrivialEntUnderscore(pkb, map);
	}
	if (stmt.type == StmtRefType::Synonym && ent.type == EntRefType::Synonym) {
		return executeNonTrivialEntSynonym(pkb, map);
	}
	if (stmt.type == StmtRefType::StmtNumber && ent.type == EntRefType::Synonym) {
		return executeNonTrivialStmtStmtNo(pkb, map);
	}

	return {};
}

QP::QueryResult QP::Relationship::ModifiesS::executeTrivialEntVarName(PKB::StorageAccessInterface& pkb,
                                                                      unordered_map<string, DesignEntity>& map) {
	StmtInfoPtrSet stmt_set = pkb.getStmtModifiesByVar(ent.ent_ref);
	DesignEntity design_entity = map[stmt.stmt_ref];
	for (auto const& stmt : stmt_set) {
		if (Utilities::checkStmtTypeMatch(stmt, design_entity)) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::ModifiesS::executeTrivialEntUnknown(PKB::StorageAccessInterface& pkb,
                                                                      unordered_map<string, DesignEntity>& map) {
	StmtInfoPtrSet stmt_set = pkb.getStatements();
	DesignEntity design_entity = map[stmt.stmt_ref];
	for (auto const& stmt : stmt_set) {
		if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
			continue;
		}

		VarRefSet var_set = pkb.getModifiesByStmt(stmt->getIdentifier());
		if (!var_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::ModifiesS::executeNonTrivialEntVarName(PKB::StorageAccessInterface& pkb,
                                                                         unordered_map<string, DesignEntity>& map) {
	StmtInfoPtrSet stmt_set = pkb.getStmtModifiesByVar(ent.ent_ref);
	DesignEntity design_entity = map[stmt.stmt_ref];
	vector<string> column;
	for (auto const& stmt : stmt_set) {
		if (Utilities::checkStmtTypeMatch(stmt, design_entity)) {
			column.push_back(to_string(stmt->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(stmt.stmt_ref, column);
	return result;
}

QP::QueryResult QP::Relationship::ModifiesS::executeNonTrivialEntUnderscore(PKB::StorageAccessInterface& pkb,
                                                                            unordered_map<string, DesignEntity>& map) {
	StmtInfoPtrSet stmt_set = pkb.getStatements();
	DesignEntity design_entity = map[stmt.stmt_ref];
	vector<string> column;
	for (auto const& stmt : stmt_set) {
		if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
			continue;
		}

		VarRefSet var_set = pkb.getModifiesByStmt(stmt->getIdentifier());
		if (!var_set.empty()) {
			column.push_back(to_string(stmt->getIdentifier()));
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(stmt.stmt_ref, column);
	return result;
}

QP::QueryResult QP::Relationship::ModifiesS::executeNonTrivialEntSynonym(PKB::StorageAccessInterface& pkb,
                                                                         unordered_map<string, DesignEntity>& map) {
	StmtInfoPtrSet stmt_set = pkb.getStatements();
	DesignEntity design_entity = map[stmt.stmt_ref];
	vector<string> stmt_column;
	vector<string> var_column;
	for (auto const& stmt : stmt_set) {
		if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
			continue;
		}

		VarRefSet var_set = pkb.getModifiesByStmt(stmt->getIdentifier());
		for (auto const& var : var_set) {
			stmt_column.push_back(to_string(stmt->getIdentifier()));
			var_column.push_back(var);
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(stmt.stmt_ref, stmt_column);
	result.addColumn(ent.ent_ref, var_column);
	return result;
}

QP::QueryResult QP::Relationship::ModifiesS::executeNonTrivialStmtStmtNo(PKB::StorageAccessInterface& pkb,
                                                                         unordered_map<string, DesignEntity>& map) {
	VarRefSet var_set = pkb.getModifiesByStmt(stoul(stmt.stmt_ref));
	vector<string> column;

	for (auto const& var : var_set) {
		column.push_back(var);
	}

	QueryResult result = QueryResult();
	result.addColumn(ent.ent_ref, column);
	return result;
}