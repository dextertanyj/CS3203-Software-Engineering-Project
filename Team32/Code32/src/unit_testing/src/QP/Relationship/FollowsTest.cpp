#include "QP/Relationship/Follows.h"

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "catch.hpp"

TEST_CASE("QP::Relationship::Follows::execute") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::WhileStmt);
	pkb.setStmtType(4, StmtType::IfStmt);
	pkb.setFollows(1, 2);
	pkb.setFollows(2, 3);
	pkb.setFollows(3, 4);

	unordered_map<string, DesignEntity> map;
	map.insert({"s", DesignEntity::Stmt});
	map.insert({"a", DesignEntity::Assign});
	map.insert({"if", DesignEntity::If});

	ReferenceArgument stmt_no1 = ReferenceArgument(1);
	ReferenceArgument stmt_no2 = ReferenceArgument(2);
	ReferenceArgument stmt_no3 = ReferenceArgument(3);
	ReferenceArgument stmt_no4 = ReferenceArgument(4);
	ReferenceArgument stmt_synonym =  ReferenceArgument({DesignEntity::Stmt, "s"});
	ReferenceArgument assign_synonym = ReferenceArgument({DesignEntity::Assign, "a"});
	ReferenceArgument if_synonym = ReferenceArgument({DesignEntity::If, "if"});
	ReferenceArgument underscore = ReferenceArgument();

	SECTION("trivial: stmtNumber & stmtNumber") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(stmt_no1, stmt_no2);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(stmt_no1, stmt_no3);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & underscore") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(stmt_no1, underscore);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(stmt_no4, underscore);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & synonym") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(stmt_no1, stmt_synonym);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(stmt_no1, assign_synonym);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & stmtNumber") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(underscore, stmt_no2);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(underscore, stmt_no1);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: underscore & underscore") {
		QP::Relationship::Follows follows = QP::Relationship::Follows(underscore, underscore);

		QP::QueryResult result = follows.execute(pkb, true, map);

		REQUIRE(result.getResult());
	}

	SECTION("trivial: underscore & synonym") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(underscore, stmt_synonym);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(underscore, assign_synonym);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & stmtNumber") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(assign_synonym, stmt_no2);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(assign_synonym, stmt_no3);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & underscore") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(assign_synonym, underscore);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(if_synonym, underscore);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & synonym") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(assign_synonym, stmt_synonym);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(assign_synonym, if_synonym);

		QP::QueryResult result1 = follows1.execute(pkb, true, map);
		QP::QueryResult result2 = follows2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & stmtNumber") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(assign_synonym, stmt_no2);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(if_synonym, stmt_no2);

		QP::QueryResult result1 = follows1.execute(pkb, false, map);
		QP::QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expected_result = {"1"};
		REQUIRE(result1.getSynonymResult("a") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & underscore") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(stmt_synonym, underscore);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(if_synonym, underscore);

		QP::QueryResult result1 = follows1.execute(pkb, false, map);
		QP::QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expected_result = {"1", "2", "3"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & synonym") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(stmt_synonym, if_synonym);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(if_synonym, assign_synonym);

		QP::QueryResult result1 = follows1.execute(pkb, false, map);
		QP::QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expected_stmt_result = {"3"};
		vector<string> expected_if_result = {"4"};
		REQUIRE(result1.getSynonymResult("s") == expected_stmt_result);
		REQUIRE(result1.getSynonymResult("if") == expected_if_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: underscore & synonym") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(underscore, if_synonym);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(underscore, assign_synonym);

		QP::QueryResult result1 = follows1.execute(pkb, false, map);
		QP::QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expected_result = {"4"};
		REQUIRE(result1.getSynonymResult("if") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: stmtNumber & synonym") {
		QP::Relationship::Follows follows1 = QP::Relationship::Follows(stmt_no1, stmt_synonym);
		QP::Relationship::Follows follows2 = QP::Relationship::Follows(stmt_no1, if_synonym);

		QP::QueryResult result1 = follows1.execute(pkb, false, map);
		QP::QueryResult result2 = follows2.execute(pkb, false, map);

		vector<string> expected_result = {"2"};
		REQUIRE(result1.getSynonymResult("s") == expected_result);
		REQUIRE(!result2.getResult());
	}
};
