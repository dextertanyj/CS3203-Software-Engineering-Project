#include "QP/QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(PKB& pkb) : pkb(pkb) {}

QueryResult QueryEvaluator::executeQuery(QueryProperties& queryProperties) {
	SuchThatClauseList suchThatClauseList = queryProperties.getSuchThatClauseList();
	PatternClauseList patternClauseList = queryProperties.getPatternClauseList();

	if (suchThatClauseList.empty() && patternClauseList.empty()) {
		Declaration select = queryProperties.getSelect();
		if (select.type == DesignEntity::stmt) {
			StmtInfoPtrList stmt_pointers = pkb.getStatements();
			vector<StmtRef> stmts;
			for (const shared_ptr<StmtInfo>& stmt : stmt_pointers) {
				stmts.push_back(stmt->reference);
			}
			return QueryResult(stmts, {});
		} else {
			// TODO: Handle other cases
			return QueryResult();
		}
	}

	return evaluateSuchThatClauses(suchThatClauseList);
}
QueryResult QueryEvaluator::evaluateSuchThatClauses(SuchThatClauseList& suchThatClauseList) {
	StmtInfoPtrList stmt_pointers = pkb.getStatements();
	vector<StmtRef> stmts;
	for (const shared_ptr<StmtInfo>& stmt : stmt_pointers) {
		stmts.push_back(stmt->reference);
	}
	QueryResult result = QueryResult(stmts, {});

	for (SuchThatClause suchThatClause : suchThatClauseList) {
		if (!suchThatClause.relation->execute(pkb, result)) {
			return QueryResult();
		}
	}

	return result;
}
