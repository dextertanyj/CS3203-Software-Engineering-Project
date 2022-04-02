#include "Common/TypeDefs.h"
#include "Common/ExpressionProcessor/ExpressionParser.h"
#include "PKB/Storage.h"
#include "QP/Executor/StatementExecutor.tpp"
#include "QP/QueryExpressionLexer.h"
#include "catch.hpp"

using namespace QP::Types;
using namespace QP::Executor::StatementExecutor;
using namespace Common::ExpressionProcessor;

TEST_CASE("StatementExecutor<ClauseType::Affects>::execute") {
	PKB::Storage pkb = PKB::Storage();
	QP::StorageAdapter store = QP::StorageAdapter(pkb);
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::IfStmt);
	pkb.setStmtType(3, StmtType::Assign);
	pkb.setStmtType(4, StmtType::Read);
	pkb.setIfControl(2, "x");
	pkb.setUses(2, "x");
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
	pkb.populateComplexRelations();

	vector<string> assign_token1 = {"3", "+", "y"};
	QP::QueryExpressionLexer lexer1 = QP::QueryExpressionLexer(assign_token1);
	auto expression1 = ExpressionParser{lexer1, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(1, "x", expression1);
	pkb.setModifies(1, "x");
	pkb.setUses(1, "y");
	vector<string> assign_token2 = {"4", "*", "x"};
	QP::QueryExpressionLexer lexer2 = QP::QueryExpressionLexer(assign_token2);
	auto expression2 = ExpressionParser{lexer2, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(3, "y", expression2);
	pkb.setModifies(3, "y");
	pkb.setUses(3, "x");
	vector<string> assign_token3 = {"x", "/", "3"};
	QP::QueryExpressionLexer lexer3 = QP::QueryExpressionLexer(assign_token3);
	auto expression3 = ExpressionParser{lexer3, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(5, "y", expression3);
	pkb.setModifies(5, "y");
	pkb.setUses(5, "x");
	vector<string> assign_token4 = {"y", "-", "2"};
	QP::QueryExpressionLexer lexer4 = QP::QueryExpressionLexer(assign_token4);
	auto expression4 = ExpressionParser{lexer4, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(6, "x", expression4);
	pkb.setModifies(6, "x");
	pkb.setUses(6, "y");
	vector<string> assign_token5 = {"8", "*", "y"};
	QP::QueryExpressionLexer lexer5 = QP::QueryExpressionLexer(assign_token5);
	auto expression5 = ExpressionParser{lexer5, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(7, "x", expression5);
	pkb.setModifies(7, "x");
	pkb.setUses(7, "y");
	vector<string> assign_token6 = {"100"};
	QP::QueryExpressionLexer lexer6 = QP::QueryExpressionLexer(assign_token6);
	auto expression6 = ExpressionParser{lexer6, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(8, "z", expression6);
	pkb.setModifies(8, "z");
	vector<string> assign_token7 = {"x", "+", "10"};
	QP::QueryExpressionLexer lexer7 = QP::QueryExpressionLexer(assign_token7);
	auto expression7 = ExpressionParser{lexer7, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(10, "y", expression7);
	pkb.setModifies(10, "y");
	pkb.setUses(10, "x");

	ReferenceArgument stmt_no1 = ReferenceArgument(1);
	ReferenceArgument stmt_no3 = ReferenceArgument(3);
	ReferenceArgument stmt_no5 = ReferenceArgument(5);
	ReferenceArgument stmt_no6 = ReferenceArgument(6);
	ReferenceArgument stmt_no7 = ReferenceArgument(7);
	ReferenceArgument stmt_no8 = ReferenceArgument(8);
	ReferenceArgument stmt_no10 = ReferenceArgument(10);
	ReferenceArgument assign_synonym = ReferenceArgument(Declaration{DesignEntity::Assign, "a"});
	ReferenceArgument assign_synonym2 = ReferenceArgument(Declaration{DesignEntity::Assign, "a1"});

	SECTION("Trivial: Index & Index") {
		QP::QueryResult result1 = executeTrivialIndexIndex<ClauseType::Affects>(store, stmt_no1, stmt_no3);
		QP::QueryResult result2 = executeTrivialIndexIndex<ClauseType::Affects>(store, stmt_no1, stmt_no5);
		QP::QueryResult result3 = executeTrivialIndexIndex<ClauseType::Affects>(store, stmt_no3, stmt_no6);
		QP::QueryResult result4 = executeTrivialIndexIndex<ClauseType::Affects>(store, stmt_no8, stmt_no10);
		QP::QueryResult result5 = executeTrivialIndexIndex<ClauseType::Affects>(store, stmt_no7, stmt_no10);

		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE_FALSE(result3.getResult());
		REQUIRE_FALSE(result4.getResult());
		REQUIRE_FALSE(result5.getResult());
	}

	SECTION("Trivial: Index & Wildcard") {
		QP::QueryResult result1 = executeTrivialIndexWildcard<ClauseType::Affects>(store, stmt_no1);
		QP::QueryResult result2 = executeTrivialIndexWildcard<ClauseType::Affects>(store, stmt_no1);
		QP::QueryResult result3 = executeTrivialIndexWildcard<ClauseType::Affects>(store, stmt_no3);
		QP::QueryResult result4 = executeTrivialIndexWildcard<ClauseType::Affects>(store, stmt_no8);
		QP::QueryResult result5 = executeTrivialIndexWildcard<ClauseType::Affects>(store, stmt_no7);

		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE_FALSE(result3.getResult());
		REQUIRE_FALSE(result4.getResult());
		REQUIRE_FALSE(result5.getResult());
	}

	SECTION("Trivial: Index & Synonym") {
		QP::QueryResult result1 = executeTrivialIndexSynonym<ClauseType::Affects>(store, stmt_no1, assign_synonym);
		QP::QueryResult result2 = executeTrivialIndexSynonym<ClauseType::Affects>(store, stmt_no1, assign_synonym);
		QP::QueryResult result3 = executeTrivialIndexSynonym<ClauseType::Affects>(store, stmt_no3, assign_synonym);
		QP::QueryResult result4 = executeTrivialIndexSynonym<ClauseType::Affects>(store, stmt_no8, assign_synonym);
		QP::QueryResult result5 = executeTrivialIndexSynonym<ClauseType::Affects>(store, stmt_no7, assign_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE_FALSE(result3.getResult());
		REQUIRE_FALSE(result4.getResult());
		REQUIRE_FALSE(result5.getResult());
	}

	SECTION("Trivial: Wildcard & Index") {
		QP::QueryResult result1 = executeTrivialWildcardIndex<ClauseType::Affects>(store, stmt_no3);
		QP::QueryResult result2 = executeTrivialWildcardIndex<ClauseType::Affects>(store, stmt_no5);
		QP::QueryResult result3 = executeTrivialWildcardIndex<ClauseType::Affects>(store, stmt_no6);
		QP::QueryResult result4 = executeTrivialWildcardIndex<ClauseType::Affects>(store, stmt_no7);
		QP::QueryResult result5 = executeTrivialWildcardIndex<ClauseType::Affects>(store, stmt_no10);

		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE(result3.getResult());
		REQUIRE_FALSE(result4.getResult());
		REQUIRE_FALSE(result5.getResult());
	}

	SECTION("Trivial: Synonym & Index") {
		QP::QueryResult result1 = executeTrivialSynonymIndex<ClauseType::Affects>(store, assign_synonym, stmt_no3);
		QP::QueryResult result2 = executeTrivialSynonymIndex<ClauseType::Affects>(store, assign_synonym, stmt_no5);
		QP::QueryResult result3 = executeTrivialSynonymIndex<ClauseType::Affects>(store, assign_synonym, stmt_no6);
		QP::QueryResult result4 = executeTrivialSynonymIndex<ClauseType::Affects>(store, assign_synonym, stmt_no7);
		QP::QueryResult result5 = executeTrivialSynonymIndex<ClauseType::Affects>(store, assign_synonym, stmt_no10);

		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE(result3.getResult());
		REQUIRE_FALSE(result4.getResult());
		REQUIRE_FALSE(result5.getResult());
	}

	SECTION("Trivial: Wildcard & Wildcard") {
		QP::QueryResult result1 = executeTrivialWildcardWildcard<ClauseType::Next>(store);

		REQUIRE(result1.getResult());
	}

	SECTION("Trivial: Wildcard & Synonym") {
		QP::QueryResult result1 = executeTrivialWildcardSynonym<ClauseType::Next>(store, assign_synonym);

		REQUIRE(result1.getResult());
	}

	SECTION("Trivial: Synonym & Wildcard") {
		QP::QueryResult result1 = executeTrivialSynonymWildcard<ClauseType::Next>(store, assign_synonym);

		REQUIRE(result1.getResult());
	}

	SECTION("Synonym & Index") {
		QP::QueryResult result1 = executeSynonymIndex<ClauseType::Affects>(store, assign_synonym, stmt_no3);
		vector<string> expected_result_1 = {"1"};
		REQUIRE(result1.getResult());
		REQUIRE(result1.getSynonymResult("a") == expected_result_1);
		QP::QueryResult result2 = executeSynonymIndex<ClauseType::Affects>(store, assign_synonym, stmt_no5);
		vector<string> expected_result_2 = {"1"};
		REQUIRE(result2.getResult());
		REQUIRE(result2.getSynonymResult("a") == expected_result_2);
		QP::QueryResult result3 = executeSynonymIndex<ClauseType::Affects>(store, assign_synonym, stmt_no6);
		vector<string> expected_result_3 = {"5"};
		REQUIRE(result3.getResult());
		REQUIRE(result3.getSynonymResult("a") == expected_result_3);
		QP::QueryResult result4 = executeSynonymIndex<ClauseType::Affects>(store, assign_synonym, stmt_no10);
		REQUIRE_FALSE(result4.getResult());
		QP::QueryResult result5 = executeSynonymIndex<ClauseType::Affects>(store, assign_synonym, stmt_no10);
		REQUIRE_FALSE(result5.getResult());
	}

	SECTION("Index & Synonym") {
		QP::QueryResult result1 = executeIndexSynonym<ClauseType::Affects>(store, stmt_no1, assign_synonym);
		vector<string> expected_result_1 = {"3", "5"};
		REQUIRE(result1.getResult());
		vector<string> actual_result = result1.getSynonymResult("a");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result_1);
		QP::QueryResult result2 = executeIndexSynonym<ClauseType::Affects>(store, stmt_no5, assign_synonym);
		vector<string> expected_result_2 = {"6"};
		REQUIRE(result2.getResult());
		REQUIRE(result2.getSynonymResult("a") == expected_result_2);
		QP::QueryResult result4 = executeIndexSynonym<ClauseType::Affects>(store, stmt_no8, assign_synonym);
		REQUIRE_FALSE(result4.getResult());
		QP::QueryResult result5 = executeIndexSynonym<ClauseType::Affects>(store, stmt_no7, assign_synonym);
		REQUIRE_FALSE(result5.getResult());
	}

	SECTION("Synonym & Wildcard") {
		QP::QueryResult result1 = executeSynonymWildcard<ClauseType::Affects>(store, assign_synonym);
		vector<string> expected_result_1 = {"1", "5"};
		REQUIRE(result1.getResult());
		vector<string> actual_result = result1.getSynonymResult("a");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result_1);
	}

	SECTION("Wildcard & Synonym") {
		QP::QueryResult result1 = executeWildcardSynonym<ClauseType::Affects>(store, assign_synonym);
		vector<string> expected_result_1 = {"3", "5", "6"};
		REQUIRE(result1.getResult());
		vector<string> actual_result = result1.getSynonymResult("a");
		sort(actual_result.begin(), actual_result.end());
		REQUIRE(actual_result == expected_result_1);
	}

	SECTION("Synonym & Synonym") {
		QP::QueryResult result1 = executeSynonymSynonym<ClauseType::Affects>(store, assign_synonym, assign_synonym2);
		vector<string> expected_result_1 = {"3", "5", "6"};
		vector<string> expected_result_2 = {"1", "1", "5"};
		REQUIRE(result1.getResult());
		vector<string> actual_result_1 = result1.getSynonymResult("a1");
		vector<string> actual_result_2 = result1.getSynonymResult("a");
		sort(actual_result_1.begin(), actual_result_1.end());
		sort(actual_result_2.begin(), actual_result_2.end());
		REQUIRE(actual_result_1 == expected_result_1);
		REQUIRE(actual_result_2 == expected_result_2);
	}
}
