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

shared_ptr<StmtInfo> PKB::getParent(StmtRef stmt) {
	shared_ptr<StmtInfo> stmtInfo = stmtInfoMap.at(stmt);
	return parentStore.getParent(stmtInfo);
}

unordered_set<shared_ptr<StmtInfo>> PKB::getChildren(StmtRef stmt) {
	shared_ptr<StmtInfo> stmtInfo = stmtInfoMap.at(stmt);
	return parentStore.getChildren(stmtInfo);
}

bool PKB::checkParents(StmtRef stmtNo1, StmtRef stmtNo2) {
	shared_ptr<StmtInfo> stmtInfo1 = stmtInfoMap.at(stmtNo1);
	shared_ptr<StmtInfo> stmtInfo2 = stmtInfoMap.at(stmtNo2);
	return parentStore.isParentChild(stmtInfo1, stmtInfo2);
}

unordered_set<shared_ptr<StmtInfo>> PKB::getParentStar(StmtRef stmt) {
    shared_ptr<StmtInfo> stmtInfo = stmtInfoMap.at(stmt);
    return parentStore.getParentStar(stmtInfo);
}

unordered_set<shared_ptr<StmtInfo>> PKB::getChildStar(StmtRef stmt) {
    shared_ptr<StmtInfo> stmtInfo = stmtInfoMap.at(stmt);
    return parentStore.getChildStar(stmtInfo);
}

void PKB::setFollows(StmtRef stmtNo1, StmtRef stmtNo2) {
	shared_ptr<StmtInfo> stmtInfo1 = stmtInfoMap.at(stmtNo1);
	shared_ptr<StmtInfo> stmtInfo2 = stmtInfoMap.at(stmtNo2);
	followStore.setFollows(stmtInfo1, stmtInfo2);
}

bool PKB::checkFollows(StmtRef stmtNo1, StmtRef stmtNo2) {
	shared_ptr<StmtInfo> stmtInfo1 = stmtInfoMap.at(stmtNo1);
	shared_ptr<StmtInfo> stmtInfo2 = stmtInfoMap.at(stmtNo2);
	return followStore.checkFollows(stmtInfo1, stmtInfo2);
}

shared_ptr<StmtInfo> PKB::getFollower(StmtRef stmt) {
	shared_ptr<StmtInfo> stmtInfo = stmtInfoMap.at(stmt);
	return followStore.getFollower(stmtInfo);
}

unordered_set<shared_ptr<StmtInfo>> PKB::getFollowerStar(StmtRef stmt) {
    shared_ptr<StmtInfo> stmtInfo = stmtInfoMap.at(stmt);
    return followStore.getFollowerStar(stmtInfo);
}

unordered_set<shared_ptr<StmtInfo>> PKB::getPrecedingStar(StmtRef stmt) {
    shared_ptr<StmtInfo> stmtInfo = stmtInfoMap.at(stmt);
    return followStore.getPrecedingStar(stmtInfo);
}

shared_ptr<StmtInfo> PKB::getPreceding(StmtRef stmt) {
	shared_ptr<StmtInfo> stmtInfo = stmtInfoMap.at(stmt);
	return followStore.getPreceding(stmtInfo);
}

void PKB::setAssign(StmtRef stmtNo, VarRef variableLHS, Common::ArithmeticProcessor::ArithmeticExpression opTree) {
	return assignStore.setAssign(stmtNo, variableLHS, opTree);
}

void PKB::setProc(ProcRef procName, StmtRefList idxList) {
	StmtInfoList stmtList;
	for (auto stmtRef : idxList) {
		stmtList.push_back(stmtInfoMap.at(stmtRef));
	}
	procStore.setProc(procName, stmtList);
}

