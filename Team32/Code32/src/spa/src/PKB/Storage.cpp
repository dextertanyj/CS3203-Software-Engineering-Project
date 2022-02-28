#include "Storage.h"

#include <utility>

using namespace std;

// This method will store information about a statement into PKB's statement map.
// Source Processor is guaranteed to call this method before storing relationships and variables.
void PKB::Storage::setStmtType(StmtRef idx, StmtType type) { statement_store.insert(idx, type); }

void PKB::Storage::setConstant(ConstVal value) { constant_store.insert(value); }

void PKB::Storage::setConstant(const unordered_set<ConstVal>& values) { constant_store.insert(values); }

void PKB::Storage::setProc(ProcRef procedure, StmtRef start, StmtRef end) {}

void PKB::Storage::setCall(StmtRef index, ProcRef name) {}

void PKB::Storage::setParent(StmtRef parent, StmtRef child) {
	shared_ptr<StmtInfo> parent_info = statement_store.get(parent);
	shared_ptr<StmtInfo> child_info = statement_store.get(child);
	if (parent_info == nullptr || child_info == nullptr) {
		throw invalid_argument("Statement does not exist.");
	}
	parent_store.set(parent_info, child_info);
}

void PKB::Storage::setFollows(StmtRef following, StmtRef follower) {
	shared_ptr<StmtInfo> following_info = statement_store.get(following);
	shared_ptr<StmtInfo> follower_info = statement_store.get(follower);
	if (following_info == nullptr || follower_info == nullptr) {
		throw invalid_argument("Statement does not exist.");
	}
	follows_store.set(following_info, follower_info);
}

void PKB::Storage::setModifies(StmtRef idx, VarRef variable) {
	shared_ptr<StmtInfo> statement = statement_store.get(idx);
	if (statement == nullptr) {
		throw invalid_argument("Statement does not exist.");
	}
	variable_store.insert(variable);
	modifies_store.set(move(statement), move(variable));
}

void PKB::Storage::setModifies(StmtRef idx, VarRefSet variables) {
	shared_ptr<StmtInfo> statement = statement_store.get(idx);
	if (statement == nullptr) {
		throw invalid_argument("Statement does not exist.");
	}
	variable_store.insert(variables);
	modifies_store.set(move(statement), move(variables));
}

void PKB::Storage::setUses(StmtRef idx, VarRef variable) {
	shared_ptr<StmtInfo> statement = statement_store.get(idx);
	if (statement == nullptr) {
		throw invalid_argument("Statement does not exist.");
	}
	variable_store.insert(variable);
	uses_store.set(statement, move(variable));
}

void PKB::Storage::setUses(StmtRef idx, VarRefSet variables) {
	shared_ptr<StmtInfo> statement = statement_store.get(idx);
	if (statement == nullptr) {
		throw invalid_argument("Statement does not exist.");
	}
	variable_store.insert(variables);
	uses_store.set(statement, move(variables));
}

void PKB::Storage::setAssign(StmtRef idx, VarRef variable, Common::ExpressionProcessor::Expression expression) {
	shared_ptr<StmtInfo> statement = statement_store.get(idx);
	return assign_store.setAssign(statement, move(variable), move(expression));
}

StmtInfoPtrSet PKB::Storage::getStatements() { return statement_store.getAll(); }

VarRefSet PKB::Storage::getVariables() { return variable_store.getAll(); }

unordered_set<ConstVal> PKB::Storage::getConstants() { return constant_store.getAll(); }

unordered_set<ProcRef> PKB::Storage::getProcedures() { return unordered_set<ProcRef>(); }

bool PKB::Storage::checkParents(StmtRef parent, StmtRef child) { return parent_store.isRelated(parent, child); }

shared_ptr<StmtInfo> PKB::Storage::getParent(StmtRef idx) {
	auto result = parent_store.getForward(idx);
	if (result.empty()) {
		return nullptr;
	}
	return *result.begin();
}

StmtInfoPtrSet PKB::Storage::getChildren(StmtRef idx) { return parent_store.getReverse(idx); }

StmtInfoPtrSet PKB::Storage::getParentStar(StmtRef idx) { return parent_store.getForwardTransitive(idx); }

StmtInfoPtrSet PKB::Storage::getChildStar(StmtRef idx) { return parent_store.getReverseTransitive(idx); }

bool PKB::Storage::checkFollows(StmtRef following, StmtRef follower) { return follows_store.isRelated(following, follower); }

