#include "PKB.h"

#include <utility>

using namespace std;

PKB::PKB() = default;

// This method will store information about a statement into PKB's statement map.
// Source Processor is guaranteed to call this method before storing relationships and variables.
void PKB::setStmtType(StmtRef idx, StmtType type) { statement_store.insert(idx, type); }

void PKB::setConstant(ConstVal value) { constant_store.insert(value); }

void PKB::setConstant(unordered_set<ConstVal> values) { constant_store.insert(values); }

StmtInfoPtrSet PKB::getStatements() { return statement_store.getAll(); }

VarRefSet PKB::getVariables() { return variable_store.getAll(); }

unordered_set<ConstVal> PKB::getConstants() { return constant_store.getAll(); }

void PKB::setParent(StmtRef parent, StmtRef child) {
	shared_ptr<StmtInfo> parent_info = statement_store.get(parent);
	shared_ptr<StmtInfo> child_info = statement_store.get(child);
	if (parent_info == nullptr || child_info == nullptr) {
		throw "Statement does not exist";
	}
	parent_store.set(parent_info, child_info);
}

bool PKB::checkParents(StmtRef parent, StmtRef child) { return parent_store.isRelated(parent, child); }

shared_ptr<StmtInfo> PKB::getParent(StmtRef idx) {
	auto result = parent_store.getForward(idx);
	if (result.empty()) {
		return nullptr;
	}
	return *result.begin();
}

unordered_set<shared_ptr<StmtInfo>> PKB::getChildren(StmtRef idx) { return parent_store.getReverse(idx); }

unordered_set<shared_ptr<StmtInfo>> PKB::getParentStar(StmtRef idx) { return parent_store.getForwardTransitive(idx); }

unordered_set<shared_ptr<StmtInfo>> PKB::getChildStar(StmtRef idx) { return parent_store.getReverseTransitive(idx); }

void PKB::setFollows(StmtRef following, StmtRef follower) {
	shared_ptr<StmtInfo> following_info = statement_store.get(following);
	shared_ptr<StmtInfo> follower_info = statement_store.get(follower);
	if (following_info == nullptr || follower_info == nullptr) {
		throw "Statement does not exist";
	}
	follows_store.set(following_info, follower_info);
}

bool PKB::checkFollows(StmtRef following, StmtRef follower) { return follows_store.isRelated(following, follower); }

shared_ptr<StmtInfo> PKB::getPreceding(StmtRef idx) {
	auto result = follows_store.getForward(idx);
	if (result.empty()) {
		return nullptr;
	}
	return *result.begin();
}

shared_ptr<StmtInfo> PKB::getFollower(StmtRef idx) {
	auto result = follows_store.getReverse(idx);
	if (result.empty()) {
		return nullptr;
	}
	return *result.begin();
}

unordered_set<shared_ptr<StmtInfo>> PKB::getPrecedingStar(StmtRef idx) { return follows_store.getForwardTransitive(idx); }

unordered_set<shared_ptr<StmtInfo>> PKB::getFollowerStar(StmtRef idx) { return follows_store.getReverseTransitive(idx); }

void PKB::setModifies(StmtRef idx, VarRef variable) {
	shared_ptr<StmtInfo> statement = statement_store.get(idx);
	if (statement == nullptr) {
		throw "Statement does not exist";
	}
	variable_store.insert(variable);
	modifies_store.set(std::move(statement), std::move(variable));
}

void PKB::setModifies(StmtRef idx, VarRefSet variables) {
	shared_ptr<StmtInfo> statement = statement_store.get(idx);
	if (statement == nullptr) {
		throw "Statement does not exist";
	}
	variable_store.insert(variables);
	modifies_store.set(std::move(statement), std::move(variables));
}

bool PKB::checkModifies(StmtRef idx, VarRef variable) { return modifies_store.check(idx, std::move(variable)); }

unordered_set<shared_ptr<StmtInfo>> PKB::getModifiesByVar(VarRef variable) { return modifies_store.getByVar(std::move(variable)); }

unordered_set<VarRef> PKB::getModifiesByStmt(StmtRef idx) { return modifies_store.getByStmt(idx); }

void PKB::setUses(StmtRef idx, VarRef variable) {
	shared_ptr<StmtInfo> statement = statement_store.get(idx);
	if (statement == nullptr) {
		throw "Statement does not exist";
	}
	variable_store.insert(variable);
	uses_store.set(statement, std::move(variable));
}

void PKB::setUses(StmtRef idx, VarRefSet variables) {
	shared_ptr<StmtInfo> statement = statement_store.get(idx);
	if (statement == nullptr) {
		throw "Statement does not exist";
	}
	variable_store.insert(variables);
	uses_store.set(statement, std::move(variables));
}

bool PKB::checkUses(StmtRef stmt, VarRef variable) { return uses_store.check(stmt, variable); }

unordered_set<shared_ptr<StmtInfo>> PKB::getUsesByVar(VarRef variable) { return uses_store.getByVar(variable); }

unordered_set<VarRef> PKB::getUsesByStmt(StmtRef stmt) { return uses_store.getByStmt(stmt); }

void PKB::setAssign(StmtRef idx, VarRef variable, Common::ExpressionProcessor::Expression expression) {
	shared_ptr<StmtInfo> statement = statement_store.get(idx);
	return assign_store.setAssign(statement, variable, expression);
}

bool PKB::patternExists(VarRef variable, Common::ExpressionProcessor::Expression expression, bool is_exact_match) {
	return assign_store.patternExists(variable, expression, is_exact_match);
}

StmtInfoPtrSet PKB::getStmtsWithPattern(VarRef variable, Common::ExpressionProcessor::Expression expression, bool is_exact_match) {
	return assign_store.getStmtsWithPattern(variable, expression, is_exact_match);
}

StmtInfoPtrSet PKB::getStmtsWithPatternLHS(VarRef variable) { return assign_store.getStmtsWithPatternLHS(variable); }

vector<pair<shared_ptr<StmtInfo>, VarRef>> PKB::getStmtsWithPatternRHS(Common::ExpressionProcessor::Expression expression,
                                                                              bool is_exact_match) {
	return assign_store.getStmtsWithPatternRHS(expression, is_exact_match);
}

void PKB::populateComplexRelations() {
	ParentRelation::optimize(parent_store);
	FollowsRelation::optimize(follows_store);
	Modifies::optimize(statement_store, parent_store, modifies_store);
	Uses::optimize(statement_store, parent_store, uses_store);
}

void PKB::clear() {
	parent_store.clear();
	follows_store.clear();
	assign_store.clear();
	uses_store.clear();
	modifies_store.clear();
	statement_store.clear();
}

unordered_map<StmtRef, shared_ptr<StmtInfo>> PKB::getStmtInfoMap() {
	StmtInfoPtrSet set = statement_store.getAll();
	unordered_map<StmtRef, shared_ptr<StmtInfo>> map;
	for (const auto& item : set) {
		map.insert({item->reference, item});
	}
	return map;
}