void PKB::setCall(StmtRef stmt, ProcRef procName) {
	shared_ptr<StmtInfo> stmtInfo = stmtInfoMap.at(stmt);
	procStore.setCall(stmtInfo, procName);
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

bool PKB::checkProcUses(ProcRef procName, VarRef varName) {
	vector<shared_ptr<StmtInfo>> procStmts = procStore.getStmtsByProc(procName);
	return useStore.checkUsesList(procStmts, varName);
}

unordered_set<shared_ptr<StmtInfo>> PKB::getUsesByVar(VarRef varName) { return useStore.getUsesByVar(varName); }

unordered_set<ProcRef> PKB::getProcUsesByVar(VarRef varName) {
	unordered_set<shared_ptr<StmtInfo>> stmtList = useStore.getUsesByVar(varName);
	return procStore.getProcListByStmtList(stmtList);
}

unordered_set<VarRef> PKB::getUsesByStmt(StmtRef stmt) {
	shared_ptr<StmtInfo> stmtInfo = stmtInfoMap.at(stmt);
	return useStore.getUsesByStmt(stmtInfo);
}

unordered_set<VarRef> PKB::getUsesByProc(ProcRef procName) {
	vector<shared_ptr<StmtInfo>> procStmts = procStore.getStmtsByProc(procName);
	return useStore.getUsesByStmtList(procStmts);
}

unordered_set<VarRef> PKB::getUsesByIf(StmtRef stmt) {
    shared_ptr<StmtInfo> ifInfo = stmtInfoMap.at(stmt);
    if (ifInfo->type != StmtType::IfStmt) {
        throw invalid_argument("Statement is not an if statement!");
    }
    unordered_set<shared_ptr<StmtInfo>> stmtList = ifStore.getStmtsFromIf(ifInfo);
    // Should change parameter type of getUsesByStmtList to unordered_set instead of vector
    unordered_set<VarRef> varSet = useStore.getUsesByStmtList(vector<shared_ptr<StmtInfo>>(stmtList.begin(), stmtList.end()));
    return varSet;
}

bool PKB::checkModifies(StmtRef stmt, VarRef varName) {
	shared_ptr<StmtInfo> stmtInfo = stmtInfoMap.at(stmt);
	return modifyStore.checkModifies(stmtInfo, varName);
}

unordered_set<VarRef> PKB::getModifiesByProc(ProcRef procName) {
	vector<shared_ptr<StmtInfo>> procStmts = procStore.getStmtsByProc(procName);
	return modifyStore.getModifiesByStmtList(procStmts);
}

bool PKB::checkProcModifies(ProcRef procName, VarRef varName) {
	vector<shared_ptr<StmtInfo>> procStmts = procStore.getStmtsByProc(procName);
	return modifyStore.checkModifiesList(procStmts, varName);
}

unordered_set<shared_ptr<StmtInfo>> PKB::getModifiesByVar(VarRef varName) { return modifyStore.getModifiesByVar(varName); }

unordered_set<VarRef> PKB::getModifiesByStmt(StmtRef stmt) {
	shared_ptr<StmtInfo> stmtInfo = stmtInfoMap.at(stmt);
	return modifyStore.getModifiesByStmt(stmtInfo);
}

unordered_set<ProcRef> PKB::getProcModifiesByVar(VarRef varName) {
	unordered_set<shared_ptr<StmtInfo>> stmtList = modifyStore.getModifiesByVar(varName);
	return procStore.getProcListByStmtList(stmtList);
}

unordered_set<VarRef> PKB::getModifiesByIf(StmtRef stmt) {
    shared_ptr<StmtInfo> modifyInfo = stmtInfoMap.at(stmt);
    if (modifyInfo->type != StmtType::IfStmt) {
        throw invalid_argument("Statement is not an if statement!");
    }
    unordered_set<shared_ptr<StmtInfo>> stmtList = ifStore.getStmtsFromIf(modifyInfo);
    // Should change parameter type of getModifiesByStmtList to unordered_set instead of vector
    unordered_set<VarRef> varSet = modifyStore.getModifiesByStmtList(vector<shared_ptr<StmtInfo>>(stmtList.begin(), stmtList.end()));
    return varSet;
}

ProcRef PKB::getProcFromCall(StmtRef stmt) {
	shared_ptr<StmtInfo> stmtInfo = stmtInfoMap.at(stmt);
	return procStore.getProcByCall(stmtInfo);
}

bool PKB::isPattern(VarRef varName, Common::ArithmeticProcessor::ArithmeticExpression e, bool isRHSExactMatchNeeded) {
	return assignStore.isPattern(varName, e, isRHSExactMatchNeeded);
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
	StmtRefList stmtList = assignStore.getAllPatternMatch(varName, e, isRHSExactMatchNeeded);
	for (auto stmtNo : stmtList) {
		stmtInfoList.push_back(stmtInfoMap.at(stmtNo));
	}
	return stmtInfoList;
}

StmtInfoList PKB::getPatternMatchLHS(VarRef varName) {
	StmtInfoList stmtInfoList;
	StmtRefList stmtList = assignStore.getPatternMatchLHS(varName);
	for (auto stmtNo : stmtList) {
		stmtInfoList.push_back(stmtInfoMap.at(stmtNo));
	}
	return stmtInfoList;
}

vector<pair<shared_ptr<StmtInfo>, VarRef>> PKB::getPatternMatchRHS(Common::ArithmeticProcessor::ArithmeticExpression e,
                                                                   bool isRHSExactMatchNeeded) {
	vector<pair<shared_ptr<StmtInfo>, VarRef>> stmtInfoVarList;
	vector<pair<StmtRef, VarRef>> stmtVarList = assignStore.getPatternMatchRHS(e, isRHSExactMatchNeeded);
	for (auto& stmtVarPair : stmtVarList) {
		shared_ptr<StmtInfo> stmtInfo = stmtInfoMap.at(stmtVarPair.first);
		pair<shared_ptr<StmtInfo>, VarRef> stmtInfoVarPair = make_pair(stmtInfo, stmtVarPair.second);
		stmtInfoVarList.push_back(stmtInfoVarPair);
	}
	return stmtInfoVarList;
}

void PKB::populateComplexRelations() {
    setNumStatements();
    parentStore.populateParent(numStatements);
    followStore.populateFollow(numStatements);
    populateIfRelations();
}

void PKB::populateIfRelations() {
    for (auto& stmt : stmtInfoMap) {
        if (stmt.second->type == StmtType::IfStmt) {
            unordered_set<shared_ptr<StmtInfo>> childStmts = parentStore.getChildStar(stmt.second);
            ifStore.populateIfStore(stmt.second, childStmts);
        }
    }
}

void PKB::clear() {
	parentStore.clear();
	followStore.clear();
	assignStore.clear();
	useStore.clear();
	modifyStore.clear();
	procStore.clear();
    ifStore.clear();
	stmtInfoMap.clear();
}

void PKB::setNumStatements() {
	this->numStatements = stmtInfoMap.size();
}

int PKB::getNumStatements() { return this->numStatements; }

unordered_map<StmtRef, shared_ptr<StmtInfo>> PKB::getStmtInfoMap() { return this->stmtInfoMap; }
