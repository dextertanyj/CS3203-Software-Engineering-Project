#include "QP/QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(PKB& pkb) : pkb(pkb) {}

QueryResult QueryEvaluator::executeQuery(QueryProperties& queryProperties) {
	if (queryProperties.getSuchThatClauseList().empty() &&
			queryProperties.getPatternClauseList().empty()) {
		return executeNoClauses(queryProperties.getSelect());
	}

	QueryGraph graph = buildGraph(queryProperties);
	Declaration select = queryProperties.getSelect();
	vector<unordered_set<string>> synonymsInGroup = graph.getSynonymsInGroup(select.symbol);
	vector<pair<SuchThatClauseList, PatternClauseList>> clausesInGroup = splitClauses(queryProperties, synonymsInGroup);

	QueryResult result;
	if (clausesInGroup[0].first.size() == 0 && clausesInGroup[0].second.size() == 0) {
		result = executeNoClauses(select);
	} else {
		result = evaluateClauses(clausesInGroup[0].first, clausesInGroup[0].second, select, false);
	}

	for (int i = 1; i < clausesInGroup.size(); i++) {
		// The clauses in last group should be evaluated independently since they are unrelated
		if (i == clausesInGroup.size() - 1) {
			for (SuchThatClause suchThatClause : clausesInGroup[i].first) {
				SuchThatClauseList suchThatList = { suchThatClause };
				PatternClauseList patternList = {};
				if (!evaluateClauses(suchThatList, patternList, select, true).getResult()) {
					return QueryResult();
				}
			}
			for (PatternClause patternClause : clausesInGroup[i].second) {
				SuchThatClauseList suchThatList = {};
				PatternClauseList patternList = { patternClause };
				if (!evaluateClauses(suchThatList, patternList, select, true).getResult()) {
					return QueryResult();
				}
			}
		}

		if (clausesInGroup[i].first.size() + clausesInGroup[i].second.size() == 0) {
			continue;
		}
		else if (clausesInGroup[i].first.size() + clausesInGroup[i].second.size() == 1) {
			QueryResult queryResult = evaluateClauses(clausesInGroup[i].first, clausesInGroup[i].second, select, true);
			if (!queryResult.getResult()) {
				return QueryResult();
			}
		} else {
			QueryResult queryResult = evaluateClauses(clausesInGroup[i].first, clausesInGroup[i].second, select, false);
			if (!queryResult.getResult()) {
				return QueryResult();
			}
		}
	}

	return result;
}

QueryResult QueryEvaluator::executeNoClauses(Declaration select) {
	if (select.type == DesignEntity::stmt) {
		StmtInfoPtrSet stmtList = pkb.getStatements();
		QueryResult result = QueryResult();

		vector<string> resultString;
		for (const shared_ptr<StmtInfo>& stmt : stmtList) {
			resultString.push_back({ to_string(stmt->reference) });
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

QueryResult QueryEvaluator::evaluateClauses(SuchThatClauseList& suchThatClauses,
																					  PatternClauseList& patternClauses,
                                            Declaration select,
                                            bool isTrivial) {
	vector<QueryResult> resultList;

	for (SuchThatClause suchThatClause : suchThatClauses) {
		QueryResult result = suchThatClause.relation->execute(pkb, isTrivial);
		if (!result.getResult()) {
			return QueryResult();
		}
		resultList.push_back(result);
	}

	// TODO: Handle pattern clause after pattern is refactored to a relation

	// For iteration one, the size of resultList is at most 2
	if (resultList.size() == 1) {
		return resultList[0];
	}

	// Merge the table without selected synonym to the one with selected synonym.
	// If both does not contain selected synonym, we merge the smaller table to larger table.
	if (resultList[0].getSynonymsStored().find(select.symbol) != resultList[0].getSynonymsStored().end()) {
		resultList[0].joinResult(resultList[1]);
		return resultList[0];
	}

	if (resultList[1].getSynonymsStored().find(select.symbol) != resultList[1].getSynonymsStored().end()) {
		resultList[1].joinResult(resultList[0]);
		return resultList[1];
	}

	if (resultList[0].getSynonymsStored().size() >= resultList[1].getSynonymsStored().size()) {
		resultList[0].joinResult(resultList[1]);
		return resultList[0];
	} else {
		resultList[1].joinResult(resultList[0]);
		return resultList[1];
	}
}

// First element contains clauses with the selected synonym.
// Last element contains clauses without synonyms.
vector<pair<SuchThatClauseList, PatternClauseList>> QueryEvaluator::splitClauses(
		QueryProperties& queryProperties, vector<unordered_set<string>>& synonymsInGroup) {
	vector<pair<SuchThatClauseList, PatternClauseList>> result(synonymsInGroup.size() + 1);

	for (SuchThatClause suchThatClause : queryProperties.getSuchThatClauseList()) {
		for (int i = 0; i <= synonymsInGroup.size(); i++) {
			vector<string> declarations = suchThatClause.relation->getDeclarationSymbols();
			if (i == synonymsInGroup.size() || declarations.size() == 0) {
				result[synonymsInGroup.size()].first.push_back(suchThatClause);
				break;
			}

			if (synonymsInGroup[i].find(declarations[0]) != synonymsInGroup[i].end()) {
				result[i].first.push_back(suchThatClause);
				break;
			}
		}
	}

	for (PatternClause patternClause : queryProperties.getPatternClauseList()) {
		for (int i = 0; i <= synonymsInGroup.size(); i++) {
			if (i == synonymsInGroup.size()) {
				result[synonymsInGroup.size()].second.push_back(patternClause);
				break;
			}

			if (synonymsInGroup[i].find(patternClause.synonym.symbol) != synonymsInGroup[i].end()) {
				result[i].second.push_back(patternClause);
				break;
			}
		}
	}

	return result;
}
