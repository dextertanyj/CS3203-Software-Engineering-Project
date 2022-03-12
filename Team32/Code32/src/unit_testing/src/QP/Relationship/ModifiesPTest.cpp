#include "QP/Relationship/ModifiesP.h"

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "catch.hpp"

TEST_CASE("QP::Relationship::ModifiesP::execute") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::Print);
	pkb.setStmtType(4, StmtType::Print);
	pkb.setModifies(1, "x");
	pkb.setModifies(2, "z");
	pkb.setProc("A", 1, 2);
	pkb.setProc("B", 3, 4);
	pkb.populateComplexRelations();

	ReferenceArgument left_proc_no1 = ReferenceArgument("A");
	ReferenceArgument left_proc_no2 = ReferenceArgument("B");
	ReferenceArgument left_proc_no3 = ReferenceArgument({QP::Types::DesignEntity::Procedure, "procedure"});
	ReferenceArgument x = ReferenceArgument("x");
	ReferenceArgument y = ReferenceArgument("y");
	ReferenceArgument var = ReferenceArgument({QP::Types::DesignEntity::Variable, "var"});

	SECTION("Trivial: Name & Name") {
		QP::QueryResult result1 = QP::Relationship::ModifiesP::executeTrivialNameName(pkb, left_proc_no1, x);
		QP::QueryResult result2 = QP::Relationship::ModifiesP::executeTrivialNameName(pkb, left_proc_no1, y);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Name & Wildcard") {
		QP::QueryResult result1 = QP::Relationship::ModifiesP::executeTrivialNameWildcardOrSynonym(pkb, left_proc_no1);
		QP::QueryResult result2 = QP::Relationship::ModifiesP::executeTrivialNameWildcardOrSynonym(pkb, left_proc_no2);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Name & Synonym") {
		QP::QueryResult result1 = QP::Relationship::ModifiesP::executeTrivialNameWildcardOrSynonym(pkb, left_proc_no1);
		QP::QueryResult result2 = QP::Relationship::ModifiesP::executeTrivialNameWildcardOrSynonym(pkb, left_proc_no2);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Name") {
		QP::QueryResult result1 = QP::Relationship::ModifiesP::executeTrivialSynonymName(pkb, x);
		QP::QueryResult result2 = QP::Relationship::ModifiesP::executeTrivialSynonymName(pkb, y);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Wildcard") {
		QP::QueryResult result1 = QP::Relationship::ModifiesP::executeTrivialSynonymWildcardOrSynonym(pkb);

		REQUIRE(result1.getResult());
	}

	SECTION("Trivial: Synonym & Synonym") {
		QP::QueryResult result1 = QP::Relationship::ModifiesP::executeTrivialSynonymWildcardOrSynonym(pkb);

		REQUIRE(result1.getResult());
	}

	SECTION("Name & Synonym") {
		QP::QueryResult result1 = QP::Relationship::ModifiesP::executeNameSynonym(pkb, left_proc_no1, var);
		QP::QueryResult result2 = QP::Relationship::ModifiesP::executeNameSynonym(pkb, left_proc_no2, var);

		vector<string> expected_result1 = {"x", "z"};
		vector<string> actual_result1 = result1.getSynonymResult("var");
		sort(actual_result1.begin(), actual_result1.end());
		REQUIRE(actual_result1 == expected_result1);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Name") {
		QP::QueryResult result1 = QP::Relationship::ModifiesP::executeSynonymName(pkb, left_proc_no3, x);
		QP::QueryResult result2 = QP::Relationship::ModifiesP::executeSynonymName(pkb, left_proc_no3, y);

		vector<string> expected_result1 = {"A"};
		vector<string> actual_result1 = result1.getSynonymResult("procedure");
		sort(actual_result1.begin(), actual_result1.end());
		REQUIRE(actual_result1 == expected_result1);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Wildcard") {
		QP::QueryResult result1 = QP::Relationship::ModifiesP::executeSynonymWildcard(pkb, left_proc_no3);

		vector<string> expected_result1 = {"A"};
		vector<string> actual_result1 = result1.getSynonymResult("procedure");
		sort(actual_result1.begin(), actual_result1.end());
		REQUIRE(actual_result1 == expected_result1);
	}

	SECTION("Synonym & Synonym") {
		QP::QueryResult result1 = QP::Relationship::ModifiesP::executeSynonymSynonym(pkb, left_proc_no3, var);

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