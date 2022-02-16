#include "QP/Relationship/Pattern.h"



Pattern::Pattern(Declaration synAssign, QueryEntRef entRef, ExpressionType expressionType,
	Common::ExpressionProcessor::Expression expression)
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
	return this->expression;
}

QueryResult Pattern::execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map) { 
	QueryResult result = QueryResult(true);
	StmtInfoPtrSet stmtSet;
		
	if (this->expressionType != ExpressionType::underscore) {
		bool isExact = this->expressionType != ExpressionType::expressionUnderscore;
		if (this->entRef.type == EntRefType::varName) {
			stmtSet = pkb.getStmtsWithPattern(this->entRef.entRef, this->expression, isExact);
		}
		else {
			auto stmtPairs = pkb.getStmtsWithPatternRHS(this->expression, isExact);
			for (auto stmtPair : stmtPairs) {
				stmtSet.insert(stmtPair.first);
			}
		}
	}
	else if (this->entRef.type == EntRefType::varName) {
		stmtSet = pkb.getStmtsWithPatternLHS(this->entRef.entRef);
	}
	else {
		stmtSet = pkb.getStatements();
	}
	vector<string> assignStmtStrings;
	for (auto const& stmt : stmtSet) {
		if (!QueryUtils::checkStmtTypeMatch(stmt, DesignEntity::assign)) {
			assignStmtStrings.push_back(to_string(stmt.get()->reference));
		}
	}
	result.addColumn(this->synAssign.symbol, assignStmtStrings);
	if (this->entRef.type == EntRefType::synonym) {
		vector<string> varStrings = { assignStmtStrings };
		result.addColumn(this->entRef.entRef, varStrings);
	}
	return result;
}

vector<string> Pattern::getDeclarationSymbols() {
	vector<string> declarationSymbols = {this->synAssign.symbol};
	if (this->entRef.type == EntRefType::synonym) {
		declarationSymbols.push_back(this->entRef.entRef);
	}
	return declarationSymbols;
}


QueryResult Pattern::executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) { return QueryResult(); }

QueryResult Pattern::executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) { return QueryResult(); }