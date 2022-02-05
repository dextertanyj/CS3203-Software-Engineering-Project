#include "QueryEvaluator.h"

PKB QueryEvaluator::pkb = PKB();

QueryResult QueryEvaluator::executeQuery(QueryProperties& queryProperties) {
	SuchThatClauseList suchThatClauseList = queryProperties.getSuchThatClauseList();
	PatternClauseList patternClauseList = queryProperties.getPatternClauseList();
	
	if (suchThatClauseList.empty() && patternClauseList.empty()) {
		Declaration select = queryProperties.getSelect();
		if (select.type == DesignEntity::stmt) {
			unordered_set<StmtRef> set = pkb.getAllStmt();
			StmtRefList stmtList;
			stmtList.insert(stmtList.end(), set.begin(), set.end());
			return QueryResult(stmtList, {});
		} else {
			// TODO: Handle other cases
			return QueryResult();
		}
	}

	return evaluateSuchThatClauses(suchThatClauseList);
}

QueryResult QueryEvaluator::evaluateSuchThatClauses(SuchThatClauseList& suchThatClauseList) {
	unordered_set<StmtRef> set = pkb.getAllStmt();
	StmtRefList stmtList;
	stmtList.insert(stmtList.end(), set.begin(), set.end());
	QueryResult result = QueryResult(stmtList, {});

	for (SuchThatClause suchThatClause : suchThatClauseList) {
		if (!suchThatClause.relation->execute(pkb, result)) {
			return QueryResult();
		}
	}

	return result;
}
