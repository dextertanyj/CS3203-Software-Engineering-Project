#include "PKB.h"

using namespace std;

PKB::PKB() {}

StmtInfoList PKB::getStatements() {
	StmtInfoList stmtList;
	for (auto key_value : stmtInfoMap) {
		stmtList.push_back(key_value.second);
	}
	return stmtList;
}

void PKB::setParent(StmtRef stmtNo1, StmtRef stmtNo2) {
	shared_ptr<StmtInfo> stmtInfo1 = stmtInfoMap.at(stmtNo1);
	shared_ptr<StmtInfo> stmtInfo2 = stmtInfoMap.at(stmtNo2);
	parentStore.setParent(stmtInfo1, stmtInfo2);
}

shared_ptr<StmtInfo> PKB::getParent(StmtRef stmt) { return parentStore.getParent(stmt); }

unordered_set<shared_ptr<StmtInfo>> PKB::getChildren(StmtRef stmt) {
	shared_ptr<StmtInfo> stmtInfo = stmtInfoMap.at(stmt);
	return parentStore.getChildren(stmt);
}

bool PKB::checkParents(StmtRef stmtNo1, StmtRef stmtNo2) { return parentStore.isParentChild(stmtNo1, stmtNo2); }

unordered_set<shared_ptr<StmtInfo>> PKB::getParentStar(StmtRef stmt) { return parentStore.getParentStar(stmt); }

unordered_set<shared_ptr<StmtInfo>> PKB::getChildStar(StmtRef stmt) { return parentStore.getChildStar(stmt); }

void PKB::setFollows(StmtRef stmtNo1, StmtRef stmtNo2) {
	shared_ptr<StmtInfo> stmtInfo1 = stmtInfoMap.at(stmtNo1);
	shared_ptr<StmtInfo> stmtInfo2 = stmtInfoMap.at(stmtNo2);
	followStore.setFollows(stmtInfo1, stmtInfo2);
}

bool PKB::checkFollows(StmtRef stmtNo1, StmtRef stmtNo2) { return followStore.checkFollows(stmtNo1, stmtNo2); }

shared_ptr<StmtInfo> PKB::getFollower(StmtRef stmt) { return followStore.getFollower(stmt); }

unordered_set<shared_ptr<StmtInfo>> PKB::getFollowerStar(StmtRef stmt) { return followStore.getFollowerStar(stmt); }

unordered_set<shared_ptr<StmtInfo>> PKB::getPrecedingStar(StmtRef stmt) { return followStore.getPrecedingStar(stmt); }

shared_ptr<StmtInfo> PKB::getPreceding(StmtRef stmt) { return followStore.getPreceding(stmt); }

void PKB::setAssign(StmtRef stmtNo, VarRef variableLHS, Common::ArithmeticProcessor::ArithmeticExpression opTree) {
	return assignStore.setAssign(stmtNo, variableLHS, opTree);
}

// This method will store information about a statement into PKB's statement map.
// Source Processor is guaranteed to call this method before storing relationships and variables.
void PKB::setStmtType(StmtRef stmtNo, StmtType type) {
	if (stmtNo <= 0) throw invalid_argument("Statement number must be a positive integer.");

	auto keyItr = stmtInfoMap.find(stmtNo);
	if (keyItr == stmtInfoMap.end()) {
		StmtInfo stmtInfo = {stmtNo, type};
		auto stmtInfoPtr = make_shared<StmtInfo>(stmtInfo);
		stmtInfoMap.insert(make_pair(stmtNo, stmtInfoPtr));
	} else {
		throw invalid_argument("Statement type has already been assigned for this statement.");
	}
}

void PKB::setUses(StmtRef stmt, VarRef varName) {
	shared_ptr<StmtInfo> stmtInfo = stmtInfoMap.at(stmt);
	useStore.setUses(stmtInfo, varName);
}

void PKB::setModifies(StmtRef stmt, VarRef varName) {
	shared_ptr<StmtInfo> stmtInfo = stmtInfoMap.at(stmt);
	modifyStore.setModify(stmtInfo, varName);
}

