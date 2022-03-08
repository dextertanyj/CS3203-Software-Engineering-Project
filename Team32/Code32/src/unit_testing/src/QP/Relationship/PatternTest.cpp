#include "QP/Relationship/Pattern.h"

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/ExpressionProcessor/OperatorAcceptor.h"
#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "QP/QueryExpressionLexer.h"
#include "catch.hpp"

TEST_CASE("QP::Relationship::Pattern::getDeclarationSymbols") {
	ReferenceArgument assign = ReferenceArgument({DesignEntity::Assign, "a"});

	PKB::Storage pkb = PKB::Storage();
	ReferenceArgument x = ReferenceArgument("x");
	ReferenceArgument var = ReferenceArgument("var");
	ReferenceArgument wildcard = ReferenceArgument();
	vector<string> query_token = {"0"};
	QP::QueryExpressionLexer lexer = QP::QueryExpressionLexer(query_token);
	auto query_expression = Common::ExpressionProcessor::Expression::parse(lexer, Common::ExpressionProcessor::ExpressionType::Arithmetic);
	ReferenceArgument expression = ReferenceArgument(query_expression, true);

	QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(assign, x, expression);
	QP::Relationship::Pattern pattern2 = QP::Relationship::Pattern(assign, var, expression);
	QP::Relationship::Pattern pattern3 = QP::Relationship::Pattern(assign, wildcard, expression);

	vector<string> symbols1 = {"a"};
	vector<string> symbols2 = {"a", "var"};

	REQUIRE(pattern1.getDeclarationSymbols() == symbols1);
	REQUIRE(pattern2.getDeclarationSymbols() == symbols2);
	REQUIRE(pattern3.getDeclarationSymbols() == symbols1);
}

