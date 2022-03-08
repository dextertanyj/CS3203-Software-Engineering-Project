#include "QP/Relationship/ModifiesS.h"

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "catch.hpp"

TEST_CASE("QP::Relationship::ModifiesS::execute") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::WhileStmt);
	pkb.setStmtType(3, StmtType::WhileStmt);
	pkb.setStmtType(4, StmtType::IfStmt);
	pkb.setModifies(1, "x");
	pkb.setModifies(2, "x");
	pkb.setModifies(3, "y");

	unordered_map<string, DesignEntity> map;
	map.insert({"s", DesignEntity::Stmt});
	map.insert({"a", DesignEntity::Assign});
	map.insert({"if", DesignEntity::If});

	ReferenceArgument stmt_no1 = ReferenceArgument(1);
	ReferenceArgument stmt_no2 = ReferenceArgument(2);
	ReferenceArgument stmt_no3 = ReferenceArgument(3);
	ReferenceArgument stmt_no4 = ReferenceArgument(4);
	ReferenceArgument stmt_synonym = ReferenceArgument({QP::Types::DesignEntity::Stmt, "s"});
	ReferenceArgument assign_synonym = ReferenceArgument({QP::Types::DesignEntity::Assign, "a"});
	ReferenceArgument if_synonym = ReferenceArgument({QP::Types::DesignEntity::If, "if"});
	ReferenceArgument x = ReferenceArgument("x");
	ReferenceArgument y = ReferenceArgument("y");
	ReferenceArgument var = ReferenceArgument({QP::Types::DesignEntity::Variable, "if"});
	ReferenceArgument wildcard = ReferenceArgument();

	SECTION("trivial: stmtNumber & varName") {
		QP::Relationship::ModifiesS modifies1 = QP::Relationship::ModifiesS(stmt_no1, x);
		QP::Relationship::ModifiesS modifies2 = QP::Relationship::ModifiesS(stmt_no1, y);

		QP::QueryResult result1 = modifies1.execute(pkb, true, map);
		QP::QueryResult result2 = modifies2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & wildcard") {
		QP::Relationship::ModifiesS modifies1 = QP::Relationship::ModifiesS(stmt_no1, wildcard);
		QP::Relationship::ModifiesS modifies2 = QP::Relationship::ModifiesS(stmt_no4, wildcard);

		QP::QueryResult result1 = modifies1.execute(pkb, true, map);
		QP::QueryResult result2 = modifies2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: stmtNumber & synonym") {
		QP::Relationship::ModifiesS modifies1 = QP::Relationship::ModifiesS(stmt_no1, var);
		QP::Relationship::ModifiesS modifies2 = QP::Relationship::ModifiesS(stmt_no4, var);

		QP::QueryResult result1 = modifies1.execute(pkb, true, map);
		QP::QueryResult result2 = modifies2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & varName") {
		QP::Relationship::ModifiesS modifies1 = QP::Relationship::ModifiesS(assign_synonym, x);
		QP::Relationship::ModifiesS modifies2 = QP::Relationship::ModifiesS(assign_synonym, y);

		QP::QueryResult result1 = modifies1.execute(pkb, true, map);
		QP::QueryResult result2 = modifies2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & wildcard") {
		QP::Relationship::ModifiesS modifies1 = QP::Relationship::ModifiesS(stmt_synonym, wildcard);
		QP::Relationship::ModifiesS modifies2 = QP::Relationship::ModifiesS(if_synonym, wildcard);

		QP::QueryResult result1 = modifies1.execute(pkb, true, map);
		QP::QueryResult result2 = modifies2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("trivial: synonym & synonym") {
		QP::Relationship::ModifiesS modifies1 = QP::Relationship::ModifiesS(stmt_synonym, var);
		QP::Relationship::ModifiesS modifies2 = QP::Relationship::ModifiesS(if_synonym, var);

		QP::QueryResult result1 = modifies1.execute(pkb, true, map);
		QP::QueryResult result2 = modifies2.execute(pkb, true, map);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & varName") {
		QP::Relationship::ModifiesS modifies1 = QP::Relationship::ModifiesS(assign_synonym, x);
		QP::Relationship::ModifiesS modifies2 = QP::Relationship::ModifiesS(if_synonym, y);

		QP::QueryResult result1 = modifies1.execute(pkb, false, map);
		QP::QueryResult result2 = modifies2.execute(pkb, false, map);

		vector<string> expected_result = {"1"};
		REQUIRE(result1.getSynonymResult("a") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & wildcard") {
		QP::Relationship::ModifiesS modifies1 = QP::Relationship::ModifiesS(stmt_synonym, wildcard);
		QP::Relationship::ModifiesS modifies2 = QP::Relationship::ModifiesS(if_synonym, wildcard);

		QP::QueryResult result1 = modifies1.execute(pkb, false, map);
		QP::QueryResult result2 = modifies2.execute(pkb, false, map);

		vector<string> expected_result = {"1", "2", "3"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: synonym & synonym") {
		QP::Relationship::ModifiesS modifies1 = QP::Relationship::ModifiesS(stmt_synonym, var);
		QP::Relationship::ModifiesS modifies2 = QP::Relationship::ModifiesS(if_synonym, var);

		QP::QueryResult result1 = modifies1.execute(pkb, false, map);
		QP::QueryResult result2 = modifies2.execute(pkb, false, map);

		vector<string> expected_stmt_result = {"1", "2", "3"};
		vector<string> expected_var_result = {"x", "x", "y"};
		vector<string> actual_var_result = result1.getSynonymResult("var");
		vector<string> actual_stmt_result = result1.getSynonymResult("s");
		sort(actual_var_result.begin(), actual_var_result.end());
		sort(actual_stmt_result.begin(), actual_stmt_result.end());
		REQUIRE(actual_var_result == expected_var_result);
		REQUIRE(actual_stmt_result == expected_stmt_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("non-trivial: stmtNumber & synonym") {
		QP::Relationship::ModifiesS modifies1 = QP::Relationship::ModifiesS(stmt_no1, var);
		QP::Relationship::ModifiesS modifies2 = QP::Relationship::ModifiesS(stmt_no4, var);

		QP::QueryResult result1 = modifies1.execute(pkb, false, map);
		QP::QueryResult result2 = modifies2.execute(pkb, false, map);

		vector<string> expected_result = {"x"};
		vector<string> actual_result = result1.getSynonymResult("var");
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}
};
