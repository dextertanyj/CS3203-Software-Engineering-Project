#include "QP/QueryGraph.h"
#include "QP/Relationship/Parent.h"

#include "catch.hpp"

TEST_CASE("QP::QueryGraph::QueryGraph Should initialize nodes") {
	DeclarationList list = { { DesignEntity::stmt, "s" }, { DesignEntity::variable, "v" } };
	QueryGraph graph = QueryGraph(list);
	unordered_map<string, Node> nodes = graph.getNodes();
	REQUIRE(nodes.size() == 2);
}

TEST_CASE("QP::QueryGraph::setEdges Should set edges") {
	DeclarationList list = { { DesignEntity::stmt, "s" }, { DesignEntity::variable, "v" }, { DesignEntity::assign, "a" } };
	QueryStmtRef s = { StmtRefType::synonym, "s" };
	QueryStmtRef a = { StmtRefType::synonym, "a" };
	SuchThatClauseList suchThatList = { {make_unique<Parent>(false, s, a)} };
	PatternClauseList patternList = { {{ DesignEntity::assign, "a" }, { EntRefType::synonym, "v"}, "x"} };

	QueryGraph graph = QueryGraph(list);
	graph.setEdges(suchThatList, patternList);

	unordered_map<string, Node> nodes = graph.getNodes();
	REQUIRE(nodes.at("s").adjacentSymbols.size() == 1);
	REQUIRE(nodes.at("a").adjacentSymbols.size() == 2);
	REQUIRE(nodes.at("v").adjacentSymbols.size() == 1);
}

TEST_CASE("QP::QueryGraph::getSynonymsInGroup Should split synonyms into connected components") {
	DeclarationList list = {
		{ DesignEntity::stmt, "a" },
		{ DesignEntity::assign, "b" },
		{ DesignEntity::if_, "c" },
		{ DesignEntity::assign, "d" },
		{ DesignEntity::variable, "e" },
	};
	QueryStmtRef a = { StmtRefType::synonym, "a" };
	QueryStmtRef b = { StmtRefType::synonym, "b" };
	QueryStmtRef c = { StmtRefType::synonym, "c" };
	SuchThatClauseList suchThatList = {
		{ make_unique<Parent>(false, a, b) },
		{ make_unique<Parent>(false, a, c) }
	};
	PatternClauseList patternList = { { { DesignEntity::assign, "d" }, { EntRefType::synonym, "e" }, "x" } };

	QueryGraph graph = QueryGraph(list);
	graph.setEdges(suchThatList, patternList);
	vector<unordered_set<string>> synonyms = graph.getSynonymsInGroup("a");
	
	unordered_set<string> expectedFirstGroup = {"a", "b", "c"};
	unordered_set<string> expectedSecondGroup = {"d", "e"};
	REQUIRE(synonyms.size() == 2);
	REQUIRE(synonyms[0] == expectedFirstGroup);
	REQUIRE(synonyms[1] == expectedSecondGroup);
}
