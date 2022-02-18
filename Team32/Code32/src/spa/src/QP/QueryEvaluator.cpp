#include "QP/QueryEvaluator.h"

#include <utility>

QueryEvaluator::QueryEvaluator(PKB& pkb) : pkb(pkb) {}

QueryResult QueryEvaluator::executeQuery(QueryProperties& query_properties) {
	if (query_properties.getSuchThatClauseList().empty() && query_properties.getPatternClauseList().empty()) {
		return executeNoClauses(query_properties.getSelect());
	}

	createSymbolToTypeMap(query_properties.getDeclarationList());
	QueryGraph graph = buildGraph(query_properties);
	Declaration select = query_properties.getSelect();
	vector<unordered_set<string>> synonyms_in_group = graph.getSynonymsInGroup(select.symbol);
	vector<pair<SuchThatClauseList, PatternClauseList>> clausesInGroup = splitClauses(query_properties, synonyms_in_group);

	QueryResult result;
	if (clausesInGroup[0].first.empty() && clausesInGroup[0].second.empty()) {
		result = executeNoClauses(select);
	} else {
		result = evaluateClauses(clausesInGroup[0].first, clausesInGroup[0].second, select, false);
	}

	for (int i = 1; i < clausesInGroup.size(); i++) {
		// The clauses in last group should be evaluated independently since they are unrelated
		if (i == clausesInGroup.size() - 1) {
			for (const SuchThatClause& such_that_clause : clausesInGroup[i].first) {
				SuchThatClauseList such_that_list = {such_that_clause};
				PatternClauseList pattern_list = {};
				if (!evaluateClauses(such_that_list, pattern_list, select, true).getResult()) {
					return {};
				}
			}
			for (const PatternClause& pattern_clause : clausesInGroup[i].second) {
				SuchThatClauseList such_that_list = {};
				PatternClauseList pattern_list = {pattern_clause};
				if (!evaluateClauses(such_that_list, pattern_list, select, true).getResult()) {
					return {};
				}
			}
		}
		else {
			if (clausesInGroup[i].first.size() + clausesInGroup[i].second.size() == 0) {
				continue;
			}
			if (clausesInGroup[i].first.size() + clausesInGroup[i].second.size() == 1) {
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
	}

	return result;
}

QueryResult QueryEvaluator::executeNoClauses(const Declaration& select) {
	switch (select.type) {
	case DesignEntity::Stmt: {
		StmtInfoPtrSet stmt_set = pkb.getStatements();
		QueryResult result = QueryResult();

		vector<string> result_string;
		for (auto const& stmt : stmt_set) {
			result_string.push_back(to_string(stmt->reference));
		}

		result.addColumn(select.symbol, result_string);
		return result;
	}
	case DesignEntity::Read: {
		return getSpecificStmtType(StmtType::Read, select.symbol);
	}
	case DesignEntity::Print: {
		return getSpecificStmtType(StmtType::Print, select.symbol);
	}
	case DesignEntity::Call: {
		return getSpecificStmtType(StmtType::Call, select.symbol);
	}
	case DesignEntity::While: {
		return getSpecificStmtType(StmtType::WhileStmt, select.symbol);
	}
	case DesignEntity::If: {
		return getSpecificStmtType(StmtType::IfStmt, select.symbol);
	}
	case DesignEntity::Assign: {
		return getSpecificStmtType(StmtType::Assign, select.symbol);
	}
	case DesignEntity::Variable: {
		VarRefSet var_set = pkb.getVariables();
		QueryResult result = QueryResult();

		vector<string> result_string;
		for (auto const& var : var_set) {
			result_string.push_back(var);
		}

		result.addColumn(select.symbol, result_string);
		return result;
	}
	case DesignEntity::Constant: {
		unordered_set<ConstVal> constants = pkb.getConstants();
		QueryResult result = QueryResult();

		vector<string> result_string;
		result_string.reserve(constants.size());
		for (auto const& constant : constants) {
			result_string.push_back(to_string(constant));
		}

		result.addColumn(select.symbol, result_string);
		return result;
	}
	default:
		return {};
	}
}

QueryResult QueryEvaluator::getSpecificStmtType(StmtType type, string symbol) {
	StmtInfoPtrSet stmt_set = pkb.getStatements();
	QueryResult result = QueryResult();

	vector<string> result_string;
	for (auto const& stmt : stmt_set) {
		if (stmt->type == type) {
			result_string.push_back(to_string(stmt->reference));
		}
	}

	result.addColumn(std::move(symbol), result_string);
	return result;
}

QueryGraph QueryEvaluator::buildGraph(QueryProperties& query_properties) {
	QueryGraph graph = QueryGraph(query_properties.getDeclarationList());
	graph.setEdges(query_properties.getSuchThatClauseList(), query_properties.getPatternClauseList());

	return graph;
}

QueryResult QueryEvaluator::evaluateClauses(SuchThatClauseList& such_that_clauses, PatternClauseList& pattern_clauses,
                                            const Declaration& select, bool is_trivial) {
	vector<QueryResult> resultList;

	for (const SuchThatClause& such_that_clause : such_that_clauses) {
		QueryResult result = such_that_clause.relation->execute(pkb, is_trivial, symbol_to_type_map);
		if (!result.getResult()) {
			return {};
		}
		resultList.push_back(result);
	}

	for (const PatternClause& pattern_clause : pattern_clauses) {
		QueryResult result = pattern_clause.relation->execute(pkb, is_trivial, symbol_to_type_map);
		if (!result.getResult()) {
			return {};
		}
		resultList.push_back(result);
	}
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
	}
	resultList[1].joinResult(resultList[0]);
	return resultList[1];
}

// First element contains clauses with the selected synonym.
// Last element contains clauses without synonyms.
vector<pair<SuchThatClauseList, PatternClauseList>> QueryEvaluator::splitClauses(QueryProperties& query_properties,
                                                                                 vector<unordered_set<string>>& synonyms_in_group) {
	vector<pair<SuchThatClauseList, PatternClauseList>> result(synonyms_in_group.size() + 1);

	for (const SuchThatClause& such_that_clause : query_properties.getSuchThatClauseList()) {
		for (int i = 0; i <= synonyms_in_group.size(); i++) {
			vector<string> declarations = such_that_clause.relation->getDeclarationSymbols();
			if (i == synonyms_in_group.size() || declarations.empty()) {
				result[synonyms_in_group.size()].first.push_back(such_that_clause);
				break;
			}

			if (synonyms_in_group[i].find(declarations[0]) != synonyms_in_group[i].end()) {
				result[i].first.push_back(such_that_clause);
				break;
			}
		}
	}
	for (const PatternClause& pattern_clause : query_properties.getPatternClauseList()) {
		for (int i = 0; i <= synonyms_in_group.size(); i++) {
			vector<string> declarations = pattern_clause.relation->getDeclarationSymbols();
			if (i == synonyms_in_group.size()) {
				result[synonyms_in_group.size()].second.push_back(pattern_clause);
				break;
			}

			if (synonyms_in_group[i].find(declarations[0]) != synonyms_in_group[i].end()) {
				result[i].second.push_back(pattern_clause);
				break;
			}
		}
	}

	return result;
}

void QueryEvaluator::createSymbolToTypeMap(const DeclarationList& declarations) {
	unordered_map<string, DesignEntity> map;
	for (Declaration const& declaration : declarations) {
		map.insert({ declaration.symbol, declaration.type });
	}
	this->symbol_to_type_map = map;
}
