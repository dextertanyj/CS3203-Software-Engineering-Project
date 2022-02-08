#include "AssignStore.h"

#include <cassert>

using namespace std;

AssignStore::AssignStore() {}

void AssignStore::setAssign(StmtRef stmtNo, VarRef variableLHS, string opTree) {
    if (stmtNo <= 0) throw invalid_argument("Statement number must be a positive integer.");

    auto keyItr = assignMap.find(stmtNo);
    AssignRelation assignRelation = { variableLHS, opTree };
    if (keyItr == assignMap.end()) {
        assignMap.insert(make_pair(stmtNo, assignRelation));
    } else {
        throw invalid_argument("Statement 1 already exists in assign map.");
    }
}

StmtRefList AssignStore::getPatternMatch(StmtInfoList stmtNoList, VarRef variableLHS, string opTree,
                                         bool isRHSExactMatchNeeded) {
    StmtRefList stmtRefList;
    for (auto& stmtInfo : stmtNoList) {
        StmtRef stmtNo = stmtInfo.reference;
        if (stmtInfo.type != StmtType::Assign) throw invalid_argument("Statement must be an Assign statement.");
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

StmtRefList AssignStore::getAllPatternMatch(VarRef variableLHS, string opTree, bool isRHSExactMatchNeeded) {
    StmtRefList stmtRefList;
    for (auto& itr : assignMap) {
        AssignRelation assignRelation = itr.second;
        if (compareOpTreeAndVar(assignRelation, variableLHS, opTree, isRHSExactMatchNeeded)) {
            stmtRefList.push_back(itr.first);
        }
    }
    return stmtRefList;
}

// To be changed when Operation Tree is created
bool AssignStore::compareOpTreeAndVar(AssignRelation assignRelation, VarRef variableLHS, string opTree, bool isRHSExactMatchNeeded) {
    if (variableLHS != "" && assignRelation.variableLHS != variableLHS) {
        return false;
    }
    if (isRHSExactMatchNeeded) {
        // call compare method of opTree object for exact comparison - to be changed when OperationTree merged in
        return assignRelation.opTree.compare(opTree);
    } else {
        // call compare method of opTree object for partial comparison - to be changed when OperationTree merged in
        return assignRelation.opTree.compare(opTree);
    }
}

void AssignStore::clear() {
    assignMap.clear();
}


