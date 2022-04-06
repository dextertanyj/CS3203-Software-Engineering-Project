#include "Common/ExpressionProcessor/ExpressionParser.h"
#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "QP/Executor/StatementExecutor.tpp"
#include "QP/Preprocessor/QueryExpressionLexer.h"
#include "catch.hpp"

using namespace QP;
using namespace Preprocessor;
using namespace Executor::StatementExecutor;
using namespace Types;
using namespace Common::ExpressionProcessor;

TEST_CASE("StatementExecutor<ClauseType::Affects>::execute") {
	/*
	 * procedure A {
	 * 	x = 3 + y;         //1
	 * 	if (x < 0) then {  //2
	 * 		y = 4 * x;     //3
	 * 	} else {
	 * 		read x;        //4
	 * 	}
	 * 	y = x / 3;         //5
	 * 	x = y - 2;         //6
	 * }
	 *
	 * procedure B {
	 * 	x = 8 * y;
	 * 	z = 100;
	 * 	call A;
	 * 	y = x + 10;
	 *}
	 */
	PKB::Storage pkb = PKB::Storage();
	StorageAdapter store = StorageAdapter(pkb);
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::If);
	pkb.setStmtType(3, StmtType::Assign);
	pkb.setStmtType(4, StmtType::Read);
	pkb.setModifies(4, "x");
	pkb.setStmtType(5, StmtType::Assign);
	pkb.setStmtType(6, StmtType::Assign);
	pkb.setProc("A", 1, 6);
	pkb.setNext(1, 2);
	pkb.setIfNext(2, 3, 4);
	pkb.setIfExit(3, 4, 2);
	pkb.setNext(2, 5);
	pkb.setNext(5, 6);
	pkb.setStmtType(7, StmtType::Assign);
	pkb.setStmtType(8, StmtType::Assign);
	pkb.setStmtType(9, StmtType::Call);
	pkb.setCall(9, "A");
	pkb.setStmtType(10, StmtType::Assign);
	pkb.setProc("B", 7, 10);

	vector<string> assign_token1 = {"3", "+", "y"};
	QueryExpressionLexer lexer1 = QueryExpressionLexer(assign_token1);
	auto expression1 = ExpressionParser{lexer1, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(1, "x", expression1);
	pkb.setModifies(1, "x");
	pkb.setUses(1, "y");
	vector<string> assign_token2 = {"4", "*", "x"};
	QueryExpressionLexer lexer2 = QueryExpressionLexer(assign_token2);
	auto expression2 = ExpressionParser{lexer2, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(3, "y", expression2);
	pkb.setModifies(3, "y");
	pkb.setUses(3, "x");
	vector<string> assign_token3 = {"x", "/", "3"};
	QueryExpressionLexer lexer3 = QueryExpressionLexer(assign_token3);
	auto expression3 = ExpressionParser{lexer3, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(5, "y", expression3);
	pkb.setModifies(5, "y");
	pkb.setUses(5, "x");
	vector<string> assign_token4 = {"y", "-", "2"};
	QueryExpressionLexer lexer4 = QueryExpressionLexer(assign_token4);
	auto expression4 = ExpressionParser{lexer4, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(6, "x", expression4);
	pkb.setModifies(6, "x");
	pkb.setUses(6, "y");
	vector<string> assign_token5 = {"8", "*", "y"};
	QueryExpressionLexer lexer5 = QueryExpressionLexer(assign_token5);
	auto expression5 = ExpressionParser{lexer5, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(7, "x", expression5);
	pkb.setModifies(7, "x");
	pkb.setUses(7, "y");
	vector<string> assign_token6 = {"100"};
	QueryExpressionLexer lexer6 = QueryExpressionLexer(assign_token6);
	auto expression6 = ExpressionParser{lexer6, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(8, "z", expression6);
	pkb.setModifies(8, "z");
	vector<string> assign_token7 = {"x", "+", "10"};
	QueryExpressionLexer lexer7 = QueryExpressionLexer(assign_token7);
	auto expression7 = ExpressionParser{lexer7, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(10, "y", expression7);
	pkb.setModifies(10, "y");
	pkb.setUses(10, "x");

	ClauseArgument stmt_no1 = ClauseArgument(1);
	ClauseArgument stmt_no3 = ClauseArgument(3);
	ClauseArgument stmt_no5 = ClauseArgument(5);
	ClauseArgument stmt_no6 = ClauseArgument(6);
	ClauseArgument stmt_no7 = ClauseArgument(7);
	ClauseArgument stmt_no8 = ClauseArgument(8);
	ClauseArgument stmt_no10 = ClauseArgument(10);
	ClauseArgument assign_synonym = ClauseArgument(Declaration{DesignEntity::Assign, "a"});
	ClauseArgument assign_synonym2 = ClauseArgument(Declaration{DesignEntity::Assign, "a1"});

	SECTION("Trivial: Index & Index") {
		QueryResult result1 = executeTrivialIndexIndex<ClauseType::Affects>(store, stmt_no1, stmt_no3);
		QueryResult result2 = executeTrivialIndexIndex<ClauseType::Affects>(store, stmt_no1, stmt_no5);
		QueryResult result3 = executeTrivialIndexIndex<ClauseType::Affects>(store, stmt_no3, stmt_no6);
		QueryResult result4 = executeTrivialIndexIndex<ClauseType::Affects>(store, stmt_no8, stmt_no10);
		QueryResult result5 = executeTrivialIndexIndex<ClauseType::Affects>(store, stmt_no7, stmt_no10);

		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE_FALSE(result3.getResult());
		REQUIRE_FALSE(result4.getResult());
		REQUIRE_FALSE(result5.getResult());
	}

	SECTION("Trivial: Index & Wildcard") {
		QueryResult result1 = executeTrivialIndexWildcard<ClauseType::Affects>(store, stmt_no1);
		QueryResult result2 = executeTrivialIndexWildcard<ClauseType::Affects>(store, stmt_no1);
		QueryResult result3 = executeTrivialIndexWildcard<ClauseType::Affects>(store, stmt_no3);
		QueryResult result4 = executeTrivialIndexWildcard<ClauseType::Affects>(store, stmt_no8);
		QueryResult result5 = executeTrivialIndexWildcard<ClauseType::Affects>(store, stmt_no7);

		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE_FALSE(result3.getResult());
		REQUIRE_FALSE(result4.getResult());
		REQUIRE_FALSE(result5.getResult());
	}

	SECTION("Trivial: Index & Synonym") {
		QueryResult result1 = executeTrivialIndexSynonym<ClauseType::Affects>(store, stmt_no1, assign_synonym);
		QueryResult result2 = executeTrivialIndexSynonym<ClauseType::Affects>(store, stmt_no1, assign_synonym);
		QueryResult result3 = executeTrivialIndexSynonym<ClauseType::Affects>(store, stmt_no3, assign_synonym);
		QueryResult result4 = executeTrivialIndexSynonym<ClauseType::Affects>(store, stmt_no8, assign_synonym);
		QueryResult result5 = executeTrivialIndexSynonym<ClauseType::Affects>(store, stmt_no7, assign_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE_FALSE(result3.getResult());
		REQUIRE_FALSE(result4.getResult());
		REQUIRE_FALSE(result5.getResult());
	}

	SECTION("Trivial: Wildcard & Index") {
		QueryResult result1 = executeTrivialWildcardIndex<ClauseType::Affects>(store, stmt_no3);
		QueryResult result2 = executeTrivialWildcardIndex<ClauseType::Affects>(store, stmt_no5);
		QueryResult result3 = executeTrivialWildcardIndex<ClauseType::Affects>(store, stmt_no6);
		QueryResult result4 = executeTrivialWildcardIndex<ClauseType::Affects>(store, stmt_no7);
		QueryResult result5 = executeTrivialWildcardIndex<ClauseType::Affects>(store, stmt_no10);

		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE(result3.getResult());
		REQUIRE_FALSE(result4.getResult());
		REQUIRE_FALSE(result5.getResult());
	}

	SECTION("Trivial: Synonym & Index") {
		QueryResult result1 = executeTrivialSynonymIndex<ClauseType::Affects>(store, assign_synonym, stmt_no3);
		QueryResult result2 = executeTrivialSynonymIndex<ClauseType::Affects>(store, assign_synonym, stmt_no5);
		QueryResult result3 = executeTrivialSynonymIndex<ClauseType::Affects>(store, assign_synonym, stmt_no6);
		QueryResult result4 = executeTrivialSynonymIndex<ClauseType::Affects>(store, assign_synonym, stmt_no7);
		QueryResult result5 = executeTrivialSynonymIndex<ClauseType::Affects>(store, assign_synonym, stmt_no10);

		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE(result3.getResult());
		REQUIRE_FALSE(result4.getResult());
		REQUIRE_FALSE(result5.getResult());
	}

	SECTION("Trivial: Wildcard & Wildcard") {
		QueryResult result1 = executeTrivialWildcardWildcard<ClauseType::Affects>(store);

		REQUIRE(result1.getResult());
	}

	SECTION("Trivial: Wildcard & Synonym") {
		QueryResult result1 = executeTrivialWildcardSynonym<ClauseType::Affects>(store, assign_synonym);

		REQUIRE(result1.getResult());
	}

	SECTION("Trivial: Synonym & Wildcard") {
		QueryResult result1 = executeTrivialSynonymWildcard<ClauseType::Affects>(store, assign_synonym);

		REQUIRE(result1.getResult());
	}

	SECTION("Synonym & Index") {
		QueryResult result1 = executeSynonymIndex<ClauseType::Affects>(store, assign_synonym, stmt_no3);
		vector<string> expected_result_1 = {"1"};
		REQUIRE(result1.getResult());
		REQUIRE(result1.getSynonymResult("a") == expected_result_1);
		QueryResult result2 = executeSynonymIndex<ClauseType::Affects>(store, assign_synonym, stmt_no5);
		vector<string> expected_result_2 = {"1"};
		REQUIRE(result2.getResult());
		REQUIRE(result2.getSynonymResult("a") == expected_result_2);
		QueryResult result3 = executeSynonymIndex<ClauseType::Affects>(store, assign_synonym, stmt_no6);
		vector<string> expected_result_3 = {"5"};
		REQUIRE(result3.getResult());
		REQUIRE(result3.getSynonymResult("a") == expected_result_3);
		QueryResult result4 = executeSynonymIndex<ClauseType::Affects>(store, assign_synonym, stmt_no10);
		REQUIRE_FALSE(result4.getResult());
		QueryResult result5 = executeSynonymIndex<ClauseType::Affects>(store, assign_synonym, stmt_no10);
		REQUIRE_FALSE(result5.getResult());
	}

	SECTION("Index & Synonym") {
		QueryResult result1 = executeIndexSynonym<ClauseType::Affects>(store, stmt_no1, assign_synonym);
		vector<string> expected_result_1 = {"3", "5"};
		REQUIRE(result1.getResult());
		vector<string> actual_result = result1.getSynonymResult("a");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result_1);
		QueryResult result2 = executeIndexSynonym<ClauseType::Affects>(store, stmt_no5, assign_synonym);
		vector<string> expected_result_2 = {"6"};
		REQUIRE(result2.getResult());
		REQUIRE(result2.getSynonymResult("a") == expected_result_2);
		QueryResult result4 = executeIndexSynonym<ClauseType::Affects>(store, stmt_no8, assign_synonym);
		REQUIRE_FALSE(result4.getResult());
		QueryResult result5 = executeIndexSynonym<ClauseType::Affects>(store, stmt_no7, assign_synonym);
		REQUIRE_FALSE(result5.getResult());
	}

	SECTION("Synonym & Wildcard") {
		QueryResult intermediate = QueryResult(vector<string>{"a"});
		intermediate.addRow({"1"});
		intermediate.addRow({"4"});
		intermediate.addRow({"5"});
		QueryResult result1 = executeSynonymWildcardOptimized<ClauseType::Affects>(store, intermediate, assign_synonym);
		vector<string> expected_result_1 = {"1", "5"};
		REQUIRE(result1.getResult());
		vector<string> actual_result = result1.getSynonymResult("a");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result_1);
	}

	SECTION("Wildcard & Synonym") {
		QueryResult intermediate = QueryResult(vector<string>{"a"});
		intermediate.addRow({"3"});
		intermediate.addRow({"5"});
		intermediate.addRow({"6"});
		intermediate.addRow({"1"});
		QueryResult result1 = executeWildcardSynonymOptimized<ClauseType::Affects>(store, intermediate, assign_synonym);
		vector<string> expected_result_1 = {"3", "5", "6"};
		REQUIRE(result1.getResult());
		vector<string> actual_result = result1.getSynonymResult("a");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result_1);
	}

	SECTION("Synonym & Synonym") {
		QueryResult intermediate = QueryResult(vector<string>{"a1"});
		intermediate.addRow({"3"});
		intermediate.addRow({"6"});
		intermediate.addRow({"1"});
		QueryResult result1 = executeSynonymSynonymOptimized<ClauseType::Affects>(store, intermediate, assign_synonym, assign_synonym2);
		REQUIRE(result1.getResult());
		vector<string> expected_result_1 = {"3", "6"};
		vector<string> actual_result_1 = result1.getSynonymResult("a1");
		sort(actual_result_1.begin(), actual_result_1.end());
		REQUIRE(actual_result_1 == expected_result_1);
		vector<string> expected_result_2 = {"1", "5"};
		vector<string> actual_result_2 = result1.getSynonymResult("a");
		sort(actual_result_2.begin(), actual_result_2.end());
		REQUIRE(actual_result_2 == expected_result_2);

		intermediate = QueryResult(vector<string>{"a"});
		intermediate.addRow({"1"});
		intermediate.addRow({"4"});
		result1 = executeSynonymSynonymOptimized<ClauseType::Affects>(store, intermediate, assign_synonym, assign_synonym2);
		REQUIRE(result1.getResult());
		expected_result_1 = {"3", "5"};
		actual_result_1 = result1.getSynonymResult("a1");
		sort(actual_result_1.begin(), actual_result_1.end());
		REQUIRE(actual_result_1 == expected_result_1);
		expected_result_2 = {"1", "1"};
		actual_result_2 = result1.getSynonymResult("a");
		sort(actual_result_2.begin(), actual_result_2.end());
		REQUIRE(actual_result_2 == expected_result_2);
	}
}
