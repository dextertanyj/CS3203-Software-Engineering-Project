#include "QP/Relationship/ModifiesS.h"

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "catch.hpp"

using namespace QP::Types;

TEST_CASE("QP::Relationship::ModifiesS::execute") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::WhileStmt);
	pkb.setStmtType(3, StmtType::WhileStmt);
	pkb.setStmtType(4, StmtType::IfStmt);
	pkb.setModifies(1, "x");
	pkb.setModifies(2, "x");
	pkb.setModifies(3, "y");

	ReferenceArgument stmt_no1 = ReferenceArgument(1);
	ReferenceArgument stmt_no2 = ReferenceArgument(2);
	ReferenceArgument stmt_no3 = ReferenceArgument(3);
	ReferenceArgument stmt_no4 = ReferenceArgument(4);
	ReferenceArgument stmt_synonym = ReferenceArgument({QP::Types::DesignEntity::Stmt, "s"});
	ReferenceArgument assign_synonym = ReferenceArgument({QP::Types::DesignEntity::Assign, "a"});
	ReferenceArgument if_synonym = ReferenceArgument({QP::Types::DesignEntity::If, "if"});
	ReferenceArgument x = ReferenceArgument("x");
	ReferenceArgument y = ReferenceArgument("y");
	ReferenceArgument var = ReferenceArgument({QP::Types::DesignEntity::Variable, "var"});

	SECTION("Trivial: Index & Name") {
		QP::QueryResult result1 = QP::Relationship::ModifiesS::executeTrivialIndexName(pkb,stmt_no1, x);
		QP::QueryResult result2 = QP::Relationship::ModifiesS::executeTrivialIndexName(pkb,stmt_no1, y);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Index & Wildcard") {
		QP::QueryResult result1 = QP::Relationship::ModifiesS::executeTrivialIndexWildcardOrSynonym(pkb, stmt_no1);
		QP::QueryResult result2 = QP::Relationship::ModifiesS::executeTrivialIndexWildcardOrSynonym(pkb, stmt_no4);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Index & Synonym") {
		QP::QueryResult result1 = QP::Relationship::ModifiesS::executeTrivialIndexWildcardOrSynonym(pkb, stmt_no1);
		QP::QueryResult result2 = QP::Relationship::ModifiesS::executeTrivialIndexWildcardOrSynonym(pkb, stmt_no4);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Name") {
		QP::QueryResult result1 = QP::Relationship::ModifiesS::executeTrivialSynonymName(pkb, assign_synonym, x);
		QP::QueryResult result2 = QP::Relationship::ModifiesS::executeTrivialSynonymName(pkb, assign_synonym, y);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Wildcard") {
		QP::QueryResult result1 = QP::Relationship::ModifiesS::executeTrivialSynonymWildcardOrSynonym(pkb, stmt_synonym);
		QP::QueryResult result2 = QP::Relationship::ModifiesS::executeTrivialSynonymWildcardOrSynonym(pkb, if_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Synonym") {
		QP::QueryResult result1 = QP::Relationship::ModifiesS::executeTrivialSynonymWildcardOrSynonym(pkb, stmt_synonym);
		QP::QueryResult result2 = QP::Relationship::ModifiesS::executeTrivialSynonymWildcardOrSynonym(pkb, if_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Name") {
		QP::QueryResult result1 = QP::Relationship::ModifiesS::executeSynonymName(pkb, assign_synonym, x);
		QP::QueryResult result2 = QP::Relationship::ModifiesS::executeSynonymName(pkb, if_synonym, y);

		vector<string> expected_result = {"1"};
		REQUIRE(result1.getSynonymResult("a") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Wildcard") {
		QP::QueryResult result1 = QP::Relationship::ModifiesS::executeSynonymWildcard(pkb, stmt_synonym);
		QP::QueryResult result2 = QP::Relationship::ModifiesS::executeSynonymWildcard(pkb, if_synonym);

		vector<string> expected_result = {"1", "2", "3"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Synonym") {
		QP::QueryResult result1 = QP::Relationship::ModifiesS::executeSynonymSynonym(pkb, stmt_synonym, var);
		QP::QueryResult result2 = QP::Relationship::ModifiesS::executeSynonymSynonym(pkb, if_synonym, var);

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

	SECTION("Index & Synonym") {
		QP::QueryResult result1 = QP::Relationship::ModifiesS::executeIndexSynonym(pkb, stmt_no1, var);
		QP::QueryResult result2 = QP::Relationship::ModifiesS::executeIndexSynonym(pkb, stmt_no4, var);

		vector<string> expected_result = {"x"};
		vector<string> actual_result = result1.getSynonymResult("var");
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}
};
