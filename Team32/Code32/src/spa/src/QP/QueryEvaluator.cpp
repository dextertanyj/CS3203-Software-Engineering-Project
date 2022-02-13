#include "QP/QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(PKB& pkb) : pkb(pkb) {}

QueryResult QueryEvaluator::executeQuery(QueryProperties& queryProperties) {
	if (queryProperties.getSuchThatClauseList().empty() &&
			queryProperties.getPatternClauseList().empty()) {
		return executeNoClauses(queryProperties.getSelect());
	}
	
	QueryGraph graph = buildGraph(queryProperties);
	unordered_set<string> nonTrivialSynonyms = graph.getNonTrivialSynonyms(queryProperties.getSelect().symbol);

	return evaluateClauses(queryProperties, nonTrivialSynonyms);
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

QueryResult QueryEvaluator::evaluateClauses(QueryProperties& queryProperties,
                                            unordered_set<string> nonTrivialSynonyms) {
	vector<QueryResult> resultList;

	for (SuchThatClause suchThatClause :queryProperties.getSuchThatClauseList()) {
		bool isTrivial = true;
		for (string declaration : suchThatClause.relation->getDeclarationSymbols()) {
			if (nonTrivialSynonyms.find(declaration) != nonTrivialSynonyms.end()) {
				isTrivial = false;
			}
		}

		QueryResult result = suchThatClause.relation->execute(pkb, isTrivial);
		if (!result.getResult()) {
			return QueryResult();
		}

		resultList.push_back(result);
	}

	// TODO: Evaluate pattern clauses

	return QueryResult();
}
