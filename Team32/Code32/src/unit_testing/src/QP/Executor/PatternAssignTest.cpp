#include "Common/ExpressionProcessor/ExpressionParser.h"
#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "QP/Executor/PatternAssignExecutor.h"
#include "QP/Preprocessor/QueryExpressionLexer.h"
#include "catch.hpp"

using namespace QP;
using namespace Preprocessor;
using namespace Executor::PatternAssignExecutor;
using namespace Types;
using namespace Common::ExpressionProcessor;

TEST_CASE("PatternAssignExecutor::execute") {
	PKB::Storage pkb = PKB::Storage();
	StorageAdapter store = StorageAdapter(pkb);
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Assign);
	pkb.setStmtType(3, StmtType::Assign);

	vector<string> assign_token1 = {"1"};
	QueryExpressionLexer lexer1 = QueryExpressionLexer(assign_token1);
	auto expression1 = ExpressionParser{lexer1, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(1, "x", expression1);
	vector<string> assign_token2 = {"x", "+", "1"};
	QueryExpressionLexer lexer2 = QueryExpressionLexer(assign_token2);
	auto expression2 = ExpressionParser{lexer2, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(2, "y", expression2);
	vector<string> assign_token3 = {"x", "+", "y"};
	QueryExpressionLexer lexer3 = QueryExpressionLexer(assign_token3);
	auto expression3 = ExpressionParser{lexer3, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(3, "y", expression3);

	pkb.setModifies(1, "x");
	pkb.setModifies(2, "y");
	pkb.setModifies(3, "y");

	unordered_set<ConstVal> constants = {0, 1};
	pkb.setConstant(constants);

	ClauseArgument syn_assign = ClauseArgument(Declaration{Types::DesignEntity::Assign, "a"});

	ClauseArgument x = ClauseArgument("x");
	ClauseArgument y = ClauseArgument("y");
	ClauseArgument z = ClauseArgument("z");
	ClauseArgument var = ClauseArgument(Declaration{Types::DesignEntity::Variable, "var"});

	vector<string> query_token1 = {"1"};
	QueryExpressionLexer lexer4 = QueryExpressionLexer(query_token1);
	auto query_expression1 = ExpressionParser{lexer4, ExpressionType::Arithmetic}.parse();
	vector<string> query_token2 = {"0"};
	QueryExpressionLexer lexer5 = QueryExpressionLexer(query_token2);
	auto query_expression2 = ExpressionParser{lexer5, ExpressionType::Arithmetic}.parse();
	vector<string> query_token3 = {"x"};
	QueryExpressionLexer lexer6 = QueryExpressionLexer(query_token3);
	auto query_expression3 = ExpressionParser{lexer6, ExpressionType::Arithmetic}.parse();

	SECTION("Trivial: Wildcard & Wildcard") {
		QueryResult result1 = executeTrivialSynonymOrWildcardWildcard(store);
		REQUIRE(result1.getResult());
	}

	SECTION("Trivial: Wildcard & Expression") {
		QueryResult result1 =
			executeTrivialSynonymOrWildcardExpression(store, ClauseArgument(query_expression1, true));
		QueryResult result2 =
			executeTrivialSynonymOrWildcardExpression(store, ClauseArgument(query_expression2, true));
		QueryResult result3 =
			executeTrivialSynonymOrWildcardExpression(store, ClauseArgument(query_expression3, true));
		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
		REQUIRE(!result3.getResult());
	}

	SECTION("Trivial: Wildcard & Sub-Expression") {
		QueryResult result1 =
			executeTrivialSynonymOrWildcardExpression(store, ClauseArgument(query_expression1, false));
		QueryResult result2 =
			executeTrivialSynonymOrWildcardExpression(store, ClauseArgument(query_expression2, false));
		QueryResult result3 =
			executeTrivialSynonymOrWildcardExpression(store, ClauseArgument(query_expression3, false));
		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
		REQUIRE(result3.getResult());
	}

	SECTION("Trivial: Name & Wildcard") {
		QueryResult result1 = executeTrivialNameWildcard(store, x);
		QueryResult result2 = executeTrivialNameWildcard(store, y);
		QueryResult result3 = executeTrivialNameWildcard(store, z);
		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE(!result3.getResult());
	}

	SECTION("Trivial: Name & Expression") {
		QueryResult result1 = executeTrivialNameExpression(store, x, ClauseArgument(query_expression1, true));
		QueryResult result2 = executeTrivialNameExpression(store, x, ClauseArgument(query_expression2, true));
		QueryResult result3 = executeTrivialNameExpression(store, x, ClauseArgument(query_expression3, true));
		QueryResult result4 = executeTrivialNameExpression(store, y, ClauseArgument(query_expression1, true));
		QueryResult result5 = executeTrivialNameExpression(store, y, ClauseArgument(query_expression2, true));
		QueryResult result6 = executeTrivialNameExpression(store, y, ClauseArgument(query_expression3, true));
		QueryResult result7 = executeTrivialNameExpression(store, z, ClauseArgument(query_expression1, true));
		QueryResult result8 = executeTrivialNameExpression(store, z, ClauseArgument(query_expression2, true));
		QueryResult result9 = executeTrivialNameExpression(store, z, ClauseArgument(query_expression3, true));
		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
		REQUIRE(!result3.getResult());
		REQUIRE(!result4.getResult());
		REQUIRE(!result5.getResult());
		REQUIRE(!result6.getResult());
		REQUIRE(!result7.getResult());
		REQUIRE(!result8.getResult());
		REQUIRE(!result9.getResult());
	}

	SECTION("Trivial: Name & Sub-Expression") {
		QueryResult result1 =
			executeTrivialNameExpression(store, x, ClauseArgument(query_expression1, false));
		QueryResult result2 =
			executeTrivialNameExpression(store, x, ClauseArgument(query_expression2, false));
		QueryResult result3 =
			executeTrivialNameExpression(store, x, ClauseArgument(query_expression3, false));
		QueryResult result4 =
			executeTrivialNameExpression(store, y, ClauseArgument(query_expression1, false));
		QueryResult result5 =
			executeTrivialNameExpression(store, y, ClauseArgument(query_expression2, false));
		QueryResult result6 =
			executeTrivialNameExpression(store, y, ClauseArgument(query_expression3, false));
		QueryResult result7 =
			executeTrivialNameExpression(store, z, ClauseArgument(query_expression1, false));
		QueryResult result8 =
			executeTrivialNameExpression(store, z, ClauseArgument(query_expression2, false));
		QueryResult result9 =
			executeTrivialNameExpression(store, z, ClauseArgument(query_expression3, false));
		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
		REQUIRE(!result3.getResult());
		REQUIRE(result4.getResult());
		REQUIRE(!result5.getResult());
		REQUIRE(result6.getResult());
		REQUIRE(!result7.getResult());
		REQUIRE(!result8.getResult());
		REQUIRE(!result9.getResult());
	}

	SECTION("Trivial: Synonym & Wildcard") {
		QueryResult result1 = executeTrivialSynonymOrWildcardWildcard(store);
		REQUIRE(result1.getResult());
	}

	SECTION("Trivial: Synonym & Expression") {
		QueryResult result1 =
			executeTrivialSynonymOrWildcardExpression(store, ClauseArgument(query_expression1, true));
		QueryResult result2 =
			executeTrivialSynonymOrWildcardExpression(store, ClauseArgument(query_expression2, true));
		QueryResult result3 =
			executeTrivialSynonymOrWildcardExpression(store, ClauseArgument(query_expression3, true));
		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
		REQUIRE(!result3.getResult());
	}

	SECTION("Trivial: Synonym & Sub-Expression") {
		QueryResult result1 =
			executeTrivialSynonymOrWildcardExpression(store, ClauseArgument(query_expression1, false));
		QueryResult result2 =
			executeTrivialSynonymOrWildcardExpression(store, ClauseArgument(query_expression2, false));
		QueryResult result3 =
			executeTrivialSynonymOrWildcardExpression(store, ClauseArgument(query_expression3, false));
		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
		REQUIRE(result3.getResult());
	}

	SECTION("Wildcard & Wildcard") {
		QueryResult result1 = executeWildcardWildcard(store, syn_assign);
		vector<string> expected_result = {"1", "2", "3"};
		REQUIRE(result1.getResult());
		auto result1vec = result1.getSynonymResult("a");
		std::sort(result1vec.begin(), result1vec.end());
		REQUIRE(result1vec == expected_result);
	}

	SECTION("Wildcard & Expression") {
		QueryResult result1 =
			executeWildcardExpression(store, syn_assign, ClauseArgument(query_expression1, true));
		QueryResult result2 =
			executeWildcardExpression(store, syn_assign, ClauseArgument(query_expression2, true));
		QueryResult result3 =
			executeWildcardExpression(store, syn_assign, ClauseArgument(query_expression3, true));
		vector<string> expected_result1 = {"1"};
		REQUIRE(result1.getResult());
		REQUIRE(result1.getSynonymResult("a") == expected_result1);
		REQUIRE(!result2.getResult());
		REQUIRE(!result3.getResult());
	}

	SECTION("Wildcard & Sub-Expression") {
		QueryResult result1 =
			executeWildcardExpression(store, syn_assign, ClauseArgument(query_expression1, false));
		QueryResult result2 =
			executeWildcardExpression(store, syn_assign, ClauseArgument(query_expression2, false));
		QueryResult result3 =
			executeWildcardExpression(store, syn_assign, ClauseArgument(query_expression3, false));
		vector<string> expected_result1 = {"1", "2"};
		vector<string> expected_result3 = {"2", "3"};
		REQUIRE(result1.getResult());
		auto result1vec = result1.getSynonymResult("a");
		std::sort(result1vec.begin(), result1vec.end());
		REQUIRE(result1vec == expected_result1);
		REQUIRE(!result2.getResult());
		REQUIRE(result3.getResult());
		auto result3vec = result3.getSynonymResult("a");
		std::sort(result3vec.begin(), result3vec.end());
		REQUIRE(result3vec == expected_result3);
	}

	SECTION("Name & Wildcard") {
		QueryResult result1 = executeNameWildcard(store, syn_assign, x);
		QueryResult result2 = executeNameWildcard(store, syn_assign, y);
		QueryResult result3 = executeNameWildcard(store, syn_assign, z);

		vector<string> expected_result1 = {"1"};
		vector<string> expected_result2 = {"2", "3"};
		REQUIRE(result1.getResult());
		REQUIRE(result1.getSynonymResult("a") == expected_result1);
		REQUIRE(result2.getResult());
		auto result2vec = result2.getSynonymResult("a");
		std::sort(result2vec.begin(), result2vec.end());
		REQUIRE(result2vec == expected_result2);
		REQUIRE(!result3.getResult());
	}

	SECTION("Name & Expression") {
		QueryResult result1 =
			executeNameExpression(store, syn_assign, x, ClauseArgument(query_expression1, true));
		QueryResult result2 =
			executeNameExpression(store, syn_assign, x, ClauseArgument(query_expression2, true));
		QueryResult result3 =
			executeNameExpression(store, syn_assign, x, ClauseArgument(query_expression3, true));
		QueryResult result4 =
			executeNameExpression(store, syn_assign, y, ClauseArgument(query_expression1, true));
		QueryResult result5 =
			executeNameExpression(store, syn_assign, y, ClauseArgument(query_expression2, true));
		QueryResult result6 =
			executeNameExpression(store, syn_assign, y, ClauseArgument(query_expression3, true));
		QueryResult result7 =
			executeNameExpression(store, syn_assign, z, ClauseArgument(query_expression1, true));
		QueryResult result8 =
			executeNameExpression(store, syn_assign, z, ClauseArgument(query_expression2, true));
		QueryResult result9 =
			executeNameExpression(store, syn_assign, z, ClauseArgument(query_expression3, true));
		vector<string> expected_result1 = {"1"};
		REQUIRE(result1.getResult());
		REQUIRE(result1.getSynonymResult("a") == expected_result1);
		REQUIRE(!result2.getResult());
		REQUIRE(!result3.getResult());
		REQUIRE(!result4.getResult());
		REQUIRE(!result5.getResult());
		REQUIRE(!result6.getResult());
		REQUIRE(!result7.getResult());
		REQUIRE(!result8.getResult());
		REQUIRE(!result9.getResult());
	}

	SECTION("Name & Sub-Expression") {
		QueryResult result1 =
			executeNameExpression(store, syn_assign, x, ClauseArgument(query_expression1, false));
		QueryResult result2 =
			executeNameExpression(store, syn_assign, x, ClauseArgument(query_expression2, false));
		QueryResult result3 =
			executeNameExpression(store, syn_assign, x, ClauseArgument(query_expression3, false));
		QueryResult result4 =
			executeNameExpression(store, syn_assign, y, ClauseArgument(query_expression1, false));
		QueryResult result5 =
			executeNameExpression(store, syn_assign, y, ClauseArgument(query_expression2, false));
		QueryResult result6 =
			executeNameExpression(store, syn_assign, y, ClauseArgument(query_expression3, false));
		QueryResult result7 =
			executeNameExpression(store, syn_assign, z, ClauseArgument(query_expression1, false));
		QueryResult result8 =
			executeNameExpression(store, syn_assign, z, ClauseArgument(query_expression2, false));
		QueryResult result9 =
			executeNameExpression(store, syn_assign, z, ClauseArgument(query_expression3, false));
		vector<string> expected_result1 = {"1"};
		vector<string> expected_result4 = {"2"};
		vector<string> expected_result6 = {"2", "3"};
		REQUIRE(result1.getResult());
		REQUIRE(result1.getSynonymResult("a") == expected_result1);
		REQUIRE(!result2.getResult());
		REQUIRE(!result3.getResult());
		REQUIRE(result4.getResult());
		REQUIRE(result4.getSynonymResult("a") == expected_result4);
		REQUIRE(!result5.getResult());
		REQUIRE(result6.getResult());
		auto result6vec = result6.getSynonymResult("a");
		std::sort(result6vec.begin(), result6vec.end());
		REQUIRE(result6vec == expected_result6);
		REQUIRE(!result7.getResult());
		REQUIRE(!result8.getResult());
		REQUIRE(!result9.getResult());
	}

	SECTION("Synonym & Wildcard") {
		QueryResult result1 = executeSynonymWildcard(store, syn_assign, var);
		vector<string> expected_result1 = {"1", "2", "3"};
		vector<string> expected_result_var1 = {"x", "y", "y"};
		REQUIRE(result1.getResult());
		auto result1vec1 = result1.getSynonymResult("a");
		std::sort(result1vec1.begin(), result1vec1.end());
		auto result1vec2 = result1.getSynonymResult("var");
		std::sort(result1vec2.begin(), result1vec2.end());
		REQUIRE(result1vec1 == expected_result1);
		REQUIRE(result1vec2 == expected_result_var1);
	}

	SECTION("Synonym & Expression") {
		QueryResult result1 =
			executeSynonymExpression(store, syn_assign, var, ClauseArgument(query_expression1, true));
		QueryResult result2 =
			executeSynonymExpression(store, syn_assign, var, ClauseArgument(query_expression2, true));
		QueryResult result3 =
			executeSynonymExpression(store, syn_assign, var, ClauseArgument(query_expression3, true));
		vector<string> expected_result1 = {"1"};
		vector<string> expected_result_var1 = {"x"};
		REQUIRE(result1.getResult());
		auto result1vec = result1.getSynonymResult("a");
		std::sort(result1vec.begin(), result1vec.end());
		REQUIRE(result1vec == expected_result1);
		REQUIRE(result1.getSynonymResult("var") == expected_result_var1);
		REQUIRE(!result2.getResult());
		REQUIRE(!result3.getResult());
	}

	SECTION("Synonym & Sub-Expression") {
		QueryResult result1 =
			executeSynonymExpression(store, syn_assign, var, ClauseArgument(query_expression1, false));
		QueryResult result2 =
			executeSynonymExpression(store, syn_assign, var, ClauseArgument(query_expression2, false));
		QueryResult result3 =
			executeSynonymExpression(store, syn_assign, var, ClauseArgument(query_expression3, false));
		vector<string> expected_result1 = {"1", "2"};
		vector<string> expected_result_var1 = {"x", "y"};
		vector<string> expected_result3 = {"2", "3"};
		vector<string> expected_result_var3 = {"y", "y"};

		REQUIRE(result1.getResult());
		auto result1vec = result1.getSynonymResult("a");
		std::sort(result1vec.begin(), result1vec.end());
		auto result1vec2 = result1.getSynonymResult("var");
		std::sort(result1vec2.begin(), result1vec2.end());
		REQUIRE(result1vec == expected_result1);
		REQUIRE(result1vec2 == expected_result_var1);
		REQUIRE(!result2.getResult());
		REQUIRE(result3.getResult());
		auto result3vec = result3.getSynonymResult("a");
		std::sort(result3vec.begin(), result3vec.end());
		REQUIRE(result3vec == expected_result3);
		REQUIRE(result3.getSynonymResult("var") == expected_result_var3);
	}
}