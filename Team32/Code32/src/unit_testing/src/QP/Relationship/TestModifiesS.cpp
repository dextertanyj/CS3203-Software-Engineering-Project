#include "QP/Relationship/ModifiesS.h"
#include "PKB/PKB.h"
#include "Common/TypeDefs.h"

#include "catch.hpp"

TEST_CASE("QP::Relationship::ModifiesS::execute") {
	PKB pkb = PKB();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::WhileStmt);
	pkb.setStmtType(3, StmtType::WhileStmt);
	pkb.setStmtType(4, StmtType::IfStmt);
	pkb.setModifies(1, "x");
	pkb.setModifies(2, "x");
	pkb.setModifies(3, "y");

	unordered_map<string, DesignEntity> map;
	map.insert({ "s", DesignEntity::stmt });
	map.insert({ "a", DesignEntity::assign });
	map.insert({ "if", DesignEntity::if_ });

	QueryStmtRef stmtNo1 = { StmtRefType::stmtNumber, "1" };
	QueryStmtRef stmtNo2 = { StmtRefType::stmtNumber, "2" };
	QueryStmtRef stmtNo3 = { StmtRefType::stmtNumber, "3" };
	QueryStmtRef stmtNo4 = { StmtRefType::stmtNumber, "4" };
	QueryStmtRef stmtSynonym = { StmtRefType::synonym, "s" };
	QueryStmtRef assignSynonym = { StmtRefType::synonym, "a" };
	QueryStmtRef ifSynonym = { StmtRefType::synonym, "if" };
	QueryStmtRef stmtUnderscore = { StmtRefType::underscore, "_"};
	QueryEntRef x = { EntRefType::varName, "x"};
	QueryEntRef y = { EntRefType::varName, "y"};
	QueryEntRef var = { EntRefType::synonym, "var"};
	QueryEntRef varUnderscore = { EntRefType::underscore, "x"};

	SECTION("trivial: stmtNumber & varName") {
		ModifiesS modifies1 = ModifiesS(stmtNo1, x);
		ModifiesS modifies2 = ModifiesS(stmtNo1, y);

		QueryResult result1 = modifies1.execute(pkb, true, map);
		QueryResult result2 = modifies2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & underscore") {
		ModifiesS modifies1 = ModifiesS(stmtNo1, varUnderscore);
		ModifiesS modifies2 = ModifiesS(stmtNo4, varUnderscore);

		QueryResult result1 = modifies1.execute(pkb, true, map);
		QueryResult result2 = modifies2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & synonym") {
		ModifiesS modifies1 = ModifiesS(stmtNo1, var);
		ModifiesS modifies2 = ModifiesS(stmtNo4, var);

		QueryResult result1 = modifies1.execute(pkb, true, map);
		QueryResult result2 = modifies2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & varName") {
		ModifiesS modifies = ModifiesS(stmtUnderscore, var);

		QueryResult result = modifies.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: underscore & underscore") {
		ModifiesS parent = ModifiesS(stmtUnderscore, varUnderscore);

		QueryResult result = parent.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: underscore & synonym") {
		ModifiesS modifies = ModifiesS(stmtUnderscore, var);

		QueryResult result = modifies.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: synonym & varName") {
		ModifiesS modifies1 = ModifiesS(assignSynonym, x);
		ModifiesS modifies2 = ModifiesS(assignSynonym, y);

		QueryResult result1 = modifies1.execute(pkb, true, map);
		QueryResult result2 = modifies2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & underscore") {
		ModifiesS modifies1 = ModifiesS(stmtSynonym, varUnderscore);
		ModifiesS modifies2 = ModifiesS(ifSynonym, varUnderscore);

		QueryResult result1 = modifies1.execute(pkb, true, map);
		QueryResult result2 = modifies2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & synonym") {
		ModifiesS modifies1 = ModifiesS(stmtSynonym, var);
		ModifiesS modifies2 = ModifiesS(ifSynonym, var);

		QueryResult result1 = modifies1.execute(pkb, true, map);
		QueryResult result2 = modifies2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & varName") {
		ModifiesS modifies1 = ModifiesS(assignSynonym, x);
		ModifiesS modifies2 = ModifiesS(ifSynonym, y);

		QueryResult result1 = modifies1.execute(pkb, false, map);
		QueryResult result2 = modifies2.execute(pkb, false, map);

		vector<string> expectedResult = { "1" };
		REQUIRE(result1.getSynonymResult("a") == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & underscore") {
		ModifiesS modifies1 = ModifiesS(stmtSynonym, varUnderscore);
		ModifiesS modifies2 = ModifiesS(ifSynonym, varUnderscore);

		QueryResult result1 = modifies1.execute(pkb, false, map);
		QueryResult result2 = modifies2.execute(pkb, false, map);

		vector<string> expectedResult = { "1", "2", "3" };
		vector<string> actualResult = result1.getSynonymResult("s");
		sort(actualResult.begin(), actualResult.end());
		REQUIRE(actualResult == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & synonym") {
		ModifiesS modifies1 = ModifiesS(stmtSynonym, var);
		ModifiesS modifies2 = ModifiesS(ifSynonym, var);

		QueryResult result1 = modifies1.execute(pkb, false, map);
		QueryResult result2 = modifies2.execute(pkb, false, map);

		vector<string> expectedStmtResult = { "1", "2", "3" };
		vector<string> expectedVarResult = { "x", "x", "y" };
		vector<string> actualVarResult = result1.getSynonymResult("var");
		vector<string> actualStmtResult = result1.getSynonymResult("s");
		sort(actualVarResult.begin(), actualVarResult.end());
		sort(actualStmtResult.begin(), actualStmtResult.end());
		REQUIRE(actualVarResult == expectedVarResult);
		REQUIRE(actualStmtResult == expectedStmtResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: underscore & synonym") {
		ModifiesS modifies = ModifiesS(stmtUnderscore, var);

		QueryResult result = modifies.execute(pkb, false, map);

		vector<string> expectedResult = { "x", "y" };
		vector<string> actualVarResult = result.getSynonymResult("var");
		sort(actualVarResult.begin(), actualVarResult.end());
		REQUIRE(actualVarResult == expectedResult);
	}

	SECTION("non-trivial: stmtNumber & synonym") {
		ModifiesS modifies1 = ModifiesS(stmtNo1, var);
		ModifiesS modifies2 = ModifiesS(stmtNo4, var);

		QueryResult result1 = modifies1.execute(pkb, false, map);
		QueryResult result2 = modifies2.execute(pkb, false, map);

		vector<string> expectedResult = { "x" };
		vector<string> actualResult = result1.getSynonymResult("var");
		REQUIRE(actualResult == expectedResult);
		REQUIRE(!result2.getResult());
	}
};
