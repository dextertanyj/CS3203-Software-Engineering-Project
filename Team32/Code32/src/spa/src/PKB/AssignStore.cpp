#include "AssignStore.h"

#include <stdexcept>
#include <utility>

using namespace std;

PKB::AssignStore::AssignStore() = default;

void PKB::AssignStore::setAssign(const shared_ptr<StmtInfo>& statement, const VarRef& variable,
                                 const Common::ExpressionProcessor::Expression& expression) {
	assert(statement->getType() == StmtType::Assign);
	assert(!variable.empty());

	auto var_to_relation_iter = var_to_relation_store.find(variable);
	AssignRelation relation = {statement, variable, expression};
	if (var_to_relation_iter == var_to_relation_store.end()) {
		var_to_relation_store.insert({variable, {relation}});
	} else {
		var_to_relation_iter->second.emplace(relation);
	}

	auto exp_to_relation_iter = exp_to_relation_store.find(expression);
	if (exp_to_relation_iter == exp_to_relation_store.end()) {
		exp_to_relation_store.insert({expression, {relation}});
	} else {
		exp_to_relation_iter->second.emplace(relation);
	}
}

bool PKB::AssignStore::patternExists(const VarRef& variable, const Common::ExpressionProcessor::Expression& expression,
                                     bool is_exact_match) {
	auto iter = var_to_relation_store.find(variable);
	if (iter == var_to_relation_store.end()) {
		return false;
	}
	return any_of(iter->second.begin(), iter->second.end(),
	              [&](const AssignRelation& ar) { return compareExpressions(ar.expression, expression, is_exact_match); });
}

StmtInfoPtrSet PKB::AssignStore::getStmtsWithPattern(const VarRef& variable, const Common::ExpressionProcessor::Expression& expression,
                                                     bool is_exact_match) {
	auto iter = var_to_relation_store.find(variable);
	if (iter == var_to_relation_store.end()) {
		return {};
	}
	StmtInfoPtrSet result;
	for (const auto& assign_relation : iter->second) {
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

StmtInfoPtrVarRefSet PKB::AssignStore::getStmtsWithPatternRHS(const Common::ExpressionProcessor::Expression& expression,
                                                              bool is_exact_match) {
	unordered_set<PKB::AssignRelation, PKB::AssignRelation::Hasher> assign_relations;
	for (const auto& expression_to_relation : exp_to_relation_store) {
		if (compareExpressions(expression_to_relation.first, expression, is_exact_match)) {
			assign_relations.insert(expression_to_relation.second.begin(), expression_to_relation.second.end());
		}
	}
	StmtInfoPtrVarRefSet result;
	for (const auto& relation : assign_relations) {
		pair<shared_ptr<StmtInfo>, VarRef> pair = make_pair(relation.node, relation.variable);
		result.insert(pair);
	}
	return result;
}

bool PKB::AssignStore::compareExpressions(const Common::ExpressionProcessor::Expression& expression,
                                          const Common::ExpressionProcessor::Expression& op_tree, bool is_exact_match) {
	if (is_exact_match) {
		return expression == op_tree;
	}
	return expression.contains(op_tree);
}

void PKB::AssignStore::clear() {
	var_to_relation_store.clear();
	exp_to_relation_store.clear();
}

unordered_map<VarRef, unordered_set<PKB::AssignRelation, PKB::AssignRelation::Hasher>> PKB::AssignStore::getAssignMap() {
	return var_to_relation_store;
}
