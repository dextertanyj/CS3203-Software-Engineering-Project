#include "QP/Relationship/ModifiesS.h"

#include <utility>

ModifiesS::ModifiesS(QueryStmtRef stmt, QueryEntRef ent) : stmt(std::move(std::move(stmt))), ent(std::move(std::move(ent))) {}

QueryStmtRef ModifiesS::getStmt() {
	return stmt;
}

QueryEntRef ModifiesS::getEnt() {
	return ent;
}

QueryResult ModifiesS::execute(PKB& pkb, bool is_trivial, unordered_map<string, DesignEntity>& map) {
	return is_trivial ? executeTrivial(pkb, map) : executeNonTrivial(pkb, map);
}

vector<string> ModifiesS::getDeclarationSymbols() {
	vector<string> declaration_symbols;
	if (this->stmt.type == StmtRefType::Synonym) {
		declaration_symbols.push_back(this->stmt.stmt_ref);
	}
	if (this->ent.type == EntRefType::Synonym) {
		declaration_symbols.push_back(this->ent.ent_ref);
	}
	return declaration_symbols;
}

QueryResult ModifiesS::executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	if (stmt.type == StmtRefType::StmtNumber && ent.type == EntRefType::VarName) {
		return QueryResult(pkb.checkModifies(stoul(stmt.stmt_ref), ent.ent_ref));
	}
	if ((stmt.type == StmtRefType::StmtNumber && ent.type == EntRefType::Underscore) ||
	    (stmt.type == StmtRefType::StmtNumber && ent.type == EntRefType::Synonym)) {
		VarRefSet varSet = pkb.getModifiesByStmt(stoul(stmt.stmt_ref));
		return QueryResult(!varSet.empty());
	} else if (stmt.type == StmtRefType::Underscore && ent.type == EntRefType::VarName) {
		StmtInfoPtrSet stmtSet = pkb.getModifiesByVar(ent.ent_ref);
		return QueryResult(!stmtSet.empty());
	} else if ((stmt.type == StmtRefType::Underscore && ent.type == EntRefType::Underscore) ||
	           (stmt.type == StmtRefType::Underscore && ent.type == EntRefType::Synonym)) {
		VarRefSet varSet = pkb.getVariables();
		for (auto const& var : varSet) {
			StmtInfoPtrSet stmtSet = pkb.getModifiesByVar(var);
			if (!stmtSet.empty()) {
				return QueryResult(true);
			}
		}
	} else if (stmt.type == StmtRefType::Synonym && ent.type == EntRefType::VarName) {
		StmtInfoPtrSet stmtSet = pkb.getModifiesByVar(ent.ent_ref);
		DesignEntity designEntity = map[stmt.stmt_ref];
		for (auto const& stmt : stmtSet) {
			if (QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				return QueryResult(true);
			}
		}
	} else if (stmt.type == StmtRefType::Synonym && ent.type == EntRefType::Underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[stmt.stmt_ref];
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			VarRefSet varSet = pkb.getModifiesByStmt(stmt.get()->reference);
			if (!varSet.empty()) {
				return QueryResult(true);
			}
		}
	} else if (stmt.type == StmtRefType::Synonym && ent.type == EntRefType::Synonym) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[stmt.stmt_ref];
		vector<string> stmtColumn;
		vector<string> varColumn;
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			VarRefSet varSet = pkb.getModifiesByStmt(stmt.get()->reference);
			if (!varSet.empty()) {
				return QueryResult(true);
			}
		}
	}

	return {};
}

QueryResult ModifiesS::executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	if (stmt.type == StmtRefType::Synonym && ent.type == EntRefType::VarName) {
		StmtInfoPtrSet stmt_set = pkb.getModifiesByVar(ent.ent_ref);
		DesignEntity design_entity = map[stmt.stmt_ref];
		vector<string> column;
		for (auto const& stmt : stmt_set) {
			if (QueryUtils::checkStmtTypeMatch(stmt, design_entity)) {
				column.push_back(to_string(stmt->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(stmt.stmt_ref, column);
		return result;
	}
	if (stmt.type == StmtRefType::Synonym && ent.type == EntRefType::Underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[stmt.stmt_ref];
		vector<string> column;
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			VarRefSet varSet = pkb.getModifiesByStmt(stmt.get()->reference);
			if (!varSet.empty()) {
				column.push_back(to_string(stmt.get()->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(stmt.stmt_ref, column);
		return result;
	} else if (stmt.type == StmtRefType::Synonym && ent.type == EntRefType::Synonym) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[stmt.stmt_ref];
		vector<string> stmtColumn;
		vector<string> varColumn;
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			VarRefSet varSet = pkb.getModifiesByStmt(stmt.get()->reference);
			for (auto const& var : varSet) {
				stmtColumn.push_back(to_string(stmt.get()->reference));
				varColumn.push_back(var);
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(stmt.stmt_ref, stmtColumn);
		result.addColumn(ent.ent_ref, varColumn);
		return result;
	} else if (stmt.type == StmtRefType::Underscore && ent.type == EntRefType::Synonym) {
		VarRefSet varSet = pkb.getVariables();
		vector<string> column;
		for (auto const& var : varSet) {
			StmtInfoPtrSet stmtSet = pkb.getModifiesByVar(var);
			if (!stmtSet.empty()) {
				column.push_back(var);
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(ent.ent_ref, column);
		return result;
	} else if (stmt.type == StmtRefType::StmtNumber && ent.type == EntRefType::Synonym) {
		VarRefSet varSet = pkb.getModifiesByStmt(stoul(stmt.stmt_ref));
		vector<string> column;

		for (auto const& var : varSet) {
			column.push_back(var);
		}

		QueryResult result = QueryResult();
		result.addColumn(ent.ent_ref, column);
		return result;
	}

	return {};
}
