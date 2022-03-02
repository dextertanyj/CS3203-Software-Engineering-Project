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

	QueryStmtRef stmt_no1 = {StmtRefType::StmtNumber, "1"};
	QueryStmtRef stmt_no2 = {StmtRefType::StmtNumber, "2"};
	QueryStmtRef stmt_no3 = {StmtRefType::StmtNumber, "3"};
	QueryStmtRef stmt_no4 = {StmtRefType::StmtNumber, "4"};
	QueryStmtRef stmt_synonym = {StmtRefType::Synonym, "s"};
	QueryStmtRef assign_synonym = {StmtRefType::Synonym, "a"};
	QueryStmtRef if_synonym = {StmtRefType::Synonym, "if"};
	QueryStmtRef stmt_underscore = {StmtRefType::Underscore, "_"};
	QueryEntRef x = {EntRefType::VarName, "x"};
	QueryEntRef y = {EntRefType::VarName, "y"};
	QueryEntRef var = {EntRefType::Synonym, "var"};
	QueryEntRef var_underscore = {EntRefType::Underscore, "x"};

	SECTION("trivial: stmtNumber & varName") {
		QP::Relationship::UsesS uses1 = QP::Relationship::UsesS(stmt_no1, x);
		QP::Relationship::UsesS uses2 = QP::Relationship::UsesS(stmt_no1, y);

		QP::QueryResult result1 = uses1.execute(pkb, true, map);
		QP::QueryResult result2 = uses2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & underscore") {
		QP::Relationship::UsesS uses1 = QP::Relationship::UsesS(stmt_no1, var_underscore);
		QP::Relationship::UsesS uses2 = QP::Relationship::UsesS(stmt_no4, var_underscore);

		QP::QueryResult result1 = uses1.execute(pkb, true, map);
		QP::QueryResult result2 = uses2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & synonym") {
		QP::Relationship::UsesS uses1 = QP::Relationship::UsesS(stmt_no1, var);
		QP::Relationship::UsesS uses2 = QP::Relationship::UsesS(stmt_no4, var);

		QP::QueryResult result1 = uses1.execute(pkb, true, map);
		QP::QueryResult result2 = uses2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & varName") {
		QP::Relationship::UsesS uses1 = QP::Relationship::UsesS(assign_synonym, x);
		QP::Relationship::UsesS uses2 = QP::Relationship::UsesS(assign_synonym, y);

		QP::QueryResult result1 = uses1.execute(pkb, true, map);
		QP::QueryResult result2 = uses2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & underscore") {
		QP::Relationship::UsesS uses1 = QP::Relationship::UsesS(stmt_synonym, var_underscore);
		QP::Relationship::UsesS uses2 = QP::Relationship::UsesS(if_synonym, var_underscore);

		QP::QueryResult result1 = uses1.execute(pkb, true, map);
		QP::QueryResult result2 = uses2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & synonym") {
		QP::Relationship::UsesS uses1 = QP::Relationship::UsesS(stmt_synonym, var);
		QP::Relationship::UsesS uses2 = QP::Relationship::UsesS(if_synonym, var);

		QP::QueryResult result1 = uses1.execute(pkb, true, map);
		QP::QueryResult result2 = uses2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & varName") {
		QP::Relationship::UsesS uses1 = QP::Relationship::UsesS(assign_synonym, x);
		QP::Relationship::UsesS uses2 = QP::Relationship::UsesS(if_synonym, y);

		QP::QueryResult result1 = uses1.execute(pkb, false, map);
		QP::QueryResult result2 = uses2.execute(pkb, false, map);

		vector<string> expected_result = {"1"};
		REQUIRE(result1.getSynonymResult("a") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & underscore") {
		QP::Relationship::UsesS uses1 = QP::Relationship::UsesS(stmt_synonym, var_underscore);
		QP::Relationship::UsesS uses2 = QP::Relationship::UsesS(if_synonym, var_underscore);

		QP::QueryResult result1 = uses1.execute(pkb, false, map);
		QP::QueryResult result2 = uses2.execute(pkb, false, map);

		vector<string> expected_result = {"1", "2", "3"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & synonym") {
		QP::Relationship::UsesS uses1 = QP::Relationship::UsesS(stmt_synonym, var);
		QP::Relationship::UsesS uses2 = QP::Relationship::UsesS(if_synonym, var);

		QP::QueryResult result1 = uses1.execute(pkb, false, map);
		QP::QueryResult result2 = uses2.execute(pkb, false, map);

		vector<string> expected_stmt_result = {"1", "1", "2", "3"};
		vector<string> expected_var_result = {"x", "x", "y", "z"};
		vector<string> actual_var_result = result1.getSynonymResult("var");
		vector<string> actual_stmt_result = result1.getSynonymResult("s");
		sort(actual_var_result.begin(), actual_var_result.end());
		sort(actual_stmt_result.begin(), actual_stmt_result.end());
		REQUIRE(actual_var_result == expected_var_result);
		REQUIRE(actual_stmt_result == expected_stmt_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: stmtNumber & synonym") {
		QP::Relationship::UsesS uses1 = QP::Relationship::UsesS(stmt_no1, var);
		QP::Relationship::UsesS uses2 = QP::Relationship::UsesS(stmt_no4, var);

		QP::QueryResult result1 = uses1.execute(pkb, false, map);
		QP::QueryResult result2 = uses2.execute(pkb, false, map);

		vector<string> expected_result = {"x", "z"};
		vector<string> actual_result = result1.getSynonymResult("var");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}
};
