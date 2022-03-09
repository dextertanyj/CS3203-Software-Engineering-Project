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

	ReferenceArgument left_proc_no1 = ReferenceArgument("A");
	ReferenceArgument left_proc_no2 = ReferenceArgument("B");
	ReferenceArgument left_proc_no3 = ReferenceArgument({QP::Types::DesignEntity::Procedure, "procedure"});
	ReferenceArgument x = ReferenceArgument("x");
	ReferenceArgument y = ReferenceArgument("y");
	ReferenceArgument var = ReferenceArgument({QP::Types::DesignEntity::Variable, "var"});
	ReferenceArgument wildcard = ReferenceArgument();

	SECTION("trivial: name & name") {
		QP::Relationship::UsesP uses1 = QP::Relationship::UsesP(left_proc_no1, x);
		QP::Relationship::UsesP uses2 = QP::Relationship::UsesP(left_proc_no1, y);

		QP::QueryResult result1 = QP::Relationship::UsesP::executeTrivialNameName(pkb, left_proc_no1, x);
		QP::QueryResult result2 = QP::Relationship::UsesP::executeTrivialNameName(pkb, left_proc_no1, y);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: name & wildcard") {
		QP::Relationship::UsesP uses1 = QP::Relationship::UsesP(left_proc_no1, wildcard);
		QP::Relationship::UsesP uses2 = QP::Relationship::UsesP(left_proc_no2, wildcard);

		QP::QueryResult result1 = QP::Relationship::UsesP::executeTrivialName(pkb, left_proc_no1);
		QP::QueryResult result2 = QP::Relationship::UsesP::executeTrivialName(pkb, left_proc_no2);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: name & synonym") {
		QP::Relationship::UsesP uses1 = QP::Relationship::UsesP(left_proc_no1, var);
		QP::Relationship::UsesP uses2 = QP::Relationship::UsesP(left_proc_no2, var);

		QP::QueryResult result1 = QP::Relationship::UsesP::executeTrivialName(pkb, left_proc_no1);
		QP::QueryResult result2 = QP::Relationship::UsesP::executeTrivialName(pkb, left_proc_no2);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & name") {
		QP::QueryResult result1 = QP::Relationship::UsesP::executeTrivialSynonymName(pkb, x);
		QP::QueryResult result2 = QP::Relationship::UsesP::executeTrivialSynonymName(pkb, y);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & wildcard") {
		QP::QueryResult result1 = QP::Relationship::UsesP::executeTrivialSynonym(pkb);

		REQUIRE(result1.getResult());
	}

	SECTION("trivial: synonym & synonym") {
		QP::QueryResult result1 = QP::Relationship::UsesP::executeTrivialSynonym(pkb);

		REQUIRE(result1.getResult());
	}

	SECTION("non-trivial: name & synonym") {
		QP::QueryResult result1 = QP::Relationship::UsesP::executeNameSynonym(pkb, left_proc_no1, var);
		QP::QueryResult result2 = QP::Relationship::UsesP::executeNameSynonym(pkb, left_proc_no2, var);

		vector<string> expected_result1 = {"x", "z"};
		vector<string> actual_result1 = result1.getSynonymResult("var");
		sort(actual_result1.begin(), actual_result1.end());
		REQUIRE(actual_result1 == expected_result1);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & name") {
		QP::QueryResult result1 = QP::Relationship::UsesP::executeSynonymName(pkb, left_proc_no3, x);
		QP::QueryResult result2 = QP::Relationship::UsesP::executeSynonymName(pkb, left_proc_no3, y);

		vector<string> expected_result1 = {"A"};
		vector<string> actual_result1 = result1.getSynonymResult("procedure");
		sort(actual_result1.begin(), actual_result1.end());
		REQUIRE(actual_result1 == expected_result1);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & wildcard") {
		QP::QueryResult result1 = QP::Relationship::UsesP::executeSynonymWildcard(pkb, left_proc_no3);

		vector<string> expected_result1 = {"A"};
		vector<string> actual_result1 = result1.getSynonymResult("procedure");
		sort(actual_result1.begin(), actual_result1.end());
		REQUIRE(actual_result1 == expected_result1);
	}

	SECTION("non-trivial: synonym & synonym") {
		QP::QueryResult result1 = QP::Relationship::UsesP::executeSynonymSynonym(pkb, left_proc_no3, var);

		vector<string> expected_proc_result1 = {"A", "A"};
		vector<string> actual_proc_result1 = result1.getSynonymResult("procedure");
		sort(actual_proc_result1.begin(), actual_proc_result1.end());
		vector<string> expected_var_result1 = {"x", "z"};
		vector<string> actual_var_result1 = result1.getSynonymResult("var");
		sort(actual_var_result1.begin(), actual_var_result1.end());
		REQUIRE(actual_proc_result1 == expected_proc_result1);
		REQUIRE(actual_var_result1 == expected_var_result1);
	}
}