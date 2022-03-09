#include "QP/Relationship/ParentT.h"

#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "catch.hpp"

TEST_CASE("QP::Relationship::ParentT::execute") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::WhileStmt);
	pkb.setStmtType(2, StmtType::IfStmt);
	pkb.setStmtType(3, StmtType::Read);
	pkb.setStmtType(4, StmtType::Assign);
	pkb.setParent(1, 2);
	pkb.setParent(1, 3);
	pkb.setParent(2, 4);
	pkb.populateComplexRelations();

	ReferenceArgument stmt_no1 = ReferenceArgument(1);
	ReferenceArgument stmt_no2 = ReferenceArgument(2);
	ReferenceArgument stmt_no3 = ReferenceArgument(3);
	ReferenceArgument stmt_no4 = ReferenceArgument(4);
	ReferenceArgument stmt_synonym = ReferenceArgument({QP::Types::DesignEntity::Stmt, "s"});
	ReferenceArgument assign_synonym = ReferenceArgument({QP::Types::DesignEntity::Assign, "a"});
	ReferenceArgument while_synonym = ReferenceArgument({QP::Types::DesignEntity::While, "w"});
	ReferenceArgument if_synonym = ReferenceArgument({QP::Types::DesignEntity::If, "if"});
	ReferenceArgument wildcard = ReferenceArgument();

	SECTION("Trivial: Index & Index") {
		QP::QueryResult result1 = QP::Relationship::ParentT::executeTrivialIndexIndex(pkb, stmt_no1, stmt_no3);
		QP::QueryResult result2 = QP::Relationship::ParentT::executeTrivialIndexIndex(pkb, stmt_no2, stmt_no1);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Index & Wildcard") {
		QP::QueryResult result1 = QP::Relationship::ParentT::executeTrivialIndexWildcard(pkb, stmt_no1);
		QP::QueryResult result2 = QP::Relationship::ParentT::executeTrivialIndexWildcard(pkb, stmt_no3);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Index & Synonym") {
		QP::QueryResult result1 = QP::Relationship::ParentT::executeTrivialIndexSynonym(pkb, stmt_no1, if_synonym);
		QP::QueryResult result2 = QP::Relationship::ParentT::executeTrivialIndexSynonym(pkb, stmt_no2, while_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard & Index") {
		QP::QueryResult result1 = QP::Relationship::ParentT::executeTrivialWildcardIndex(pkb, stmt_no3);
		QP::QueryResult result2 = QP::Relationship::ParentT::executeTrivialWildcardIndex(pkb, stmt_no1);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Wildcard & Wildcard") {
		QP::Relationship::ParentT parent = QP::Relationship::ParentT(wildcard, wildcard);

		QP::QueryResult result = QP::Relationship::ParentT::executeTrivialWildcardWildcard(pkb);

		REQUIRE(result.getResult());
	}

	SECTION("Trivial: Wildcard & Synonym") {
		QP::QueryResult result1 = QP::Relationship::ParentT::executeTrivialWildcardSynonym(pkb, assign_synonym);
		QP::QueryResult result2 = QP::Relationship::ParentT::executeTrivialWildcardSynonym(pkb, while_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Index") {
		QP::QueryResult result1 = QP::Relationship::ParentT::executeTrivialSynonymIndex(pkb, while_synonym, stmt_no4);
		QP::QueryResult result2 = QP::Relationship::ParentT::executeTrivialSynonymIndex(pkb, assign_synonym, stmt_no3);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Wildcard") {
		QP::QueryResult result1 = QP::Relationship::ParentT::executeTrivialSynonymWildcard(pkb, while_synonym);
		QP::QueryResult result2 = QP::Relationship::ParentT::executeTrivialSynonymWildcard(pkb, assign_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Trivial: Synonym & Synonym") {
		QP::QueryResult result1 = QP::Relationship::ParentT::executeTrivialSynonymSynonym(pkb, while_synonym, assign_synonym);
		QP::QueryResult result2 = QP::Relationship::ParentT::executeTrivialSynonymSynonym(pkb, assign_synonym, stmt_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Index") {
		QP::QueryResult result1 = QP::Relationship::ParentT::executeSynonymIndex(pkb, while_synonym, stmt_no4);
		QP::QueryResult result2 = QP::Relationship::ParentT::executeSynonymIndex(pkb, assign_synonym, stmt_no2);

		vector<string> expected_result = {"1"};
		REQUIRE(result1.getSynonymResult("w") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Wildcard") {
		QP::QueryResult result1 = QP::Relationship::ParentT::executeSynonymWildcard(pkb, stmt_synonym);
		QP::QueryResult result2 = QP::Relationship::ParentT::executeSynonymWildcard(pkb, assign_synonym);

		vector<string> expected_result = {"1", "2"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Synonym & Synonym") {
		QP::QueryResult result1 = QP::Relationship::ParentT::executeSynonymSynonym(pkb, stmt_synonym, assign_synonym);
		QP::QueryResult result2 = QP::Relationship::ParentT::executeSynonymSynonym(pkb, assign_synonym, while_synonym);

		vector<string> expected_stmt_result = {"1", "2"};
		vector<string> expected_assign_result = {"4", "4"};
		vector<string> actual_stmt_result = result1.getSynonymResult("s");
		sort(actual_stmt_result.begin(), actual_stmt_result.end());
		REQUIRE(actual_stmt_result == expected_stmt_result);
		REQUIRE(result1.getSynonymResult("a") == expected_assign_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Wildcard & Synonym") {
		QP::QueryResult result1 = QP::Relationship::ParentT::executeWildcardSynonym(pkb, assign_synonym);
		QP::QueryResult result2 = QP::Relationship::ParentT::executeWildcardSynonym(pkb, while_synonym);

		vector<string> expected_result = {"4"};
		REQUIRE(result1.getSynonymResult("a") == expected_result);
		REQUIRE(!result2.getResult());
	}

	SECTION("Index & Synonym") {
		QP::QueryResult result1 = QP::Relationship::ParentT::executeIndexSynonym(pkb, stmt_no1, stmt_synonym);
		QP::QueryResult result2 = QP::Relationship::ParentT::executeIndexSynonym(pkb, stmt_no3, if_synonym);

		vector<string> expected_result = {"2", "3", "4"};
		vector<string> actual_result = result1.getSynonymResult("s");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
		REQUIRE(!result2.getResult());
	}
};
