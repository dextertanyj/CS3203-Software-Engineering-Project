#include "PKB/AssignStore.h"

#include "Common/ExpressionProcessor/ArithmeticNode.h"
#include "Common/ExpressionProcessor/Expression.h"
#include "Common/ExpressionProcessor/TerminalNode.tpp"
#include "TestUtilities.h"
#include "catch.hpp"
#include "catch_tools.h"

using namespace std;
using namespace Common::ExpressionProcessor;

Expression getBasicOpTree() {
	shared_ptr<ExpressionNode> root = make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_shared<TerminalNode<ConstVal>>(1),
	                                                              make_shared<TerminalNode<ConstVal>>(2));
	unordered_set<VarRef> variables = unordered_set<VarRef>();
	unordered_set<ConstVal> constants = unordered_set<ConstVal>({1, 2});
	Expression node = Expression(root, variables, constants);
	return node;
}

Expression getPartialOpTree() {
	shared_ptr<ExpressionNode> root = make_shared<TerminalNode<ConstVal>>(2);
	unordered_set<VarRef> variables = unordered_set<VarRef>();
	unordered_set<ConstVal> constants = unordered_set<ConstVal>({2});
	Expression node = Expression(root, variables, constants);
	return node;
}

TEST_CASE("PKB::AssignStore::setAssign Success Test") {
	PKB::AssignStore store = PKB::AssignStore();
	store.setAssign(TestUtilities::createStmtInfo(1, StmtType::Assign), "x", getBasicOpTree());
	unordered_map<VarRef, unordered_set<PKB::AssignRelation>> map = store.getAssignMap();
	auto obj = map.find("x");
	REQUIRE_EQUALS(map.size(), 1);
	REQUIRE_EQUALS(obj->first, "x");
	REQUIRE_EQUALS(obj->second.size(), 1);
	REQUIRE(obj->second.begin()->expression == getBasicOpTree());
}

TEST_CASE("PKB::AssignStore::patternExists Exact Match Test") {
	PKB::AssignStore store = PKB::AssignStore();
	store.setAssign(TestUtilities::createStmtInfo(1, StmtType::Assign), "x", getBasicOpTree());
	REQUIRE(store.patternExists("x", getBasicOpTree(), true));
	// Different LHS Variable should return false.
	REQUIRE_FALSE(store.patternExists("y", getBasicOpTree(), true));
	// RHS is a partial subtree, requesting an exact match should return false.
	REQUIRE_FALSE(store.patternExists("x", getPartialOpTree(), true));
}

TEST_CASE("PKB::AssignStore::patternExists Partial Match Test") {
	PKB::AssignStore store = PKB::AssignStore();
	store.setAssign(TestUtilities::createStmtInfo(1, StmtType::Assign), "x", getBasicOpTree());
	REQUIRE(store.patternExists("x", getPartialOpTree(), false));
	// Different LHS Variable should return false.
	REQUIRE_FALSE(store.patternExists("y", getPartialOpTree(), false));
}

TEST_CASE("PKB::AssignStore::patternExists Partial Match RHS No Match Test") {
	PKB::AssignStore store = PKB::AssignStore();
	store.setAssign(TestUtilities::createStmtInfo(1, StmtType::Assign), "x", getPartialOpTree());
	REQUIRE_FALSE(store.patternExists("x", getBasicOpTree(), false));
}

