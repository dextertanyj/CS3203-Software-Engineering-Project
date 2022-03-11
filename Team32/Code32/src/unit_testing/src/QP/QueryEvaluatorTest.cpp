#include "QP/QueryEvaluator.h"

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/TypeDefs.h"
#include "QP/Dispatcher.h"
#include "QP/QueryExpressionLexer.h"
#include "QP/ReferenceArgument.h"
#include "catch.hpp"

using namespace QP::Types;

TEST_CASE("QP::QueryEvaluator::splitClauses Should split clauses into groups") {
	QP::Dispatcher dispatcher;
	DeclarationList declarations = {
		{DesignEntity::Stmt, "s1"},  {DesignEntity::Stmt, "s2"}, {DesignEntity::Variable, "v"},
		{DesignEntity::Assign, "a"}, {DesignEntity::If, "i"},
	};
	Declaration select = {DesignEntity::Stmt, "s1"};
	ReferenceArgument s1 = ReferenceArgument({DesignEntity::Stmt, "s1"});
	ReferenceArgument s2 = ReferenceArgument({DesignEntity::Stmt, "s2"});
	ReferenceArgument a = ReferenceArgument({DesignEntity::Assign, "a"});
	ReferenceArgument i = ReferenceArgument({DesignEntity::If, "i"});
	ReferenceArgument stmt_no1 = ReferenceArgument(1);
	ReferenceArgument stmt_no2 = ReferenceArgument(2);
	ReferenceArgument v = ReferenceArgument({DesignEntity::Variable, "v"});

	ClauseList clauses = {
		{make_unique<QP::Relationship::Relation>(ClauseType::Parent, vector<ReferenceArgument>({s1, s2}),
	                                             dispatcher.dispatch_map.at(ClauseType::Parent)({s1, s2}).second)},
		{make_unique<QP::Relationship::Relation>(ClauseType::Follows, vector<ReferenceArgument>({a, i}),
	                                             dispatcher.dispatch_map.at(ClauseType::Follows)({a, i}).second)},
		{make_unique<QP::Relationship::Relation>(ClauseType::Parent, vector<ReferenceArgument>({stmt_no1, stmt_no2}),
	                                             dispatcher.dispatch_map.at(ClauseType::Parent)({stmt_no1, stmt_no2}).second)},
		{make_unique<QP::Relationship::Relation>(ClauseType::ModifiesS, vector<ReferenceArgument>({a, v}),
	                                             dispatcher.dispatch_map.at(ClauseType::UnknownModifies)({a, v}).second)},
	};
	QP::QueryProperties properties = QP::QueryProperties(declarations, {select}, clauses);
	unordered_map<string, size_t> synonyms_in_group = {
		{"s1", 0}, {"s2", 0}, {"a", 1}, {"v", 1}, {"i", 1},
	};

	PKB::Storage pkb = PKB::Storage();
	vector<ClauseList> clauses_in_group = QP::QueryEvaluator::splitClauses(properties, synonyms_in_group);

	REQUIRE(clauses_in_group.size() == 3);
	REQUIRE(clauses_in_group[0].size() == 1);
	REQUIRE(clauses_in_group[1].size() == 2);
	REQUIRE(clauses_in_group[2].size() == 1);
};

