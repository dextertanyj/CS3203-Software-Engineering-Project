#include "QP/QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(PKB& pkb) : pkb(pkb) {}

QueryResult QueryEvaluator::executeQuery(QueryProperties& queryProperties) {
	if (queryProperties.getSuchThatClauseList().empty() &&
			queryProperties.getPatternClauseList().empty()) {
		return executeNoClauses(queryProperties.getSelect());
	}
	
	QueryGraph graph = buildGraph(queryProperties);

	return QueryResult();
}

QueryResult QueryEvaluator::executeNoClauses(Declaration select) {
	if (select.type == DesignEntity::stmt) {
		StmtRefList stmtList = pkb.getStatements();
		QueryResult result = QueryResult();

		vector<string> resultString;
		for (StmtRef stmt : stmtList) {
			resultString.push_back({ to_string(stmt) });
		}

		result.addColumn(select.symbol, resultString);

		return result;

	} else {
		// TODO: Handle other cases
		return QueryResult();
	}
}

QueryGraph QueryEvaluator::buildGraph(QueryProperties& queryProperties) {
	QueryGraph graph = QueryGraph(queryProperties.getDeclarationList());
	graph.setEdges(queryProperties.getSuchThatClauseList(), queryProperties.getPatternClauseList());

	return graph;
}

QueryResult QueryEvaluator::evaluateSuchThatClauses(SuchThatClauseList& suchThatClauseList) {
	QueryResult result = QueryResult();

	for (SuchThatClause suchThatClause : suchThatClauseList) {
		result = suchThatClause.relation->execute(pkb, result);
		if (!result.getResult()) {
			return QueryResult();
		}
	}

	return result;
}
