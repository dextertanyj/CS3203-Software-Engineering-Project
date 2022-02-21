#include "AssignStore.h"

#include <stdexcept>
#include <utility>

using namespace std;

PKB::AssignStore::AssignStore() = default;

void PKB::AssignStore::setAssign(const shared_ptr<StmtInfo>& statement, VarRef variable, Common::ExpressionProcessor::Expression expression) {
	StmtRef idx = statement->reference;
	if (statement->type != StmtType::Assign) {
		throw invalid_argument("Statement type cannot be bound to expression.");
	}
	if (variable.empty()) {
		throw invalid_argument("Variable cannot be an empty string.");
	}
	auto iter = store.find(idx);
	if (iter != store.end()) {
		throw invalid_argument("Statement already bound to existing expression");
	}
	AssignRelation relation = {statement, move(variable), move(expression)};
	store.insert({idx, relation});
}

bool PKB::AssignStore::patternExists(const VarRef& variable, const Common::ExpressionProcessor::Expression& expression, bool is_exact_match) {
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
			result.insert(assignment.second.node);
		}
	}
	return result;
}

StmtInfoPtrSet PKB::AssignStore::getStmtsWithPatternLHS(const VarRef& var_name) {
	StmtInfoPtrSet result;
	for (const auto& assignment : store) {
		if (assignment.second.variable == var_name) {
			result.insert(assignment.second.node);
		}
	}
	return result;
}

vector<pair<shared_ptr<StmtInfo>, VarRef>> PKB::AssignStore::getStmtsWithPatternRHS(const Common::ExpressionProcessor::Expression& expression,
                                                                               bool is_exact_match) {
	vector<pair<shared_ptr<StmtInfo>, VarRef>> result;
	for (auto& assignment : store) {
		AssignRelation assign_relation = assignment.second;
		if (compareExpressions(assign_relation, "", expression, is_exact_match)) {
			pair<shared_ptr<StmtInfo>, VarRef> pair = make_pair(assignment.second.node, assignment.second.variable);
			result.push_back(pair);
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
		return relation.expression.equals(op_tree);
	}
	return relation.expression.contains(op_tree);
}

void PKB::AssignStore::clear() { store.clear(); }

unordered_map<StmtRef, PKB::AssignRelation> PKB::AssignStore::getAssignMap() { return store; }
