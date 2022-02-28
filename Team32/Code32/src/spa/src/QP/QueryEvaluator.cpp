#include "QP/QueryEvaluator.h"

#include <utility>

QP::QueryEvaluator::QueryEvaluator(PKB::StorageAccessInterface& pkb) : pkb(pkb) {}

QP::QueryResult QP::QueryEvaluator::executeQuery(QueryProperties& query_properties) {
	if (query_properties.getSuchThatClauseList().empty() && query_properties.getPatternClauseList().empty()) {
		return executeNoClauses(query_properties.getSelect());
	}

	createSymbolToTypeMap(query_properties.getDeclarationList());
	QueryGraph graph = buildGraph(query_properties);
	Declaration select = query_properties.getSelect();
	vector<unordered_set<string>> synonyms_in_group = graph.getSynonymsInGroup(select.symbol);
	vector<pair<SuchThatClauseList, PatternClauseList>> clauses_in_group = splitClauses(query_properties, synonyms_in_group);

	QueryResult result;
	if (clauses_in_group[0].first.empty() && clauses_in_group[0].second.empty()) {
		result = executeNoClauses(select);
	} else {
		result = evaluateClauses(clauses_in_group[0].first, clauses_in_group[0].second, select, false);
	}

	// Execute clauses without synonyms first
	size_t last_group = clauses_in_group.size() - 1;
	if (!executeClausesWithoutSynonym(clauses_in_group[last_group].first, clauses_in_group[last_group].second, select).getResult()) {
		return {};
	}

	for (size_t i = 1; i < last_group; i++) {
		if (clauses_in_group[i].first.size() + clauses_in_group[i].second.size() == 0) {
			continue;
		}
		if (clauses_in_group[i].first.size() + clauses_in_group[i].second.size() == 1) {
			QueryResult query_result = evaluateClauses(clauses_in_group[i].first, clauses_in_group[i].second, select, true);
			if (!query_result.getResult()) {
				return {};
			}
		} else {
			QueryResult query_result = evaluateClauses(clauses_in_group[i].first, clauses_in_group[i].second, select, false);
			if (!query_result.getResult()) {
				return {};
			}
		}
	}

	return result;
}

QP::QueryResult QP::QueryEvaluator::executeClausesWithoutSynonym(SuchThatClauseList& such_that_clauses, PatternClauseList& pattern_clauses,
                                                                 const Declaration& select) {
	// These clauses should be evaluated independently since they are unrelated
	for (const SuchThatClause& such_that_clause : such_that_clauses) {
		SuchThatClauseList such_that_list = {such_that_clause};
		PatternClauseList pattern_list = {};
		if (!evaluateClauses(such_that_list, pattern_list, select, true).getResult()) {
			return {};
		}
	}
	for (const PatternClause& pattern_clause : pattern_clauses) {
		SuchThatClauseList such_that_list = {};
		PatternClauseList pattern_list = {pattern_clause};
		if (!evaluateClauses(such_that_list, pattern_list, select, true).getResult()) {
			return {};
		}
	}

	return QueryResult(true);
}

QP::QueryResult QP::QueryEvaluator::executeNoClauses(const Declaration& select) {
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

QP::QueryResult QP::QueryEvaluator::getSpecificStmtType(StmtType type, const string& symbol) {
	StmtInfoPtrSet stmt_set = pkb.getStatements();
	QueryResult result = QueryResult();

	vector<string> result_string;
	for (auto const& stmt : stmt_set) {
		if (stmt->type == type) {
			result_string.push_back(to_string(stmt->reference));
		}
	}

	result.addColumn(symbol, result_string);
	return result;
}

QP::QueryGraph QP::QueryEvaluator::buildGraph(QueryProperties& query_properties) {
	QueryGraph graph = QueryGraph(query_properties.getDeclarationList());
	graph.setEdges(query_properties.getSuchThatClauseList(), query_properties.getPatternClauseList());

	return graph;
}

QP::QueryResult QP::QueryEvaluator::evaluateClauses(SuchThatClauseList& such_that_clauses, PatternClauseList& pattern_clauses,
                                                    const Declaration& select, bool is_trivial) {
	vector<QueryResult> result_list;

	for (const SuchThatClause& such_that_clause : such_that_clauses) {
		QueryResult result = such_that_clause.relation->execute(pkb, is_trivial, symbol_to_type_map);
		if (!result.getResult()) {
			return {};
		}
		result_list.push_back(result);
	}

	for (const PatternClause& pattern_clause : pattern_clauses) {
		QueryResult result = pattern_clause.relation->execute(pkb, is_trivial, symbol_to_type_map);
		if (!result.getResult()) {
			return {};
		}
		result_list.push_back(result);
	}
	// For iteration one, the size of resultList is at most 2
	if (result_list.size() == 1) {
		return result_list[0];
	}

	// Merge the table without selected synonym to the one with selected synonym.
	// If both does not contain selected synonym, we merge the smaller table to larger table.
	const unordered_set<string> synonyms_in_first_result = result_list[0].getSynonymsStored();
	const unordered_set<string> synonyms_in_second_result = result_list[1].getSynonymsStored();

	if (synonyms_in_first_result.find(select.symbol) != synonyms_in_first_result.end()) {
		result_list[0].joinResult(result_list[1]);
		return result_list[0];
	}

	if (synonyms_in_second_result.find(select.symbol) != synonyms_in_second_result.end()) {
		result_list[1].joinResult(result_list[0]);
		return result_list[1];
	}

	if (synonyms_in_first_result.size() >= synonyms_in_second_result.size()) {
		result_list[0].joinResult(result_list[1]);
		return result_list[0];
	}
	result_list[1].joinResult(result_list[0]);
	return result_list[1];
}

// First element contains clauses with the selected synonym.
// Last element contains clauses without synonyms.
vector<pair<SuchThatClauseList, PatternClauseList>> QP::QueryEvaluator::splitClauses(QueryProperties& query_properties,
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

void QP::QueryEvaluator::createSymbolToTypeMap(const DeclarationList& declarations) {
	unordered_map<string, DesignEntity> map;
	for (Declaration const& declaration : declarations) {
		map.insert({declaration.symbol, declaration.type});
	}
	this->symbol_to_type_map = map;
}
