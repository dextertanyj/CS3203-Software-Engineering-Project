#include "QP/QueryGraph.h"

#include "QP/ConnectedSynonyms.h"
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
	Declaration declaration_s = {DesignEntity::Stmt, "s"};
	Declaration declaration_v = {DesignEntity::Variable, "v"};
	Declaration declaration_a = {DesignEntity::Assign, "a"};
	DeclarationList list = {declaration_s, declaration_v, declaration_a};
	ReferenceArgument s = ReferenceArgument(declaration_s);
	ReferenceArgument a = ReferenceArgument(declaration_a);
	ReferenceArgument v = ReferenceArgument(declaration_v);
	ClauseList clause_list = {
		{make_unique<QP::Relationship::Relation>(ClauseType::Parent, vector<ReferenceArgument>({s, a}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
		{make_unique<QP::Relationship::Relation>(ClauseType::UsesS, vector<ReferenceArgument>({a, v}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
	};

	QP::QueryGraph graph = QP::QueryGraph(list);
	graph.setEdges(clause_list);

	unordered_map<string, Node> nodes = graph.getNodes();
	REQUIRE(nodes.at("s").adjacent_symbols.size() == 1);
	REQUIRE(nodes.at("a").adjacent_symbols.size() == 2);
	REQUIRE(nodes.at("v").adjacent_symbols.size() == 1);
}

TEST_CASE("QP::QueryGraph::getSynonymsInGroup Should split synonyms into connected components") {
	Declaration declaration_a = {DesignEntity::Stmt, "a"};
	Declaration declaration_b = {DesignEntity::Assign, "b"};
	Declaration declaration_c = {DesignEntity::If, "c"};
	Declaration declaration_d = {DesignEntity::Assign, "d"};
	Declaration declaration_e = {DesignEntity::Variable, "e"};
	Declaration declaration_f = {DesignEntity::Variable, "f"};
	DeclarationList declaration_list = {
		declaration_a, declaration_b, declaration_c, declaration_d, declaration_e, declaration_f,
	};
	DeclarationList select_list = {
		declaration_a,
		declaration_b,
		declaration_e,
	};
	ReferenceArgument a = ReferenceArgument(declaration_a);
	ReferenceArgument b = ReferenceArgument(declaration_b);
	ReferenceArgument c = ReferenceArgument(declaration_c);
	ReferenceArgument d = ReferenceArgument(declaration_d);
	ReferenceArgument e = ReferenceArgument(declaration_e);
	ReferenceArgument f = ReferenceArgument(declaration_f);
	ReferenceArgument wildcard = ReferenceArgument();
	ClauseList clause_list = {
		{make_unique<QP::Relationship::Relation>(ClauseType::Parent, vector<ReferenceArgument>({a, b}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
		{make_unique<QP::Relationship::Relation>(ClauseType::Parent, vector<ReferenceArgument>({a, c}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
		{make_unique<QP::Relationship::Relation>(ClauseType::UsesS, vector<ReferenceArgument>({d, e}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
		{make_unique<QP::Relationship::Relation>(ClauseType::UsesS, vector<ReferenceArgument>({d, wildcard}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
	};
	QP::QueryGraph graph = QP::QueryGraph(declaration_list);
	graph.setEdges(clause_list);

	ConnectedSynonyms synonyms = graph.getConnectedSynonyms(select_list);

	size_t number_of_groups = synonyms.getNumberOfGroups();
	size_t group_with_a = synonyms.getGroupNumber("a");
	size_t group_with_d = synonyms.getGroupNumber("d");
	size_t group_with_f = synonyms.getGroupNumber("f");
	REQUIRE(number_of_groups == 3);
	REQUIRE(synonyms.getGroupNumber("b") == group_with_a);
	REQUIRE(synonyms.getGroupNumber("c") == group_with_a);
	REQUIRE(synonyms.getGroupNumber("e") == group_with_d);
	REQUIRE(group_with_d != group_with_a);
	REQUIRE(group_with_f != group_with_a);
	REQUIRE(group_with_f != group_with_d);
	REQUIRE(synonyms.getGroupSynonyms(group_with_a).size() == 2);
	REQUIRE(synonyms.getGroupSynonyms(group_with_d).size() == 1);
	REQUIRE(synonyms.getGroupSynonyms(group_with_f).empty());
}

TEST_CASE("QP::QueryGraph::sortGroup Should sort clauses in group") {
	Declaration declaration_s = {DesignEntity::Stmt, "s"};
	Declaration declaration_v = {DesignEntity::Variable, "v"};
	Declaration declaration_a = {DesignEntity::Assign, "a"};
	Declaration declaration_s1 = {DesignEntity::Stmt, "s1"};
	ReferenceArgument s = ReferenceArgument(declaration_s);
	ReferenceArgument a = ReferenceArgument(declaration_a);
	ReferenceArgument v = ReferenceArgument(declaration_v);
	ReferenceArgument s1 = ReferenceArgument(declaration_s1);
	ReferenceArgument wildcard = ReferenceArgument();
	ReferenceArgument stmt_no1 = ReferenceArgument(1);
	DeclarationList list = {declaration_s, declaration_v, declaration_a, declaration_s1};
	ClauseList clause_list = {
		{make_unique<QP::Relationship::Relation>(ClauseType::Affects, vector<ReferenceArgument>({s, a}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
		{make_unique<QP::Relationship::Relation>(ClauseType::UsesS, vector<ReferenceArgument>({a, v}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
		{make_unique<QP::Relationship::Relation>(ClauseType::Parent, vector<ReferenceArgument>({a, wildcard}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
		{make_unique<QP::Relationship::Relation>(ClauseType::Next, vector<ReferenceArgument>({stmt_no1, a}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
		{make_unique<QP::Relationship::Relation>(ClauseType::Follows, vector<ReferenceArgument>({s1, s}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
		{make_unique<QP::Relationship::Relation>(ClauseType::NextT, vector<ReferenceArgument>({s1, s}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
	};

	QP::QueryGraph graph = QP::QueryGraph(list);
	graph.setEdges(clause_list);
	graph.getConnectedSynonyms({});

	ClauseList sorted_clauses = graph.sortGroup(0);

	REQUIRE(sorted_clauses.size() == 6);
	REQUIRE(sorted_clauses[0].relation->getType() == ClauseType::Parent);
	REQUIRE(sorted_clauses[1].relation->getType() == ClauseType::UsesS);
	REQUIRE(sorted_clauses[2].relation->getType() == ClauseType::Next);
	REQUIRE(sorted_clauses[3].relation->getType() == ClauseType::Affects);
	REQUIRE(sorted_clauses[4].relation->getType() == ClauseType::Follows);
	REQUIRE(sorted_clauses[5].relation->getType() == ClauseType::NextT);
}
