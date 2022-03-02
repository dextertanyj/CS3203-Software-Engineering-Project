#include "QP/Relationship/UsesS.h"

#include <utility>

QP::Relationship::UsesS::UsesS(QueryStmtRef stmt, QueryEntRef ent) : stmt(std::move(std::move(stmt))), ent(std::move(std::move(ent))) {}

QueryStmtRef QP::Relationship::UsesS::getStmt() { return stmt; }

QueryEntRef QP::Relationship::UsesS::getEnt() { return ent; }

vector<string> QP::Relationship::UsesS::getDeclarationSymbols() {
	vector<string> declaration_symbols;
	if (this->stmt.type == StmtRefType::Synonym) {
		declaration_symbols.push_back(this->stmt.stmt_ref);
	}
	if (this->ent.type == EntRefType::Synonym) {
		declaration_symbols.push_back(this->ent.ent_ref);
	}
	return declaration_symbols;
}

QP::QueryResult QP::Relationship::UsesS::executeTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) {
	if (stmt.type == StmtRefType::StmtNumber && ent.type == EntRefType::VarName) {
		return QueryResult(pkb.checkUses(stoul(stmt.stmt_ref), ent.ent_ref));
	}
	if ((stmt.type == StmtRefType::StmtNumber && ent.type == EntRefType::Underscore) ||
	    (stmt.type == StmtRefType::StmtNumber && ent.type == EntRefType::Synonym)) {
		VarRefSet var_set = pkb.getUsesByStmt(stoul(stmt.stmt_ref));
		return QueryResult(!var_set.empty());
	}
	if (stmt.type == StmtRefType::Underscore) {
		return QueryResult(false);
	}
	if (stmt.type == StmtRefType::Synonym && ent.type == EntRefType::VarName) {
		StmtInfoPtrSet stmt_set = pkb.getStmtUsesByVar(ent.ent_ref);
		DesignEntity design_entity = map[stmt.stmt_ref];
		for (auto const& stmt : stmt_set) {
			if (Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				return QueryResult(true);
			}
		}
	} else if (stmt.type == StmtRefType::Synonym && ent.type == EntRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[stmt.stmt_ref];
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			VarRefSet var_set = pkb.getUsesByStmt(stmt->reference);
			if (!var_set.empty()) {
				return QueryResult(true);
			}
		}
	} else if (stmt.type == StmtRefType::Synonym && ent.type == EntRefType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[stmt.stmt_ref];
		vector<string> stmt_column;
		vector<string> var_column;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			VarRefSet var_set = pkb.getUsesByStmt(stmt->reference);
			if (!var_set.empty()) {
				return QueryResult(true);
			}
		}
	}

	return {};
}

QP::QueryResult QP::Relationship::UsesS::executeNonTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) {
	if (stmt.type == StmtRefType::Synonym && ent.type == EntRefType::VarName) {
		StmtInfoPtrSet stmt_set = pkb.getStmtUsesByVar(ent.ent_ref);
		DesignEntity design_entity = map[stmt.stmt_ref];
		vector<string> column;
		for (auto const& stmt : stmt_set) {
			if (Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				column.push_back(to_string(stmt->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(stmt.stmt_ref, column);
		return result;
	}
	if (stmt.type == StmtRefType::Synonym && ent.type == EntRefType::Underscore) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[stmt.stmt_ref];
		vector<string> column;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			VarRefSet var_set = pkb.getUsesByStmt(stmt->reference);
			if (!var_set.empty()) {
				column.push_back(to_string(stmt->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(stmt.stmt_ref, column);
		return result;
	}
	if (stmt.type == StmtRefType::Synonym && ent.type == EntRefType::Synonym) {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		DesignEntity design_entity = map[stmt.stmt_ref];
		vector<string> stmt_column;
		vector<string> var_column;
		for (auto const& stmt : stmt_set) {
			if (!Utilities::checkStmtTypeMatch(stmt, design_entity)) {
				continue;
			}

			VarRefSet var_set = pkb.getUsesByStmt(stmt->reference);
			for (auto const& var : var_set) {
				stmt_column.push_back(to_string(stmt->reference));
				var_column.push_back(var);
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(stmt.stmt_ref, stmt_column);
		result.addColumn(ent.ent_ref, var_column);
		return result;
	}
	if (stmt.type == StmtRefType::StmtNumber && ent.type == EntRefType::Synonym) {
		VarRefSet var_set = pkb.getUsesByStmt(stoul(stmt.stmt_ref));
		vector<string> column;

		for (auto const& var : var_set) {
			column.push_back(var);
		}

		QueryResult result = QueryResult();
		result.addColumn(ent.ent_ref, column);
		return result;
	}
	if (stmt.type == StmtRefType::Underscore) {
		QueryResult result = QueryResult();
		return result;
	}
	return {};
}
