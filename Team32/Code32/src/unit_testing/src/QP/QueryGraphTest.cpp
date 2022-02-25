#include "QP/QueryGraph.h"

#include "QP/Relationship/Parent.h"
#include "QP/Relationship/UsesS.h"
#include "catch.hpp"

TEST_CASE("QP::QueryGraph::QueryGraph Should initialize nodes") {
	DeclarationList list = {{DesignEntity::Stmt, "s"}, {DesignEntity::Variable, "v"}};
	QP::QueryGraph graph = QP::QueryGraph(list);
	unordered_map<string, Node> nodes = graph.getNodes();
	REQUIRE(nodes.size() == 2);
}

TEST_CASE("QP::QueryGraph::setEdges Should set edges") {
	DeclarationList list = {{DesignEntity::Stmt, "s"}, {DesignEntity::Variable, "v"}, {DesignEntity::Assign, "a"}};
	QueryStmtRef s = {StmtRefType::Synonym, "s"};
	QueryStmtRef a = {StmtRefType::Synonym, "a"};
	QueryEntRef v = {EntRefType::Synonym, "v"};
	SuchThatClauseList such_that_list = {
		{make_unique<QP::Relationship::Parent>(s, a)},
		{make_unique<QP::Relationship::UsesS>(a, v)},
	};

	QP::QueryGraph graph = QP::QueryGraph(list);
	graph.setEdges(such_that_list, {});

	unordered_map<string, Node> nodes = graph.getNodes();
	REQUIRE(nodes.at("s").adjacent_symbols.size() == 1);
	REQUIRE(nodes.at("a").adjacent_symbols.size() == 2);
	REQUIRE(nodes.at("v").adjacent_symbols.size() == 1);
}

TEST_CASE("QP::QueryGraph::getSynonymsInGroup Should split synonyms into connected components") {
	DeclarationList list = {
		{DesignEntity::Stmt, "a"},   {DesignEntity::Assign, "b"},   {DesignEntity::If, "c"},
		{DesignEntity::Assign, "d"}, {DesignEntity::Variable, "e"},
	};
	QueryStmtRef a = {StmtRefType::Synonym, "a"};
	QueryStmtRef b = {StmtRefType::Synonym, "b"};
	QueryStmtRef c = {StmtRefType::Synonym, "c"};
	QueryStmtRef d = {StmtRefType::Synonym, "d"};
	QueryEntRef e = {EntRefType::Synonym, "e"};
	SuchThatClauseList such_that_list = {
		{make_unique<QP::Relationship::Parent>(a, b)},
		{make_unique<QP::Relationship::Parent>(a, c)},
		{make_unique<QP::Relationship::UsesS>(d, e)},
	};

	QP::QueryGraph graph = QP::QueryGraph(list);
	graph.setEdges(such_that_list, {});
	vector<unordered_set<string>> synonyms = graph.getSynonymsInGroup("a");

	unordered_set<string> expected_first_group = {"a", "b", "c"};
	unordered_set<string> expected_second_group = {"d", "e"};
	REQUIRE(synonyms.size() == 2);
	REQUIRE(synonyms[0] == expected_first_group);
	REQUIRE(synonyms[1] == expected_second_group);
}
