#include "PKB/AssignStore.h"

#include <algorithm>
#include <cassert>

using namespace std;

PKB::AssignStore::AssignStore() = default;

void PKB::AssignStore::setAssign(const StmtInfoPtr& statement, const VarRef& variable, const Common::EP::Expression& expression) {
	assert(statement->getType() == StmtType::Assign);
	assert(!variable.empty());

	AssignRelation relation = {statement, variable, expression};
	var_to_relation_store[variable].emplace(relation);
	exp_to_relation_store[expression].emplace(relation);
}

bool PKB::AssignStore::patternExists(const VarRef& variable, const Common::EP::Expression& expression, bool is_exact_match) {
	auto iter = var_to_relation_store.find(variable);
	if (iter == var_to_relation_store.end()) {
		return false;
	}
	return any_of(iter->second.begin(), iter->second.end(),
	              [&](const AssignRelation& rel) { return compareExpressions(rel.expression, expression, is_exact_match); });
}

StmtInfoPtrSet PKB::AssignStore::getStmtsWithPattern(const VarRef& variable, const Common::EP::Expression& expression,
                                                     bool is_exact_match) {
	auto iter = var_to_relation_store.find(variable);
	if (iter == var_to_relation_store.end()) {
		return {};
	}
	StmtInfoPtrSet result;
	for (const AssignRelation& assign_relation : iter->second) {
		if (compareExpressions(assign_relation.expression, expression, is_exact_match)) {
			result.emplace(assign_relation.node);
		}
	}
	return result;
}

StmtInfoPtrSet PKB::AssignStore::getStmtsWithPatternLHS(const VarRef& var_name) {
	auto iter = var_to_relation_store.find(var_name);
	if (iter == var_to_relation_store.end()) {
		return {};
	}
	StmtInfoPtrSet result;
	for (const auto& assign_relation : iter->second) {
		result.emplace(assign_relation.node);
	}
	return result;
}

StmtInfoPtrVarRefSet PKB::AssignStore::getStmtsWithPatternRHS(const Common::EP::Expression& expression, bool is_exact_match) {
	unordered_set<AssignRelation> assign_relations;
	StmtInfoPtrVarRefSet result;

	auto iter = exp_to_relation_store.find(expression);
	if (is_exact_match && iter != exp_to_relation_store.end()) {
		for_each(iter->second.begin(), iter->second.end(), [&](const AssignRelation& rel) { result.emplace(rel.node, rel.variable); });
		return result;
	}
	if (is_exact_match) {  // Exact match and expression does not exist.
		return {};
	}

	for (const auto& expression_to_relation : exp_to_relation_store) {
		if (compareExpressions(expression_to_relation.first, expression, is_exact_match)) {
			assign_relations.insert(expression_to_relation.second.begin(), expression_to_relation.second.end());
		}
	}
	for (const auto& relation : assign_relations) {
		result.emplace(relation.node, relation.variable);
	}
	return result;
}

bool PKB::AssignStore::compareExpressions(const Common::EP::Expression& expression, const Common::EP::Expression& op_tree,
                                          bool is_exact_match) {
	if (is_exact_match) {
		return expression == op_tree;
	}
	return expression.contains(op_tree);
}

void PKB::AssignStore::clear() {
	var_to_relation_store.clear();
	exp_to_relation_store.clear();
}

unordered_map<VarRef, unordered_set<PKB::AssignRelation>> PKB::AssignStore::getAssignMap() { return var_to_relation_store; }
