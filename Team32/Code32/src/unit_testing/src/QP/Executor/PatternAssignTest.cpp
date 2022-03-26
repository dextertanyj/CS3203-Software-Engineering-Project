#include "Common/ExpressionProcessor/ExpressionParser.h"
#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "QP/Executor/PatternAssignExecutor.h"
#include "QP/QueryExpressionLexer.h"
#include "catch.hpp"

using namespace QP::Types;
using namespace QP::Executor;
using namespace Common::ExpressionProcessor;

TEST_CASE("PatternAssignExecutor::execute") {
	PKB::Storage pkb = PKB::Storage();
	QP::StorageAdapter store = QP::StorageAdapter(pkb);
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Assign);
	pkb.setStmtType(3, StmtType::Assign);

	vector<string> assign_token1 = {"1"};
	QP::QueryExpressionLexer lexer1 = QP::QueryExpressionLexer(assign_token1);
	auto expression1 = ExpressionParser{lexer1, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(1, "x", expression1);
	vector<string> assign_token2 = {"x", "+", "1"};
	QP::QueryExpressionLexer lexer2 = QP::QueryExpressionLexer(assign_token2);
	auto expression2 = ExpressionParser{lexer2, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(2, "y", expression2);
	vector<string> assign_token3 = {"x", "+", "y"};
	QP::QueryExpressionLexer lexer3 = QP::QueryExpressionLexer(assign_token3);
	auto expression3 = ExpressionParser{lexer3, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(3, "y", expression3);

	pkb.setModifies(1, "x");
	pkb.setModifies(2, "y");
	pkb.setModifies(3, "y");

	unordered_set<ConstVal> constants = {0, 1};
	pkb.setConstant(constants);

	ReferenceArgument syn_assign = ReferenceArgument(Declaration{QP::Types::DesignEntity::Assign, "a"});

	ReferenceArgument x = ReferenceArgument("x");
	ReferenceArgument y = ReferenceArgument("y");
	ReferenceArgument z = ReferenceArgument("z");
	ReferenceArgument var = ReferenceArgument(Declaration{QP::Types::DesignEntity::Variable, "var"});

	vector<string> query_token1 = {"1"};
	QP::QueryExpressionLexer lexer4 = QP::QueryExpressionLexer(query_token1);
	auto query_expression1 = ExpressionParser{lexer4, ExpressionType::Arithmetic}.parse();
	vector<string> query_token2 = {"0"};
	QP::QueryExpressionLexer lexer5 = QP::QueryExpressionLexer(query_token2);
	auto query_expression2 = ExpressionParser{lexer5, ExpressionType::Arithmetic}.parse();
	vector<string> query_token3 = {"x"};
	QP::QueryExpressionLexer lexer6 = QP::QueryExpressionLexer(query_token3);
	auto query_expression3 = ExpressionParser{lexer6, ExpressionType::Arithmetic}.parse();

	SECTION("Trivial: Wildcard & Wildcard") {
		QP::QueryResult result1 = PatternAssignExecutor::executeTrivialSynonymOrWildcardWildcard(store);
		REQUIRE(result1.getResult());
	}

	SECTION("Trivial: Wildcard & Expression") {
		QP::QueryResult result1 =
			PatternAssignExecutor::executeTrivialSynonymOrWildcardExpression(store, ReferenceArgument(query_expression1, true));
		QP::QueryResult result2 =
			PatternAssignExecutor::executeTrivialSynonymOrWildcardExpression(store, ReferenceArgument(query_expression2, true));
		QP::QueryResult result3 =
			PatternAssignExecutor::executeTrivialSynonymOrWildcardExpression(store, ReferenceArgument(query_expression3, true));
		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
		REQUIRE(!result3.getResult());
	}

	SECTION("Trivial: Wildcard & Sub-Expression") {
		QP::QueryResult result1 =
			PatternAssignExecutor::executeTrivialSynonymOrWildcardExpression(store, ReferenceArgument(query_expression1, false));
		QP::QueryResult result2 =
			PatternAssignExecutor::executeTrivialSynonymOrWildcardExpression(store, ReferenceArgument(query_expression2, false));
		QP::QueryResult result3 =
			PatternAssignExecutor::executeTrivialSynonymOrWildcardExpression(store, ReferenceArgument(query_expression3, false));
		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
		REQUIRE(result3.getResult());
	}

	SECTION("Trivial: Name & Wildcard") {
		QP::QueryResult result1 = PatternAssignExecutor::executeTrivialNameWildcard(store, x);
		QP::QueryResult result2 = PatternAssignExecutor::executeTrivialNameWildcard(store, y);
		QP::QueryResult result3 = PatternAssignExecutor::executeTrivialNameWildcard(store, z);
		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE(!result3.getResult());
	}

	SECTION("Trivial: Name & Expression") {
		QP::QueryResult result1 = PatternAssignExecutor::executeTrivialNameExpression(store, x, ReferenceArgument(query_expression1, true));
		QP::QueryResult result2 = PatternAssignExecutor::executeTrivialNameExpression(store, x, ReferenceArgument(query_expression2, true));
		QP::QueryResult result3 = PatternAssignExecutor::executeTrivialNameExpression(store, x, ReferenceArgument(query_expression3, true));
		QP::QueryResult result4 = PatternAssignExecutor::executeTrivialNameExpression(store, y, ReferenceArgument(query_expression1, true));
		QP::QueryResult result5 = PatternAssignExecutor::executeTrivialNameExpression(store, y, ReferenceArgument(query_expression2, true));
		QP::QueryResult result6 = PatternAssignExecutor::executeTrivialNameExpression(store, y, ReferenceArgument(query_expression3, true));
		QP::QueryResult result7 = PatternAssignExecutor::executeTrivialNameExpression(store, z, ReferenceArgument(query_expression1, true));
		QP::QueryResult result8 = PatternAssignExecutor::executeTrivialNameExpression(store, z, ReferenceArgument(query_expression2, true));
		QP::QueryResult result9 = PatternAssignExecutor::executeTrivialNameExpression(store, z, ReferenceArgument(query_expression3, true));
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
		QP::QueryResult result1 =
			PatternAssignExecutor::executeTrivialNameExpression(store, x, ReferenceArgument(query_expression1, false));
		QP::QueryResult result2 =
			PatternAssignExecutor::executeTrivialNameExpression(store, x, ReferenceArgument(query_expression2, false));
		QP::QueryResult result3 =
			PatternAssignExecutor::executeTrivialNameExpression(store, x, ReferenceArgument(query_expression3, false));
		QP::QueryResult result4 =
			PatternAssignExecutor::executeTrivialNameExpression(store, y, ReferenceArgument(query_expression1, false));
		QP::QueryResult result5 =
			PatternAssignExecutor::executeTrivialNameExpression(store, y, ReferenceArgument(query_expression2, false));
		QP::QueryResult result6 =
			PatternAssignExecutor::executeTrivialNameExpression(store, y, ReferenceArgument(query_expression3, false));
		QP::QueryResult result7 =
			PatternAssignExecutor::executeTrivialNameExpression(store, z, ReferenceArgument(query_expression1, false));
		QP::QueryResult result8 =
			PatternAssignExecutor::executeTrivialNameExpression(store, z, ReferenceArgument(query_expression2, false));
		QP::QueryResult result9 =
			PatternAssignExecutor::executeTrivialNameExpression(store, z, ReferenceArgument(query_expression3, false));
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
		QP::QueryResult result1 = PatternAssignExecutor::executeTrivialSynonymOrWildcardWildcard(store);
		REQUIRE(result1.getResult());
	}

	SECTION("Trivial: Synonym & Expression") {
		QP::QueryResult result1 =
			PatternAssignExecutor::executeTrivialSynonymOrWildcardExpression(store, ReferenceArgument(query_expression1, true));
		QP::QueryResult result2 =
			PatternAssignExecutor::executeTrivialSynonymOrWildcardExpression(store, ReferenceArgument(query_expression2, true));
		QP::QueryResult result3 =
			PatternAssignExecutor::executeTrivialSynonymOrWildcardExpression(store, ReferenceArgument(query_expression3, true));
		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
		REQUIRE(!result3.getResult());
	}

	SECTION("Trivial: Synonym & Sub-Expression") {
		QP::QueryResult result1 =
			PatternAssignExecutor::executeTrivialSynonymOrWildcardExpression(store, ReferenceArgument(query_expression1, false));
		QP::QueryResult result2 =
			PatternAssignExecutor::executeTrivialSynonymOrWildcardExpression(store, ReferenceArgument(query_expression2, false));
		QP::QueryResult result3 =
			PatternAssignExecutor::executeTrivialSynonymOrWildcardExpression(store, ReferenceArgument(query_expression3, false));
		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
		REQUIRE(result3.getResult());
	}

	SECTION("Wildcard & Wildcard") {
		QP::QueryResult result1 = PatternAssignExecutor::executeWildcardWildcard(store, syn_assign);
		vector<string> expected_result = {"1", "2", "3"};
		REQUIRE(result1.getResult());
		auto result1vec = result1.getSynonymResult("a");
		std::sort(result1vec.begin(), result1vec.end());
		REQUIRE(result1vec == expected_result);
	}

	SECTION("Wildcard & Expression") {
		QP::QueryResult result1 =
			PatternAssignExecutor::executeWildcardExpression(store, syn_assign, ReferenceArgument(query_expression1, true));
		QP::QueryResult result2 =
			PatternAssignExecutor::executeWildcardExpression(store, syn_assign, ReferenceArgument(query_expression2, true));
		QP::QueryResult result3 =
			PatternAssignExecutor::executeWildcardExpression(store, syn_assign, ReferenceArgument(query_expression3, true));
		vector<string> expected_result1 = {"1"};
		REQUIRE(result1.getResult());
		REQUIRE(result1.getSynonymResult("a") == expected_result1);
		REQUIRE(!result2.getResult());
		REQUIRE(!result3.getResult());
	}

	SECTION("Wildcard & Sub-Expression") {
		QP::QueryResult result1 =
			PatternAssignExecutor::executeWildcardExpression(store, syn_assign, ReferenceArgument(query_expression1, false));
		QP::QueryResult result2 =
			PatternAssignExecutor::executeWildcardExpression(store, syn_assign, ReferenceArgument(query_expression2, false));
		QP::QueryResult result3 =
			PatternAssignExecutor::executeWildcardExpression(store, syn_assign, ReferenceArgument(query_expression3, false));
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
		QP::QueryResult result1 = PatternAssignExecutor::executeNameWildcard(store, syn_assign, x);
		QP::QueryResult result2 = PatternAssignExecutor::executeNameWildcard(store, syn_assign, y);
		QP::QueryResult result3 = PatternAssignExecutor::executeNameWildcard(store, syn_assign, z);

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
		QP::QueryResult result1 =
			PatternAssignExecutor::executeNameExpression(store, syn_assign, x, ReferenceArgument(query_expression1, true));
		QP::QueryResult result2 =
			PatternAssignExecutor::executeNameExpression(store, syn_assign, x, ReferenceArgument(query_expression2, true));
		QP::QueryResult result3 =
			PatternAssignExecutor::executeNameExpression(store, syn_assign, x, ReferenceArgument(query_expression3, true));
		QP::QueryResult result4 =
			PatternAssignExecutor::executeNameExpression(store, syn_assign, y, ReferenceArgument(query_expression1, true));
		QP::QueryResult result5 =
			PatternAssignExecutor::executeNameExpression(store, syn_assign, y, ReferenceArgument(query_expression2, true));
		QP::QueryResult result6 =
			PatternAssignExecutor::executeNameExpression(store, syn_assign, y, ReferenceArgument(query_expression3, true));
		QP::QueryResult result7 =
			PatternAssignExecutor::executeNameExpression(store, syn_assign, z, ReferenceArgument(query_expression1, true));
		QP::QueryResult result8 =
			PatternAssignExecutor::executeNameExpression(store, syn_assign, z, ReferenceArgument(query_expression2, true));
		QP::QueryResult result9 =
			PatternAssignExecutor::executeNameExpression(store, syn_assign, z, ReferenceArgument(query_expression3, true));
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
		QP::QueryResult result1 =
			PatternAssignExecutor::executeNameExpression(store, syn_assign, x, ReferenceArgument(query_expression1, false));
		QP::QueryResult result2 =
			PatternAssignExecutor::executeNameExpression(store, syn_assign, x, ReferenceArgument(query_expression2, false));
		QP::QueryResult result3 =
			PatternAssignExecutor::executeNameExpression(store, syn_assign, x, ReferenceArgument(query_expression3, false));
		QP::QueryResult result4 =
			PatternAssignExecutor::executeNameExpression(store, syn_assign, y, ReferenceArgument(query_expression1, false));
		QP::QueryResult result5 =
			PatternAssignExecutor::executeNameExpression(store, syn_assign, y, ReferenceArgument(query_expression2, false));
		QP::QueryResult result6 =
			PatternAssignExecutor::executeNameExpression(store, syn_assign, y, ReferenceArgument(query_expression3, false));
		QP::QueryResult result7 =
			PatternAssignExecutor::executeNameExpression(store, syn_assign, z, ReferenceArgument(query_expression1, false));
		QP::QueryResult result8 =
			PatternAssignExecutor::executeNameExpression(store, syn_assign, z, ReferenceArgument(query_expression2, false));
		QP::QueryResult result9 =
			PatternAssignExecutor::executeNameExpression(store, syn_assign, z, ReferenceArgument(query_expression3, false));
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
		QP::QueryResult result1 = PatternAssignExecutor::executeSynonymWildcard(store, syn_assign, var);
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
		QP::QueryResult result1 =
			PatternAssignExecutor::executeSynonymExpression(store, syn_assign, var, ReferenceArgument(query_expression1, true));
		QP::QueryResult result2 =
			PatternAssignExecutor::executeSynonymExpression(store, syn_assign, var, ReferenceArgument(query_expression2, true));
		QP::QueryResult result3 =
			PatternAssignExecutor::executeSynonymExpression(store, syn_assign, var, ReferenceArgument(query_expression3, true));
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
		QP::QueryResult result1 =
			PatternAssignExecutor::executeSynonymExpression(store, syn_assign, var, ReferenceArgument(query_expression1, false));
		QP::QueryResult result2 =
			PatternAssignExecutor::executeSynonymExpression(store, syn_assign, var, ReferenceArgument(query_expression2, false));
		QP::QueryResult result3 =
			PatternAssignExecutor::executeSynonymExpression(store, syn_assign, var, ReferenceArgument(query_expression3, false));
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