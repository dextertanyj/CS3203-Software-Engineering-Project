#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "catch.hpp"

PKB generateUsesTestPKB() {
	PKB pkb = PKB();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::IfStmt);
	pkb.setStmtType(3, StmtType::Print);
	pkb.setStmtType(INT32_MAX, StmtType::WhileStmt);
	return pkb;
}

TEST_CASE("Uses Methods") {
	StmtRef s1 = 1;
	StmtRef s2 = 2;
	StmtRef s3 = 3;
	StmtRef s_int_max = INT32_MAX;
	StmtRef s_minus_one = -1;
	VarRef x = "x";
	VarRef y = "y";
	VarRef z = "z";
	VarRef xyz = "xyz";
	PKB pkb = generateUsesTestPKB();

	SECTION("Validity of arguments") {
		CHECK_THROWS(pkb.setUses(s_minus_one, x));
		CHECK_THROWS(pkb.setUses(s1, ""));
		CHECK_THROWS(pkb.setUses(s_minus_one, ""));
		CHECK_NOTHROW(pkb.setUses(s1, x));
	}

	SECTION("Set Uses") {
		// 1 statement, multiple vars.
		CHECK_NOTHROW(pkb.setUses(s1, x));
		CHECK_NOTHROW(pkb.setUses(s1, y));
		CHECK_NOTHROW(pkb.setUses(s1, z));

		// 1 var, multiple statements.
		CHECK_NOTHROW(pkb.setUses(s2, x));
		CHECK_NOTHROW(pkb.setUses(s3, x));
	}

	SECTION("Check Uses") {
		pkb.setUses(s1, x);
		pkb.setUses(s1, y);
		pkb.setUses(s1, z);
		pkb.setUses(s2, x);
		pkb.setUses(s3, x);

		CHECK(pkb.checkUses(s1,x));
		CHECK(pkb.checkUses(s1,y));
		CHECK(pkb.checkUses(s1,z));
		CHECK(pkb.checkUses(s2,x));
		CHECK(pkb.checkUses(s3,x));

		// Negative Cases
		CHECK_FALSE(pkb.checkUses(s_int_max, x));
		CHECK_FALSE(pkb.checkUses(s2, y));
		CHECK_FALSE(pkb.checkUses(s3, z));

		// Invalid arguments
		CHECK_THROWS(pkb.checkUses(s_minus_one, x));
		CHECK_THROWS(pkb.checkUses(s_int_max, ""));
	}

	SECTION("Get Uses by Var") {
		pkb.setUses(s1, x);
		pkb.setUses(s1, y);
		pkb.setUses(s1, z);
		pkb.setUses(s2, x);
		pkb.setUses(s3, x);

		CHECK(pkb.getUsesByVar(x).size() == 3);
		CHECK(pkb.getUsesByVar(y).size() == 1);
		CHECK(pkb.getUsesByVar(z).size() == 1);

		CHECK(pkb.getUsesByVar(xyz).empty());
		CHECK_THROWS(pkb.getUsesByVar(""));
	}

	SECTION("Get Uses By Stmt") {
		pkb.setUses(s1, x);
		pkb.setUses(s1, y);
		pkb.setUses(s1, z);
		pkb.setUses(s2, y);
		pkb.setUses(s2, z);
		pkb.setUses(s3, x);
		pkb.setUses(s_int_max, xyz);

		unordered_set<string> used_by_s1 = {x, y, z};
		unordered_set<string> used_by_s2 = {y, z};
		unordered_set<string> used_by_s3 = {x};
		unordered_set<string> used_by_s_int_max = {xyz};

		CHECK(pkb.getUsesByStmt(s1) == used_by_s1);
		CHECK(pkb.getUsesByStmt(s2) == used_by_s2);
		CHECK(pkb.getUsesByStmt(s3) == used_by_s3);
		CHECK(pkb.getUsesByStmt(s_int_max) == used_by_s_int_max);

		// Invalid arguments
		CHECK_THROWS(pkb.getUsesByStmt(s_minus_one));
		CHECK_THROWS(pkb.getUsesByStmt(0));
	}

	SECTION("Check Procedure Uses") {
		// TODO after Proc Store is tested
	}

	SECTION("Get Uses By Procedure") {
		// TODO after Proc Store is tested
	}
}