TEST_CASE("PKB::AssignStore::getStmtsWithPattern Test") {
	PKB::AssignStore store = PKB::AssignStore();
	shared_ptr<StmtInfo> s_1 = TestUtilities::createStmtInfo(1, StmtType::Assign);
	shared_ptr<StmtInfo> s_3 = TestUtilities::createStmtInfo(3, StmtType::Assign);
	shared_ptr<StmtInfo> s_4 = TestUtilities::createStmtInfo(4, StmtType::Assign);
	store.setAssign(s_1, "x", getPartialOpTree());
	store.setAssign(s_3, "x", getBasicOpTree());
	store.setAssign(s_4, "x", getPartialOpTree());
	unordered_map<VarRef, unordered_set<PKB::AssignRelation>> map = store.getAssignMap();
	REQUIRE_EQUALS(map.size(), 1);
	REQUIRE_EQUALS(map.at("x").size(), 3);

	// Requesting exact match should return 2 results.
	StmtInfoPtrSet lst = store.getStmtsWithPattern("x", getPartialOpTree(), true);
	REQUIRE_EQUALS(lst.size(), 2);
	REQUIRE(find(lst.begin(), lst.end(), s_1) != lst.end());
	REQUIRE(find(lst.begin(), lst.end(), s_4) != lst.end());
	REQUIRE_FALSE(find(lst.begin(), lst.end(), s_3) != lst.end());

	// Requesting partial match should return 3 results.
	StmtInfoPtrSet lst_2 = store.getStmtsWithPattern("x", getPartialOpTree(), false);
	REQUIRE_EQUALS(lst_2.size(), 3);
	REQUIRE(find(lst_2.begin(), lst_2.end(), s_1) != lst_2.end());
	REQUIRE(find(lst_2.begin(), lst_2.end(), s_4) != lst_2.end());
	REQUIRE(find(lst_2.begin(), lst_2.end(), s_3) != lst_2.end());
}

TEST_CASE("PKB::AssignStore::getStmtsWithPattern Variable LHS Test") {
	PKB::AssignStore store = PKB::AssignStore();
	shared_ptr<StmtInfo> s_1 = TestUtilities::createStmtInfo(1, StmtType::Assign);
	shared_ptr<StmtInfo> s_3 = TestUtilities::createStmtInfo(3, StmtType::Assign);
	shared_ptr<StmtInfo> s_4 = TestUtilities::createStmtInfo(4, StmtType::Assign);
	store.setAssign(s_1, "x", getPartialOpTree());
	store.setAssign(s_4, "y", getPartialOpTree());
	store.setAssign(s_3, "x", getBasicOpTree());
	unordered_map<VarRef, unordered_set<PKB::AssignRelation>> map = store.getAssignMap();
	REQUIRE_EQUALS(map.size(), 2);
	REQUIRE_EQUALS(map.at("x").size(), 2);
	REQUIRE_EQUALS(map.at("y").size(), 1);

	// Requesting exact match should return only 1 result.
	StmtInfoPtrSet lst = store.getStmtsWithPattern("x", getPartialOpTree(), true);
	REQUIRE_EQUALS(lst.size(), 1);
	REQUIRE(find(lst.begin(), lst.end(), s_1) != lst.end());
	REQUIRE_FALSE(find(lst.begin(), lst.end(), s_4) != lst.end());
	REQUIRE_FALSE(find(lst.begin(), lst.end(), s_3) != lst.end());

	// Requesting partial match should lead to 2 results.
	StmtInfoPtrSet lst_2 = store.getStmtsWithPattern("x", getPartialOpTree(), false);
	REQUIRE_EQUALS(lst_2.size(), 2);
	REQUIRE(find(lst_2.begin(), lst_2.end(), s_1) != lst_2.end());
	REQUIRE_FALSE(find(lst_2.begin(), lst_2.end(), s_4) != lst_2.end());
	REQUIRE(find(lst_2.begin(), lst_2.end(), s_3) != lst_2.end());
}

TEST_CASE("PKB::AssignStore::getStmtsWithPatternLHS Test") {
	PKB::AssignStore store = PKB::AssignStore();
	shared_ptr<StmtInfo> s_1 = TestUtilities::createStmtInfo(1, StmtType::Assign);
	shared_ptr<StmtInfo> s_3 = TestUtilities::createStmtInfo(3, StmtType::Assign);
	shared_ptr<StmtInfo> s_4 = TestUtilities::createStmtInfo(4, StmtType::Assign);
	store.setAssign(s_1, "x", getBasicOpTree());
	store.setAssign(s_4, "y", getBasicOpTree());
	store.setAssign(s_3, "x", getPartialOpTree());
	unordered_map<VarRef, unordered_set<PKB::AssignRelation>> map = store.getAssignMap();
	REQUIRE_EQUALS(map.size(), 2);
	StmtInfoPtrSet lst = store.getStmtsWithPatternLHS("x");
	REQUIRE_EQUALS(lst.size(), 2);
	REQUIRE(find(lst.begin(), lst.end(), s_1) != lst.end());
	REQUIRE_FALSE(find(lst.begin(), lst.end(), s_4) != lst.end());
	REQUIRE(find(lst.begin(), lst.end(), s_3) != lst.end());

	// No match for variable not stored into the store.
	REQUIRE_EQUALS(store.getStmtsWithPatternLHS("z").size(), 0);
}

