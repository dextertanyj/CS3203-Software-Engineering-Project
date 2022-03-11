#include "QP/QueryGraph.h"

#include "QP/ReferenceArgument.h"
#include "QP/Relationship/Relation.h"
#include "catch.hpp"

using namespace QP::Types;

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
		{make_unique<QP::Relationship::Relation>(ClauseType::Parent, vector<ReferenceArgument>({s, a}),
	                                             [](PKB::StorageAccessInterface& pkb) { return QP::QueryResult(); })},
		{make_unique<QP::Relationship::Relation>(ClauseType::UsesS, vector<ReferenceArgument>({a, v}),
	                                             [](PKB::StorageAccessInterface& pkb) { return QP::QueryResult(); })},
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
		{make_unique<QP::Relationship::Relation>(ClauseType::Parent, vector<ReferenceArgument>({a, b}),
	                                             [](PKB::StorageAccessInterface& pkb) { return QP::QueryResult(); })},
		{make_unique<QP::Relationship::Relation>(ClauseType::Parent, vector<ReferenceArgument>({a, c}),
	                                             [](PKB::StorageAccessInterface& pkb) { return QP::QueryResult(); })},
		{make_unique<QP::Relationship::Relation>(ClauseType::UsesS, vector<ReferenceArgument>({d, e}),
	                                             [](PKB::StorageAccessInterface& pkb) { return QP::QueryResult(); })},
	};

	QP::QueryGraph graph = QP::QueryGraph(list);
	graph.setEdges(clause_list);
	unordered_map<string, size_t> synonyms = graph.getSynonymsInGroup("a");

	REQUIRE(synonyms["a"] == 0);
	REQUIRE(synonyms["b"] == 0);
	REQUIRE(synonyms["c"] == 0);
	REQUIRE(synonyms["d"] == 1);
	REQUIRE(synonyms["e"] == 1);
}
