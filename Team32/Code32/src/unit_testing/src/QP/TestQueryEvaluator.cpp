#include "QP/QueryEvaluator.h"
#include "QP/Relationship/Parent.h"

#include "catch.hpp"

TEST_CASE("QP::QueryEvaluator::splitClauses Should split clauses into groups") {
	DeclarationList declarations = {
		{ DesignEntity::stmt, "s1" },
		{ DesignEntity::stmt, "s2" },
		{ DesignEntity::variable, "v" },
		{ DesignEntity::assign, "a" },
		{ DesignEntity::if_, "i" },
	};
	Declaration select = { DesignEntity::stmt, "s1" };
	SuchThatClauseList suchThatClauses = {
		{new Parent(false, { StmtRefType::synonym, "s1"}, { StmtRefType::synonym, "s2"})},
		{new Parent(false, { StmtRefType::synonym, "a"}, { StmtRefType::synonym, "i"})},
		{new Parent(false, { StmtRefType::stmtNumber, "1"}, { StmtRefType::stmtNumber, "2"})},
	};
	PatternClauseList patternClauses = {
		{{ DesignEntity::assign, "a" }, { EntRefType::synonym, "v"}, "x"},
	};
	QueryProperties properties = QueryProperties(declarations, select, suchThatClauses, patternClauses);
	vector<unordered_set<string>> synonymsInGroup = {
		{"s1", "s2"},
		{"a", "v", "i"},
	};

	PKB pkb = PKB();
	vector<pair<SuchThatClauseList, PatternClauseList>> clausesInGroup = 
		QueryEvaluator(pkb).splitClauses(properties, synonymsInGroup);

	REQUIRE(clausesInGroup.size() == 3);
	REQUIRE(clausesInGroup[0].first.size() == 1);
	REQUIRE(clausesInGroup[0].second.size() == 0);
	REQUIRE(clausesInGroup[1].first.size() == 1);
	REQUIRE(clausesInGroup[1].second.size() == 1);
	REQUIRE(clausesInGroup[2].first.size() == 1);
	REQUIRE(clausesInGroup[2].second.size() == 0);
}
