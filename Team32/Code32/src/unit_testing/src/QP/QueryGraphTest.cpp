#include "QP/QueryGraph.h"

#include "QP/Relationship/Parent.h"
#include "QP/Relationship/UsesS.h"
#include "QP/ReferenceArgument.h"
#include "catch.hpp"

TEST_CASE("QP::QueryGraph::QueryGraph Should initialize nodes") {
	DeclarationList list = {{DesignEntity::Stmt, "s"}, {DesignEntity::Variable, "v"}};
	QP::QueryGraph graph = QP::QueryGraph(list);
	unordered_map<string, Node> nodes = graph.getNodes();
	REQUIRE(nodes.size() == 2);
}

TEST_CASE("QP::QueryGraph::setEdges Should set edges") {
	DeclarationList list = {{DesignEntity::Stmt, "s"}, {DesignEntity::Variable, "v"}, {DesignEntity::Assign, "a"}};
	ReferenceArgument s = ReferenceArgument({DesignEntity::Stmt, "s"});
	ReferenceArgument a = ReferenceArgument({DesignEntity::Assign, "a"});
	ReferenceArgument v = ReferenceArgument({DesignEntity::Variable, "v"});
	ClauseList clause_list = {
		{make_unique<QP::Relationship::Parent>(s, a)},
		{make_unique<QP::Relationship::UsesS>(a, v)},
	};

	QP::QueryGraph graph = QP::QueryGraph(list);
	graph.setEdges(clause_list);

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
	ReferenceArgument a = ReferenceArgument({DesignEntity::Stmt, "a"});
	ReferenceArgument b = ReferenceArgument({DesignEntity::Assign, "b"});
	ReferenceArgument c = ReferenceArgument({DesignEntity::If, "c"});
	ReferenceArgument d = ReferenceArgument({DesignEntity::Assign, "d"});
	ReferenceArgument e = ReferenceArgument({DesignEntity::Variable, "e"});
	ClauseList clause_list = {
		{make_unique<QP::Relationship::Parent>(a, b)},
		{make_unique<QP::Relationship::Parent>(a, c)},
		{make_unique<QP::Relationship::UsesS>(d, e)},
	};

	QP::QueryGraph graph = QP::QueryGraph(list);
	graph.setEdges(clause_list);
	ConnectedSynonyms synonyms = graph.getConnectedSynonyms({{DesignEntity::Stmt, "a"}});

	size_t groupWithA = synonyms.synonyms_in_group["a"];
	size_t groupWithD = synonyms.synonyms_in_group["d"];
	REQUIRE(synonyms.number_of_groups == 2);
	REQUIRE(synonyms.synonyms_in_group["a"] == groupWithA);
	REQUIRE(synonyms.synonyms_in_group["b"] == groupWithA);
	REQUIRE(synonyms.synonyms_in_group["c"] == groupWithA);
	REQUIRE(synonyms.synonyms_in_group["d"] == groupWithD);
	REQUIRE(synonyms.synonyms_in_group["e"] == groupWithD);
	REQUIRE(synonyms.group_to_selected_declarations[0].size() == 1);
	REQUIRE(synonyms.group_to_selected_declarations[1].empty());
}