TEST_CASE("QP::QueryEvaluator::execute") {
	QP::Dispatcher dispatcher;
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

	DeclarationList declarations = {
		{DesignEntity::Stmt, "s1"},  {DesignEntity::Stmt, "s2"},     {DesignEntity::Variable, "v"},
		{DesignEntity::Assign, "a"}, {DesignEntity::Procedure, "p"},
	};
	Declaration assign_synonym = {DesignEntity::Assign, "a"};
	Declaration var_synonym = {DesignEntity::Variable, "v"};
	Declaration stmt_synonym = {DesignEntity::Stmt, "s1"};
	Declaration proc_synonym = {DesignEntity::Procedure, "p"};
	ReferenceArgument v = ReferenceArgument({DesignEntity::Variable, "v"});
	ReferenceArgument var_underscore = ReferenceArgument();
	ReferenceArgument a = ReferenceArgument({DesignEntity::Assign, "a"});
	ReferenceArgument stmt_no1 = ReferenceArgument(1);
	ReferenceArgument stmt_underscore = ReferenceArgument();

	vector<string> assign_token = {"x", "+", "1"};
	QP::QueryExpressionLexer lexer = QP::QueryExpressionLexer(assign_token);
	auto expression = Common::ExpressionProcessor::Expression::parse(lexer, Common::ExpressionProcessor::ExpressionType::Arithmetic);
	pkb.setAssign(1, "x", expression);

	vector<string> token = {"1"};
	QP::QueryExpressionLexer query_lexer = QP::QueryExpressionLexer(token);
	auto query_expression =
		Common::ExpressionProcessor::Expression::parse(query_lexer, Common::ExpressionProcessor::ExpressionType::Arithmetic);

	SECTION("No clause, select assign statement") {
		QP::QueryProperties properties = QP::QueryProperties(declarations, {assign_synonym}, {});
		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"1", "3"};
		vector<string> actual_result = result.getSynonymResult("a");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
	};

	SECTION("No clause, select variable") {
		QP::QueryProperties properties = QP::QueryProperties(declarations, {var_synonym}, {});
		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"x"};
		vector<string> actual_result = result.getSynonymResult("v");
		REQUIRE(actual_result == expected_result);
	};

	SECTION("No clause, select procedure") {
		QP::QueryProperties properties = QP::QueryProperties(declarations, {proc_synonym}, {});
		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"proc1", "proc2"};
		vector<string> actual_result = result.getSynonymResult("p");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
	};

	SECTION("One non-trivial such that clause") {
		ClauseList clauses = {
			{make_unique<QP::Relationship::Relation>(ClauseType::ModifiesS, vector<ReferenceArgument>({a, v}),
		                                             dispatcher.dispatch_map.at(ClauseType::UnknownModifies)({a, v}).second)}};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {assign_synonym}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"1"};
		vector<string> actual_result = result.getSynonymResult("a");
		REQUIRE(actual_result == expected_result);
	};

	SECTION("One trivial such that clause") {
		ClauseList clauses = {
			{make_unique<QP::Relationship::Relation>(ClauseType::ModifiesS, vector<ReferenceArgument>({stmt_no1, v}),
		                                             dispatcher.dispatch_map.at(ClauseType::UnknownModifies)({stmt_no1, v}).second)}};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {assign_synonym}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"1", "3"};
		vector<string> actual_result = result.getSynonymResult("a");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
	};

	SECTION("One trivial pattern clause") {
		ClauseList clauses = {{make_unique<QP::Relationship::Relation>(
			ClauseType::PatternAssign, vector<ReferenceArgument>({a, var_underscore, ReferenceArgument(query_expression, false)}),
			dispatcher.dispatch_map.at(ClauseType::PatternAssign)({a, var_underscore, ReferenceArgument(query_expression, false)})
				.second)}};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {var_synonym}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"x"};
		vector<string> actual_result = result.getSynonymResult("v");
		REQUIRE(actual_result == expected_result);
	};

	SECTION("One non-trivial pattern clause") {
		ClauseList clauses = {{make_unique<QP::Relationship::Relation>(
			ClauseType::PatternAssign, vector<ReferenceArgument>({a, v, ReferenceArgument(query_expression, false)}),
			dispatcher.dispatch_map.at(ClauseType::PatternAssign)({a, v, ReferenceArgument(query_expression, false)}).second)}};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {var_synonym}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"x"};
		vector<string> actual_result = result.getSynonymResult("v");
		REQUIRE(actual_result == expected_result);
	};

	SECTION("Trivial pattern clause and trival such that clause") {
		ClauseList clauses = {
			{make_unique<QP::Relationship::Relation>(ClauseType::ModifiesS, vector<ReferenceArgument>({stmt_no1, v}),
		                                             dispatcher.dispatch_map.at(ClauseType::UnknownModifies)({stmt_no1, v}).second)},
			{make_unique<QP::Relationship::Relation>(
				ClauseType::PatternAssign, vector<ReferenceArgument>({a, v, ReferenceArgument(query_expression, false)}),
				dispatcher.dispatch_map.at(ClauseType::PatternAssign)({a, v, ReferenceArgument(query_expression, false)}).second)},
		};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {stmt_synonym}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"1", "2", "3", "4"};
		vector<string> actual_result = result.getSynonymResult("s1");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
	};

	SECTION("Non-trivial pattern clause and such that clause") {
		ClauseList clauses = {
			{make_unique<QP::Relationship::Relation>(ClauseType::ModifiesS, vector<ReferenceArgument>({stmt_no1, v}),
		                                             dispatcher.dispatch_map.at(ClauseType::UnknownModifies)({stmt_no1, v}).second)},
			{make_unique<QP::Relationship::Relation>(
				ClauseType::PatternAssign, vector<ReferenceArgument>({a, v, ReferenceArgument(query_expression, false)}),
				dispatcher.dispatch_map.at(ClauseType::PatternAssign)({a, v, ReferenceArgument(query_expression, false)}).second)},
		};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {var_synonym}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"x"};
		vector<string> actual_result = result.getSynonymResult("v");
		REQUIRE(actual_result == expected_result);
	};

	SECTION("Multiple clauses") {
		ClauseList clauses = {
			{make_unique<QP::Relationship::Relation>(ClauseType::ModifiesS, vector<ReferenceArgument>({stmt_no1, v}),
		                                             dispatcher.dispatch_map.at(ClauseType::UnknownModifies)({stmt_no1, v}).second)},
			{make_unique<QP::Relationship::Relation>(ClauseType::Follows, vector<ReferenceArgument>({a, stmt_underscore}),
		                                             dispatcher.dispatch_map.at(ClauseType::Follows)({a, stmt_underscore}).second)},
			{make_unique<QP::Relationship::Relation>(
				ClauseType::PatternAssign, vector<ReferenceArgument>({a, v, ReferenceArgument(query_expression, false)}),
				dispatcher.dispatch_map.at(ClauseType::PatternAssign)({a, v, ReferenceArgument(query_expression, false)}).second)},
		};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {var_synonym}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"x"};
		vector<string> actual_result = result.getSynonymResult("v");
		REQUIRE(actual_result == expected_result);
	};
};
