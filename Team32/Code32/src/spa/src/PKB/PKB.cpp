#include "PKB.h"

using namespace std;

PKB::PKB() {}


StmtInfoPtrSet PKB::getStatements()
{
  return StmtInfoPtrSet();
}

VarRefSet PKB::getVariables()
{
  return VarRefSet();
}

unordered_set<int> PKB::getConstants()
{
  return unordered_set<int>();
}

void PKB::setParent(StmtRef stmtNo1, StmtRef stmtNo2) {
    checkInvalidStmts(stmtNo1, stmtNo2);
    parentStore.setParent(stmtNo1, stmtNo2);
}


shared_ptr<StmtInfo> PKB::getParent(StmtRef)
{
  return shared_ptr<StmtInfo>();
}

StmtInfoPtrSet PKB::getChildren(StmtRef)
{
  return StmtInfoPtrSet();
}

bool PKB::checkParents(StmtRef stmtNo1, StmtRef stmtNo2) {
    try {
        checkInvalidStmts(stmtNo1, stmtNo2);
    } catch (invalid_argument) {
        return false;
    }
    return parentStore.isParentChild(stmtNo1, stmtNo2);
}

StmtInfoPtrSet PKB::getParentStar(StmtRef)
{
  return StmtInfoPtrSet();
}

StmtInfoPtrSet PKB::getChildStar(StmtRef)
{
  return StmtInfoPtrSet();
}

void PKB::setFollows(StmtRef stmtNo1, StmtRef stmtNo2) {
    checkInvalidStmts(stmtNo1, stmtNo2);
    followStore.setFollows(stmtNo1, stmtNo2);
}

bool PKB::checkFollows(StmtRef stmtNo1, StmtRef stmtNo2) {
    try {
        checkInvalidStmts(stmtNo1, stmtNo2);
    } catch (invalid_argument) {
        return false;
    }
    return followStore.checkFollows(stmtNo1, stmtNo2);
}

StmtInfoPtrSet PKB::getFollowerStar(StmtRef)
{
  return StmtInfoPtrSet();
}

StmtInfoPtrSet PKB::getPrecedingStar(StmtRef)
{
  return StmtInfoPtrSet();
}

bool PKB::checkUses(StmtRef, VarRef)
{
  return false;
}

StmtInfoPtrSet PKB::getUsesByVar(VarRef)
{
  return StmtInfoPtrSet();
}

VarRefSet PKB::getUsesByStmt(StmtRef)
{
  return VarRefSet();
}

bool PKB::checkModifies(StmtRef, VarRef)
{
  return false;
}

StmtInfoPtrSet PKB::getModifiesByVar(VarRef)
{
  return StmtInfoPtrSet();
}

VarRefSet PKB::getModifiesByStmt(StmtRef)
{
  return VarRefSet();
}

bool PKB::patternExists(VarRef varName, Common::ExpressionProcessor::Expression e, bool isRHSExactMatchNeeded)
{
  return false;
}

StmtInfoPtrSet PKB::getStmtsWithPattern(VarRef varName, Common::ExpressionProcessor::Expression e, bool isRHSExactMatchNeeded)
{
  return StmtInfoPtrSet();
}

StmtInfoPtrSet PKB::getStmtsWithPatternLHS(VarRef varName)
{
  return StmtInfoPtrSet();
}

vector<pair<shared_ptr<StmtInfo>, VarRef>> PKB::getStmtsWithPatternRHS(Common::ExpressionProcessor::Expression e, bool isRHSExactMatchNeeded)
{
  return vector<pair<shared_ptr<StmtInfo>, VarRef>>();
}

shared_ptr<StmtInfo> PKB::getPreceding(StmtRef stmt)
{
  return shared_ptr<StmtInfo>();
}

shared_ptr<StmtInfo> PKB::getFollower(StmtRef)
{
  return shared_ptr<StmtInfo>();
}



void PKB::setStmtType(StmtRef stmtNo, StmtType type) {
    checkInvalidStmt(stmtNo);

    auto keyItr = typeMap.find(stmtNo);
    if (keyItr == typeMap.end()) {
        typeMap.insert(make_pair(stmtNo, type));
    } else {
        keyItr->second = type;
    }
}

void PKB::setConstant(int)
{
}

void PKB::setConstant(unordered_set<int>)
{
}

void PKB::setUses(StmtRef stmtNo, VarRef var_name) {
    checkInvalidStmt(stmtNo);
    useStore.setUses(stmtNo, var_name);
}

void PKB::setModifies(StmtRef stmtNo, VarRef var_name) {
    checkInvalidStmt(stmtNo);
    modifyStore.setModify(stmtNo, var_name);
}

void PKB::setUses(StmtRef, VarRefSet)
{
}

void PKB::setModifies(StmtRef, VarRefSet)
{
}

void PKB::setAssign(StmtRef, VarRef variableLHS, Common::ExpressionProcessor::Expression opTree)
{
}

void PKB::populateComplexRelations() {
    parentStore.populateParentStar(numStatements);
}

void PKB::clear() {
    parentStore.clear();
    followStore.clear();
    assignStore.clear();
    useStore.clear();
    modifyStore.clear();
    procMap.clear();
    typeMap.clear();
}

void PKB::setNumStatements(int size) {
    if (size < 0) throw invalid_argument("Size must be a positive integer.");
    this->numStatements = size;
}

void PKB::checkInvalidStmts(StmtRef stmtNo1, StmtRef stmtNo2) {
    if (stmtNo1 >= stmtNo2) throw invalid_argument("Second statement must come after the first statement.");
    if (stmtNo1 <= 0 || stmtNo2 <= 0) throw invalid_argument("Statement number must be a positive integer.");
}

void PKB::checkInvalidStmt(StmtRef stmtNo1) {
    if (stmtNo1 <= 0) throw invalid_argument("Statement number must be a positive integer.");
}
