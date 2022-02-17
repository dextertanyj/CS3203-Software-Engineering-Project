#include "QP/Relationship/Pattern.h"



Pattern::Pattern(Declaration synAssign, QueryEntRef entRef, ExpressionType expressionType,
	optional<Common::ExpressionProcessor::Expression> expression)
	: synAssign(std::move(synAssign)),
	  entRef(std::move(entRef)),
	  expressionType(expressionType),
	  expression(std::move(expression)) {}


Declaration Pattern::getSynAssign() {
	return this->synAssign;
}

QueryEntRef Pattern::getEntRef() {
	return this->entRef;
}

ExpressionType Pattern::getExpressionType() {
	return this->expressionType;
}

Common::ExpressionProcessor::Expression Pattern::getExpression() {
	return this->expression.value();
}

QueryResult Pattern::execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map) {
	return isTrivial ? executeTrivial(pkb, map) : executeNonTrivial(pkb, map);
}

vector<string> Pattern::getDeclarationSymbols() {
	vector<string> declarationSymbols = {this->synAssign.symbol};
	if (this->entRef.type == EntRefType::synonym) {
		declarationSymbols.push_back(this->entRef.entRef);
	}
	return declarationSymbols;
}


QueryResult Pattern::executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	StmtInfoPtrSet stmtSet;
	if (this->expressionType != ExpressionType::underscore) {
		bool isExact = this->expressionType != ExpressionType::expressionUnderscore;
		if (this->entRef.type == EntRefType::varName) {
			return QueryResult(pkb.patternExists(this->entRef.entRef, getExpression(), isExact)); 
		}
		else {
			auto stmtPairs = pkb.getStmtsWithPatternRHS(getExpression(), isExact);
			for (auto stmtPair : stmtPairs) {
				if (QueryUtils::checkStmtTypeMatch(stmtPair.first, DesignEntity::assign)) {
					return QueryResult(true);
				}
			}
		}
	}
	else if (this->entRef.type == EntRefType::varName) {
		stmtSet = pkb.getStmtsWithPatternLHS(this->entRef.entRef);
	}
	else {
		stmtSet = pkb.getStatements();
	}
	for (auto const& stmt : stmtSet) {
		if (QueryUtils::checkStmtTypeMatch(stmt, DesignEntity::assign)) {
			return QueryResult(true);
		}
	}
	return QueryResult(false);
}

QueryResult Pattern::executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) {
	QueryResult result = QueryResult();
	StmtInfoPtrSet stmtSet;
	unordered_set<string> varSet;
	if (this->expressionType != ExpressionType::underscore) {
		bool isExact = this->expressionType != ExpressionType::expressionUnderscore;
		if (this->entRef.type == EntRefType::varName) {
			stmtSet = pkb.getStmtsWithPattern(this->entRef.entRef, getExpression(), isExact);
		}
		else {
			auto stmtPairs = pkb.getStmtsWithPatternRHS(getExpression(), isExact);
			for (auto stmtPair : stmtPairs) {
				stmtSet.insert(stmtPair.first);
				varSet.insert(stmtPair.second);
			}
		}
	}
	else if (this->entRef.type == EntRefType::varName) {
		stmtSet = pkb.getStmtsWithPatternLHS(this->entRef.entRef);
	}
	else if (this->entRef.type == EntRefType::synonym) {
		/* For the case of a(synonym, _). Can be replaced with a more efficient 
		getter of all variables in assign statements */
		auto tempVarSet = pkb.getVariables();
		for (VarRef varRef : tempVarSet) {
			if (!pkb.getStmtsWithPatternLHS(varRef).empty()) {
				varSet.insert(varRef);
			}
		}
		stmtSet = pkb.getStatements();
	}
	else {
		stmtSet = pkb.getStatements();
	}
	vector<string> assignStmtStrings;
	for (auto const& stmt : stmtSet) {
		if (QueryUtils::checkStmtTypeMatch(stmt, DesignEntity::assign)) {
			assignStmtStrings.push_back(to_string(stmt.get()->reference));
		}
	}
	result.addColumn(this->synAssign.symbol, assignStmtStrings);
	if (this->entRef.type == EntRefType::synonym) {
		vector<string> varStrings;
		varStrings.insert(varStrings.end(), varSet.begin(), varSet.end());
		result.addColumn(this->entRef.entRef, varStrings);
	}
	return result;

}