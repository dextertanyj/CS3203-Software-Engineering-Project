#include "Common/TypeDefs.h"
#include "Common/ExpressionProcessor/ExpressionParser.h"
#include "PKB/Storage.h"
#include "QP/Executor/StatementExecutor.tpp"
#include "QP/QueryExpressionLexer.h"
#include "catch.hpp"

using namespace QP::Types;
using namespace QP::Executor::StatementExecutor;
using namespace Common::ExpressionProcessor;

TEST_CASE("StatementExecutor<ClauseType::AffectsT>::execute") {
	/*
	 * procedure A {
	 * 	if (x < 0) then {   //1
	 * 		y = 4 * x;      //2
	 * 	} else {
	 * 		read x;         //3
 	 * 		read y;         //4
	 * 	}
	 * }
	 *
	 * procedure B {
	 * 	x = 8 * y;           //5
	 * 	while (y < 0) {      //6
	 *		z = a + 100;     //7
	 *		y = x / 6;       //8
	 *		while (z > 0) {  //9
	 *			a = x + a;   //10
	 *			x = y + 10;  //11
	 *			call A;      //12
	 *			x = y + 10;  //13
	 *			a = z - a;   //14
	 *		}
	 *	}
	 *}
	 */
	PKB::Storage pkb = PKB::Storage();
	QP::StorageAdapter store = QP::StorageAdapter(pkb);
	pkb.setStmtType(1, StmtType::IfStmt);
	pkb.setStmtType(2, StmtType::Assign);
	pkb.setStmtType(3, StmtType::Read);
	pkb.setModifies(3, "x");
	pkb.setStmtType(4, StmtType::Read);
	pkb.setModifies(4, "z");
	pkb.setProc("A", 1, 4);
	pkb.setIfNext(1, 2, 3);
	pkb.setIfExit(2, 3, 1);
	pkb.setStmtType(5, StmtType::Assign);
	pkb.setStmtType(6, StmtType::WhileStmt);
	pkb.setStmtType(7, StmtType::Assign);
	pkb.setStmtType(8, StmtType::Assign);
	pkb.setStmtType(9, StmtType::WhileStmt);
	pkb.setStmtType(10, StmtType::Assign);
	pkb.setStmtType(11, StmtType::Assign);
	pkb.setStmtType(12, StmtType::Call);
	pkb.setCall(12, "A");
	pkb.setStmtType(13, StmtType::Assign);
	pkb.setStmtType(14, StmtType::Assign);
	pkb.setNext(5,6);
	pkb.setNext(6, 7);
	pkb.setNext(7, 8);
	pkb.setNext(8, 9);
	pkb.setNext(9, 10);
	pkb.setNext(10, 11);
	pkb.setNext(11, 12);
	pkb.setNext(12, 13);
	pkb.setNext(13, 14);
	pkb.setNext(14, 9);
	pkb.setNext(9, 6);
	pkb.setProc("B", 5, 14);

	vector<string> assign_token1 = {"4", "*", "x"};
	QP::QueryExpressionLexer lexer1 = QP::QueryExpressionLexer(assign_token1);
	auto expression1 = ExpressionParser{lexer1, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(2, "y", expression1);
	pkb.setModifies(2, "y");
	pkb.setUses(2, "x");
	vector<string> assign_token2 = {"8", "*", "y"};
	QP::QueryExpressionLexer lexer2 = QP::QueryExpressionLexer(assign_token2);
	auto expression2 = ExpressionParser{lexer2, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(5, "x", expression2);
	pkb.setModifies(5, "x");
	pkb.setUses(5, "y");
	vector<string> assign_token3 = {"a", "+", "100"};
	QP::QueryExpressionLexer lexer3 = QP::QueryExpressionLexer(assign_token3);
	auto expression3 = ExpressionParser{lexer3, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(7, "z", expression3);
	pkb.setModifies(7, "z");
	pkb.setUses(7, "a");
	vector<string> assign_token4 = {"x", "/", "6"};
	QP::QueryExpressionLexer lexer4 = QP::QueryExpressionLexer(assign_token4);
	auto expression4 = ExpressionParser{lexer4, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(8, "y", expression4);
	pkb.setModifies(8, "y");
	pkb.setUses(8, "x");
	vector<string> assign_token5 = {"x", "+", "a"};
	QP::QueryExpressionLexer lexer5 = QP::QueryExpressionLexer(assign_token5);
	auto expression5 = ExpressionParser{lexer5, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(10, "a", expression5);
	pkb.setModifies(10, "a");
	pkb.setUses(10, "x");
	pkb.setUses(10, "a");
	vector<string> assign_token6 = {"y", "+", "10"};
	QP::QueryExpressionLexer lexer6 = QP::QueryExpressionLexer(assign_token6);
	auto expression6 = ExpressionParser{lexer6, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(11, "x", expression6);
	pkb.setModifies(11, "x");
	pkb.setUses(11, "y");
	pkb.setAssign(13, "x", expression6);
	pkb.setModifies(13, "x");
	pkb.setUses(13, "y");
	vector<string> assign_token7 = {"z", "-", "a"};
	QP::QueryExpressionLexer lexer7 = QP::QueryExpressionLexer(assign_token7);
	auto expression7 = ExpressionParser{lexer7, ExpressionType::Arithmetic}.parse();
	pkb.setAssign(14, "a", expression7);
	pkb.setModifies(14, "a");
	pkb.setUses(14, "z");
	pkb.setUses(14, "a");
	pkb.populateComplexRelations();

	ReferenceArgument stmt_no5 = ReferenceArgument(5);
	ReferenceArgument stmt_no7 = ReferenceArgument(7);
	ReferenceArgument stmt_no8 = ReferenceArgument(8);
	ReferenceArgument stmt_no11 = ReferenceArgument(11);
	ReferenceArgument stmt_no13 = ReferenceArgument(13);
	ReferenceArgument stmt_no14 = ReferenceArgument(14);
	ReferenceArgument assign_synonym = ReferenceArgument(Declaration{DesignEntity::Assign, "a"});
	ReferenceArgument assign_synonym2 = ReferenceArgument(Declaration{DesignEntity::Assign, "a1"});

	SECTION("Trivial: Index & Index") {
		QP::QueryResult result1 = executeTrivialIndexIndex<ClauseType::AffectsT>(store, stmt_no5, stmt_no11);
		QP::QueryResult result2 = executeTrivialIndexIndex<ClauseType::AffectsT>(store, stmt_no5, stmt_no14);
		QP::QueryResult result3 = executeTrivialIndexIndex<ClauseType::AffectsT>(store, stmt_no14, stmt_no7);
		QP::QueryResult result4 = executeTrivialIndexIndex<ClauseType::AffectsT>(store, stmt_no5, stmt_no13);

		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE(result3.getResult());
		REQUIRE_FALSE(result4.getResult());
	}

	SECTION("Trivial: Index & Wildcard") {
		QP::QueryResult result1 = executeTrivialIndexWildcard<ClauseType::AffectsT>(store, stmt_no5);
		QP::QueryResult result2 = executeTrivialIndexWildcard<ClauseType::AffectsT>(store, stmt_no14);
		QP::QueryResult result3 = executeTrivialIndexWildcard<ClauseType::AffectsT>(store, stmt_no11);

		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE_FALSE(result3.getResult());
	}

	SECTION("Trivial: Index & Synonym") {
		QP::QueryResult result1 = executeTrivialIndexSynonym<ClauseType::AffectsT>(store, stmt_no5, assign_synonym);
		QP::QueryResult result2 = executeTrivialIndexSynonym<ClauseType::AffectsT>(store, stmt_no14, assign_synonym);
		QP::QueryResult result3 = executeTrivialIndexSynonym<ClauseType::AffectsT>(store, stmt_no11, assign_synonym);

		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE_FALSE(result3.getResult());
	}

	SECTION("Trivial: Wildcard & Index") {
		QP::QueryResult result1 = executeTrivialWildcardIndex<ClauseType::AffectsT>(store, stmt_no11);
		QP::QueryResult result2 = executeTrivialWildcardIndex<ClauseType::AffectsT>(store, stmt_no14);
		QP::QueryResult result3 = executeTrivialWildcardIndex<ClauseType::AffectsT>(store, stmt_no7);
		QP::QueryResult result4 = executeTrivialWildcardIndex<ClauseType::AffectsT>(store, stmt_no13);

		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE(result3.getResult());
		REQUIRE_FALSE(result4.getResult());

	}

	SECTION("Trivial: Synonym & Index") {
		QP::QueryResult result1 = executeTrivialSynonymIndex<ClauseType::AffectsT>(store, assign_synonym, stmt_no11);
		QP::QueryResult result2 = executeTrivialSynonymIndex<ClauseType::AffectsT>(store, assign_synonym, stmt_no14);
		QP::QueryResult result3 = executeTrivialSynonymIndex<ClauseType::AffectsT>(store, assign_synonym, stmt_no7);
		QP::QueryResult result4 = executeTrivialSynonymIndex<ClauseType::AffectsT>(store, assign_synonym, stmt_no13);

		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE(result3.getResult());
		REQUIRE_FALSE(result4.getResult());
	}

	SECTION("Trivial: Wildcard & Wildcard") {
		QP::QueryResult result1 = executeTrivialWildcardWildcard<ClauseType::AffectsT>(store);

		REQUIRE(result1.getResult());
	}

	SECTION("Trivial: Wildcard & Synonym") {
		QP::QueryResult result1 = executeTrivialWildcardSynonym<ClauseType::AffectsT>(store, assign_synonym);

		REQUIRE(result1.getResult());
	}

	SECTION("Trivial: Synonym & Wildcard") {
		QP::QueryResult result1 = executeTrivialSynonymWildcard<ClauseType::AffectsT>(store, assign_synonym);

		REQUIRE(result1.getResult());
	}

	SECTION("Synonym & Index") {
		QP::QueryResult result1 = executeSynonymIndex<ClauseType::AffectsT>(store, assign_synonym, stmt_no11);
		vector<string> expected_result_1 = {"13", "5", "8"};
		REQUIRE(result1.getResult());
		vector<string> actual_result_1 = result1.getSynonymResult("a");
		sort(actual_result_1.begin(), actual_result_1.end());
		REQUIRE(actual_result_1 == expected_result_1);
		QP::QueryResult result2 = executeSynonymIndex<ClauseType::AffectsT>(store, assign_synonym, stmt_no14);
		vector<string> expected_result_2 = {"10", "13", "14", "5"};
		REQUIRE(result2.getResult());
		vector<string> actual_result_2 = result2.getSynonymResult("a");
		sort(actual_result_2.begin(), actual_result_2.end());
		REQUIRE(actual_result_2 == expected_result_2);
		QP::QueryResult result3 = executeSynonymIndex<ClauseType::AffectsT>(store, assign_synonym, stmt_no8);
		vector<string> expected_result_3 = {"13", "5"};
		REQUIRE(result3.getResult());
		vector<string> actual_result_3 = result3.getSynonymResult("a");
		sort(actual_result_3.begin(), actual_result_3.end());
		REQUIRE(actual_result_3 == expected_result_3);
		QP::QueryResult result4 = executeSynonymIndex<ClauseType::AffectsT>(store, assign_synonym, stmt_no13);
	}

	SECTION("Index & Synonym") {
		QP::QueryResult result1 = executeIndexSynonym<ClauseType::AffectsT>(store, stmt_no5, assign_synonym);
		vector<string> expected_result_1 = {"10", "11", "14", "7", "8"};
		REQUIRE(result1.getResult());
		vector<string> actual_result_1 = result1.getSynonymResult("a");
		sort(actual_result_1.begin(), actual_result_1.end());
		REQUIRE(actual_result_1 == expected_result_1);
		QP::QueryResult result2 = executeIndexSynonym<ClauseType::AffectsT>(store, stmt_no13, assign_synonym);
		vector<string> expected_result_2 = {"10", "11", "14", "7", "8"};
		REQUIRE(result2.getResult());
		vector<string> actual_result_2 = result2.getSynonymResult("a");
		sort(actual_result_2.begin(), actual_result_2.end());
		REQUIRE(actual_result_2 == expected_result_2);
		QP::QueryResult result3 = executeIndexSynonym<ClauseType::AffectsT>(store, stmt_no14, assign_synonym);
		vector<string> expected_result_3 = {"10", "14", "7"};
		REQUIRE(result3.getResult());
		vector<string> actual_result_3 = result3.getSynonymResult("a");
		sort(actual_result_3.begin(), actual_result_3.end());
		REQUIRE(actual_result_3 == expected_result_3);
		QP::QueryResult result4 = executeIndexSynonym<ClauseType::AffectsT>(store, stmt_no11, assign_synonym);
	}

	SECTION("Synonym & Wildcard") {
		QP::QueryResult intermediate = QP::QueryResult(vector<string>{"a"});
		intermediate.addRow({"10"});
		intermediate.addRow({"11"});
		intermediate.addRow({"14"});
		intermediate.addRow({"7"});
		intermediate.addRow({"8"});
		QP::QueryResult result1 = executeSynonymWildcard<ClauseType::AffectsT>(store, assign_synonym);
		vector<string> expected_result_1 = {"10", "13", "14", "5", "8"};
		REQUIRE(result1.getResult());
		vector<string> actual_result_1 = result1.getSynonymResult("a");
		sort(actual_result_1.begin(), actual_result_1.end());
		REQUIRE(actual_result_1 == expected_result_1);
	}

	SECTION("Wildcard & Synonym") {
		QP::QueryResult intermediate = QP::QueryResult(vector<string>{"a"});
		intermediate.addRow({"10"});
		intermediate.addRow({"11"});
		intermediate.addRow({"14"});
		intermediate.addRow({"7"});
		intermediate.addRow({"8"});
		intermediate.addRow({"5"});
		intermediate.addRow({"13"});
		QP::QueryResult result1 = executeWildcardSynonymOptimized<ClauseType::AffectsT>(store, intermediate, assign_synonym);
		vector<string> expected_result_1 = {"10", "11", "14", "7", "8"};
		REQUIRE(result1.getResult());
		vector<string> actual_result_1 = result1.getSynonymResult("a");
		sort(actual_result_1.begin(), actual_result_1.end());
		REQUIRE(actual_result_1 == expected_result_1);
	}

	SECTION("Synonym & Synonym") {
		QP::QueryResult result1 = executeSynonymSynonym<ClauseType::AffectsT>(store, assign_synonym, assign_synonym2);
		REQUIRE(result1.getResult());
		vector<string> expected_result_1 = {"10", "10", "10", "13", "13", "13", "13", "13", "14", "14", "14", "5", "5", "5", "5", "5", "8"};
		vector<string> actual_result_1 = result1.getSynonymResult("a");
		sort(actual_result_1.begin(), actual_result_1.end());
		REQUIRE(actual_result_1 == expected_result_1);
		vector<string> expected_result_2 = {"10", "10", "10", "10", "11", "11", "11", "14", "14", "14", "14", "7", "7", "7", "7", "8", "8"};
		vector<string> actual_result_2 = result1.getSynonymResult("a1");
		sort(actual_result_2.begin(), actual_result_2.end());
		REQUIRE(actual_result_2 == expected_result_2);
	}
}