#include "PKB/AssignStore.h"

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/ExpressionProcessor/ConstantNode.h"
#include "Common/ExpressionProcessor/ArithmeticNode.h"
#include "catch.hpp"
#include "catch_tools.h"

#define ASSIGN(index) make_shared<StmtInfo>(StmtInfo{index, StmtType::Assign})  

using namespace std;
using namespace Common::ExpressionProcessor;

Expression getBasicOpTree() {
    shared_ptr<ExpressionNode> root =
            make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_shared<ConstantNode>("1"), make_shared<ConstantNode>("2"));
    unordered_set<VarRef> variables = unordered_set<VarRef>();
    unordered_set<ConstVal> constants = unordered_set<ConstVal>({1, 2});
	Expression node = Expression(root, variables, constants);
    return node;
}

Expression getPartialOpTree() {
    shared_ptr<ExpressionNode> root = make_shared<ConstantNode>("2");
    unordered_set<VarRef> variables = unordered_set<VarRef>();
    unordered_set<ConstVal> constants = unordered_set<ConstVal>({2});
	Expression node = Expression(root, variables, constants);
    return node;
}

TEST_CASE("PKB::AssignStore::setAssign Success Test") {
    AssignStore store = AssignStore();
    store.setAssign(ASSIGN(1), "x", getBasicOpTree());
    unordered_map<StmtRef, AssignRelation> map = store.getAssignMap();
    auto obj = map.find(1);
    REQUIRE_EQUALS(map.size(), 1);
    REQUIRE_EQUALS(obj->first, 1);
    REQUIRE_EQUALS(obj->second.variable, "x");
    REQUIRE(obj->second.expression.equals(getBasicOpTree()));
}

TEST_CASE("PKB::AssignStore::setAssign Duplicate StmtNo Test") {
    AssignStore store = AssignStore();
    store.setAssign(ASSIGN(1), "x", getBasicOpTree());
    REQUIRE_THROWS_AS(store.setAssign(ASSIGN(1), "x", getBasicOpTree()), invalid_argument);
}

TEST_CASE("PKB::AssignStone::patternExists Exact Match Test") {
    AssignStore store = AssignStore();
    store.setAssign(ASSIGN(1), "x", getBasicOpTree());
    REQUIRE(store.patternExists("x", getBasicOpTree(), true));
}

TEST_CASE("PKB::AssignStone::patternExists Exact Match Different LHS Test") {
    AssignStore store = AssignStore();
    store.setAssign(ASSIGN(1), "x", getBasicOpTree());
    REQUIRE_FALSE(store.patternExists("y", getBasicOpTree(), true));
}

TEST_CASE("PKB::AssignStone::patternExists Exact Match RHS Partial Match Test") {
    AssignStore store = AssignStore();
    store.setAssign(ASSIGN(1), "x", getBasicOpTree());
    REQUIRE_FALSE(store.patternExists("x", getPartialOpTree(), true));
}

TEST_CASE("PKB::AssignStone::patternExists Partial Match Test") {
    AssignStore store = AssignStore();
    store.setAssign(ASSIGN(1), "x", getBasicOpTree());
    REQUIRE(store.patternExists("x", getPartialOpTree(), false));
}

TEST_CASE("PKB::AssignStone::patternExists Partial Match Different LHS Test") {
    AssignStore store = AssignStore();
    store.setAssign(ASSIGN(1), "x", getBasicOpTree());
    REQUIRE_FALSE(store.patternExists("y", getPartialOpTree(), false));
}

TEST_CASE("PKB::AssignStone::patternExists Partial Match RHS No Match Test") {
    AssignStore store = AssignStore();
    store.setAssign(ASSIGN(1), "x", getPartialOpTree());
    REQUIRE_FALSE(store.patternExists("x", getBasicOpTree(), false));
}

TEST_CASE("PKB::AssignStone::getStmtsWithPattern Exact Match Test") {
    AssignStore store = AssignStore();
	shared_ptr<StmtInfo> s1 = ASSIGN(1);
	shared_ptr<StmtInfo> s3 = ASSIGN(3);
	shared_ptr<StmtInfo> s4 = ASSIGN(4);
    store.setAssign(s1, "x", getPartialOpTree());
    store.setAssign(s4, "x", getPartialOpTree());
    store.setAssign(s3, "x", getBasicOpTree());
    unordered_map<StmtRef, AssignRelation> map = store.getAssignMap();
    REQUIRE_EQUALS(map.size(), 3);
    StmtInfoPtrSet lst = store.getStmtsWithPattern("x", getPartialOpTree(), true);
    REQUIRE_EQUALS(lst.size(), 2);
    REQUIRE(find(lst.begin(), lst.end(), s1) != lst.end());
    REQUIRE(find(lst.begin(), lst.end(), s4) != lst.end());
    REQUIRE_FALSE(find(lst.begin(), lst.end(), s3) != lst.end());
}

