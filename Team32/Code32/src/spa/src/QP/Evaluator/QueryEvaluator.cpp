#include "QP/Evaluator/QueryEvaluator.h"

#include "Common/TypeDefs.h"
#include "QP/Evaluator/Clause.h"
#include "QP/Optimizer/QueryGraph.h"
#include "QP/QueryUtils.h"

using namespace std;
using namespace QP;
using namespace Evaluator;
using namespace Types;

QueryEvaluator::QueryEvaluator(StorageAdapter& store) : store(store) {}

QueryResult QueryEvaluator::executeQuery(const QueryProperties& query_properties) const {
	ClauseList clauses_without_synonyms = getClausesWithoutSynonyms(query_properties);
	if (!executeTrivialGroup(clauses_without_synonyms).getResult()) {
		return {};
	}

	auto declarations = query_properties.getDeclarationList();
	auto clauses = query_properties.getClauseList();
	auto select_list = query_properties.getSelectSynonymList();
	Optimizer::QueryGraph graph = Optimizer::QueryGraph(declarations, clauses, select_list);
	size_t number_of_groups = graph.getNumberOfGroups();

	vector<QueryResult> results;
	for (size_t i = 0; i < number_of_groups; i++) {
		ClauseList group_clauses = graph.getGroupClauses(i);
		DeclarationList group_select_list = graph.getGroupSelectedSynonyms(i);
		bool result = executeGroup(group_clauses, group_select_list, results);
		if (!result) {
			return {};
		}
	}

	QueryResult final_result = QueryResult::joinResults(results);
	if (select_list.empty()) {
		return QueryResult(true);
	}
	return final_result;
}

bool QueryEvaluator::executeGroup(const ClauseList& clauses, const DeclarationList& select_list, vector<QueryResult>& results) const {
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

QueryResult QueryEvaluator::executeGroupWithSelected(const ClauseList& clauses, const DeclarationList& select_list) const {
	if (clauses.empty()) {
		return executeNoClauses(select_list[0]);
	}

	return executeNonTrivialGroup(clauses, select_list);
}

QueryResult QueryEvaluator::executeGroupWithoutSelected(const ClauseList& clauses, const DeclarationList& select_list) const {
	if (clauses.empty()) {
		return QueryResult(true);
	}

	if (clauses.size() == 1) {
		return executeTrivialGroup(clauses);
	}

	return executeNonTrivialGroup(clauses, select_list);
}

QueryResult QueryEvaluator::executeTrivialGroup(const ClauseList& clauses) const {
	for (const auto& clause : clauses) {
		QueryResult result = clause->executeTrivial(store);
		if (!result.getResult()) {
			return {};
		}
	}

	return QueryResult(true);
}

QueryResult QueryEvaluator::executeNonTrivialGroup(const ClauseList& clauses, const DeclarationList& select_list) const {
	vector<QueryResult> result_list;
	result_list.reserve(clauses.size());

	for (const auto& clause : clauses) {
		QueryResult result = clause->execute(store, result_list);
		if (!result.getResult()) {
			return {};
		}
		result_list.push_back(result);
	}

	QueryResult final_result = QueryResult::joinIntraGroupResults(result_list);
	final_result.filterBySelect(select_list);

	return final_result;
}

QueryResult QueryEvaluator::executeNoClauses(const Declaration& select) const {
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

QueryResult QueryEvaluator::getSpecificStmtType(const Declaration& declaration) const {
	StmtInfoPtrSet stmt_set = store.getStatements();
	QueryResult result = QueryResult({declaration.symbol});

	for (const auto& stmt : stmt_set) {
		if (Utilities::checkStmtTypeMatch(stmt, declaration.type)) {
			result.addRow({to_string(stmt->getIdentifier())});
		}
	}

	return result;
}

QueryResult QueryEvaluator::getConstants(const string& symbol) const {
	ConstValSet constants = store.getConstants();
	QueryResult result = QueryResult({symbol});

	for (const auto& constant : constants) {
		result.addRow({to_string(constant)});
	}
	return result;
}

QueryResult QueryEvaluator::getVariables(const string& symbol) const {
	VarRefSet var_set = store.getVariables();
	QueryResult result = QueryResult({symbol});

	for (const auto& var : var_set) {
		result.addRow({var});
	}

	return result;
}

QueryResult QueryEvaluator::getProcedures(const string& symbol) const {
	ProcRefSet proc_set = store.getProcedures();
	QueryResult result = QueryResult({symbol});

	for (const auto& proc : proc_set) {
		result.addRow({proc});
	}

	return result;
}

ClauseList QueryEvaluator::getClausesWithoutSynonyms(const QueryProperties& query_properties) {
	ClauseList clauses;
	for (const auto& clause : query_properties.getClauseList()) {
		if (clause->getDeclarationSymbols().empty()) {
			clauses.push_back(clause);
		}
	}

	return clauses;
}
