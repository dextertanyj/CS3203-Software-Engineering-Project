#include "QP/QueryEvaluator.h"

#include "Common/ExpressionProcessor/Expression.h"
#include "QP/ReferenceArgument.h"
#include "Common/TypeDefs.h"
#include "QP/QueryExpressionLexer.h"
#include "QP/Relationship/Follows.h"
#include "QP/Relationship/ModifiesS.h"
#include "QP/Relationship/Parent.h"
#include "catch.hpp"

TEST_CASE("QP::QueryEvaluator::splitClauses Should split clauses into groups") {
	DeclarationList declarations = {
		{DesignEntity::Stmt, "s1"},  {DesignEntity::Stmt, "s2"}, {DesignEntity::Variable, "v"},
		{DesignEntity::Assign, "a"}, {DesignEntity::If, "i"},
	};
	Declaration select = {DesignEntity::Stmt, "s1"};
	ReferenceArgument s1 = ReferenceArgument({QP::Types::DesignEntity::Stmt, "s1"});
	ReferenceArgument s2 = ReferenceArgument({QP::Types::DesignEntity::Stmt, "s2"});
	ReferenceArgument a = ReferenceArgument({QP::Types::DesignEntity::Assign, "a"});
	ReferenceArgument i = ReferenceArgument({QP::Types::DesignEntity::If, "i"});
	ReferenceArgument stmt_no1 = ReferenceArgument(1);
	ReferenceArgument stmt_no2 = ReferenceArgument(2);
	ReferenceArgument v = ReferenceArgument({QP::Types::DesignEntity::Variable, "v"});

	ClauseList clauses = {
		{make_unique<QP::Relationship::Parent>(s1, s2)},
		{make_unique<QP::Relationship::Parent>(a, i)},
		{make_unique<QP::Relationship::Parent>(stmt_no1, stmt_no2)},
		{make_unique<QP::Relationship::ModifiesS>(a, v)},
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
	ReferenceArgument v = ReferenceArgument({QP::Types::DesignEntity::Variable, "v"});
	ReferenceArgument var_underscore = ReferenceArgument();
	ReferenceArgument a = ReferenceArgument({QP::Types::DesignEntity::Assign, "a"});
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
		ClauseList clauses = {{make_unique<QP::Relationship::ModifiesS>(a, v)}};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {assign_synonym}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"1"};
		vector<string> actual_result = result.getSynonymResult("a");
		REQUIRE(actual_result == expected_result);
	};

	SECTION("One trivial such that clause") {
		ClauseList clauses = {{make_unique<QP::Relationship::ModifiesS>(stmt_no1, v)}};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {assign_synonym}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"1", "3"};
		vector<string> actual_result = result.getSynonymResult("a");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
	};

	SECTION("One trivial pattern clause") {
		ClauseList clauses = {{make_unique<QP::Relationship::Pattern>(ReferenceArgument(assign_synonym), var_underscore,
		                                                              ReferenceArgument(query_expression, false))}};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {var_synonym}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"x"};
		vector<string> actual_result = result.getSynonymResult("v");
		REQUIRE(actual_result == expected_result);
	};

	SECTION("One non-trivial pattern clause") {
		ClauseList clauses = {
			{make_unique<QP::Relationship::Pattern>(ReferenceArgument(assign_synonym), v, ReferenceArgument(query_expression, false))}};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {var_synonym}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"x"};
		vector<string> actual_result = result.getSynonymResult("v");
		REQUIRE(actual_result == expected_result);
	};

	SECTION("Trivial pattern clause and trival such that clause") {
		ClauseList clauses = {
			{make_unique<QP::Relationship::ModifiesS>(stmt_no1, v)},
			{make_unique<QP::Relationship::Pattern>(ReferenceArgument(assign_synonym), v, ReferenceArgument(query_expression, false))}};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {stmt_synonym}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"1", "2", "3", "4"};
		vector<string> actual_result = result.getSynonymResult("s1");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
	};

	SECTION("Non-trivial pattern clause and such that clause") {
		ClauseList clauses = {
			{make_unique<QP::Relationship::ModifiesS>(stmt_no1, v)},
			{make_unique<QP::Relationship::Pattern>(ReferenceArgument(assign_synonym), v, ReferenceArgument(query_expression, false))},
		};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {var_synonym}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"x"};
		vector<string> actual_result = result.getSynonymResult("v");
		REQUIRE(actual_result == expected_result);
	};

	SECTION("Multiple clauses") {
		ClauseList clauses = {
			{make_unique<QP::Relationship::ModifiesS>(stmt_no1, v)},
			{make_unique<QP::Relationship::Follows>(a, stmt_underscore)},
			{make_unique<QP::Relationship::Pattern>(ReferenceArgument(assign_synonym), v, ReferenceArgument(query_expression, false))},
		};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {var_synonym}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"x"};
		vector<string> actual_result = result.getSynonymResult("v");
		REQUIRE(actual_result == expected_result);
	};
};
