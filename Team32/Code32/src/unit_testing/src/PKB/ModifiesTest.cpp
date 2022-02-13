#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "catch.hpp"

PKB generateModifyTestPKB() {
	PKB pkb = PKB();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::Call);
	pkb.setStmtType(INT32_MAX, StmtType::Assign);
	return pkb;
}

TEST_CASE("Modifies Methods") {
	StmtRef s1 = 1;
	StmtRef s2 = 2;
	StmtRef s3 = 3;
	StmtRef s_int_max = INT32_MAX;
	StmtRef s_minus_one = -1;
	VarRef x = "x";
	VarRef y = "y";
	VarRef z = "z";
	VarRef xyz = "xyz";
	PKB pkb = generateModifyTestPKB();

	SECTION("Validity of arguments") {
		CHECK_THROWS(pkb.setModifies(s_minus_one, x));
		CHECK_THROWS(pkb.setModifies(s1, ""));
		CHECK_THROWS(pkb.setModifies(s_minus_one, ""));
		CHECK_NOTHROW(pkb.setModifies(s1, x));
	}

	SECTION("Set Modifies") {
		// 1 statement, 1 var.
		CHECK_NOTHROW(pkb.setModifies(s1, x));
		CHECK_NOTHROW(pkb.setModifies(s2, y));
		CHECK_NOTHROW(pkb.setModifies(s3, z));

		// 1 var can be modified in multiple statements.
		pkb = generateModifyTestPKB();
		pkb.setModifies(s1, x);
		CHECK_NOTHROW(pkb.setModifies(s2, x));
		CHECK_NOTHROW(pkb.setModifies(s3, x));
	}

	SECTION("Check Modifies") {
		pkb.setModifies(s1, x);
		pkb.setModifies(s2, y);
		pkb.setModifies(s3, z);
		pkb.setModifies(s_int_max, x);

		CHECK(pkb.checkModifies(s1, x));
		CHECK(pkb.checkModifies(s2, y));
		CHECK(pkb.checkModifies(s3, z));
		CHECK(pkb.checkModifies(s_int_max, x));

		// Negative Cases
		CHECK_FALSE(pkb.checkModifies(s_int_max, z));
		CHECK_FALSE(pkb.checkModifies(s2, x));
		CHECK_FALSE(pkb.checkModifies(s3, y));

		// Invalid arguments
		CHECK_THROWS(pkb.checkModifies(s_minus_one, x));
		CHECK_THROWS(pkb.checkModifies(s_int_max, ""));
	}

	SECTION("Get Modifies by Var") {
		pkb.setModifies(s1, x);
		pkb.setModifies(s2, x);
		pkb.setModifies(s3, y);
		pkb.setModifies(s_int_max, z);

		unordered_set<shared_ptr<StmtInfo>> result_x = pkb.getModifiesByVar(x);
		unordered_set<shared_ptr<StmtInfo>> result_y = pkb.getModifiesByVar(y);
		unordered_set<shared_ptr<StmtInfo>> result_z = pkb.getModifiesByVar(z);

		CHECK(result_x.size() == 2);
		CHECK(result_x.find(pkb.getStmtInfoMap().at(s1)) != result_x.end());
		CHECK(result_x.find(pkb.getStmtInfoMap().at(s2)) != result_x.end());
		CHECK(result_y.size() == 1);
		CHECK(result_y.find(pkb.getStmtInfoMap().at(s3)) != result_y.end());
		CHECK(result_z.size() == 1);
		CHECK(result_z.find(pkb.getStmtInfoMap().at(s_int_max)) != result_z.end());

		CHECK(pkb.getUsesByVar(xyz).empty());
		CHECK_THROWS(pkb.getUsesByVar(""));
	}

	SECTION("Get Modifies By Stmt") {
		pkb.setModifies(s1, x);
		pkb.setModifies(s2, x);
		pkb.setModifies(s3, y);
		pkb.setModifies(s_int_max, z);

		CHECK(pkb.getModifiesByStmt(s1) == unordered_set<VarRef>({x}));
		CHECK(pkb.getModifiesByStmt(s2) == unordered_set<VarRef>({x}));
		CHECK(pkb.getModifiesByStmt(s3) == unordered_set<VarRef>({y}));
		CHECK(pkb.getModifiesByStmt(s_int_max) == unordered_set<VarRef>({z}));

		// Invalid arguments
		CHECK_THROWS(pkb.getModifiesByStmt(s_minus_one));
		CHECK_THROWS(pkb.getModifiesByStmt(0));

		// Statement not stored into store.
		pkb = generateModifyTestPKB();
		CHECK(pkb.getModifiesByStmt(s1).empty());
	}

	SECTION("getModifiesByStmtList") {
		// TODO after Proc Store is tested
	}

	SECTION("Check Procedure Modifies") {
		// TODO after Proc Store is tested
	}

	SECTION("Get Modifies By Procedure") {
		// TODO after Proc Store is tested
	}

	SECTION("Get Procedure Modifies By Variable") {
		// TODO after Proc Store is tested
	}
}