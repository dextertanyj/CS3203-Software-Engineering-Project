#include "QP/QueryGraph.h"
#include "QP/Relationship/Parent.h"
#include "QP/Relationship/UsesS.h"

#include "catch.hpp"

TEST_CASE("QP::QueryGraph::QueryGraph Should initialize nodes") {
	DeclarationList list = { { DesignEntity::Stmt, "s" }, { DesignEntity::Variable, "v" } };
	QueryGraph graph = QueryGraph(list);
	unordered_map<string, Node> nodes = graph.getNodes();
	REQUIRE(nodes.size() == 2);
}

TEST_CASE("QP::QueryGraph::setEdges Should set edges") {
	DeclarationList list = { { DesignEntity::Stmt, "s" }, { DesignEntity::Variable, "v" }, { DesignEntity::Assign, "a" } };
	QueryStmtRef s = { StmtRefType::Synonym, "s" };
	QueryStmtRef a = { StmtRefType::Synonym, "a" };
	QueryEntRef v = { EntRefType::Synonym, "v" };
	SuchThatClauseList suchThatList = {
		{ make_unique<Parent>(s, a) },
		{ make_unique<UsesS>(a, v) },
	};

	QueryGraph graph = QueryGraph(list);
	graph.setEdges(suchThatList, {});

	unordered_map<string, Node> nodes = graph.getNodes();
	REQUIRE(nodes.at("s").adjacent_symbols.size() == 1);
	REQUIRE(nodes.at("a").adjacent_symbols.size() == 2);
	REQUIRE(nodes.at("v").adjacent_symbols.size() == 1);
}

TEST_CASE("QP::QueryGraph::getSynonymsInGroup Should split synonyms into connected components") {
	DeclarationList list = {
		{ DesignEntity::Stmt, "a" },
		{ DesignEntity::Assign, "b" },
		{ DesignEntity::If, "c" },
		{ DesignEntity::Assign, "d" },
		{ DesignEntity::Variable, "e" },
	};
	QueryStmtRef a = { StmtRefType::Synonym, "a" };
	QueryStmtRef b = { StmtRefType::Synonym, "b" };
	QueryStmtRef c = { StmtRefType::Synonym, "c" };
	QueryStmtRef d = { StmtRefType::Synonym, "d" };
	QueryEntRef e = { EntRefType::Synonym, "e" };
	SuchThatClauseList suchThatList = {
		{ make_unique<Parent>(a, b) },
		{ make_unique<Parent>(a, c) },
		{ make_unique<UsesS>(d, e)},
	};

	QueryGraph graph = QueryGraph(list);
	graph.setEdges(suchThatList, {});
	vector<unordered_set<string>> synonyms = graph.getSynonymsInGroup("a");
	
	unordered_set<string> expectedFirstGroup = {"a", "b", "c"};
	unordered_set<string> expectedSecondGroup = {"d", "e"};
	REQUIRE(synonyms.size() == 2);
	REQUIRE(synonyms[0] == expectedFirstGroup);
	REQUIRE(synonyms[1] == expectedSecondGroup);
}