TEST_CASE("PKB::AssignStone::getStmtsWithPattern Exact Match Variable LHS Test") {
    AssignStore store = AssignStore();
	shared_ptr<StmtInfo> s1 = ASSIGN(1);
	shared_ptr<StmtInfo> s3 = ASSIGN(3);
	shared_ptr<StmtInfo> s4 = ASSIGN(4);
    store.setAssign(s1, "x", getPartialOpTree());
    store.setAssign(s4, "y", getPartialOpTree());
    store.setAssign(s3, "x", getBasicOpTree());
    unordered_map<StmtRef, AssignRelation> map = store.getAssignMap();
    REQUIRE_EQUALS(map.size(), 3);
	StmtInfoPtrSet lst = store.getStmtsWithPattern("x", getPartialOpTree(), true);
    REQUIRE_EQUALS(lst.size(), 1);
    REQUIRE(find(lst.begin(), lst.end(), s1) != lst.end());
    REQUIRE_FALSE(find(lst.begin(), lst.end(), s4) != lst.end());
    REQUIRE_FALSE(find(lst.begin(), lst.end(), s3) != lst.end());
}

TEST_CASE("PKB::AssignStone::getStmtsWithPattern Partial Match Test") {
    AssignStore store = AssignStore();
	shared_ptr<StmtInfo> s1 = ASSIGN(1);
	shared_ptr<StmtInfo> s3 = ASSIGN(3);
	shared_ptr<StmtInfo> s4 = ASSIGN(4);
    store.setAssign(s1, "x", getBasicOpTree());
    store.setAssign(s4, "x", getBasicOpTree());
    store.setAssign(s3, "x", getPartialOpTree());
    unordered_map<StmtRef, AssignRelation> map = store.getAssignMap();
    REQUIRE_EQUALS(map.size(), 3);
	StmtInfoPtrSet lst = store.getStmtsWithPattern("x", getPartialOpTree(), false);
    REQUIRE_EQUALS(lst.size(), 3);
    REQUIRE(find(lst.begin(), lst.end(), s1) != lst.end());
    REQUIRE(find(lst.begin(), lst.end(), s4) != lst.end());
    REQUIRE(find(lst.begin(), lst.end(), s3) != lst.end());
}

TEST_CASE("PKB::AssignStone::getStmtsWithPattern Partial Match Variable LHS Test") {
    AssignStore store = AssignStore();
	shared_ptr<StmtInfo> s1 = ASSIGN(1);
	shared_ptr<StmtInfo> s3 = ASSIGN(3);
	shared_ptr<StmtInfo> s4 = ASSIGN(4);
    store.setAssign(s1, "x", getBasicOpTree());
    store.setAssign(s4, "y", getBasicOpTree());
    store.setAssign(s3, "x", getPartialOpTree());
    unordered_map<StmtRef, AssignRelation> map = store.getAssignMap();
    REQUIRE_EQUALS(map.size(), 3);
	StmtInfoPtrSet lst = store.getStmtsWithPattern("x", getPartialOpTree(), false);
    REQUIRE_EQUALS(lst.size(), 2);
    REQUIRE(find(lst.begin(), lst.end(), s1) != lst.end());
    REQUIRE_FALSE(find(lst.begin(), lst.end(), s4) != lst.end());
    REQUIRE(find(lst.begin(), lst.end(), s3) != lst.end());
}

