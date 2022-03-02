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
		result = evaluateClauses(clauses_in_group[0].first, clauses_in_group[0].second, false);
	}

	// Execute clauses without synonyms first
	size_t last_group = clauses_in_group.size() - 1;
	if (!executeClausesWithoutSynonym(clauses_in_group[last_group].first, clauses_in_group[last_group].second).getResult()) {
		return {};
	}

	for (size_t i = 1; i < last_group; i++) {
		if (!executeGroup(clauses_in_group[i].first, clauses_in_group[i].second)) {
			return {};
		}
	}

	return result;
}

QP::QueryResult QP::QueryEvaluator::executeClausesWithoutSynonym(SuchThatClauseList& such_that_clauses,
                                                                 PatternClauseList& pattern_clauses) {
	// These clauses should be evaluated independently since they are unrelated
	for (const SuchThatClause& such_that_clause : such_that_clauses) {
		SuchThatClauseList such_that_list = {such_that_clause};
		PatternClauseList pattern_list = {};
		if (!evaluateClauses(such_that_list, pattern_list, true).getResult()) {
			return {};
		}
	}
	for (const PatternClause& pattern_clause : pattern_clauses) {
		SuchThatClauseList such_that_list = {};
		PatternClauseList pattern_list = {pattern_clause};
		if (!evaluateClauses(such_that_list, pattern_list, true).getResult()) {
			return {};
		}
	}

	return QueryResult(true);
}

bool QP::QueryEvaluator::executeGroup(SuchThatClauseList& such_that_clauses, PatternClauseList& pattern_clauses) {
	if (such_that_clauses.size() + pattern_clauses.size() == 0) {
		return true;
	}

	QueryResult query_result;
	if (such_that_clauses.size() + pattern_clauses.size() == 1) {
		query_result = evaluateClauses(such_that_clauses, pattern_clauses, true);
	} else {
		query_result = evaluateClauses(such_that_clauses, pattern_clauses, false);
	}

	return query_result.getResult();
}

QP::QueryResult QP::QueryEvaluator::executeNoClauses(const Declaration& select) {
	switch (select.type) {
		case DesignEntity::Stmt:
		case DesignEntity::Read:
		case DesignEntity::Print:
		case DesignEntity::Call:
		case DesignEntity::While:
		case DesignEntity::If:
		case DesignEntity::Assign: {
			return getSpecificStmtType(select.type, select.symbol);
		}
		case DesignEntity::Variable: {
			return getVariables(select.symbol);
		}
		case DesignEntity::Constant: {
			return getConstants(select.symbol);
		}
		case DesignEntity::Procedure: {
			return getProcedures(select.symbol);
		}
		default:
			return {};
	}
}

QP::QueryResult QP::QueryEvaluator::getSpecificStmtType(DesignEntity design_entity, const string& symbol) {
	StmtInfoPtrSet stmt_set = pkb.getStatements();
	QueryResult result = QueryResult();

	vector<string> result_string;
	for (auto const& stmt : stmt_set) {
		if (Utilities::checkStmtTypeMatch(stmt, design_entity)) {
			result_string.push_back(to_string(stmt->reference));
		}
	}

	result.addColumn(symbol, result_string);
	return result;
}

QP::QueryResult QP::QueryEvaluator::getConstants(const string& symbol) {
	unordered_set<ConstVal> constants = pkb.getConstants();
	QueryResult result = QueryResult();

	vector<string> result_string;
	result_string.reserve(constants.size());
	for (auto const& constant : constants) {
		result_string.push_back(to_string(constant));
	}

	result.addColumn(symbol, result_string);
	return result;
}

QP::QueryResult QP::QueryEvaluator::getVariables(const string& symbol) {
	VarRefSet var_set = pkb.getVariables();
	QueryResult result = QueryResult();

	vector<string> result_string;
	for (auto const& var : var_set) {
		result_string.push_back(var);
	}

	result.addColumn(symbol, result_string);
	return result;
}

QP::QueryResult QP::QueryEvaluator::getProcedures(const string& symbol) {
	ProcRefSet proc_set = pkb.getProcedures();
	QueryResult result = QueryResult();

	vector<string> result_string;
	for (auto const& proc : proc_set) {
		result_string.push_back(proc);
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
                                                    bool is_trivial) {
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

	for (int i = 1; i < result_list.size(); i++) {
		result_list[0].joinResult(result_list[i]);
	}
	return result_list[0];
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
