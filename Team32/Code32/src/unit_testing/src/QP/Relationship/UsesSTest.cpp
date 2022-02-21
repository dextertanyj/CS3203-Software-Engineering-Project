#include "QP/Relationship/UsesS.h"
#include "PKB/Storage.h"
#include "Common/TypeDefs.h"

#include "catch.hpp"

TEST_CASE("QP::Relationship::UsesS::execute") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Print);
	pkb.setStmtType(3, StmtType::WhileStmt);
	pkb.setStmtType(4, StmtType::IfStmt);
	pkb.setUses(1, "x");
	pkb.setUses(1, "z");
	pkb.setUses(2, "x");
	pkb.setUses(3, "y");

	unordered_map<string, DesignEntity> map;
	map.insert({ "s", DesignEntity::Stmt });
	map.insert({ "a", DesignEntity::Assign });
	map.insert({ "if", DesignEntity::If });

	QueryStmtRef stmtNo1 = { StmtRefType::StmtNumber, "1" };
	QueryStmtRef stmtNo2 = { StmtRefType::StmtNumber, "2" };
	QueryStmtRef stmtNo3 = { StmtRefType::StmtNumber, "3" };
	QueryStmtRef stmtNo4 = { StmtRefType::StmtNumber, "4" };
	QueryStmtRef stmtSynonym = { StmtRefType::Synonym, "s" };
	QueryStmtRef assignSynonym = { StmtRefType::Synonym, "a" };
	QueryStmtRef ifSynonym = { StmtRefType::Synonym, "if" };
	QueryStmtRef stmtUnderscore = { StmtRefType::Underscore, "_"};
	QueryEntRef x = { EntRefType::VarName, "x"};
	QueryEntRef y = { EntRefType::VarName, "y"};
	QueryEntRef var = { EntRefType::Synonym, "var"};
	QueryEntRef varUnderscore = { EntRefType::Underscore, "x"};

	SECTION("trivial: stmtNumber & varName") {
		UsesS uses1 = UsesS(stmtNo1, x);
		UsesS uses2 = UsesS(stmtNo1, y);

		QueryResult result1 = uses1.execute(pkb, true, map);
		QueryResult result2 = uses2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & underscore") {
		UsesS uses1 = UsesS(stmtNo1, varUnderscore);
		UsesS uses2 = UsesS(stmtNo4, varUnderscore);

		QueryResult result1 = uses1.execute(pkb, true, map);
		QueryResult result2 = uses2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & synonym") {
		UsesS uses1 = UsesS(stmtNo1, var);
		UsesS uses2 = UsesS(stmtNo4, var);

		QueryResult result1 = uses1.execute(pkb, true, map);
		QueryResult result2 = uses2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & varName") {
		UsesS uses = UsesS(stmtUnderscore, var);

		QueryResult result = uses.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: underscore & underscore") {
		UsesS parent = UsesS(stmtUnderscore, varUnderscore);

		QueryResult result = parent.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: underscore & synonym") {
		UsesS uses = UsesS(stmtUnderscore, var);

		QueryResult result = uses.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: synonym & varName") {
		UsesS uses1 = UsesS(assignSynonym, x);
		UsesS uses2 = UsesS(assignSynonym, y);

		QueryResult result1 = uses1.execute(pkb, true, map);
		QueryResult result2 = uses2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & underscore") {
		UsesS uses1 = UsesS(stmtSynonym, varUnderscore);
		UsesS uses2 = UsesS(ifSynonym, varUnderscore);

		QueryResult result1 = uses1.execute(pkb, true, map);
		QueryResult result2 = uses2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & synonym") {
		UsesS uses1 = UsesS(stmtSynonym, var);
		UsesS uses2 = UsesS(ifSynonym, var);

		QueryResult result1 = uses1.execute(pkb, true, map);
		QueryResult result2 = uses2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & varName") {
		UsesS uses1 = UsesS(assignSynonym, x);
		UsesS uses2 = UsesS(ifSynonym, y);

		QueryResult result1 = uses1.execute(pkb, false, map);
		QueryResult result2 = uses2.execute(pkb, false, map);

		vector<string> expectedResult = { "1" };
		REQUIRE(result1.getSynonymResult("a") == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & underscore") {
		UsesS uses1 = UsesS(stmtSynonym, varUnderscore);
		UsesS uses2 = UsesS(ifSynonym, varUnderscore);

		QueryResult result1 = uses1.execute(pkb, false, map);
		QueryResult result2 = uses2.execute(pkb, false, map);

		vector<string> expectedResult = { "1", "2", "3" };
		vector<string> actualResult = result1.getSynonymResult("s");
		sort(actualResult.begin(), actualResult.end());
		REQUIRE(actualResult == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & synonym") {
		UsesS uses1 = UsesS(stmtSynonym, var);
		UsesS uses2 = UsesS(ifSynonym, var);

		QueryResult result1 = uses1.execute(pkb, false, map);
		QueryResult result2 = uses2.execute(pkb, false, map);

		vector<string> expectedStmtResult = { "1", "1", "2", "3" };
		vector<string> expectedVarResult = { "x", "x", "y", "z" };
		vector<string> actualVarResult = result1.getSynonymResult("var");
		vector<string> actualStmtResult = result1.getSynonymResult("s");
		sort(actualVarResult.begin(), actualVarResult.end());
		sort(actualStmtResult.begin(), actualStmtResult.end());
		REQUIRE(actualVarResult == expectedVarResult);
		REQUIRE(actualStmtResult == expectedStmtResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: underscore & synonym") {
		UsesS uses = UsesS(stmtUnderscore, var);

		QueryResult result = uses.execute(pkb, false, map);

		vector<string> expectedResult = { "x", "y", "z" };
		vector<string> actualVarResult = result.getSynonymResult("var");
		sort(actualVarResult.begin(), actualVarResult.end());
		REQUIRE(actualVarResult == expectedResult);
	}

	SECTION("non-trivial: stmtNumber & synonym") {
		UsesS uses1 = UsesS(stmtNo1, var);
		UsesS uses2 = UsesS(stmtNo4, var);

		QueryResult result1 = uses1.execute(pkb, false, map);
		QueryResult result2 = uses2.execute(pkb, false, map);

		vector<string> expectedResult = { "x", "z" };
		vector<string> actualResult = result1.getSynonymResult("var");
		sort(actualResult.begin(), actualResult.end());
		REQUIRE(actualResult == expectedResult);
		REQUIRE(!result2.getResult());
	}
};

