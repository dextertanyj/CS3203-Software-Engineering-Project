#include "QP/QueryEvaluator.h"

#include <utility>

#include "QP/ConnectedSynonyms.h"
#include "QP/QueryUtils.h"

using QP::Types::Clause;
using QP::Types::ConnectedSynonyms;
using QP::Types::DesignEntity;

QP::QueryEvaluator::QueryEvaluator(QP::StorageAdapter& store) : store(store) {}

QP::QueryResult QP::QueryEvaluator::executeQuery(QueryProperties& query_properties) {
	QueryGraph graph = buildGraph(query_properties);
	DeclarationList select_list = query_properties.getSelectSynonymList();
	ConnectedSynonyms connected_synonyms = graph.getConnectedSynonyms(select_list);
	size_t number_of_groups = connected_synonyms.getNumberOfGroups();

	vector<QueryResult> results;

	ClauseList clauses_without_synonyms = getClausesWithoutSynonyms(query_properties);
	if (!executeTrivialGroup(clauses_without_synonyms).getResult()) {
		return {};
	}

	for (size_t i = 0; i < number_of_groups; i++) {
		ClauseList group_clauses = graph.sortGroup(i);
		DeclarationList group_select_list = connected_synonyms.getGroupSelectedSynonyms(i);

		bool has_result = executeGroup(group_clauses, group_select_list, results);

		if (!has_result) {
			return {};
		}
	}

	QueryResult final_result = QueryResult::joinResults(results);

	if (select_list.empty()) {
		return QueryResult(true);
	}

	return final_result;
}

bool QP::QueryEvaluator::executeGroup(ClauseList& clauses, DeclarationList& select_list, vector<QueryResult>& results) {
	if (select_list.empty()) {
		return executeGroupWithoutSelected(clauses, select_list).getResult();
	}

	QueryResult group_result = executeGroupWithSelected(clauses, select_list);
	if (group_result.getResult()) {
		results.push_back(group_result);
		return true;
	}

	return false;
}

QP::QueryResult QP::QueryEvaluator::executeGroupWithSelected(ClauseList& clauses, DeclarationList& select_list) {
	if (clauses.empty()) {
		return executeNoClauses(select_list[0]);
	}

	return executeNonTrivialGroup(clauses, select_list);
}

QP::QueryResult QP::QueryEvaluator::executeGroupWithoutSelected(ClauseList& clauses, DeclarationList& select_list) {
	if (clauses.empty()) {
		return QueryResult(true);
	}

	if (clauses.size() == 1) {
		return executeTrivialGroup(clauses);
	}

	return executeNonTrivialGroup(clauses, select_list);
}

QP::QueryResult QP::QueryEvaluator::executeTrivialGroup(ClauseList& clauses) {
	for (const Clause& clause : clauses) {
		QueryResult result = clause.relation->executeTrivial(store);
		if (!result.getResult()) {
			return {};
		}
	}

	return QueryResult(true);
}

QP::QueryResult QP::QueryEvaluator::executeNonTrivialGroup(ClauseList& clauses, DeclarationList& select_list) {
	vector<QueryResult> result_list;

	for (const Clause& clause : clauses) {
		QueryResult result = clause.relation->execute(store);
		if (!result.getResult()) {
			return {};
		}
		result_list.push_back(result);
	}

	QueryResult final_result = QueryResult::joinResults(result_list);
	final_result.filterBySelect(select_list);

	return final_result;
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
			return getSpecificStmtType(select);
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

QP::QueryResult QP::QueryEvaluator::getSpecificStmtType(const Declaration& declaration) {
	StmtInfoPtrSet stmt_set = store.getStatements();
	QueryResult result = QueryResult({declaration.symbol});

	for (auto const& stmt : stmt_set) {
		if (Utilities::checkStmtTypeMatch(stmt, declaration.type)) {
			result.addRow({to_string(stmt->getIdentifier())});
		}
	}

	return result;
}

QP::QueryResult QP::QueryEvaluator::getConstants(const string& symbol) {
	unordered_set<ConstVal> constants = store.getConstants();
	QueryResult result = QueryResult({symbol});

	for (auto const& constant : constants) {
		result.addRow({to_string(constant)});
	}
	return result;
}

QP::QueryResult QP::QueryEvaluator::getVariables(const string& symbol) {
	VarRefSet var_set = store.getVariables();
	QueryResult result = QueryResult({symbol});

	for (auto const& var : var_set) {
		result.addRow({var});
	}

	return result;
}

QP::QueryResult QP::QueryEvaluator::getProcedures(const string& symbol) {
	ProcRefSet proc_set = store.getProcedures();
	QueryResult result = QueryResult({symbol});

	for (auto const& proc : proc_set) {
		result.addRow({proc});
	}

	return result;
}

QP::QueryGraph QP::QueryEvaluator::buildGraph(QueryProperties& query_properties) {
	QueryGraph graph = QueryGraph(query_properties.getDeclarationList());
	graph.setEdges(query_properties.getClauseList());

	return graph;
}

ClauseList QP::QueryEvaluator::getClausesWithoutSynonyms(QueryProperties& query_properties) {
	ClauseList clauses;
	for (const Clause& clause : query_properties.getClauseList()) {
		if (clause.relation->getDeclarationSymbols().empty()) {
			clauses.push_back(clause);
		}
	}

	return clauses;
}
