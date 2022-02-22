#include "QP/Relationship/UsesS.h"

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
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
	map.insert({"s", DesignEntity::Stmt});
	map.insert({"a", DesignEntity::Assign});
	map.insert({"if", DesignEntity::If});

	QueryStmtRef stmtNo1 = {StmtRefType::StmtNumber, "1"};
	QueryStmtRef stmtNo2 = {StmtRefType::StmtNumber, "2"};
	QueryStmtRef stmtNo3 = {StmtRefType::StmtNumber, "3"};
	QueryStmtRef stmtNo4 = {StmtRefType::StmtNumber, "4"};
	QueryStmtRef stmtSynonym = {StmtRefType::Synonym, "s"};
	QueryStmtRef assignSynonym = {StmtRefType::Synonym, "a"};
	QueryStmtRef ifSynonym = {StmtRefType::Synonym, "if"};
	QueryStmtRef stmtUnderscore = {StmtRefType::Underscore, "_"};
	QueryEntRef x = {EntRefType::VarName, "x"};
	QueryEntRef y = {EntRefType::VarName, "y"};
	QueryEntRef var = {EntRefType::Synonym, "var"};
	QueryEntRef varUnderscore = {EntRefType::Underscore, "x"};

	SECTION("trivial: stmtNumber & varName") {
		QP::Relationship::UsesS uses1 = QP::Relationship::UsesS(stmtNo1, x);
		QP::Relationship::UsesS uses2 = QP::Relationship::UsesS(stmtNo1, y);

		QP::QueryResult result1 = uses1.execute(pkb, true, map);
		QP::QueryResult result2 = uses2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & underscore") {
		QP::Relationship::UsesS uses1 = QP::Relationship::UsesS(stmtNo1, varUnderscore);
		QP::Relationship::UsesS uses2 = QP::Relationship::UsesS(stmtNo4, varUnderscore);

		QP::QueryResult result1 = uses1.execute(pkb, true, map);
		QP::QueryResult result2 = uses2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & synonym") {
		QP::Relationship::UsesS uses1 = QP::Relationship::UsesS(stmtNo1, var);
		QP::Relationship::UsesS uses2 = QP::Relationship::UsesS(stmtNo4, var);

		QP::QueryResult result1 = uses1.execute(pkb, true, map);
		QP::QueryResult result2 = uses2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & varName") {
		QP::Relationship::UsesS uses = QP::Relationship::UsesS(stmtUnderscore, var);

		QP::QueryResult result = uses.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: underscore & underscore") {
		QP::Relationship::UsesS parent = QP::Relationship::UsesS(stmtUnderscore, varUnderscore);

		QP::QueryResult result = parent.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: underscore & synonym") {
		QP::Relationship::UsesS uses = QP::Relationship::UsesS(stmtUnderscore, var);

		QP::QueryResult result = uses.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: synonym & varName") {
		QP::Relationship::UsesS uses1 = QP::Relationship::UsesS(assignSynonym, x);
		QP::Relationship::UsesS uses2 = QP::Relationship::UsesS(assignSynonym, y);

		QP::QueryResult result1 = uses1.execute(pkb, true, map);
		QP::QueryResult result2 = uses2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & underscore") {
		QP::Relationship::UsesS uses1 = QP::Relationship::UsesS(stmtSynonym, varUnderscore);
		QP::Relationship::UsesS uses2 = QP::Relationship::UsesS(ifSynonym, varUnderscore);

		QP::QueryResult result1 = uses1.execute(pkb, true, map);
		QP::QueryResult result2 = uses2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & synonym") {
		QP::Relationship::UsesS uses1 = QP::Relationship::UsesS(stmtSynonym, var);
		QP::Relationship::UsesS uses2 = QP::Relationship::UsesS(ifSynonym, var);

		QP::QueryResult result1 = uses1.execute(pkb, true, map);
		QP::QueryResult result2 = uses2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & varName") {
		QP::Relationship::UsesS uses1 = QP::Relationship::UsesS(assignSynonym, x);
		QP::Relationship::UsesS uses2 = QP::Relationship::UsesS(ifSynonym, y);

		QP::QueryResult result1 = uses1.execute(pkb, false, map);
		QP::QueryResult result2 = uses2.execute(pkb, false, map);

		vector<string> expectedResult = {"1"};
		REQUIRE(result1.getSynonymResult("a") == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & underscore") {
		QP::Relationship::UsesS uses1 = QP::Relationship::UsesS(stmtSynonym, varUnderscore);
		QP::Relationship::UsesS uses2 = QP::Relationship::UsesS(ifSynonym, varUnderscore);

		QP::QueryResult result1 = uses1.execute(pkb, false, map);
		QP::QueryResult result2 = uses2.execute(pkb, false, map);

		vector<string> expectedResult = {"1", "2", "3"};
		vector<string> actualResult = result1.getSynonymResult("s");
		sort(actualResult.begin(), actualResult.end());
		REQUIRE(actualResult == expectedResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & synonym") {
		QP::Relationship::UsesS uses1 = QP::Relationship::UsesS(stmtSynonym, var);
		QP::Relationship::UsesS uses2 = QP::Relationship::UsesS(ifSynonym, var);

		QP::QueryResult result1 = uses1.execute(pkb, false, map);
		QP::QueryResult result2 = uses2.execute(pkb, false, map);

		vector<string> expectedStmtResult = {"1", "1", "2", "3"};
		vector<string> expectedVarResult = {"x", "x", "y", "z"};
		vector<string> actualVarResult = result1.getSynonymResult("var");
		vector<string> actualStmtResult = result1.getSynonymResult("s");
		sort(actualVarResult.begin(), actualVarResult.end());
		sort(actualStmtResult.begin(), actualStmtResult.end());
		REQUIRE(actualVarResult == expectedVarResult);
		REQUIRE(actualStmtResult == expectedStmtResult);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: underscore & synonym") {
		QP::Relationship::UsesS uses = QP::Relationship::UsesS(stmtUnderscore, var);

		QP::QueryResult result = uses.execute(pkb, false, map);

		vector<string> expectedResult = {"x", "y", "z"};
		vector<string> actualVarResult = result.getSynonymResult("var");
		sort(actualVarResult.begin(), actualVarResult.end());
		REQUIRE(actualVarResult == expectedResult);
	}

	SECTION("non-trivial: stmtNumber & synonym") {
		QP::Relationship::UsesS uses1 = QP::Relationship::UsesS(stmtNo1, var);
		QP::Relationship::UsesS uses2 = QP::Relationship::UsesS(stmtNo4, var);

		QP::QueryResult result1 = uses1.execute(pkb, false, map);
		QP::QueryResult result2 = uses2.execute(pkb, false, map);

		vector<string> expectedResult = {"x", "z"};
		vector<string> actualResult = result1.getSynonymResult("var");
		sort(actualResult.begin(), actualResult.end());
		REQUIRE(actualResult == expectedResult);
		REQUIRE(!result2.getResult());
	}
};
