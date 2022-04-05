#include "AssignStore.h"

#include <stdexcept>
#include <utility>

using namespace std;

PKB::AssignStore::AssignStore() = default;

void PKB::AssignStore::setAssign(const shared_ptr<StmtInfo>& statement, VarRef variable,
                                 Common::ExpressionProcessor::Expression expression) {
	StmtRef idx = statement->getIdentifier();
	assert(statement->getType() == StmtType::Assign);
	assert(!variable.empty());
	auto iter = store.find(idx);
	assert(iter == store.end());
	AssignRelation relation = {statement, move(variable), move(expression)};
	store.emplace(idx, relation);
}

bool PKB::AssignStore::patternExists(const VarRef& variable, const Common::ExpressionProcessor::Expression& expression,
                                     bool is_exact_match) {
	for (auto& assignment : store) {
		if (compareExpressions(assignment.second, variable, expression, is_exact_match)) {
			return true;
		}
	}
	return false;
}

StmtInfoPtrSet PKB::AssignStore::getStmtsWithPattern(const VarRef& variable, const Common::ExpressionProcessor::Expression& expression,
                                                     bool is_exact_match) {
	StmtInfoPtrSet result;
	for (auto& assignment : store) {
		if (compareExpressions(assignment.second, variable, expression, is_exact_match)) {
			result.emplace(assignment.second.node);
		}
	}
	return result;
}

StmtInfoPtrSet PKB::AssignStore::getStmtsWithPatternLHS(const VarRef& var_name) {
	StmtInfoPtrSet result;
	for (const auto& assignment : store) {
		if (assignment.second.variable == var_name) {
			result.emplace(assignment.second.node);
		}
	}
	return result;
}

StmtInfoPtrVarRefSet PKB::AssignStore::getStmtsWithPatternRHS(const Common::ExpressionProcessor::Expression& expression,
                                                              bool is_exact_match) {
	StmtInfoPtrVarRefSet result;
	for (auto& assignment : store) {
		AssignRelation assign_relation = assignment.second;
		if (compareExpressions(assign_relation, "", expression, is_exact_match)) {
			pair<shared_ptr<StmtInfo>, VarRef> pair = make_pair(assignment.second.node, assignment.second.variable);
			result.emplace(pair);
		}
	}
	return result;
}

bool PKB::AssignStore::compareExpressions(PKB::AssignRelation& relation, const VarRef& variable,
                                          const Common::ExpressionProcessor::Expression& op_tree, bool is_exact_match) {
	if (!variable.empty() && relation.variable != variable) {
		return false;
	}
	if (is_exact_match) {
		return relation.expression == op_tree;
	}
	return relation.expression.contains(op_tree);
}

void PKB::AssignStore::clear() { store.clear(); }

unordered_map<StmtRef, PKB::AssignRelation> PKB::AssignStore::getAssignMap() { return store; }
