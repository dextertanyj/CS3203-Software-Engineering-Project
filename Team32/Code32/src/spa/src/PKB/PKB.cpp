#include "PKB.h"

using namespace std;

PKB::PKB() {}

StmtInfoPtrSet PKB::getStatements() { return statement_store.getAll(); }

// This method will store information about a statement into PKB's statement map.
// Source Processor is guaranteed to call this method before storing relationships and variables.
void PKB::setStmtType(StmtRef idx, StmtType type) { statement_store.insert(idx, type); }

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

void PKB::setAssign(StmtRef stmtNo, VarRef variableLHS, Common::ArithmeticProcessor::ArithmeticExpression opTree) {
	return assign_store.setAssign(stmtNo, variableLHS, opTree);
}

void PKB::setUses(StmtRef stmt, VarRef varName) {
	shared_ptr<StmtInfo> stmtInfo = statement_store.get(stmt);
	uses_store.set(stmtInfo, varName);
}

void PKB::setModifies(StmtRef stmt, VarRef varName) {
	shared_ptr<StmtInfo> stmtInfo = statement_store.get(stmt);
	modifies_store.set(stmtInfo, varName);
}

bool PKB::checkUses(StmtRef stmt, VarRef varName) { return uses_store.check(stmt, varName); }

unordered_set<shared_ptr<StmtInfo>> PKB::getUsesByVar(VarRef var_name) { return uses_store.getByVar(var_name); }

unordered_set<VarRef> PKB::getUsesByStmt(StmtRef stmt) { return uses_store.getByStmt(stmt); }

bool PKB::checkModifies(StmtRef stmt, VarRef varName) { return modifies_store.check(stmt, varName); }

unordered_set<shared_ptr<StmtInfo>> PKB::getModifiesByVar(VarRef varName) { return modifies_store.getByVar(varName); }

unordered_set<VarRef> PKB::getModifiesByStmt(StmtRef stmt) { return modifies_store.getByStmt(stmt); }

bool PKB::patternExists(VarRef varName, Common::ArithmeticProcessor::ArithmeticExpression e, bool isRHSExactMatchNeeded) {
	return assign_store.patternExists(varName, e, isRHSExactMatchNeeded);
}

StmtInfoList PKB::getStmtsWithPattern(VarRef varName, Common::ArithmeticProcessor::ArithmeticExpression e, bool isRHSExactMatchNeeded) {
	StmtInfoList stmtInfoList;
	StmtRefList stmtList = assign_store.getStmtsWithPattern(varName, e, isRHSExactMatchNeeded);
	for (auto stmtNo : stmtList) {
		stmtInfoList.push_back(statement_store.get(stmtNo));
	}
	return stmtInfoList;
}

StmtInfoList PKB::getStmtsWithPatternLHS(VarRef varName) {
	StmtInfoList stmtInfoList;
	StmtRefList stmtList = assign_store.getStmtsWithPatternLHS(varName);
	for (auto stmtNo : stmtList) {
		stmtInfoList.push_back(statement_store.get(stmtNo));
	}
	return stmtInfoList;
}

vector<pair<shared_ptr<StmtInfo>, VarRef>> PKB::getStmtsWithPatternRHS(Common::ArithmeticProcessor::ArithmeticExpression e,
                                                                       bool isRHSExactMatchNeeded) {
	vector<pair<shared_ptr<StmtInfo>, VarRef>> stmtInfoVarList;
	vector<pair<StmtRef, VarRef>> stmtVarList = assign_store.getStmtsWithPatternRHS(e, isRHSExactMatchNeeded);
	for (auto& stmtVarPair : stmtVarList) {
		shared_ptr<StmtInfo> stmtInfo = statement_store.get(stmtVarPair.first);
		pair<shared_ptr<StmtInfo>, VarRef> stmtInfoVarPair = make_pair(stmtInfo, stmtVarPair.second);
		stmtInfoVarList.push_back(stmtInfoVarPair);
	}
	return stmtInfoVarList;
}

void PKB::populateComplexRelations() {
	ParentPKB::optimize(parent_store);
	FollowsPKB::optimize(follows_store);
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
