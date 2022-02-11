#include "QP/QueryGraph.h"
#include "QP/Relationship/Parent.h"

#include "catch.hpp"

TEST_CASE("QP::QueryGraph::QueryGraph Should initialize nodes") {
	DeclarationList list = { { DesignEntity::stmt, "s" }, { DesignEntity::variable, "v" } };
	QueryGraph graph = QueryGraph(list);
	unordered_map<string, Node> nodes = graph.getNodes();
	REQUIRE(nodes.size() == 2);
}

TEST_CASE("QP::QueryGraph::QueryGraph Should set edges") {
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
