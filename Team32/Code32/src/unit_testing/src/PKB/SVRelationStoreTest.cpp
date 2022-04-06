#include "PKB/SVRelationStore.h"

#include "PKB/Storage.h"
#include "TestUtilities.h"
#include "catch.hpp"

TEST_CASE("PKB::SVRelationStore") {
	PKB::SVRelationStore uses_store = PKB::SVRelationStore<PKB::UsesSRelation>();
	PKB::SVRelationStore if_control_store = PKB::SVRelationStore<PKB::IfControlRelation>();
	PKB::SVRelationStore while_control_store = PKB::SVRelationStore<PKB::WhileControlRelation>();
	shared_ptr<StmtInfo> s_1 = TestUtilities::createStmtInfo(5, StmtType::Print);
	shared_ptr<StmtInfo> s_2 = TestUtilities::createStmtInfo(6, StmtType::Assign);
	shared_ptr<StmtInfo> s_3 = TestUtilities::createStmtInfo(7, StmtType::If);
	shared_ptr<StmtInfo> s_4 = TestUtilities::createStmtInfo(1, StmtType::Read);
	shared_ptr<StmtInfo> s_5 = TestUtilities::createStmtInfo(8, StmtType::WhileStmt);

	VarRefSet v_1 = {"x", "y", "z"};
	VarRefSet v_2 = {"a", "b"};
	VarRefSet v_3 = {"a", ""};
	VarRefSet v_4 = {};
	VarRefSet v_5 = {"a"};

	SECTION("PKB::SVRelationStore::set by VarRef Test") {
		// Variable length less than 1
		REQUIRE_THROWS_AS(uses_store.set(s_1, ""), invalid_argument);
		// Read statement does not use a variable
		REQUIRE_THROWS_AS(uses_store.set(s_4, "x"), invalid_argument);
		CHECK_NOTHROW(uses_store.set(s_1, "x"));
		CHECK_NOTHROW(uses_store.set(s_2, "y"));
		CHECK_NOTHROW(uses_store.set(s_2, "a"));
		CHECK_NOTHROW(uses_store.set(s_3, "x"));

		// Print statement cannot use more than one variable
		CHECK_THROWS(uses_store.set(s_1, "a"));
		CHECK(uses_store.check(5, "x"));
		CHECK(uses_store.check(6, "y"));
		CHECK(uses_store.check(6, "a"));
		CHECK(uses_store.check(7, "x"));

		// Statements that are not of type If should throw an error
		CHECK_THROWS(if_control_store.set(s_1, "x"));
		CHECK_THROWS(if_control_store.set(s_2, "y"));
		CHECK_NOTHROW(if_control_store.set(s_3, "x"));

		// Statements that are not of type WhileStmt should throw an error
		CHECK_THROWS(while_control_store.set(s_3, "x"));
		CHECK_THROWS(while_control_store.set(s_4, "y"));
		CHECK_NOTHROW(while_control_store.set(s_5, "x"));
	}

	SECTION("PKB::SVRelationStore::set by VarRefSet Test") {
		// Variable set contains empty string
		REQUIRE_THROWS_AS(uses_store.set(s_2, v_3), invalid_argument);
		// Read statement does not use variables
		REQUIRE_THROWS_AS(uses_store.set(s_4, v_2), invalid_argument);
		uses_store.set(s_1, v_5);
		uses_store.set(s_2, v_1);
		uses_store.set(s_3, v_4);
		// Print statement cannot use more than one variable
		CHECK_THROWS(uses_store.set(s_1, v_1));
		CHECK(uses_store.check(5, "a"));
		CHECK(uses_store.check(6, "x"));
		CHECK(uses_store.check(6, "y"));
		CHECK(uses_store.check(6, "z"));

		// Statements that are not of type If should throw an error
		CHECK_THROWS(if_control_store.set(s_1, v_5));
		CHECK_THROWS(if_control_store.set(s_2, v_1));
		CHECK_NOTHROW(if_control_store.set(s_3, v_2));

		// Statements that are not of type WhileStmt should throw an error
		CHECK_THROWS(while_control_store.set(s_3, v_2));
		CHECK_THROWS(while_control_store.set(s_4, v_1));
		CHECK_NOTHROW(while_control_store.set(s_5, v_5));
	}

	SECTION("PKB::SVRelationStore::check Test") {
		uses_store.set(s_1, "x");
		uses_store.set(s_2, v_1);
		uses_store.set(s_3, v_2);

		// StmtRef index less than or equal to 0
		REQUIRE_THROWS_AS(uses_store.check(0, "x"), invalid_argument);
		// Variable as an empty string
		REQUIRE_THROWS_AS(uses_store.check(2, ""), invalid_argument);
		// Check negative cases
		CHECK_FALSE(uses_store.check(2, "x"));
		CHECK_FALSE(uses_store.check(5, "y"));

		CHECK(uses_store.check(5, "x"));
		CHECK(uses_store.check(6, "x"));
		CHECK(uses_store.check(6, "y"));
		CHECK(uses_store.check(6, "z"));
		CHECK(uses_store.check(7, "a"));
		CHECK(uses_store.check(7, "b"));
	}

	SECTION("PKB::SVRelationStore::getByStmt Test") {
		uses_store.set(s_1, "a");
		uses_store.set(s_2, v_1);
		uses_store.set(s_3, v_2);

		// StmtRef index less than or equal to 0
		REQUIRE_THROWS_AS(uses_store.getByStmt(0), invalid_argument);
		CHECK(uses_store.getByStmt(5) == v_5);
		CHECK(uses_store.getByStmt(6) == v_1);
		CHECK(uses_store.getByStmt(7) == v_2);
		CHECK(uses_store.getByStmt(2).empty());
	}

	SECTION("PKB::SVRelationStore::getByVar Test") {
		uses_store.set(s_1, "a");
		uses_store.set(s_2, v_1);
		uses_store.set(s_3, v_2);

		// Variable as an empty string
		REQUIRE_THROWS_AS(uses_store.getByVar(""), invalid_argument);
		unordered_set<shared_ptr<StmtInfo>> expected_set_a = {s_1, s_3};
		unordered_set<shared_ptr<StmtInfo>> expected_set_x = {s_2};
		unordered_set<shared_ptr<StmtInfo>> expected_set_y = {s_2};
		unordered_set<shared_ptr<StmtInfo>> expected_set_b = {s_3};
		CHECK(uses_store.getByVar("a") == expected_set_a);
		CHECK(uses_store.getByVar("x") == expected_set_x);
		CHECK(uses_store.getByVar("y") == expected_set_y);
		CHECK(uses_store.getByVar("b") == expected_set_b);
	}
}
