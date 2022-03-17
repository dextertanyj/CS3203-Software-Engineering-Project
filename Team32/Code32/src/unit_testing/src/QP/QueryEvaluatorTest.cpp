#include "QP/QueryEvaluator.h"

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/TypeDefs.h"
#include "QP/Dispatcher/DispatchMap.h"
#include "QP/QueryExpressionLexer.h"
#include "QP/ReferenceArgument.h"
#include "catch.hpp"

using namespace QP::Types;

TEST_CASE("QP::QueryEvaluator::splitClauses Should split clauses into groups") {
	QP::Dispatcher::DispatchMap dispatcher;
	Declaration assign_declaration = {DesignEntity::Assign, "a"};
	Declaration var_declaration = {DesignEntity::Variable, "v"};
	Declaration stmt1_declaration = {DesignEntity::Stmt, "s1"};
	Declaration stmt2_declaration = {DesignEntity::Stmt, "s2"};
	Declaration if_declaration = {DesignEntity::If, "i"};
	Declaration proc_declaration = {DesignEntity::Procedure, "p"};
	DeclarationList declarations = {
		assign_declaration, var_declaration, stmt1_declaration, stmt2_declaration, if_declaration, proc_declaration,
	};
	DeclarationList select_list = {stmt1_declaration, proc_declaration};
	DeclarationList select_a = {assign_declaration};
	DeclarationList select_p = {proc_declaration};
	ReferenceArgument s1 = ReferenceArgument(stmt1_declaration);
	ReferenceArgument s2 = ReferenceArgument(stmt2_declaration);
	ReferenceArgument a = ReferenceArgument(assign_declaration);
	ReferenceArgument i = ReferenceArgument(if_declaration);
	ReferenceArgument v = ReferenceArgument(var_declaration);
	ReferenceArgument p = ReferenceArgument(proc_declaration);
	ReferenceArgument stmt_no1 = ReferenceArgument(1);
	ReferenceArgument stmt_no2 = ReferenceArgument(2);
	ReferenceArgument wildcard = ReferenceArgument();
	ClauseList clauses = {
		{make_unique<QP::Relationship::Relation>(ClauseType::Parent, vector<ReferenceArgument>({s1, s2}),
	                                             dispatcher.dispatch_map.at(ClauseType::Parent)({s1, s2}).second)},
		{make_unique<QP::Relationship::Relation>(ClauseType::Follows, vector<ReferenceArgument>({a, i}),
	                                             dispatcher.dispatch_map.at(ClauseType::Follows)({a, i}).second)},
		{make_unique<QP::Relationship::Relation>(ClauseType::Parent, vector<ReferenceArgument>({stmt_no1, stmt_no2}),
	                                             dispatcher.dispatch_map.at(ClauseType::Parent)({stmt_no1, stmt_no2}).second)},
		{make_unique<QP::Relationship::Relation>(ClauseType::ModifiesS, vector<ReferenceArgument>({a, v}),
	                                             dispatcher.dispatch_map.at(ClauseType::UnknownModifies)({a, v}).second)},
		{make_unique<QP::Relationship::Relation>(ClauseType::Calls, vector<ReferenceArgument>({p, wildcard}),
	                                             dispatcher.dispatch_map.at(ClauseType::Calls)({p, wildcard}).second)},
	};
	QP::QueryProperties properties = QP::QueryProperties(declarations, select_list, clauses);
	ConnectedSynonyms connected_synonyms = {
		3,
		{{"s1", 0}, {"s2", 0}, {"a", 1}, {"v", 1}, {"i", 1}, {"p", 2}},
		{{0, select_a}, {1, {}}, {2, select_p}},
	};

	PKB::Storage pkb = PKB::Storage();
	vector<pair<ClauseList, DeclarationList>> clauses_in_group = QP::QueryEvaluator::splitClauses(properties, connected_synonyms);

	REQUIRE(clauses_in_group.size() == 4);
	REQUIRE(clauses_in_group[0].first.size() == 1);
	REQUIRE(clauses_in_group[1].first.size() == 2);
	REQUIRE(clauses_in_group[2].first.size() == 1);
	REQUIRE(clauses_in_group[3].first.size() == 1);
	REQUIRE(clauses_in_group[0].second.size() == 1);
	REQUIRE(clauses_in_group[1].second.empty());
	REQUIRE(clauses_in_group[2].second.size() == 1);
	REQUIRE(clauses_in_group[3].second.empty());
};