TEST_CASE("QP::Relationship::Pattern::execute") {
	PKB::Storage pkb = PKB::Storage();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Assign);
	pkb.setStmtType(3, StmtType::Assign);
	pkb.setStmtType(4, StmtType::Print);

	vector<string> assign_token1 = {"1"};
	QP::QueryExpressionLexer lexer1 = QP::QueryExpressionLexer(assign_token1);
	auto expression1 = Common::ExpressionProcessor::Expression::parse(lexer1, Common::ExpressionProcessor::ExpressionType::Arithmetic);
	pkb.setAssign(1, "x", expression1);
	vector<string> assign_token2 = {"x", "+", "1"};
	QP::QueryExpressionLexer lexer2 = QP::QueryExpressionLexer(assign_token2);
	auto expression2 = Common::ExpressionProcessor::Expression::parse(lexer2, Common::ExpressionProcessor::ExpressionType::Arithmetic);
	pkb.setAssign(2, "y", expression2);
	vector<string> assign_token3 = {"x", "+", "y"};
	QP::QueryExpressionLexer lexer3 = QP::QueryExpressionLexer(assign_token3);
	auto expression3 = Common::ExpressionProcessor::Expression::parse(lexer3, Common::ExpressionProcessor::ExpressionType::Arithmetic);
	pkb.setAssign(3, "y", expression3);

	pkb.setModifies(1, "x");
	pkb.setModifies(2, "y");
	pkb.setModifies(3, "y");

	unordered_set<ConstVal> constants = {0, 1};
	pkb.setConstant(constants);

	ReferenceArgument syn_assign = ReferenceArgument({QP::Types::DesignEntity::Assign, "a"});

	ReferenceArgument x = ReferenceArgument("x");
	ReferenceArgument y = ReferenceArgument("y");
	ReferenceArgument z = ReferenceArgument("z");
	ReferenceArgument var = ReferenceArgument("var");
	ReferenceArgument wildcard = ReferenceArgument();

	vector<string> query_token1 = {"1"};
	QP::QueryExpressionLexer lexer4 = QP::QueryExpressionLexer(query_token1);
	auto query_expression1 =
		Common::ExpressionProcessor::Expression::parse(lexer4, Common::ExpressionProcessor::ExpressionType::Arithmetic);
	vector<string> query_token2 = {"0"};
	QP::QueryExpressionLexer lexer5 = QP::QueryExpressionLexer(query_token2);
	auto query_expression2 =
		Common::ExpressionProcessor::Expression::parse(lexer5, Common::ExpressionProcessor::ExpressionType::Arithmetic);
	vector<string> query_token3 = {"x"};
	QP::QueryExpressionLexer lexer6 = QP::QueryExpressionLexer(query_token3);
	auto query_expression3 =
		Common::ExpressionProcessor::Expression::parse(lexer6, Common::ExpressionProcessor::ExpressionType::Arithmetic);

	SECTION("trivial: _, _") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(syn_assign, wildcard, ReferenceArgument());
		QP::QueryResult result1 = pattern1.execute(pkb, true);
		REQUIRE(result1.getResult());
	}

	SECTION("trivial: _, expr") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(syn_assign, wildcard, ReferenceArgument(query_expression1, true));
		QP::Relationship::Pattern pattern2 = QP::Relationship::Pattern(syn_assign, wildcard, ReferenceArgument(query_expression2, true));
		QP::Relationship::Pattern pattern3 = QP::Relationship::Pattern(syn_assign, wildcard, ReferenceArgument(query_expression3, true));
		QP::QueryResult result1 = pattern1.execute(pkb, true);
		QP::QueryResult result2 = pattern2.execute(pkb, true);
		QP::QueryResult result3 = pattern3.execute(pkb, true);
		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
		REQUIRE(!result3.getResult());
	}

	SECTION("trivial: _, _expr_") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(syn_assign, wildcard, ReferenceArgument(query_expression1, false));
		QP::Relationship::Pattern pattern2 = QP::Relationship::Pattern(syn_assign, wildcard, ReferenceArgument(query_expression2, false));
		QP::Relationship::Pattern pattern3 = QP::Relationship::Pattern(syn_assign, wildcard, ReferenceArgument(query_expression3, false));
		QP::QueryResult result1 = pattern1.execute(pkb, true);
		QP::QueryResult result2 = pattern2.execute(pkb, true);
		QP::QueryResult result3 = pattern3.execute(pkb, true);
		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
		REQUIRE(result3.getResult());
	}

	SECTION("trivial: varName, _") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(syn_assign, x, ReferenceArgument());
		QP::Relationship::Pattern pattern2 = QP::Relationship::Pattern(syn_assign, y, ReferenceArgument());
		QP::Relationship::Pattern pattern3 = QP::Relationship::Pattern(syn_assign, z, ReferenceArgument());
		QP::QueryResult result1 = pattern1.execute(pkb, true);
		QP::QueryResult result2 = pattern2.execute(pkb, true);
		QP::QueryResult result3 = pattern3.execute(pkb, true);
		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE(!result3.getResult());
	}

	SECTION("trivial: varName, expr") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(syn_assign, x, ReferenceArgument(query_expression1, true));
		QP::Relationship::Pattern pattern2 = QP::Relationship::Pattern(syn_assign, x, ReferenceArgument(query_expression2, true));
		QP::Relationship::Pattern pattern3 = QP::Relationship::Pattern(syn_assign, x, ReferenceArgument(query_expression3, true));
		QP::Relationship::Pattern pattern4 = QP::Relationship::Pattern(syn_assign, y, ReferenceArgument(query_expression1, true));
		QP::Relationship::Pattern pattern5 = QP::Relationship::Pattern(syn_assign, y, ReferenceArgument(query_expression2, true));
		QP::Relationship::Pattern pattern6 = QP::Relationship::Pattern(syn_assign, y, ReferenceArgument(query_expression3, true));
		QP::Relationship::Pattern pattern7 = QP::Relationship::Pattern(syn_assign, z, ReferenceArgument(query_expression1, true));
		QP::Relationship::Pattern pattern8 = QP::Relationship::Pattern(syn_assign, z, ReferenceArgument(query_expression2, true));
		QP::Relationship::Pattern pattern9 = QP::Relationship::Pattern(syn_assign, z, ReferenceArgument(query_expression3, true));
		QP::QueryResult result1 = pattern1.execute(pkb, true);
		QP::QueryResult result2 = pattern2.execute(pkb, true);
		QP::QueryResult result3 = pattern3.execute(pkb, true);
		QP::QueryResult result4 = pattern4.execute(pkb, true);
		QP::QueryResult result5 = pattern5.execute(pkb, true);
		QP::QueryResult result6 = pattern6.execute(pkb, true);
		QP::QueryResult result7 = pattern7.execute(pkb, true);
		QP::QueryResult result8 = pattern8.execute(pkb, true);
		QP::QueryResult result9 = pattern9.execute(pkb, true);
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

	SECTION("trivial: varName, _expr_") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(syn_assign, x, ReferenceArgument(query_expression1, false));
		QP::Relationship::Pattern pattern2 = QP::Relationship::Pattern(syn_assign, x, ReferenceArgument(query_expression2, false));
		QP::Relationship::Pattern pattern3 = QP::Relationship::Pattern(syn_assign, x, ReferenceArgument(query_expression3, false));
		QP::Relationship::Pattern pattern4 = QP::Relationship::Pattern(syn_assign, y, ReferenceArgument(query_expression1, false));
		QP::Relationship::Pattern pattern5 = QP::Relationship::Pattern(syn_assign, y, ReferenceArgument(query_expression2, false));
		QP::Relationship::Pattern pattern6 = QP::Relationship::Pattern(syn_assign, y, ReferenceArgument(query_expression3, false));
		QP::Relationship::Pattern pattern7 = QP::Relationship::Pattern(syn_assign, z, ReferenceArgument(query_expression1, false));
		QP::Relationship::Pattern pattern8 = QP::Relationship::Pattern(syn_assign, z, ReferenceArgument(query_expression2, false));
		QP::Relationship::Pattern pattern9 = QP::Relationship::Pattern(syn_assign, z, ReferenceArgument(query_expression3, false));
		QP::QueryResult result1 = pattern1.execute(pkb, true);
		QP::QueryResult result2 = pattern2.execute(pkb, true);
		QP::QueryResult result3 = pattern3.execute(pkb, true);
		QP::QueryResult result4 = pattern4.execute(pkb, true);
		QP::QueryResult result5 = pattern5.execute(pkb, true);
		QP::QueryResult result6 = pattern6.execute(pkb, true);
		QP::QueryResult result7 = pattern7.execute(pkb, true);
		QP::QueryResult result8 = pattern8.execute(pkb, true);
		QP::QueryResult result9 = pattern9.execute(pkb, true);
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

	SECTION("trivial: synonym, _") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(syn_assign, var, ReferenceArgument());
		QP::QueryResult result1 = pattern1.execute(pkb, true);
		REQUIRE(result1.getResult());
	}

	SECTION("trivial: synonym, expr") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(syn_assign, var, ReferenceArgument(query_expression1, true));
		QP::Relationship::Pattern pattern2 = QP::Relationship::Pattern(syn_assign, var, ReferenceArgument(query_expression2, true));
		QP::Relationship::Pattern pattern3 = QP::Relationship::Pattern(syn_assign, var, ReferenceArgument(query_expression3, true));
		QP::QueryResult result1 = pattern1.execute(pkb, true);
		QP::QueryResult result2 = pattern2.execute(pkb, true);
		QP::QueryResult result3 = pattern3.execute(pkb, true);
		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
		REQUIRE(!result3.getResult());
	}

	SECTION("trivial: synonym, _expr_") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(syn_assign, var, ReferenceArgument(query_expression1, false));
		QP::Relationship::Pattern pattern2 = QP::Relationship::Pattern(syn_assign, var, ReferenceArgument(query_expression2, false));
		QP::Relationship::Pattern pattern3 = QP::Relationship::Pattern(syn_assign, var, ReferenceArgument(query_expression3, false));
		QP::QueryResult result1 = pattern1.execute(pkb, true);
		QP::QueryResult result2 = pattern2.execute(pkb, true);
		QP::QueryResult result3 = pattern3.execute(pkb, true);
		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
		REQUIRE(result3.getResult());
	}

	SECTION("non-trivial: _, _") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(syn_assign, wildcard, ReferenceArgument());
		QP::QueryResult result1 = pattern1.execute(pkb, false);
		vector<string> expected_result = {"1", "2", "3"};
		REQUIRE(result1.getResult());
		auto result1vec = result1.getSynonymResult("a");
		std::sort(result1vec.begin(), result1vec.end());
		REQUIRE(result1vec == expected_result);
	}

	SECTION("non-trivial: _, expr") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(syn_assign, wildcard, ReferenceArgument(query_expression1, true));
		QP::Relationship::Pattern pattern2 = QP::Relationship::Pattern(syn_assign, wildcard, ReferenceArgument(query_expression2, true));
		QP::Relationship::Pattern pattern3 = QP::Relationship::Pattern(syn_assign, wildcard, ReferenceArgument(query_expression3, true));
		QP::QueryResult result1 = pattern1.execute(pkb, false);
		QP::QueryResult result2 = pattern2.execute(pkb, false);
		QP::QueryResult result3 = pattern3.execute(pkb, false);
		vector<string> expected_result1 = {"1"};
		REQUIRE(result1.getResult());
		REQUIRE(result1.getSynonymResult("a") == expected_result1);
		REQUIRE(!result2.getResult());
		REQUIRE(!result3.getResult());
	}

	SECTION("non-trivial: _, _expr_") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(syn_assign, wildcard, ReferenceArgument(query_expression1, false));
		QP::Relationship::Pattern pattern2 = QP::Relationship::Pattern(syn_assign, wildcard, ReferenceArgument(query_expression2, false));
		QP::Relationship::Pattern pattern3 = QP::Relationship::Pattern(syn_assign, wildcard, ReferenceArgument(query_expression3, false));
		QP::QueryResult result1 = pattern1.execute(pkb, false);
		QP::QueryResult result2 = pattern2.execute(pkb, false);
		QP::QueryResult result3 = pattern3.execute(pkb, false);
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

	SECTION("non-trivial: varName, _") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(syn_assign, x, ReferenceArgument());
		QP::Relationship::Pattern pattern2 = QP::Relationship::Pattern(syn_assign, y, ReferenceArgument());
		QP::Relationship::Pattern pattern3 = QP::Relationship::Pattern(syn_assign, z, ReferenceArgument());

		QP::QueryResult result1 = pattern1.execute(pkb, false);
		QP::QueryResult result2 = pattern2.execute(pkb, false);
		QP::QueryResult result3 = pattern3.execute(pkb, false);

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

	SECTION("non-trivial: varName, expr") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(syn_assign, x, ReferenceArgument(query_expression1, true));
		QP::Relationship::Pattern pattern2 = QP::Relationship::Pattern(syn_assign, x, ReferenceArgument(query_expression2, true));
		QP::Relationship::Pattern pattern3 = QP::Relationship::Pattern(syn_assign, x, ReferenceArgument(query_expression3, true));
		QP::Relationship::Pattern pattern4 = QP::Relationship::Pattern(syn_assign, y, ReferenceArgument(query_expression1, true));
		QP::Relationship::Pattern pattern5 = QP::Relationship::Pattern(syn_assign, y, ReferenceArgument(query_expression2, true));
		QP::Relationship::Pattern pattern6 = QP::Relationship::Pattern(syn_assign, y, ReferenceArgument(query_expression3, true));
		QP::Relationship::Pattern pattern7 = QP::Relationship::Pattern(syn_assign, z, ReferenceArgument(query_expression1, true));
		QP::Relationship::Pattern pattern8 = QP::Relationship::Pattern(syn_assign, z, ReferenceArgument(query_expression2, true));
		QP::Relationship::Pattern pattern9 = QP::Relationship::Pattern(syn_assign, z, ReferenceArgument(query_expression3, true));
		QP::QueryResult result1 = pattern1.execute(pkb, false);
		QP::QueryResult result2 = pattern2.execute(pkb, false);
		QP::QueryResult result3 = pattern3.execute(pkb, false);
		QP::QueryResult result4 = pattern4.execute(pkb, false);
		QP::QueryResult result5 = pattern5.execute(pkb, false);
		QP::QueryResult result6 = pattern6.execute(pkb, false);
		QP::QueryResult result7 = pattern7.execute(pkb, false);
		QP::QueryResult result8 = pattern8.execute(pkb, false);
		QP::QueryResult result9 = pattern9.execute(pkb, false);
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

	SECTION("non-trivial: varName, _expr_") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(syn_assign, x, ReferenceArgument(query_expression1, false));
		QP::Relationship::Pattern pattern2 = QP::Relationship::Pattern(syn_assign, x, ReferenceArgument(query_expression2, false));
		QP::Relationship::Pattern pattern3 = QP::Relationship::Pattern(syn_assign, x, ReferenceArgument(query_expression3, false));
		QP::Relationship::Pattern pattern4 = QP::Relationship::Pattern(syn_assign, y, ReferenceArgument(query_expression1, false));
		QP::Relationship::Pattern pattern5 = QP::Relationship::Pattern(syn_assign, y, ReferenceArgument(query_expression2, false));
		QP::Relationship::Pattern pattern6 = QP::Relationship::Pattern(syn_assign, y, ReferenceArgument(query_expression3, false));
		QP::Relationship::Pattern pattern7 = QP::Relationship::Pattern(syn_assign, z, ReferenceArgument(query_expression1, false));
		QP::Relationship::Pattern pattern8 = QP::Relationship::Pattern(syn_assign, z, ReferenceArgument(query_expression2, false));
		QP::Relationship::Pattern pattern9 = QP::Relationship::Pattern(syn_assign, z, ReferenceArgument(query_expression3, false));
		QP::QueryResult result1 = pattern1.execute(pkb, false);
		QP::QueryResult result2 = pattern2.execute(pkb, false);
		QP::QueryResult result3 = pattern3.execute(pkb, false);
		QP::QueryResult result4 = pattern4.execute(pkb, false);
		QP::QueryResult result5 = pattern5.execute(pkb, false);
		QP::QueryResult result6 = pattern6.execute(pkb, false);
		QP::QueryResult result7 = pattern7.execute(pkb, false);
		QP::QueryResult result8 = pattern8.execute(pkb, false);
		QP::QueryResult result9 = pattern9.execute(pkb, false);
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

	SECTION("non-trivial: synonym, _") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(syn_assign, var, ReferenceArgument());
		QP::QueryResult result1 = pattern1.execute(pkb, false);
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

	SECTION("non-trivial: synonym, expr") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(syn_assign, var, ReferenceArgument(query_expression1, true));
		QP::Relationship::Pattern pattern2 = QP::Relationship::Pattern(syn_assign, var, ReferenceArgument(query_expression2, true));
		QP::Relationship::Pattern pattern3 = QP::Relationship::Pattern(syn_assign, var, ReferenceArgument(query_expression3, true));
		QP::QueryResult result1 = pattern1.execute(pkb, false);
		QP::QueryResult result2 = pattern2.execute(pkb, false);
		QP::QueryResult result3 = pattern3.execute(pkb, false);
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

	SECTION("non-trivial: synonym, _expr_") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(syn_assign, var, ReferenceArgument(query_expression1, false));
		QP::Relationship::Pattern pattern2 = QP::Relationship::Pattern(syn_assign, var, ReferenceArgument(query_expression2, false));
		QP::Relationship::Pattern pattern3 = QP::Relationship::Pattern(syn_assign, var, ReferenceArgument(query_expression3, false));

		QP::QueryResult result1 = pattern1.execute(pkb, false);
		QP::QueryResult result2 = pattern2.execute(pkb, false);
		QP::QueryResult result3 = pattern3.execute(pkb, false);

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