shared_ptr<StmtInfo> PKB::Storage::getPreceding(StmtRef idx) {
	auto result = follows_store.getForward(idx);
	if (result.empty()) {
		return nullptr;
	}
	return *result.begin();
}

shared_ptr<StmtInfo> PKB::Storage::getFollower(StmtRef idx) {
	auto result = follows_store.getReverse(idx);
	if (result.empty()) {
		return nullptr;
	}
	return *result.begin();
}

StmtInfoPtrSet PKB::Storage::getPrecedingStar(StmtRef idx) { return follows_store.getForwardTransitive(idx); }

StmtInfoPtrSet PKB::Storage::getFollowerStar(StmtRef idx) { return follows_store.getReverseTransitive(idx); }

bool PKB::Storage::checkCall(const ProcRef& caller, const ProcRef& callee) { return false; }

ProcRefSet PKB::Storage::getCallee(const ProcRef& caller) { return ProcRefSet(); }

ProcRefSet PKB::Storage::getCaller(const ProcRef& callee) { return ProcRefSet(); }

ProcRefSet PKB::Storage::getCalleeStar(const ProcRef& caller) { return ProcRefSet(); }

ProcRefSet PKB::Storage::getCallerStar(const ProcRef& callee) { return ProcRefSet(); }

bool PKB::Storage::checkModifies(StmtRef idx, const VarRef& variable) { return modifies_store.check(idx, variable); }

bool PKB::Storage::checkModifies(const ProcRef& procedure_name, const VarRef& variable_name) { return false; }

StmtInfoPtrSet PKB::Storage::getStmtModifiesByVar(const VarRef& variable) { return modifies_store.getByVar(variable); }

ProcRefSet PKB::Storage::getProcModifiesByVar(const VarRef& name) { return ProcRefSet(); }

unordered_set<VarRef> PKB::Storage::getModifiesByStmt(StmtRef idx) { return modifies_store.getByStmt(idx); }

VarRefSet PKB::Storage::getModifiesByProc(const ProcRef& name) { return VarRefSet(); }

bool PKB::Storage::checkUses(StmtRef stmt, const VarRef& variable) { return uses_store.check(stmt, variable); }

bool PKB::Storage::checkUses(const ProcRef& procedure_name, const VarRef& variable_name) { return false; }

StmtInfoPtrSet PKB::Storage::getStmtUsesByVar(const VarRef& variable) { return uses_store.getByVar(variable); }

ProcRefSet PKB::Storage::getProcUsesByVar(const VarRef& name) { return ProcRefSet(); }

unordered_set<VarRef> PKB::Storage::getUsesByStmt(StmtRef stmt) { return uses_store.getByStmt(stmt); }

VarRefSet PKB::Storage::getUsesByProc(const ProcRef& name) { return VarRefSet(); }

bool PKB::Storage::patternExists(const VarRef& variable, const Common::ExpressionProcessor::Expression& exp, bool is_exact_match) {
	return assign_store.patternExists(variable, exp, is_exact_match);
}

StmtInfoPtrSet PKB::Storage::getStmtsWithPattern(const VarRef& variable, const Common::ExpressionProcessor::Expression& exp,
                                                 bool is_exact_match) {
	return assign_store.getStmtsWithPattern(variable, exp, is_exact_match);
}

StmtInfoPtrSet PKB::Storage::getStmtsWithPatternLHS(const VarRef& variable) { return assign_store.getStmtsWithPatternLHS(variable); }

vector<pair<shared_ptr<StmtInfo>, VarRef>> PKB::Storage::getStmtsWithPatternRHS(const Common::ExpressionProcessor::Expression& exp,
                                                                                bool is_exact_match) {
	return assign_store.getStmtsWithPatternRHS(exp, is_exact_match);
}

void PKB::Storage::populateComplexRelations() {
	ParentRelation::optimize(parent_store);
	FollowsRelation::optimize(follows_store);
	ModifiesRelation::optimize(statement_store, parent_store, modifies_store);
	UsesRelation::optimize(statement_store, parent_store, uses_store);
}

void PKB::Storage::clear() {
	parent_store.clear();
	follows_store.clear();
	assign_store.clear();
	uses_store.clear();
	modifies_store.clear();
	statement_store.clear();
}

unordered_map<StmtRef, shared_ptr<StmtInfo>> PKB::Storage::getStmtInfoMap() {
	StmtInfoPtrSet set = statement_store.getAll();
	unordered_map<StmtRef, shared_ptr<StmtInfo>> map;
	for (const auto& item : set) {
		map.insert({item->reference, item});
	}
	return map;
}
