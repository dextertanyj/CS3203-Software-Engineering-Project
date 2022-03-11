#include "QP/Relationship/Parent.h"

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "catch.hpp"

using namespace QP::Types;

TEST_CASE("QP::Relationship::Parent::execute") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::WhileStmt);
	pkb.setStmtType(4, StmtType::IfStmt);
	pkb.setParent(1, 2);
	pkb.setParent(2, 3);
	pkb.setParent(3, 4);

	ReferenceArgument stmt_no1 = ReferenceArgument(1);
	ReferenceArgument stmt_no2 = ReferenceArgument(2);
	ReferenceArgument stmt_no3 = ReferenceArgument(3);
	ReferenceArgument stmt_no4 = ReferenceArgument(4);
	ReferenceArgument stmt_synonym = ReferenceArgument({QP::Types::DesignEntity::Stmt, "s"});
	ReferenceArgument assign_synonym = ReferenceArgument({QP::Types::DesignEntity::Assign, "a"});
	ReferenceArgument if_synonym = ReferenceArgument({QP::Types::DesignEntity::If, "if"});
	ReferenceArgument wildcard = ReferenceArgument();

	SECTION("Trivial: Index & Index") {
		QP::QueryResult result1 = QP::Relationship::Parent::executeTrivialIndexIndex(pkb, stmt_no1, stmt_no2);
		QP::QueryResult result2 = QP::Relationship::Parent::executeTrivialIndexIndex(pkb, stmt_no1, stmt_no3);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Index & Wildcard") {
		QP::QueryResult result1 = QP::Relationship::Parent::executeTrivialIndexWildcard(pkb, stmt_no1);
		QP::QueryResult result2 = QP::Relationship::Parent::executeTrivialIndexWildcard(pkb, stmt_no4);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Index & Synonym") {
		QP::QueryResult result1 = QP::Relationship::Parent::executeTrivialIndexSynonym(pkb, stmt_no1, stmt_synonym);
		QP::QueryResult result2 = QP::Relationship::Parent::executeTrivialIndexSynonym(pkb, stmt_no1, assign_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard & Index") {
		QP::QueryResult result1 = QP::Relationship::Parent::executeTrivialWildcardIndex(pkb, stmt_no2);
		QP::QueryResult result2 = QP::Relationship::Parent::executeTrivialWildcardIndex(pkb, stmt_no1);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard & Wildcard") {
		QP::QueryResult result = QP::Relationship::Parent::executeTrivialWildcardWildcard(pkb);

		REQUIRE(result.getResult());
	}

	SECTION("Trivial: Wildcard & Synonym") {
		QP::QueryResult result1 = QP::Relationship::Parent::executeTrivialWildcardSynonym(pkb, stmt_synonym);
		QP::QueryResult result2 = QP::Relationship::Parent::executeTrivialWildcardSynonym(pkb, assign_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Index") {
		QP::QueryResult result1 = QP::Relationship::Parent::executeTrivialSynonymIndex(pkb, assign_synonym, stmt_no2);
		QP::QueryResult result2 = QP::Relationship::Parent::executeTrivialSynonymIndex(pkb, assign_synonym, stmt_no3);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Wildcard") {
		QP::QueryResult result1 = QP::Relationship::Parent::executeTrivialSynonymWildcard(pkb, assign_synonym);
		QP::QueryResult result2 = QP::Relationship::Parent::executeTrivialSynonymWildcard(pkb, if_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Synonym") {
		QP::QueryResult result1 = QP::Relationship::Parent::executeTrivialSynonymSynonym(pkb, assign_synonym, stmt_synonym);
		QP::QueryResult result2 = QP::Relationship::Parent::executeTrivialSynonymSynonym(pkb, if_synonym, stmt_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Index") {
		QP::QueryResult result1 = QP::Relationship::Parent::executeSynonymIndex(pkb, assign_synonym, stmt_no2);
		QP::QueryResult result2 = QP::Relationship::Parent::executeSynonymIndex(pkb, if_synonym, stmt_no2);

		vector<string> expected_result = {"1"};
		REQUIRE(result1.getSynonymResult("a") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Wildcard") {
		QP::QueryResult result1 = QP::Relationship::Parent::executeSynonymWildcard(pkb, stmt_synonym);
		QP::QueryResult result2 = QP::Relationship::Parent::executeSynonymWildcard(pkb, if_synonym);

		vector<string> expected_result = {"1", "2", "3"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Synonym") {
		QP::QueryResult result1 = QP::Relationship::Parent::executeSynonymSynonym(pkb, stmt_synonym, if_synonym);
		QP::QueryResult result2 = QP::Relationship::Parent::executeSynonymSynonym(pkb, if_synonym, assign_synonym);

		vector<string> expected_stmt_result = {"3"};
		vector<string> expected_if_result = {"4"};
		REQUIRE(result1.getSynonymResult("s") == expected_stmt_result);
		REQUIRE(result1.getSynonymResult("if") == expected_if_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Wildcard & Synonym") {
		QP::QueryResult result1 = QP::Relationship::Parent::executeWildcardSynonym(pkb, if_synonym);
		QP::QueryResult result2 = QP::Relationship::Parent::executeWildcardSynonym(pkb, assign_synonym);

		vector<string> expected_result = {"4"};
		REQUIRE(result1.getSynonymResult("if") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Index & Synonym") {
		QP::QueryResult result1 = QP::Relationship::Parent::executeIndexSynonym(pkb, stmt_no1, stmt_synonym);
		QP::QueryResult result2 = QP::Relationship::Parent::executeIndexSynonym(pkb, stmt_no1, if_synonym);

		vector<string> expected_result = {"2"};
		REQUIRE(result1.getSynonymResult("s") == expected_result);
		REQUIRE(!result2.getResult());
	}
};
