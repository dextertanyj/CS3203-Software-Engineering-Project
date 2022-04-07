#include "QP/Optimizer/QueryGraph.h"

#include "QP/Optimizer/ClauseGroups.h"
#include "QP/ClauseArgument.h"
#include "QP/Evaluator/Clause.h"
#include "catch.hpp"

using namespace QP;
using namespace Evaluator;
using namespace Optimizer;
using namespace Types;

TEST_CASE("QueryGraph::QueryGraph Should initialize nodes") {
	DeclarationList list = {{DesignEntity::Stmt, "s"}, {DesignEntity::Variable, "v"}};
	QueryGraph graph = QueryGraph(list, {}, {});
	unordered_map<string, QueryGraph::Node> nodes = graph.getNodes();
	REQUIRE(nodes.size() == 2);
}

TEST_CASE("QueryGraph::setEdges Should set edges") {
	Declaration declaration_s = {DesignEntity::Stmt, "s"};
	Declaration declaration_v = {DesignEntity::Variable, "v"};
	Declaration declaration_a = {DesignEntity::Assign, "a"};
	DeclarationList list = {declaration_s, declaration_v, declaration_a};
	ClauseArgument s = ClauseArgument(declaration_s);
	ClauseArgument a = ClauseArgument(declaration_a);
	ClauseArgument v = ClauseArgument(declaration_v);
	ClauseList clause_list = {
		{make_unique<Clause>(ClauseType::Parent, vector<ClauseArgument>({s, a}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
		{make_unique<Clause>(ClauseType::UsesS, vector<ClauseArgument>({a, v}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
	};

	QueryGraph graph = QueryGraph(list, clause_list, {});

	unordered_map<string, QueryGraph::Node> nodes = graph.getNodes();
	REQUIRE(nodes.at("s").adjacent_symbols.size() == 1);
	REQUIRE(nodes.at("a").adjacent_symbols.size() == 2);
	REQUIRE(nodes.at("v").adjacent_symbols.size() == 1);
}

TEST_CASE("QueryGraph::getSynonymsInGroup Should split synonyms into connected components") {
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
	ClauseArgument a = ClauseArgument(declaration_a);
	ClauseArgument b = ClauseArgument(declaration_b);
	ClauseArgument c = ClauseArgument(declaration_c);
	ClauseArgument d = ClauseArgument(declaration_d);
	ClauseArgument e = ClauseArgument(declaration_e);
	ClauseArgument f = ClauseArgument(declaration_f);
	ClauseArgument wildcard = ClauseArgument();
	ClauseList clause_list = {
		{make_unique<Clause>(ClauseType::Parent, vector<ClauseArgument>({a, b}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
		{make_unique<Clause>(ClauseType::Parent, vector<ClauseArgument>({a, c}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
		{make_unique<Clause>(ClauseType::UsesS, vector<ClauseArgument>({d, e}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
		{make_unique<Clause>(ClauseType::UsesS, vector<ClauseArgument>({d, wildcard}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
	};
	QueryGraph graph = QueryGraph(declaration_list, clause_list, select_list);

	size_t number_of_groups = graph.getNumberOfGroups();

	vector<string> group_with_a = {"a", "b", "c"};
	vector<string> group_with_d = {"d", "e"};
	vector<string> group_with_f = {"f"};
	vector<string> group_one = graph.getGroupSynonyms(0);
	vector<string> group_two = graph.getGroupSynonyms(1);
	vector<string> group_three = graph.getGroupSynonyms(2);
	sort(group_one.begin(), group_one.end());
	sort(group_two.begin(), group_two.end());
	sort(group_three.begin(), group_three.end());
	REQUIRE(number_of_groups == 3);
	REQUIRE(group_one == group_with_f);
	REQUIRE(group_two == group_with_d);
	REQUIRE(group_three == group_with_a);
}

TEST_CASE("QueryGraph::getGroupClauses Should sort clauses in group") {
	Declaration declaration_s = {DesignEntity::Stmt, "s"};
	Declaration declaration_v = {DesignEntity::Variable, "v"};
	Declaration declaration_a = {DesignEntity::Assign, "a"};
	Declaration declaration_s1 = {DesignEntity::Stmt, "s1"};
	ClauseArgument s = ClauseArgument(declaration_s);
	ClauseArgument a = ClauseArgument(declaration_a);
	ClauseArgument v = ClauseArgument(declaration_v);
	ClauseArgument s1 = ClauseArgument(declaration_s1);
	ClauseArgument wildcard = ClauseArgument();
	ClauseArgument stmt_no1 = ClauseArgument(1);
	DeclarationList list = {declaration_s, declaration_v, declaration_a, declaration_s1};
	ClauseList clause_list = {
		{make_unique<Clause>(ClauseType::Affects, vector<ClauseArgument>({s, a}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
		{make_unique<Clause>(ClauseType::UsesS, vector<ClauseArgument>({a, v}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
		{make_unique<Clause>(ClauseType::Parent, vector<ClauseArgument>({a, wildcard}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
		{make_unique<Clause>(ClauseType::Next, vector<ClauseArgument>({stmt_no1, a}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
		{make_unique<Clause>(ClauseType::AffectsT, vector<ClauseArgument>({s1, s}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
		{make_unique<Clause>(ClauseType::NextT, vector<ClauseArgument>({s1, s}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
		{make_unique<Clause>(ClauseType::NextT, vector<ClauseArgument>({s1, s1}),
	                                             [](const QP::StorageAdapter& store) { return QP::QueryResult(); })},
	};

	QueryGraph graph = QueryGraph(list, clause_list, {});

	ClauseList sorted_clauses = graph.getGroupClauses(0);

	REQUIRE(sorted_clauses.size() == 7);
	REQUIRE(sorted_clauses[0]->getType() == ClauseType::Parent);
	REQUIRE(sorted_clauses[1]->getType() == ClauseType::UsesS);
	REQUIRE(sorted_clauses[2]->getType() == ClauseType::Next);
	REQUIRE(sorted_clauses[3]->getType() == ClauseType::Affects);
	REQUIRE(sorted_clauses[4]->getType() == ClauseType::NextT);
	REQUIRE(sorted_clauses[5]->getType() == ClauseType::NextT);
	REQUIRE(sorted_clauses[6]->getType() == ClauseType::AffectsT);
}
