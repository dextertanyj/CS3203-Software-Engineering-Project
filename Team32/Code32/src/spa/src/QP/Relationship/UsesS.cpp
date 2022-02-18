#include "QP/Relationship/UsesS.h"

UsesS::UsesS(QueryStmtRef stmt, QueryEntRef ent)
	: stmt(stmt),
	  ent(ent) {}

QueryStmtRef UsesS::getStmt() {
	return stmt;
}

QueryEntRef UsesS::getEnt() {
	return ent;
}

QueryResult UsesS::execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map) {
	return isTrivial ? executeTrivial(pkb, map) : executeNonTrivial(pkb, map);
}

vector<string> UsesS::getDeclarationSymbols() {
	vector<string> declarationSymbols;
	if (this->stmt.type == StmtRefType::synonym) {
		declarationSymbols.push_back(this->stmt.stmtRef);
	}
	if (this->ent.type == EntRefType::synonym) {
		declarationSymbols.push_back(this->ent.entRef);
	}
	return declarationSymbols;
}

QueryResult UsesS::executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	if (stmt.type == StmtRefType::stmtNumber && ent.type == EntRefType::varName) {
		return QueryResult(pkb.checkUses(stoul(stmt.stmtRef), ent.entRef));
	}
	else if ((stmt.type == StmtRefType::stmtNumber && ent.type == EntRefType::underscore) ||
	         (stmt.type == StmtRefType::stmtNumber && ent.type == EntRefType::synonym)) {
		VarRefSet varSet = pkb.getUsesByStmt(stoul(stmt.stmtRef));
		return QueryResult(!varSet.empty());
	}
	else if (stmt.type == StmtRefType::underscore && ent.type == EntRefType::varName) {
		StmtInfoPtrSet stmtSet = pkb.getUsesByVar(ent.entRef);
		return QueryResult(!stmtSet.empty());
	}
	else if ((stmt.type == StmtRefType::underscore && ent.type == EntRefType::underscore) ||
	         (stmt.type == StmtRefType::underscore && ent.type == EntRefType::synonym)) {
		VarRefSet varSet = pkb.getVariables();
		for (auto const& var : varSet) {
			StmtInfoPtrSet stmtSet = pkb.getUsesByVar(var);
			if (!stmtSet.empty()) {
				return QueryResult(true);
			}
		}
	}
	else if (stmt.type == StmtRefType::synonym && ent.type == EntRefType::varName) {
		StmtInfoPtrSet stmtSet = pkb.getUsesByVar(ent.entRef);
		DesignEntity designEntity = map[stmt.stmtRef];
		for (auto const& stmt : stmtSet) {
			if (QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				return QueryResult(true);
			}
		}
	}
	else if (stmt.type == StmtRefType::synonym && ent.type == EntRefType::underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[stmt.stmtRef];
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			VarRefSet varSet = pkb.getUsesByStmt(stmt.get()->reference);
			if (!varSet.empty()) {
				return QueryResult(true);
			}
		}
	}
	else if (stmt.type == StmtRefType::synonym && ent.type == EntRefType::synonym) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[stmt.stmtRef];
		vector<string> stmtColumn;
		vector<string> varColumn;
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			VarRefSet varSet = pkb.getUsesByStmt(stmt.get()->reference);
			if (!varSet.empty()) {
				return QueryResult(true);
			}
		}
	}
	
	return QueryResult();
}

QueryResult UsesS::executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	if (stmt.type == StmtRefType::synonym && ent.type == EntRefType::varName) {
		StmtInfoPtrSet stmtSet = pkb.getUsesByVar(ent.entRef);
		DesignEntity designEntity = map[stmt.stmtRef];
		vector<string> column;
		for (auto const& stmt : stmtSet) {
			if (QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				column.push_back(to_string(stmt.get()->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(stmt.stmtRef, column);
		return result;
	}
	else if (stmt.type == StmtRefType::synonym && ent.type == EntRefType::underscore) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[stmt.stmtRef];
		vector<string> column;
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			VarRefSet varSet = pkb.getUsesByStmt(stmt.get()->reference);
			if (!varSet.empty()) {
				column.push_back(to_string(stmt.get()->reference));
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(stmt.stmtRef, column);
		return result;
	}
	else if (stmt.type == StmtRefType::synonym && ent.type == EntRefType::synonym) {
		StmtInfoPtrSet stmtSet = pkb.getStatements();
		DesignEntity designEntity = map[stmt.stmtRef];
		vector<string> stmtColumn;
		vector<string> varColumn;
		for (auto const& stmt : stmtSet) {
			if (!QueryUtils::checkStmtTypeMatch(stmt, designEntity)) {
				continue;
			}

			VarRefSet varSet = pkb.getUsesByStmt(stmt.get()->reference);
			for (auto const& var : varSet) {
				stmtColumn.push_back(to_string(stmt.get()->reference));
				varColumn.push_back(var);
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(stmt.stmtRef, stmtColumn);
		result.addColumn(ent.entRef, varColumn);
		return result;
	}
	else if (stmt.type == StmtRefType::underscore && ent.type == EntRefType::synonym) {
		VarRefSet varSet = pkb.getVariables();
		vector<string> column;
		for (auto const& var : varSet) {
			StmtInfoPtrSet stmtSet = pkb.getUsesByVar(var);
			if (!stmtSet.empty()) {
				column.push_back(var);
			}
		}
		QueryResult result = QueryResult();
		result.addColumn(ent.entRef, column);
		return result;
	}
	else if (stmt.type == StmtRefType::stmtNumber && ent.type == EntRefType::synonym) {
		VarRefSet varSet = pkb.getUsesByStmt(stoul(stmt.stmtRef));
		vector<string> column;

		for (auto const& var : varSet) {
			column.push_back(var);
		}

		QueryResult result = QueryResult();
		result.addColumn(ent.entRef, column);
		return result;
	}

 	return QueryResult();
}
