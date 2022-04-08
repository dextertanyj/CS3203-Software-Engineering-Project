#include "QP/Evaluator/QueryEvaluator.h"

#include "Common/ExpressionProcessor/ExpressionParser.h"
#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "QP/ClauseArgument.h"
#include "QP/Evaluator/Clause.h"
#include "QP/Preprocessor/Maps.h"
#include "QP/Preprocessor/QueryExpressionLexer.h"
#include "catch.hpp"

using namespace std;
using namespace QP;
using namespace Evaluator;
using namespace Preprocessor;
using namespace Types;

TEST_CASE("QueryEvaluator::execute") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::Assign);
	pkb.setStmtType(4, StmtType::If);
	pkb.setFollows(1, 2);
	pkb.setFollows(3, 4);
	pkb.setModifies(1, "x");
	pkb.setProc("proc1", 1, 2);
	pkb.setProc("proc2", 3, 4);
	pkb.populateComplexRelations();

	unordered_set<ConstVal> constants = {1};
	pkb.setConstant(constants);

	QP::StorageAdapter store(pkb);
	QueryEvaluator evaluator = QueryEvaluator(store);

	Declaration assign_declaration = {DesignEntity::Assign, "a"};
	Declaration var_declaration = {DesignEntity::Variable, "v"};
	Declaration stmt1_declaration = {DesignEntity::Stmt, "s1"};
	Declaration stmt2_declaration = {DesignEntity::Stmt, "s2"};
	Declaration proc_declaration = {DesignEntity::Procedure, "p"};
	DeclarationList declarations = {
		assign_declaration, var_declaration, stmt1_declaration, stmt2_declaration, proc_declaration,
	};
	ClauseArgument assign_syn = ClauseArgument(assign_declaration);
	ClauseArgument var_syn = ClauseArgument(var_declaration);
	ClauseArgument stmt1_syn = ClauseArgument(stmt1_declaration);
	ClauseArgument stmt_no1 = ClauseArgument(1);
	ClauseArgument stmt_no2 = ClauseArgument(2);
	ClauseArgument wildcard = ClauseArgument();

	vector<string> assign_token = {"x", "+", "1"};
	QueryExpressionLexer lexer = QueryExpressionLexer(assign_token);
	auto expression = Common::ExpressionProcessor::ExpressionParser{lexer, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
	pkb.setAssign(1, "x", expression);

	vector<string> token = {"1"};
	QueryExpressionLexer query_lexer = QueryExpressionLexer(token);
	auto query_expression =
		Common::ExpressionProcessor::ExpressionParser{query_lexer, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();

	SECTION("No clause, select assign statement") {
		QP::QueryProperties properties = QP::QueryProperties(declarations, {ClauseArgument{assign_declaration}}, {});
		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"1", "3"};
		vector<string> actual_result = result.getSynonymResult("a");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
	};

	SECTION("No clause, select variable") {
		QP::QueryProperties properties = QP::QueryProperties(declarations, {ClauseArgument{var_declaration}}, {});
		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"x"};
		vector<string> actual_result = result.getSynonymResult("v");
		REQUIRE(actual_result == expected_result);
	};

	SECTION("No clause, select procedure") {
		QP::QueryProperties properties = QP::QueryProperties(declarations, {ClauseArgument{proc_declaration}}, {});
		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"proc1", "proc2"};
		vector<string> actual_result = result.getSynonymResult("p");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
	};

	SECTION("One non-trivial such that clause") {
		vector<ClauseArgument> args = {assign_syn, var_syn};
		ClauseList clauses = {
			{make_unique<Clause>(ClauseType::ModifiesS, args, Maps::dispatch_map.at(ClauseType::UnknownModifies)(args).second)}};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {ClauseArgument{assign_declaration}}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"1"};
		vector<string> actual_result = result.getSynonymResult("a");
		REQUIRE(actual_result == expected_result);
	};

	SECTION("One trivial such that clause") {
		vector<ClauseArgument> args = {stmt_no1, var_syn};
		ClauseList clauses = {
			{make_unique<Clause>(ClauseType::ModifiesS, args, Maps::dispatch_map.at(ClauseType::UnknownModifies)(args).second)}};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {ClauseArgument{assign_declaration}}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"1", "3"};
		vector<string> actual_result = result.getSynonymResult("a");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
	};

	SECTION("One trivial pattern clause") {
		vector<ClauseArgument> args = {assign_syn, wildcard, ClauseArgument(query_expression, false)};
		ClauseList clauses = {
			{make_unique<Clause>(ClauseType::PatternAssign, args, Maps::dispatch_map.at(ClauseType::PatternAssign)(args).second)}};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {ClauseArgument{var_declaration}}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"x"};
		vector<string> actual_result = result.getSynonymResult("v");
		REQUIRE(actual_result == expected_result);
	};

	SECTION("One non-trivial pattern clause") {
		vector<ClauseArgument> args = {assign_syn, var_syn, ClauseArgument(query_expression, false)};
		ClauseList clauses = {
			{make_unique<Clause>(ClauseType::PatternAssign, args, Maps::dispatch_map.at(ClauseType::PatternAssign)(args).second)}};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {ClauseArgument{var_declaration}}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"x"};
		vector<string> actual_result = result.getSynonymResult("v");
		REQUIRE(actual_result == expected_result);
	};

	SECTION("Trivial pattern clause and trival such that clause") {
		vector<ClauseArgument> args_1 = {stmt_no1, var_syn};
		vector<ClauseArgument> args_2 = {assign_syn, var_syn, ClauseArgument(query_expression, false)};
		ClauseList clauses = {
			{make_unique<Clause>(ClauseType::ModifiesS, vector<ClauseArgument>(args_1),
		                         Maps::dispatch_map.at(ClauseType::UnknownModifies)(args_1).second)},
			{make_unique<Clause>(ClauseType::PatternAssign, vector<ClauseArgument>(args_2),
		                         Maps::dispatch_map.at(ClauseType::PatternAssign)(args_2).second)},
		};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {ClauseArgument{stmt1_declaration}}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"1", "2", "3", "4"};
		vector<string> actual_result = result.getSynonymResult("s1");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result);
	};

	SECTION("Non-trivial pattern clause and such that clause") {
		vector<ClauseArgument> args_1 = {stmt_no1, var_syn};
		vector<ClauseArgument> args_2 = {assign_syn, var_syn, ClauseArgument(query_expression, false)};
		ClauseList clauses = {
			{make_unique<Clause>(ClauseType::ModifiesS, args_1, Maps::dispatch_map.at(ClauseType::UnknownModifies)(args_1).second)},
			{make_unique<Clause>(ClauseType::PatternAssign, args_2, Maps::dispatch_map.at(ClauseType::PatternAssign)(args_2).second)},
		};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {ClauseArgument{var_declaration}}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		vector<string> expected_result = {"x"};
		vector<string> actual_result = result.getSynonymResult("v");
		REQUIRE(actual_result == expected_result);
	};

	SECTION("Multiple clauses") {
		vector<ClauseArgument> args_1 = {stmt_no1, var_syn};
		vector<ClauseArgument> args_2 = {assign_syn, wildcard};
		vector<ClauseArgument> args_3 = {assign_syn, var_syn, ClauseArgument(query_expression, false)};
		vector<ClauseArgument> args_4 = {stmt_no1, stmt_no2};
		vector<ClauseArgument> args_5 = {stmt_no1, wildcard};

		ClauseList clauses = {
			{make_unique<Clause>(ClauseType::ModifiesS, args_1, Maps::dispatch_map.at(ClauseType::UnknownModifies)(args_1).second)},
			{make_unique<Clause>(ClauseType::Follows, args_2, Maps::dispatch_map.at(ClauseType::Follows)(args_2).second)},
			{make_unique<Clause>(ClauseType::PatternAssign, args_3, Maps::dispatch_map.at(ClauseType::PatternAssign)(args_3).second)},
			{make_unique<Clause>(ClauseType::FollowsT, args_4, Maps::dispatch_map.at(ClauseType::FollowsT)(args_4).second)},
			{make_unique<Clause>(ClauseType::ModifiesS, args_5, Maps::dispatch_map.at(ClauseType::UnknownModifies)(args_5).second)},
		};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {ClauseArgument{var_declaration}}, clauses);

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
		vector<ClauseArgument> args_1 = {stmt_no1, var_syn};
		vector<ClauseArgument> args_2 = {assign_syn, wildcard};

		ClauseList clauses = {
			{make_unique<Clause>(ClauseType::ModifiesS, vector<ClauseArgument>(args_1),
		                         Maps::dispatch_map.at(ClauseType::UnknownModifies)(args_1).second)},
			{make_unique<Clause>(ClauseType::Follows, vector<ClauseArgument>(args_2),
		                         Maps::dispatch_map.at(ClauseType::Follows)(args_2).second)},
		};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		REQUIRE(result.getResult());
	};

	SECTION("Select boolean negative test") {
		vector<ClauseArgument> args_1 = {wildcard, stmt_no1};
		vector<ClauseArgument> args_2 = {wildcard, stmt1_syn};

		ClauseList clauses = {
			{make_unique<Clause>(ClauseType::Follows, vector<ClauseArgument>(args_1),
		                         Maps::dispatch_map.at(ClauseType::Follows)(args_1).second)},
			{make_unique<Clause>(ClauseType::Follows, vector<ClauseArgument>(args_2),
		                         Maps::dispatch_map.at(ClauseType::Follows)(args_2).second)},
		};
		QP::QueryProperties properties = QP::QueryProperties(declarations, {}, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		REQUIRE(!result.getResult());
	};

	SECTION("Select tuple, no clause") {
		SelectList select_list = {
			ClauseArgument{{DesignEntity::Stmt, "s1"}},
			ClauseArgument{{DesignEntity::Assign, "a"}},
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
		SelectList select_list = {
			ClauseArgument{{DesignEntity::Variable, "v"}},
			ClauseArgument{{DesignEntity::Assign, "a"}},
		};

		vector<ClauseArgument> args = {assign_syn, var_syn};

		ClauseList clauses = {
			{make_unique<Clause>(ClauseType::ModifiesS, vector<ClauseArgument>(args),
		                         Maps::dispatch_map.at(ClauseType::UnknownModifies)(args).second)},
		};
		QP::QueryProperties properties = QP::QueryProperties(declarations, select_list, clauses);

		QP::QueryResult result = evaluator.executeQuery(properties);

		REQUIRE(result.getSynonymResult("a") == vector<string>({"1"}));
		REQUIRE(result.getSynonymResult("v") == vector<string>({"x"}));
	};

	SECTION("Select tuple, different group") {
		SelectList select_list = {
			ClauseArgument{{DesignEntity::Stmt, "s1"}},
			ClauseArgument{{DesignEntity::Assign, "a"}},
		};
		vector<ClauseArgument> args_1 = {assign_syn, var_syn};
		vector<ClauseArgument> args_2 = {wildcard, stmt1_syn};
		vector<ClauseArgument> args_3 = {assign_syn, wildcard};
		ClauseList clauses = {
			{make_unique<Clause>(ClauseType::ModifiesS, vector<ClauseArgument>(args_1),
		                         Maps::dispatch_map.at(ClauseType::UnknownModifies)(args_1).second)},
			{make_unique<Clause>(ClauseType::Follows, vector<ClauseArgument>(args_2),
		                         Maps::dispatch_map.at(ClauseType::Follows)(args_2).second)},
			{make_unique<Clause>(ClauseType::Follows, vector<ClauseArgument>(args_3),
		                         Maps::dispatch_map.at(ClauseType::Follows)(args_3).second)},
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
