#include "QP/QueryPattern.h"



QueryPattern::QueryPattern(Declaration synonym, QueryEntRef entRef, ExpressionType expressionType,
	Common::ExpressionProcessor::Expression expression)
	: synonym(std::move(synonym)),
	  entRef(std::move(entRef)),
	  expressionType(expressionType),
	  expression(std::move(expression)) {}


Declaration QueryPattern::getSynonym() {
	return this->synonym;
}

QueryEntRef QueryPattern::getEntRef() {
	return this->entRef;
}

ExpressionType QueryPattern::getExpressionType() {
	return this->expressionType;
}

Common::ExpressionProcessor::Expression QueryPattern::getExpression() {
	return this->expression;
}

QueryResult QueryPattern::execute(PKB& pkb, bool isTrivial, unordered_map<string, DesignEntity>& map) { return QueryResult(); }

vector<string> QueryPattern::getDeclarationSymbols() {
	vector<string> vec = { "0" };
	return vec; 

}


QueryResult QueryPattern::executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) { return QueryResult(); }

QueryResult QueryPattern::executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) { return QueryResult(); }