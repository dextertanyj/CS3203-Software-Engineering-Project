#include "AssignStore.h"

#include <cassert>

using namespace std;

AssignStore::AssignStore() {}

void AssignStore::setAssign(StmtRef stmtNo, VarRef variableLHS, Common::ArithmeticProcessor::ArithmeticExpression opTree) {
    if (stmtNo <= 0) throw invalid_argument("Statement number must be a positive integer.");

    auto keyItr = assignMap.find(stmtNo);
    AssignRelation assignRelation = { variableLHS, opTree };
    if (keyItr == assignMap.end()) {
        assignMap.insert(make_pair(stmtNo, assignRelation));
    } else {
        throw invalid_argument("Statement 1 already exists in assign map.");
    }
}

bool AssignStore::isPattern(VarRef varName, Common::ArithmeticProcessor::ArithmeticExpression opTree, bool isRHSExactMatchNeeded) {
    for (auto assignStmt : assignMap) {
        if (compareOpTreeAndVar(assignStmt.second, varName, opTree, isRHSExactMatchNeeded)) {
            return true;
        }
    }
    return false;
}

StmtRefList AssignStore::getPatternMatch(StmtInfoList stmtNoList, VarRef variableLHS, Common::ArithmeticProcessor::ArithmeticExpression opTree,
                                         bool isRHSExactMatchNeeded) {
    StmtRefList stmtRefList;
    for (auto& stmtInfo : stmtNoList) {
        StmtRef stmtNo = stmtInfo->reference;
        if (stmtInfo->type != StmtType::Assign) throw invalid_argument("Statement must be an Assign statement.");
        if (stmtNo <= 0) throw invalid_argument("Statement number must be a positive integer.");

        auto keyItr = assignMap.find(stmtNo);
        if (keyItr == assignMap.end()) {
            continue;
        }
        AssignRelation assignRelation = keyItr->second;

        if (compareOpTreeAndVar(assignRelation, variableLHS, opTree, isRHSExactMatchNeeded)) {
            stmtRefList.push_back(stmtNo);
        }
    }
    return stmtRefList;
}

StmtRefList AssignStore::getAllPatternMatch(VarRef variableLHS, Common::ArithmeticProcessor::ArithmeticExpression opTree, bool isRHSExactMatchNeeded) {
    StmtRefList stmtRefList;
    for (auto& itr : assignMap) {
        AssignRelation assignRelation = itr.second;
        if (compareOpTreeAndVar(assignRelation, variableLHS, opTree, isRHSExactMatchNeeded)) {
            stmtRefList.push_back(itr.first);
        }
    }
    return stmtRefList;
}

StmtRefList AssignStore::getPatternMatchLHS(VarRef varName) {
    StmtRefList stmtRefList;
    for (auto& itr : assignMap) {
        AssignRelation assignRelation = itr.second;
        if (assignRelation.variableLHS == varName) {
            stmtRefList.push_back(itr.first);
        }
    }
    return stmtRefList;
}

vector<pair<StmtRef, VarRef>> AssignStore::getPatternMatchRHS(Common::ArithmeticProcessor::ArithmeticExpression opTree, bool isRHSExactMatchNeeded) {
    vector<pair<StmtRef, VarRef>> stmtVarList;
    for (auto& itr : assignMap) {
        AssignRelation assignRelation = itr.second;
        if (compareOpTreeAndVar(assignRelation, "", opTree, isRHSExactMatchNeeded)) {
            pair<StmtRef, VarRef> stmtVarPair = make_pair(itr.first, itr.second.variableLHS);
            stmtVarList.push_back(stmtVarPair);
        }
    }
    return stmtVarList;
}

// To be changed when Operation Tree is created
bool AssignStore::compareOpTreeAndVar(AssignRelation assignRelation, VarRef variableLHS, Common::ArithmeticProcessor::ArithmeticExpression opTree, bool isRHSExactMatchNeeded) {
    if (!variableLHS.empty() && assignRelation.variableLHS != variableLHS) {
        return false;
    }
    if (isRHSExactMatchNeeded) {
        return assignRelation.opTree.equals(opTree);
    } else {
        return assignRelation.opTree.contains(opTree);
    }
}

void AssignStore::clear() {
    assignMap.clear();
}