TEST_CASE("QP::QueryEvaluator::execute") {
	QP::Dispatcher::DispatchMap dispatcher;
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::Assign);
	pkb.setStmtType(4, StmtType::IfStmt);
	pkb.setFollows(1, 2);
	pkb.setFollows(3, 4);
	pkb.setModifies(1, "x");
	pkb.setProc("proc1", 1, 2);
	pkb.setProc("proc2", 3, 4);

	unordered_set<ConstVal> constants = {1};
	pkb.setConstant(constants);

	QP::QueryEvaluator evaluator = QP::QueryEvaluator(pkb);

	Declaration assign_declaration = {DesignEntity::Assign, "a"};
	Declaration var_declaration = {DesignEntity::Variable, "v"};
	Declaration stmt1_declaration = {DesignEntity::Stmt, "s1"};
	Declaration stmt2_declaration = {DesignEntity::Stmt, "s2"};
	Declaration proc_declaration = {DesignEntity::Procedure, "p"};
	DeclarationList declarations = {
		assign_declaration, var_declaration, stmt1_declaration, stmt2_declaration, proc_declaration,
	};
	ReferenceArgument assign_syn = ReferenceArgument(assign_declaration);
	ReferenceArgument var_syn = ReferenceArgument(var_declaration);
	ReferenceArgument stmt1_syn = ReferenceArgument(stmt1_declaration);
	ReferenceArgument stmt_no1 = ReferenceArgument(1);
	ReferenceArgument wildcard = ReferenceArgument();

	vector<string> assign_token = {"x", "+", "1"};
	QP::QueryExpressionLexer lexer = QP::QueryExpressionLexer(assign_token);
	auto expression = Common::ExpressionProcessor::Expression::parse(lexer, Common::ExpressionProcessor::ExpressionType::Arithmetic);
	pkb.setAssign(1, "x", expression);

	vector<string> token = {"1"};
	QP::QueryExpressionLexer query_lexer = QP::QueryExpressionLexer(token);
	auto query_expression =
		Common::ExpressionProcessor::Expression::parse(query_lexer, Common::ExpressionProcessor::ExpressionType::Arithmetic);

	SECTION("No clause, select assign statement") {
		QP::QueryProperties properties = QP::QueryProperties(declarations, {assign_declaration}, {});
		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"1", "3"};
		vector<string> actual_result = result.getSynonymResult("a");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
	};

	SECTION("No clause, select variable") {
		QP::QueryProperties properties = QP::QueryProperties(declarations, {var_declaration}, {});
		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"x"};
		vector<string> actual_result = result.getSynonymResult("v");
		REQUIRE(actual_result == expected_result);
	};

	SECTION("No clause, select procedure") {
		QP::QueryProperties properties = QP::QueryProperties(declarations, {proc_declaration}, {});
		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"proc1", "proc2"};
		vector<string> actual_result = result.getSynonymResult("p");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
	};

	SECTION("One non-trivial such that clause") {
		ClauseList clauses = {{make_unique<QP::Relationship::Relation>(
			ClauseType::ModifiesS, vector<ReferenceArgument>({assign_syn, var_syn}),
			dispatcher.dispatch_map.at(ClauseType::UnknownModifies)({assign_syn, var_syn}).second)}};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {assign_declaration}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"1"};
		vector<string> actual_result = result.getSynonymResult("a");
		REQUIRE(actual_result == expected_result);
	};

	SECTION("One trivial such that clause") {
		ClauseList clauses = {
			{make_unique<QP::Relationship::Relation>(ClauseType::ModifiesS, vector<ReferenceArgument>({stmt_no1, var_syn}),
		                                             dispatcher.dispatch_map.at(ClauseType::UnknownModifies)({stmt_no1, var_syn}).second)}};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {assign_declaration}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"1", "3"};
		vector<string> actual_result = result.getSynonymResult("a");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
	};

	SECTION("One trivial pattern clause") {
		ClauseList clauses = {{make_unique<QP::Relationship::Relation>(
			ClauseType::PatternAssign, vector<ReferenceArgument>({assign_syn, wildcard, ReferenceArgument(query_expression, false)}),
			dispatcher.dispatch_map.at(ClauseType::PatternAssign)({assign_syn, wildcard, ReferenceArgument(query_expression, false)})
				.second)}};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {var_declaration}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"x"};
		vector<string> actual_result = result.getSynonymResult("v");
		REQUIRE(actual_result == expected_result);
	};

	SECTION("One non-trivial pattern clause") {
		ClauseList clauses = {{make_unique<QP::Relationship::Relation>(
			ClauseType::PatternAssign, vector<ReferenceArgument>({assign_syn, var_syn, ReferenceArgument(query_expression, false)}),
			dispatcher.dispatch_map.at(ClauseType::PatternAssign)({assign_syn, var_syn, ReferenceArgument(query_expression, false)})
				.second)}};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {var_declaration}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"x"};
		vector<string> actual_result = result.getSynonymResult("v");
		REQUIRE(actual_result == expected_result);
	};

	SECTION("Trivial pattern clause and trival such that clause") {
		ClauseList clauses = {
			{make_unique<QP::Relationship::Relation>(ClauseType::ModifiesS, vector<ReferenceArgument>({stmt_no1, var_syn}),
		                                             dispatcher.dispatch_map.at(ClauseType::UnknownModifies)({stmt_no1, var_syn}).second)},
			{make_unique<QP::Relationship::Relation>(
				ClauseType::PatternAssign, vector<ReferenceArgument>({assign_syn, var_syn, ReferenceArgument(query_expression, false)}),
				dispatcher.dispatch_map.at(ClauseType::PatternAssign)({assign_syn, var_syn, ReferenceArgument(query_expression, false)})
					.second)},
		};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {stmt1_declaration}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"1", "2", "3", "4"};
		vector<string> actual_result = result.getSynonymResult("s1");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
	};

	SECTION("Non-trivial pattern clause and such that clause") {
		ClauseList clauses = {
			{make_unique<QP::Relationship::Relation>(ClauseType::ModifiesS, vector<ReferenceArgument>({stmt_no1, var_syn}),
		                                             dispatcher.dispatch_map.at(ClauseType::UnknownModifies)({stmt_no1, var_syn}).second)},
			{make_unique<QP::Relationship::Relation>(
				ClauseType::PatternAssign, vector<ReferenceArgument>({assign_syn, var_syn, ReferenceArgument(query_expression, false)}),
				dispatcher.dispatch_map.at(ClauseType::PatternAssign)({assign_syn, var_syn, ReferenceArgument(query_expression, false)})
					.second)},
		};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {var_declaration}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"x"};
		vector<string> actual_result = result.getSynonymResult("v");
		REQUIRE(actual_result == expected_result);
	};

	SECTION("Multiple clauses") {
		ClauseList clauses = {
			{make_unique<QP::Relationship::Relation>(ClauseType::ModifiesS, vector<ReferenceArgument>({stmt_no1, var_syn}),
		                                             dispatcher.dispatch_map.at(ClauseType::UnknownModifies)({stmt_no1, var_syn}).second)},
			{make_unique<QP::Relationship::Relation>(ClauseType::Follows, vector<ReferenceArgument>({assign_syn, wildcard}),
		                                             dispatcher.dispatch_map.at(ClauseType::Follows)({assign_syn, wildcard}).second)},
			{make_unique<QP::Relationship::Relation>(
				ClauseType::PatternAssign, vector<ReferenceArgument>({assign_syn, var_syn, ReferenceArgument(query_expression, false)}),
				dispatcher.dispatch_map.at(ClauseType::PatternAssign)({assign_syn, var_syn, ReferenceArgument(query_expression, false)})
					.second)},
		};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {var_declaration}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"x"};
		vector<string> actual_result = result.getSynonymResult("v");
		REQUIRE(actual_result == expected_result);
	};

	SECTION("Select boolean no clause") {
		QP::QueryProperties properties = QP::QueryProperties({}, {}, {});

		QP::QueryResult result = evaluator.executeQuery(properties);

		REQUIRE(result.getResult());
	};

	SECTION("Select boolean positive test") {
		ClauseList clauses = {
			{make_unique<QP::Relationship::Relation>(ClauseType::ModifiesS, vector<ReferenceArgument>({stmt_no1, var_syn}),
		                                             dispatcher.dispatch_map.at(ClauseType::UnknownModifies)({stmt_no1, var_syn}).second)},
			{make_unique<QP::Relationship::Relation>(ClauseType::Follows, vector<ReferenceArgument>({assign_syn, wildcard}),
		                                             dispatcher.dispatch_map.at(ClauseType::Follows)({assign_syn, wildcard}).second)},
		};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		REQUIRE(result.getResult());
	};

	SECTION("Select boolean negative test") {
		ClauseList clauses = {
			{make_unique<QP::Relationship::Relation>(ClauseType::Follows, vector<ReferenceArgument>({wildcard, stmt_no1}),
		                                             dispatcher.dispatch_map.at(ClauseType::Follows)({wildcard, stmt_no1}).second)},
			{make_unique<QP::Relationship::Relation>(ClauseType::Follows, vector<ReferenceArgument>({wildcard, stmt1_syn}),
		                                             dispatcher.dispatch_map.at(ClauseType::Follows)({wildcard, stmt1_syn}).second)},
		};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		REQUIRE(!result.getResult());
	};

	SECTION("Select tuple, no clause") {
		DeclarationList select_list = {
			{DesignEntity::Stmt, "s1"},
			{DesignEntity::Assign, "a"},
		};
		QP::QueryProperties properties = QP::QueryProperties(declarations, select_list, {});

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result_s1 = {"1", "1", "2", "2", "3", "3", "4", "4"};
		vector<string> expected_result_a = {"1", "1", "1", "1", "3", "3", "3", "3"};
		vector<string> actual_result_s1 = result.getSynonymResult("s1");
		vector<string> actual_result_a = result.getSynonymResult("a");
		sort(actual_result_s1.begin(), actual_result_s1.end());
		sort(actual_result_a.begin(), actual_result_a.end());
		REQUIRE(actual_result_s1 == expected_result_s1);
		REQUIRE(actual_result_a == expected_result_a);
	};

	SECTION("Select tuple, same group") {
		DeclarationList select_list = {
			{DesignEntity::Variable, "v"},
			{DesignEntity::Assign, "a"},
		};
		ClauseList clauses = {
			{make_unique<QP::Relationship::Relation>(
				ClauseType::ModifiesS, vector<ReferenceArgument>({assign_syn, var_syn}),
				dispatcher.dispatch_map.at(ClauseType::UnknownModifies)({assign_syn, var_syn}).second)},
		};
		QP::QueryProperties properties = QP::QueryProperties(declarations, select_list, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		REQUIRE(result.getSynonymResult("a") == vector<string>({"1"}));
		REQUIRE(result.getSynonymResult("v") == vector<string>({"x"}));
	};

	SECTION("Select tuple, different group") {
		DeclarationList select_list = {
			{DesignEntity::Stmt, "s1"},
			{DesignEntity::Assign, "a"},
		};
		ClauseList clauses = {
			{make_unique<QP::Relationship::Relation>(
				ClauseType::ModifiesS, vector<ReferenceArgument>({assign_syn, var_syn}),
				dispatcher.dispatch_map.at(ClauseType::UnknownModifies)({assign_syn, var_syn}).second)},
			{make_unique<QP::Relationship::Relation>(ClauseType::Follows, vector<ReferenceArgument>({wildcard, stmt1_syn}),
		                                             dispatcher.dispatch_map.at(ClauseType::Follows)({wildcard, stmt1_syn}).second)},
			{make_unique<QP::Relationship::Relation>(ClauseType::Follows, vector<ReferenceArgument>({wildcard, wildcard}),
		                                             dispatcher.dispatch_map.at(ClauseType::Follows)({wildcard, wildcard}).second)},
		};
		QP::QueryProperties properties = QP::QueryProperties(declarations, select_list, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result_s1 = {"2", "4"};
		vector<string> expected_result_a = {"1", "1"};
		vector<string> actual_result_s1 = result.getSynonymResult("s1");
		vector<string> actual_result_a = result.getSynonymResult("a");
		sort(actual_result_s1.begin(), actual_result_s1.end());
		REQUIRE(actual_result_s1 == expected_result_s1);
		REQUIRE(actual_result_a == expected_result_a);
	};
};
