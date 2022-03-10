#include "QP/QueryEvaluator.h"

#include <utility>

QP::QueryEvaluator::QueryEvaluator(PKB::StorageAccessInterface& pkb) : pkb(pkb) {}

QP::QueryResult QP::QueryEvaluator::executeQuery(QueryProperties& query_properties) {
	if (query_properties.getClauseList().empty()) {
		return executeNoClauses(query_properties.getSelectList()[0]);
	}

	QueryGraph graph = buildGraph(query_properties);
	Declaration select = query_properties.getSelectList()[0];
	unordered_map<string, size_t> synonyms_in_group = graph.getSynonymsInGroup(select.symbol);
	vector<ClauseList> clauses_in_group = splitClauses(query_properties, synonyms_in_group);

	QueryResult result;
	if (clauses_in_group[0].empty()) {
		result = executeNoClauses(select);
	} else {
		result = evaluateClauses(clauses_in_group[0], false);
	}

	// Execute clauses without synonyms first
	size_t last_group = clauses_in_group.size() - 1;
	if (!executeClausesWithoutSynonym(clauses_in_group[last_group]).getResult()) {
		return {};
	}

	for (size_t i = 1; i < last_group; i++) {
		if (!executeGroup(clauses_in_group[i])) {
			return {};
		}
	}

	return result;
}

QP::QueryResult QP::QueryEvaluator::executeClausesWithoutSynonym(ClauseList& clauses) {
	// These clauses should be evaluated independently since they are unrelated
	for (const Clause& clause : clauses) {
		ClauseList list = {clause};
		if (!evaluateClauses(list, true).getResult()) {
			return {};
		}
	}

	return QueryResult(true);
}

bool QP::QueryEvaluator::executeGroup(ClauseList& clauses) {
	if (clauses.empty()) {
		return true;
	}

	QueryResult query_result;
	if (clauses.size() == 1) {
		query_result = evaluateClauses(clauses, true);
	} else {
		query_result = evaluateClauses(clauses, false);
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
			result_string.push_back(to_string(stmt->getIdentifier()));
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
	graph.setEdges(query_properties.getClauseList());

	return graph;
}

QP::QueryResult QP::QueryEvaluator::evaluateClauses(ClauseList& clauses, bool is_trivial) {
	vector<QueryResult> result_list;

	for (const Clause& clause : clauses) {
		QueryResult result = clause.relation->execute(pkb, is_trivial);
		if (!result.getResult()) {
			return {};
		}
		result_list.push_back(result);
	}

	for (size_t i = 1; i < result_list.size(); i++) {
		result_list[0].joinResult(result_list[i]);
	}
	return result_list[0];
}

// First element contains clauses with the selected synonym.
// Last element contains clauses without synonyms.
vector<ClauseList> QP::QueryEvaluator::splitClauses(QueryProperties& query_properties, unordered_map<string, size_t>& synonyms_in_group) {
	size_t number_of_groups = 0;
	for (auto const& pair : synonyms_in_group) {
		if (pair.second + 1 > number_of_groups) {
			number_of_groups = pair.second + 1;
		}
	}

	vector<ClauseList> result(number_of_groups + 1);

	for (const Clause& clause : query_properties.getClauseList()) {
		vector<string> declarations = clause.relation->getDeclarationSymbols();
		if (declarations.empty()) {
			result[number_of_groups].push_back(clause);
		} else {
			size_t group_number = synonyms_in_group[declarations[0]];
			result[group_number].push_back(clause);
		}
	}

	return result;
}
