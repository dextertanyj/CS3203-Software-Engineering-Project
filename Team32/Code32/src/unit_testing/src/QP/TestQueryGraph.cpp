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
	SuchThatClauseList suchThatList = { {new Parent(false, { StmtRefType::synonym, "s"}, { StmtRefType::synonym, "a"})} };
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
	SuchThatClauseList suchThatList = {
		{ new Parent(false, { StmtRefType::synonym, "a"}, { StmtRefType::synonym, "b"}) },
		{ new Parent(false, { StmtRefType::synonym, "a"}, { StmtRefType::synonym, "c"}) },
	};
	PatternClauseList patternList = { {{ DesignEntity::assign, "d" }, { EntRefType::synonym, "e"}, "x"} };

	QueryGraph graph = QueryGraph(list);
	graph.setEdges(suchThatList, patternList);
	vector<unordered_set<string>> synonyms = graph.getSynonymsInGroup("a");
	
	unordered_set<string> expectedFirstGroup = {"a", "b", "c"};
	unordered_set<string> expectedSecondGroup = {"d", "e"};
	REQUIRE(synonyms.size() == 2);
	REQUIRE(synonyms[0] == expectedFirstGroup);
	REQUIRE(synonyms[1] == expectedSecondGroup);
}