TEST_CASE("PKB::AssignStore::getStmtsWithPatternRHS Exact Match Test") {
	PKB::AssignStore store = PKB::AssignStore();
	shared_ptr<StmtInfo> s_1 = TestUtilities::createStmtInfo(1, StmtType::Assign);
	shared_ptr<StmtInfo> s_3 = TestUtilities::createStmtInfo(3, StmtType::Assign);
	shared_ptr<StmtInfo> s_4 = TestUtilities::createStmtInfo(4, StmtType::Assign);
	store.setAssign(s_1, "x", getBasicOpTree());
	store.setAssign(s_4, "y", getBasicOpTree());
	store.setAssign(s_3, "z", getPartialOpTree());
	unordered_map<VarRef, unordered_set<PKB::AssignRelation>> map = store.getAssignMap();
	REQUIRE_EQUALS(map.size(), 3);

	// Exact match of partialOpTree- only statement 3 should be in the list
	StmtInfoPtrVarRefSet lst = store.getStmtsWithPatternRHS(getPartialOpTree(), true);
	REQUIRE_EQUALS(lst.size(), 1);
	REQUIRE(find_if(lst.begin(), lst.end(), [](const pair<shared_ptr<StmtInfo>, VarRef>& node) {
				return node.first->getIdentifier() == 3 && node.second == "z";
			}) != lst.end());

	// Partial match of partialOpTree- all 3 statements should be in the list
	lst = store.getStmtsWithPatternRHS(getPartialOpTree(), false);
	REQUIRE_EQUALS(lst.size(), 3);

	// Exact match of basicOpTree - statements 1 and 2 should be in the list
	lst = store.getStmtsWithPatternRHS(getBasicOpTree(), true);
	REQUIRE_EQUALS(lst.size(), 2);
	REQUIRE_FALSE(find_if(lst.begin(), lst.end(),
	                      [](const pair<shared_ptr<StmtInfo>, VarRef>& node) { return node.first->getIdentifier() == 3; }) != lst.end());

	// Partial match of basicOpTree - statements 1 and 2 should be in the list
	lst = store.getStmtsWithPatternRHS(getBasicOpTree(), false);
	REQUIRE_EQUALS(lst.size(), 2);
	REQUIRE_FALSE(find_if(lst.begin(), lst.end(),
	                      [](const pair<shared_ptr<StmtInfo>, VarRef>& node) { return node.first->getIdentifier() == 3; }) != lst.end());
}

TEST_CASE("PKB::AssignStore::getStmtsWithPatternRHS Partial Match Test") {
	PKB::AssignStore store = PKB::AssignStore();
	shared_ptr<StmtInfo> s_1 = TestUtilities::createStmtInfo(1, StmtType::Assign);
	shared_ptr<StmtInfo> s_3 = TestUtilities::createStmtInfo(3, StmtType::Assign);
	shared_ptr<StmtInfo> s_4 = TestUtilities::createStmtInfo(4, StmtType::Assign);
	store.setAssign(s_1, "x", getBasicOpTree());
	store.setAssign(s_4, "y", getBasicOpTree());
	store.setAssign(s_3, "z", getPartialOpTree());
	unordered_map<VarRef, unordered_set<PKB::AssignRelation>> map = store.getAssignMap();
	REQUIRE_EQUALS(map.size(), 3);
}
