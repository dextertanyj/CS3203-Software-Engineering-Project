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
	QueryResult result;

	for (int i = 0; i < suchThatClauseList.size(); i++) {
		SuchThatClause suchThatClause = suchThatClauseList[i];

		if (suchThatClause.relation->isTrivialCase()) {
			// Stop once a suchThatClause returns false
			if (!suchThatClause.relation->executeTrivial(pkb)) {
				return QueryResult();
			}
		}
		else {
			QueryResult newResult = suchThatClause.relation->executeNonTrivial(pkb);
			if (i == 0) {
				result = newResult;
			}
			else {
				joinResult(result, newResult);
			}
		}
	}

	return result;
}

void QueryEvaluator::joinResult(QueryResult& currentResult, QueryResult& newResult) {
	// TODO: Find intersection of results
}


