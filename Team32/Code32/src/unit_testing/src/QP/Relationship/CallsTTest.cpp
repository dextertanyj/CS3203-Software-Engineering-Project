#include "QP/Relationship/CallsT.h"

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "catch.hpp"

TEST_CASE("QP::Relationship::CallsT::execute") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Call);
	pkb.setStmtType(3, StmtType::Call);
	pkb.setStmtType(4, StmtType::Read);
	pkb.setProc("proc1", 1, 2);
	pkb.setProc("proc2", 3, 3);
	pkb.setProc("proc3", 4, 4);
	pkb.setCall(2, "proc2");
	pkb.setCall(3, "proc3");
	pkb.populateComplexRelations();

	unordered_map<string, DesignEntity> map;
	map.insert({"p1", DesignEntity::Procedure});
	map.insert({"p2", DesignEntity::Procedure});

	ReferenceArgument proc1 = ReferenceArgument("proc1");
	ReferenceArgument proc2 = ReferenceArgument("proc2");
	ReferenceArgument proc3 = ReferenceArgument("proc3");
	ReferenceArgument proc_synonym_1 = ReferenceArgument({DesignEntity::Procedure, "p1"});
	ReferenceArgument proc_synonym_2 = ReferenceArgument({DesignEntity::Procedure, "p2"});
	ReferenceArgument wildcard = ReferenceArgument();

	SECTION("trivial: varName & varName") {
		QP::Relationship::CallsT callsT1 = QP::Relationship::CallsT(proc1, proc3);
		QP::Relationship::CallsT callsT2 = QP::Relationship::CallsT(proc2, proc1);

		QP::QueryResult result1 = callsT1.execute(pkb, true, map);
		QP::QueryResult result2 = callsT2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: varName & underscore") {
		QP::Relationship::CallsT callsT1 = QP::Relationship::CallsT(proc1, wildcard);
		QP::Relationship::CallsT callsT2 = QP::Relationship::CallsT(proc3, wildcard);

		QP::QueryResult result1 = callsT1.execute(pkb, true, map);
		QP::QueryResult result2 = callsT2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: varName & synonym") {
		QP::Relationship::CallsT callsT1 = QP::Relationship::CallsT(proc1, proc_synonym_1);
		QP::Relationship::CallsT callsT2 = QP::Relationship::CallsT(proc3, proc_synonym_1);

		QP::QueryResult result1 = callsT1.execute(pkb, true, map);
		QP::QueryResult result2 = callsT2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & varName") {
		QP::Relationship::CallsT callsT1 = QP::Relationship::CallsT(wildcard, proc2);
		QP::Relationship::CallsT callsT2 = QP::Relationship::CallsT(wildcard, proc1);

		QP::QueryResult result1 = callsT1.execute(pkb, true, map);
		QP::QueryResult result2 = callsT2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & underscore") {
		QP::Relationship::CallsT callsT = QP::Relationship::CallsT(wildcard, wildcard);

		QP::QueryResult result = callsT.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: underscore & synonym") {
		QP::Relationship::CallsT callsT = QP::Relationship::CallsT(wildcard, proc_synonym_1);

		QP::QueryResult result = callsT.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: synonym & varName") {
		QP::Relationship::CallsT callsT1 = QP::Relationship::CallsT(proc_synonym_1, proc2);
		QP::Relationship::CallsT callsT2 = QP::Relationship::CallsT(proc_synonym_1, proc1);

		QP::QueryResult result1 = callsT1.execute(pkb, true, map);
		QP::QueryResult result2 = callsT2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & underscore") {
		QP::Relationship::CallsT callsT = QP::Relationship::CallsT(proc_synonym_1, wildcard);

		QP::QueryResult result = callsT.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: synonym & synonym") {
		QP::Relationship::CallsT callsT1 = QP::Relationship::CallsT(proc_synonym_1, proc_synonym_2);
		QP::Relationship::CallsT callsT2 = QP::Relationship::CallsT(proc_synonym_1, proc_synonym_1);

		QP::QueryResult result1 = callsT1.execute(pkb, true, map);
		QP::QueryResult result2 = callsT2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: varName & synonym") {
		QP::Relationship::CallsT callsT1 = QP::Relationship::CallsT(proc1, proc_synonym_1);
		QP::Relationship::CallsT callsT2 = QP::Relationship::CallsT(proc3, proc_synonym_1);

		QP::QueryResult result1 = callsT1.execute(pkb, false, map);
		QP::QueryResult result2 = callsT2.execute(pkb, false, map);

		vector<string> expected_result = {"proc2", "proc3"};
		vector<string> actual_result = result1.getSynonymResult("p1");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: underscore & synonym") {
		QP::Relationship::CallsT callsT = QP::Relationship::CallsT(wildcard, proc_synonym_1);

		QP::QueryResult result = callsT.execute(pkb, false, map);

		vector<string> expected_result = {"proc2", "proc3"};
		vector<string> actual_result = result.getSynonymResult("p1");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
	}

	SECTION("non-trivial: synonym & varName") {
		QP::Relationship::CallsT callsT1 = QP::Relationship::CallsT(proc_synonym_1, proc3);
		QP::Relationship::CallsT callsT2 = QP::Relationship::CallsT(proc_synonym_1, proc1);

		QP::QueryResult result1 = callsT1.execute(pkb, false, map);
		QP::QueryResult result2 = callsT2.execute(pkb, false, map);

		vector<string> expected_result = {"proc1", "proc2"};
		vector<string> actual_result = result1.getSynonymResult("p1");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & underscore") {
		QP::Relationship::CallsT callsT = QP::Relationship::CallsT(proc_synonym_1, wildcard);

		QP::QueryResult result = callsT.execute(pkb, false, map);

		vector<string> expected_result = {"proc1", "proc2"};
		vector<string> actual_result = result.getSynonymResult("p1");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
	}

	SECTION("non-trivial: synonym & synonym") {
		QP::Relationship::CallsT callsT1 = QP::Relationship::CallsT(proc_synonym_1, proc_synonym_2);
		QP::Relationship::CallsT callsT2 = QP::Relationship::CallsT(proc_synonym_1, proc_synonym_1);

		QP::QueryResult result1 = callsT1.execute(pkb, false, map);
		QP::QueryResult result2 = callsT2.execute(pkb, false, map);

		vector<string> expected_result_1 = {"proc1", "proc1", "proc2"};
		vector<string> expected_result_2 = {"proc2", "proc3", "proc3"};
		vector<string> actual_result_1 = result1.getSynonymResult("p1");
		vector<string> actual_result_2 = result1.getSynonymResult("p2");
		sort(actual_result_1.begin(), actual_result_1.end());
		sort(actual_result_2.begin(), actual_result_2.end());
		REQUIRE(actual_result_1 == expected_result_1);
		REQUIRE(actual_result_2 == expected_result_2);
		REQUIRE(!result2.getResult());
	}
}
