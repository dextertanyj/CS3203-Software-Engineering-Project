#include "QP/QueryEvaluator.h"

#include <utility>

#include "QP/ConnectedSynonyms.h"
#include "QP/QueryUtils.h"

using QP::Types::Clause;
using QP::Types::ConnectedSynonyms;
using QP::Types::DesignEntity;

QP::QueryEvaluator::QueryEvaluator(PKB::StorageAccessInterface& pkb) : store(QP::StorageAdapter(pkb)) {}

QP::QueryResult QP::QueryEvaluator::executeQuery(QueryProperties& query_properties) {
	QueryGraph graph = buildGraph(query_properties);
	DeclarationList select_list = query_properties.getSelectList();
	ConnectedSynonyms connected_synonyms = graph.getConnectedSynonyms(select_list);
	vector<pair<Types::ClauseList, Types::DeclarationList>> clauses_in_group = splitClauses(query_properties, connected_synonyms);

	QueryResult result = QueryResult();

	// Execute clauses without synonyms first
	size_t last_group = clauses_in_group.size() - 1;
	if (!executeTrivialGroup(clauses_in_group[last_group].first).getResult()) {
		return {};
	}

	for (size_t i = 0; i < last_group; i++) {
		ClauseList clauses = clauses_in_group[i].first;
		DeclarationList select_list = clauses_in_group[i].second;
		if (select_list.empty()) {
			if (!executeGroupWithoutSelected(clauses, select_list).getResult()) {
				return {};
			}
		} else {
			QueryResult group_result = executeGroupWithSelected(clauses, select_list);
			if (!group_result.getResult()) {
				return {};
			}

			if (result.getResult()) {
				result.joinResult(group_result);
			} else {
				result = group_result;
			}
		}
	}

	if (select_list.empty()) {
		return QueryResult(true);
	}

	return result;
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

	for (size_t i = 1; i < result_list.size(); i++) {
		result_list[0].joinResult(result_list[i]);
	}

	if (!select_list.empty()) {
		result_list[0].filterBySelect(select_list);
	}

	return result_list[0];
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
	QueryResult result = QueryResult();

	vector<string> result_string;
	for (auto const& stmt : stmt_set) {
		if (Utilities::checkStmtTypeMatch(stmt, declaration.type)) {
			result_string.push_back(to_string(stmt->getIdentifier()));
		}
	}

	result.addColumn(declaration.symbol, result_string);
	return result;
}

QP::QueryResult QP::QueryEvaluator::getConstants(const string& symbol) {
	unordered_set<ConstVal> constants = store.getConstants();
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
	VarRefSet var_set = store.getVariables();
	QueryResult result = QueryResult();

	vector<string> result_string;
	for (auto const& var : var_set) {
		result_string.push_back(var);
	}

	result.addColumn(symbol, result_string);
	return result;
}

QP::QueryResult QP::QueryEvaluator::getProcedures(const string& symbol) {
	ProcRefSet proc_set = store.getProcedures();
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

vector<pair<ClauseList, DeclarationList>> QP::QueryEvaluator::splitClauses(QueryProperties& query_properties,
                                                                           ConnectedSynonyms& connected_synonyms) {
	size_t number_of_groups = connected_synonyms.getNumberOfGroups();

	vector<pair<ClauseList, DeclarationList>> result(number_of_groups + 1);
	for (const Clause& clause : query_properties.getClauseList()) {
		vector<string> declarations = clause.relation->getDeclarationSymbols();
		if (declarations.empty()) {
			result[number_of_groups].first.push_back(clause);
		} else {
			size_t group_number = connected_synonyms.getGroupNumber(declarations[0]);
			result[group_number].first.push_back(clause);
		}
	}

	result[number_of_groups].second = {};
	for (size_t i = 0; i < number_of_groups; i++) {
		result[i].second = connected_synonyms.getGroupSynonyms(i);
	}

	return result;
}