bool PKB::checkUses(StmtRef stmt, VarRef varName) {
	shared_ptr<StmtInfo> stmtInfo = stmtInfoMap.at(stmt);
	return useStore.checkUses(stmtInfo, varName);
}

unordered_set<shared_ptr<StmtInfo>> PKB::getUsesByVar(VarRef var_name) { return useStore.getUsesByVar(var_name); }

unordered_set<VarRef> PKB::getUsesByStmt(StmtRef stmt) {
	shared_ptr<StmtInfo> stmtInfo = stmtInfoMap.at(stmt);
	return useStore.getUsesByStmt(stmtInfo);
}

bool PKB::checkModifies(StmtRef stmt, VarRef varName) { return modifyStore.checkModifies(stmt, varName); }

unordered_set<shared_ptr<StmtInfo>> PKB::getModifiesByVar(VarRef varName) { return modifyStore.getModifiesByVar(varName); }

unordered_set<VarRef> PKB::getModifiesByStmt(StmtRef stmt) { return modifyStore.getModifiesByStmt(stmt); }

bool PKB::isPattern(VarRef varName, Common::ArithmeticProcessor::ArithmeticExpression e, bool isRHSExactMatchNeeded) {
	return assignStore.patternExists(varName, e, isRHSExactMatchNeeded);
}

StmtInfoList PKB::getPatternMatch(StmtInfoList stmtInfoList, VarRef varName, Common::ArithmeticProcessor::ArithmeticExpression e,
                                  bool isRHSExactMatchNeeded) {
	StmtInfoList stmtInfos;
	StmtRefList stmtList = assignStore.getPatternMatch(stmtInfoList, varName, e, isRHSExactMatchNeeded);
	for (auto stmtNo : stmtList) {
		stmtInfos.push_back(stmtInfoMap.at(stmtNo));
	}
	return stmtInfos;
}

StmtInfoList PKB::getAllPatternMatch(VarRef varName, Common::ArithmeticProcessor::ArithmeticExpression e, bool isRHSExactMatchNeeded) {
	StmtInfoList stmtInfoList;
	StmtRefList stmtList = assignStore.getStmtsWithPattern(varName, e, isRHSExactMatchNeeded);
	for (auto stmtNo : stmtList) {
		stmtInfoList.push_back(stmtInfoMap.at(stmtNo));
	}
	return stmtInfoList;
}

StmtInfoList PKB::getPatternMatchLHS(VarRef varName) {
	StmtInfoList stmtInfoList;
	StmtRefList stmtList = assignStore.getStmtsWithPatternLHS(varName);
	for (auto stmtNo : stmtList) {
		stmtInfoList.push_back(stmtInfoMap.at(stmtNo));
	}
	return stmtInfoList;
}

vector<pair<shared_ptr<StmtInfo>, VarRef>> PKB::getPatternMatchRHS(Common::ArithmeticProcessor::ArithmeticExpression e,
                                                                   bool isRHSExactMatchNeeded) {
	vector<pair<shared_ptr<StmtInfo>, VarRef>> stmtInfoVarList;
	vector<pair<StmtRef, VarRef>> stmtVarList = assignStore.getStmtsWithPatternRHS(e, isRHSExactMatchNeeded);
	for (auto& stmtVarPair : stmtVarList) {
		shared_ptr<StmtInfo> stmtInfo = stmtInfoMap.at(stmtVarPair.first);
		pair<shared_ptr<StmtInfo>, VarRef> stmtInfoVarPair = make_pair(stmtInfo, stmtVarPair.second);
		stmtInfoVarList.push_back(stmtInfoVarPair);
	}
	return stmtInfoVarList;
}

void PKB::populateComplexRelations() {
	parentStore.optimize();
	followStore.optimize();
	modifyStore.optimize(stmtInfoMap, parentStore);
}

void PKB::clear() {
	parentStore.clear();
	followStore.clear();
	assignStore.clear();
	useStore.clear();
	modifyStore.clear();
	stmtInfoMap.clear();
}
unordered_map<StmtRef, shared_ptr<StmtInfo>> PKB::getStmtInfoMap() { return this->stmtInfoMap; }
