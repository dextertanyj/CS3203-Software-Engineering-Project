#include "QP/Relationship/UsesP.h"

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "catch.hpp"

TEST_CASE("QP::Relationship::UsesP::execute") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Print);
	pkb.setStmtType(3, StmtType::Read);
	pkb.setStmtType(4, StmtType::Read);
	pkb.setUses(1, "x");
	pkb.setUses(1, "z");
	pkb.setUses(2, "x");
	pkb.setProc("A", 1, 2);
	pkb.setProc("B", 3, 4);
	pkb.populateComplexRelations();

	unordered_map<string, DesignEntity> map;

	QueryEntRef left_proc_no1 = {EntRefType::VarName, "A"};
	QueryEntRef left_proc_no2 = {EntRefType::VarName, "B"};
	QueryEntRef left_proc_no3 = {EntRefType::Synonym, "procedure"};
	QueryEntRef x = {EntRefType::VarName, "x"};
	QueryEntRef y = {EntRefType::VarName, "y"};
	QueryEntRef var = {EntRefType::Synonym, "var"};
	QueryEntRef var_underscore = {EntRefType::Underscore, "x"};

	SECTION("trivial: varName & varName") {
		QP::Relationship::UsesP uses1 = QP::Relationship::UsesP(left_proc_no1, x);
		QP::Relationship::UsesP uses2 = QP::Relationship::UsesP(left_proc_no1, y);

		QP::QueryResult result1 = uses1.execute(pkb, true, map);
		QP::QueryResult result2 = uses2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: varName & underscore") {
		QP::Relationship::UsesP uses1 = QP::Relationship::UsesP(left_proc_no1, var_underscore);
		QP::Relationship::UsesP uses2 = QP::Relationship::UsesP(left_proc_no2, var_underscore);

		QP::QueryResult result1 = uses1.execute(pkb, true, map);
		QP::QueryResult result2 = uses2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: varName & synonym") {
		QP::Relationship::UsesP uses1 = QP::Relationship::UsesP(left_proc_no1, var);
		QP::Relationship::UsesP uses2 = QP::Relationship::UsesP(left_proc_no2, var);

		QP::QueryResult result1 = uses1.execute(pkb, true, map);
		QP::QueryResult result2 = uses2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & varName") {
		QP::Relationship::UsesP uses1 = QP::Relationship::UsesP(left_proc_no3, x);
		QP::Relationship::UsesP uses2 = QP::Relationship::UsesP(left_proc_no3, y);

		QP::QueryResult result1 = uses1.execute(pkb, true, map);
		QP::QueryResult result2 = uses2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & underscore") {
		QP::Relationship::UsesP uses1 = QP::Relationship::UsesP(left_proc_no3, var_underscore);

		QP::QueryResult result1 = uses1.execute(pkb, true, map);

		REQUIRE(result1.getResult());
	}

	SECTION("trivial: synonym & synonym") {
		QP::Relationship::UsesP uses1 = QP::Relationship::UsesP(left_proc_no3, var);

		QP::QueryResult result1 = uses1.execute(pkb, true, map);

		REQUIRE(result1.getResult());
	}

	SECTION("non-trivial: varName & synonym") {
		QP::Relationship::UsesP uses1 = QP::Relationship::UsesP(left_proc_no1, var);
		QP::Relationship::UsesP uses2 = QP::Relationship::UsesP(left_proc_no2, var);

		QP::QueryResult result1 = uses1.execute(pkb, false, map);
		QP::QueryResult result2 = uses2.execute(pkb, false, map);

		vector<string> expected_result1 = {"x", "z"};
		vector<string> actual_result1 = result1.getSynonymResult("var");
		sort(actual_result1.begin(), actual_result1.end());
		REQUIRE(actual_result1 == expected_result1);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & VarName") {
		QP::Relationship::UsesP uses1 = QP::Relationship::UsesP(left_proc_no3, x);
		QP::Relationship::UsesP uses2 = QP::Relationship::UsesP(left_proc_no3, y);

		QP::QueryResult result1 = uses1.execute(pkb, false, map);
		QP::QueryResult result2 = uses2.execute(pkb, false, map);

		vector<string> expected_result1 = {"A"};
		vector<string> actual_result1 = result1.getSynonymResult("procedure");
		sort(actual_result1.begin(), actual_result1.end());
		REQUIRE(actual_result1 == expected_result1);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & underscore") {
		QP::Relationship::UsesP uses1 = QP::Relationship::UsesP(left_proc_no3, var_underscore);

		QP::QueryResult result1 = uses1.execute(pkb, false, map);

		vector<string> expected_result1 = {"A"};
		vector<string> actual_result1 = result1.getSynonymResult("procedure");
		sort(actual_result1.begin(), actual_result1.end());
		REQUIRE(actual_result1 == expected_result1);
	}

	SECTION("non-trivial: synonym & synonym") {
		QP::Relationship::UsesP uses1 = QP::Relationship::UsesP(left_proc_no3, var);

		QP::QueryResult result1 = uses1.execute(pkb, false, map);

		vector<string> expected_proc_result1 = {"A", "A"};
		vector<string> actual_proc_result1 = result1.getSynonymResult("procedure");
		sort(actual_proc_result1.begin(), actual_proc_result1.end());
		vector<string> expected_var_result1 = {"x", "z"};
		vector<string> actual_var_result1 = result1.getSynonymResult("var");
		REQUIRE(actual_proc_result1 == expected_proc_result1);
		REQUIRE(actual_var_result1 == expected_var_result1);
	}
}