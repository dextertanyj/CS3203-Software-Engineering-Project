#include "AssignStore.h"

#include <cassert>
#include <utility>

using namespace std;

AssignStore::AssignStore() = default;

void AssignStore::setAssign(StmtRef stmtNo, VarRef variableLHS, Common::ExpressionProcessor::Expression opTree) {
    if (stmtNo <= 0) {
        throw invalid_argument("Statement number must be a positive integer.");
    }

    auto key_itr = assignMap.find(stmtNo);
    AssignRelation assign_relation = { move(variableLHS), move(opTree) };
    if (key_itr == assignMap.end()) {
        assignMap.insert(make_pair(stmtNo, assign_relation));
    } else {
        throw invalid_argument("Statement number already exists in assign map.");
    }
}

bool AssignStore::patternExists(const VarRef& varName, const Common::ExpressionProcessor::Expression& opTree, bool isRHSExactMatchNeeded) {
    for (const auto& assign_stmt : assignMap) {
        if (compareOpTreeAndVar(assign_stmt.second, varName, opTree, isRHSExactMatchNeeded)) {
            return true;
        }
    }
    return false;
}

StmtRefList AssignStore::getStmtsWithPattern(const VarRef& variableLHS, const Common::ExpressionProcessor::Expression& opTree, bool isRHSExactMatchNeeded) {
    StmtRefList stmt_ref_list;
    for (auto& itr : assignMap) {
        AssignRelation assign_relation = itr.second;
        if (compareOpTreeAndVar(assign_relation, variableLHS, opTree, isRHSExactMatchNeeded)) {
            stmt_ref_list.push_back(itr.first);
        }
    }
    return stmt_ref_list;
}

StmtRefList AssignStore::getStmtsWithPatternLHS(const VarRef& varName) {
    StmtRefList stmt_ref_list;
    for (auto& itr : assignMap) {
        AssignRelation assign_relation = itr.second;
        if (assign_relation.variableLHS == varName) {
            stmt_ref_list.push_back(itr.first);
        }
    }
    return stmt_ref_list;
}

vector<pair<StmtRef, VarRef>> AssignStore::getStmtsWithPatternRHS(const Common::ExpressionProcessor::Expression& opTree, bool isRHSExactMatchNeeded) {
    vector<pair<StmtRef, VarRef>> stmt_var_list;
    for (auto& itr : assignMap) {
        AssignRelation assign_relation = itr.second;
        if (compareOpTreeAndVar(assign_relation, "", opTree, isRHSExactMatchNeeded)) {
            pair<StmtRef, VarRef> stmt_var_pair = make_pair(itr.first, itr.second.variableLHS);
            stmt_var_list.push_back(stmt_var_pair);
        }
    }
    return stmt_var_list;
}

// To be changed when Operation Tree is created
bool AssignStore::compareOpTreeAndVar(AssignRelation assignRelation, const VarRef& variableLHS, const Common::ExpressionProcessor::Expression& opTree, bool isRHSExactMatchNeeded) {
    if (!variableLHS.empty() && assignRelation.variableLHS != variableLHS) {
        return false;
    }
    if (isRHSExactMatchNeeded) {
        return assignRelation.opTree.equals(opTree);
    }
    return assignRelation.opTree.contains(opTree);
}

void AssignStore::clear() {
    assignMap.clear();
}

unordered_map<StmtRef, AssignRelation> AssignStore::getAssignMap() {
    return assignMap;
}