TEST_CASE("PKB::AssignStone::getStmtsWithPatternLHS Test") {
    AssignStore store = AssignStore();
	shared_ptr<StmtInfo> s1 = ASSIGN(1);
	shared_ptr<StmtInfo> s3 = ASSIGN(3);
	shared_ptr<StmtInfo> s4 = ASSIGN(4);
    store.setAssign(s1, "x", getBasicOpTree());
    store.setAssign(s4, "y", getBasicOpTree());
    store.setAssign(s3, "x", getPartialOpTree());
    unordered_map<StmtRef, AssignRelation> map = store.getAssignMap();
    REQUIRE_EQUALS(map.size(), 3);
	StmtInfoPtrSet lst = store.getStmtsWithPatternLHS("x");
    REQUIRE_EQUALS(lst.size(), 2);
    REQUIRE(find(lst.begin(), lst.end(), s1) != lst.end());
    REQUIRE_FALSE(find(lst.begin(), lst.end(), s4) != lst.end());
    REQUIRE(find(lst.begin(), lst.end(), s3) != lst.end());
}

TEST_CASE("PKB::AssignStone::getStmtsWithPatternLHS No Match Test") {
    AssignStore store = AssignStore();
	shared_ptr<StmtInfo> s1 = ASSIGN(1);
	shared_ptr<StmtInfo> s3 = ASSIGN(3);
	shared_ptr<StmtInfo> s4 = ASSIGN(4);
    store.setAssign(s1, "x", getBasicOpTree());
    store.setAssign(s4, "y", getBasicOpTree());
    store.setAssign(s3, "x", getPartialOpTree());
    unordered_map<StmtRef, AssignRelation> map = store.getAssignMap();
    REQUIRE_EQUALS(map.size(), 3);
	StmtInfoPtrSet lst = store.getStmtsWithPatternLHS("z");
    REQUIRE_EQUALS(lst.size(), 0);
}

TEST_CASE("PKB::AssignStone::getStmtsWithPatternRHS Exact Match Test") {
    AssignStore store = AssignStore();
	shared_ptr<StmtInfo> s1 = ASSIGN(1);
	shared_ptr<StmtInfo> s3 = ASSIGN(3);
	shared_ptr<StmtInfo> s4 = ASSIGN(4);
    store.setAssign(s1, "x", getBasicOpTree());
    store.setAssign(s4, "y", getBasicOpTree());
    store.setAssign(s3, "z", getPartialOpTree());
    unordered_map<StmtRef, AssignRelation> map = store.getAssignMap();
    REQUIRE_EQUALS(map.size(), 3);
    vector<pair<shared_ptr<StmtInfo>, VarRef>> lst = store.getStmtsWithPatternRHS(getPartialOpTree(), true);
    REQUIRE_EQUALS(lst.size(), 1);
    REQUIRE(find_if(lst.begin(), lst.end(), [](const pair<shared_ptr<StmtInfo>, VarRef>& node)
    {
        return node.first->reference == 3;
    }) != lst.end());
    vector<pair<shared_ptr<StmtInfo>, VarRef>> lst_2 = store.getStmtsWithPatternRHS(getBasicOpTree(), true);
    REQUIRE_EQUALS(lst_2.size(), 2);
    REQUIRE_FALSE(find_if(lst_2.begin(), lst_2.end(), [](const pair<shared_ptr<StmtInfo>, VarRef>& node)
    {
        return node.first->reference == 3;
    }) != lst_2.end());
}

TEST_CASE("PKB::AssignStone::getStmtsWithPatternRHS Partial Match Test") {
    AssignStore store = AssignStore();
	shared_ptr<StmtInfo> s1 = ASSIGN(1);
	shared_ptr<StmtInfo> s3 = ASSIGN(3);
	shared_ptr<StmtInfo> s4 = ASSIGN(4);
    store.setAssign(s1, "x", getBasicOpTree());
    store.setAssign(s4, "y", getBasicOpTree());
    store.setAssign(s3, "z", getPartialOpTree());
    unordered_map<StmtRef, AssignRelation> map = store.getAssignMap();
    REQUIRE_EQUALS(map.size(), 3);
	vector<pair<shared_ptr<StmtInfo>, VarRef>> lst = store.getStmtsWithPatternRHS(getPartialOpTree(), false);
    REQUIRE_EQUALS(lst.size(), 3);
	vector<pair<shared_ptr<StmtInfo>, VarRef>> lst_2 = store.getStmtsWithPatternRHS(getBasicOpTree(), false);
    REQUIRE_EQUALS(lst_2.size(), 2);
    REQUIRE_FALSE(find_if(lst_2.begin(), lst_2.end(), [](const pair<shared_ptr<StmtInfo>, VarRef>& node)
    {
        return node.first->reference == 3;
    }) != lst_2.end());
}
