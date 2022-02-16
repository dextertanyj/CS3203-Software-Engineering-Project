#include "QP/Relationship/Pattern.h"

#include "PKB/PKB.h"
#include "Common/ExpressionProcessor/Expression.h"
#include "Common/ExpressionProcessor/OperatorAcceptor.h"
#include "Common/TypeDefs.h"
#include "QP/QueryExpressionLexer.h"

#include "catch.hpp"

TEST_CASE("QP::Relationship::Pattern::execute") {
	PKB pkb = PKB();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Assign);
	pkb.setStmtType(3, StmtType::Assign);
	pkb.setStmtType(4, StmtType::Print);

	vector<string> assignToken1 = { "1" };
	QueryExpressionLexer lexer1 = QueryExpressionLexer(assignToken1);
	auto expression1 = Common::ExpressionProcessor::Expression::parse(lexer1, Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic);
	pkb.setAssign(1, "x", expression1);
	vector<string> assignToken2 = { "x", "+", "1" };
	QueryExpressionLexer lexer2 = QueryExpressionLexer(assignToken2);
	auto expression2 = Common::ExpressionProcessor::Expression::parse(lexer2, Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic);
	pkb.setAssign(2, "y", expression2);
	vector<string> assignToken3 = { "x", "+", "y" };
	QueryExpressionLexer lexer3 = QueryExpressionLexer(assignToken3);
	auto expression3 = Common::ExpressionProcessor::Expression::parse(lexer3, Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic);
	pkb.setAssign(3, "y", expression3);

	unordered_set<int> constants = { 0, 1 };
	pkb.setConstant(constants);

	unordered_map<string, DesignEntity> map;
	map.insert({ "s", DesignEntity::stmt });
	map.insert({ "a", DesignEntity::assign });
	map.insert({ "var", DesignEntity::variable });

	Declaration synAssign = { DesignEntity::assign, "a" };


	QueryEntRef x = { EntRefType::varName, "x" };
	QueryEntRef y = { EntRefType::varName, "y" };
	QueryEntRef z = { EntRefType::varName, "z" };
	QueryEntRef var = { EntRefType::synonym, "var" };
	QueryEntRef varUnderscore = { EntRefType::underscore, "x" };

	vector<string> queryToken1 = { "1" };
	QueryExpressionLexer lexer4 = QueryExpressionLexer(queryToken1);
	auto queryExpression1 = Common::ExpressionProcessor::Expression::parse(lexer4, Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic);
	vector<string> queryToken2 = { "0" };
	QueryExpressionLexer lexer5 = QueryExpressionLexer(queryToken2);
	auto queryExpression2 = Common::ExpressionProcessor::Expression::parse(lexer5, Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic);
	vector<string> queryToken3 = { "x" };
	QueryExpressionLexer lexer6 = QueryExpressionLexer(queryToken3);
	auto queryExpression3 = Common::ExpressionProcessor::Expression::parse(lexer6, Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic);

	SECTION("_, _") {
		Pattern pattern1 = Pattern(synAssign, varUnderscore, ExpressionType::underscore, queryExpression1);
		QueryResult result1 = pattern1.execute(pkb, false, map);
		QueryResult result2 = pattern1.execute(pkb, true, map);
		vector<string> expectedResult = { "1", "2", "3" };
		auto result1vec = result1.getSynonymResult("a");
		std::sort(result1vec.begin(), result1vec.end());
		REQUIRE(result1vec == expectedResult);
		auto result2vec = result2.getSynonymResult("a");
		std::sort(result2vec.begin(), result2vec.end());
		REQUIRE(result2vec == expectedResult);
	}

	SECTION("_, expr") {
		Pattern pattern1 = Pattern(synAssign, varUnderscore, ExpressionType::expression, queryExpression1);
		Pattern pattern2 = Pattern(synAssign, varUnderscore, ExpressionType::expression, queryExpression2);
		Pattern pattern3 = Pattern(synAssign, varUnderscore, ExpressionType::expression, queryExpression3);
		QueryResult result1 = pattern1.execute(pkb, false, map);
		QueryResult result2 = pattern2.execute(pkb, false, map);
		QueryResult result3 = pattern3.execute(pkb, false, map);
		vector<string> expectedResult1 = { "1"};
		REQUIRE(result1.getResult());
		REQUIRE(result1.getSynonymResult("a") == expectedResult1);
		REQUIRE(!result2.getResult());
		REQUIRE(!result3.getResult());
	}

	SECTION("_, _expr_") {
		Pattern pattern1 = Pattern(synAssign, varUnderscore, ExpressionType::expressionUnderscore, queryExpression1);
		Pattern pattern2 = Pattern(synAssign, varUnderscore, ExpressionType::expressionUnderscore, queryExpression2);
		Pattern pattern3 = Pattern(synAssign, varUnderscore, ExpressionType::expressionUnderscore, queryExpression3);
		QueryResult result1 = pattern1.execute(pkb, false, map);
		QueryResult result2 = pattern2.execute(pkb, false, map);
		QueryResult result3 = pattern3.execute(pkb, false, map);
		vector<string> expectedResult1 = { "1", "2"};
		vector<string> expectedResult3 = { "2", "3" };
		REQUIRE(result1.getResult());
		auto result1vec = result1.getSynonymResult("a");
		std::sort(result1vec.begin(), result1vec.end());
		REQUIRE(result1vec == expectedResult1);
		REQUIRE(!result2.getResult());
		REQUIRE(result3.getResult());
		auto result3vec = result3.getSynonymResult("a");
		std::sort(result3vec.begin(), result3vec.end());
		REQUIRE(result3vec == expectedResult3);
	}

	SECTION("varName, _") {
		Pattern pattern1 = Pattern(synAssign, x, ExpressionType::underscore, queryExpression1);
		Pattern pattern2 = Pattern(synAssign, y, ExpressionType::underscore, queryExpression1);
		Pattern pattern3 = Pattern(synAssign, z, ExpressionType::underscore, queryExpression1);

		QueryResult result1 = pattern1.execute(pkb, false, map);
		QueryResult result2 = pattern2.execute(pkb, false, map);
		QueryResult result3 = pattern3.execute(pkb, false, map);

		vector<string> expectedResult1 = { "1" };
		vector<string> expectedResult2 = { "2", "3" };
		REQUIRE(result1.getResult());
		REQUIRE(result1.getSynonymResult("a") == expectedResult1);
		REQUIRE(result2.getResult());
		auto result2vec = result2.getSynonymResult("a");
		std::sort(result2vec.begin(), result2vec.end());
		REQUIRE(result2vec == expectedResult2);
		REQUIRE(!result3.getResult());
	}

	SECTION("varName, expr") {
		Pattern pattern1 = Pattern(synAssign, x, ExpressionType::expression, queryExpression1);
		Pattern pattern2 = Pattern(synAssign, x, ExpressionType::expression, queryExpression2);
		Pattern pattern3 = Pattern(synAssign, x, ExpressionType::expression, queryExpression3);
		Pattern pattern4 = Pattern(synAssign, y, ExpressionType::expression, queryExpression1);
		Pattern pattern5 = Pattern(synAssign, y, ExpressionType::expression, queryExpression2);
		Pattern pattern6 = Pattern(synAssign, y, ExpressionType::expression, queryExpression3);
		Pattern pattern7 = Pattern(synAssign, z, ExpressionType::expression, queryExpression1);
		Pattern pattern8 = Pattern(synAssign, z, ExpressionType::expression, queryExpression2);
		Pattern pattern9 = Pattern(synAssign, z, ExpressionType::expression, queryExpression3);
		QueryResult result1 = pattern1.execute(pkb, false, map);
		QueryResult result2 = pattern2.execute(pkb, false, map);
		QueryResult result3 = pattern3.execute(pkb, false, map);
		QueryResult result4 = pattern4.execute(pkb, false, map);
		QueryResult result5 = pattern5.execute(pkb, false, map);
		QueryResult result6 = pattern6.execute(pkb, false, map);
		QueryResult result7 = pattern7.execute(pkb, false, map);
		QueryResult result8 = pattern8.execute(pkb, false, map);
		QueryResult result9 = pattern9.execute(pkb, false, map);
		vector<string> expectedResult1 = { "1" };
		REQUIRE(result1.getResult());
		REQUIRE(result1.getSynonymResult("a") == expectedResult1);
		REQUIRE(!result2.getResult());
		REQUIRE(!result3.getResult());
		REQUIRE(!result4.getResult());
		REQUIRE(!result5.getResult());
		REQUIRE(!result6.getResult());
		REQUIRE(!result7.getResult());
		REQUIRE(!result8.getResult());
		REQUIRE(!result9.getResult());
	}

	SECTION("varName, _expr_") {
		Pattern pattern1 = Pattern(synAssign, x, ExpressionType::expressionUnderscore, queryExpression1);
		Pattern pattern2 = Pattern(synAssign, x, ExpressionType::expressionUnderscore, queryExpression2);
		Pattern pattern3 = Pattern(synAssign, x, ExpressionType::expressionUnderscore, queryExpression3);
		Pattern pattern4 = Pattern(synAssign, y, ExpressionType::expressionUnderscore, queryExpression1);
		Pattern pattern5 = Pattern(synAssign, y, ExpressionType::expressionUnderscore, queryExpression2);
		Pattern pattern6 = Pattern(synAssign, y, ExpressionType::expressionUnderscore, queryExpression3);
		Pattern pattern7 = Pattern(synAssign, z, ExpressionType::expressionUnderscore, queryExpression1);
		Pattern pattern8 = Pattern(synAssign, z, ExpressionType::expressionUnderscore, queryExpression2);
		Pattern pattern9 = Pattern(synAssign, z, ExpressionType::expressionUnderscore, queryExpression3);
		QueryResult result1 = pattern1.execute(pkb, false, map);
		QueryResult result2 = pattern2.execute(pkb, false, map);
		QueryResult result3 = pattern3.execute(pkb, false, map);
		QueryResult result4 = pattern4.execute(pkb, false, map);
		QueryResult result5 = pattern5.execute(pkb, false, map);
		QueryResult result6 = pattern6.execute(pkb, false, map);
		QueryResult result7 = pattern7.execute(pkb, false, map);
		QueryResult result8 = pattern8.execute(pkb, false, map);
		QueryResult result9 = pattern9.execute(pkb, false, map);
		vector<string> expectedResult1 = { "1"};
		vector<string> expectedResult4 = { "2" };
		vector<string> expectedResult6 = { "2", "3" };

		REQUIRE(result1.getResult());
		REQUIRE(result1.getSynonymResult("a") == expectedResult1);
		REQUIRE(!result2.getResult());
		REQUIRE(!result3.getResult());
		REQUIRE(result4.getResult());
		REQUIRE(result4.getSynonymResult("a") == expectedResult4);
		REQUIRE(!result5.getResult());
		REQUIRE(result6.getResult());
		auto result6vec = result6.getSynonymResult("a");
		std::sort(result6vec.begin(), result6vec.end());
		REQUIRE(result6vec == expectedResult6);
		REQUIRE(!result7.getResult());
		REQUIRE(!result8.getResult());
		REQUIRE(!result9.getResult());
	}

	SECTION("synonym, _") {
		Pattern pattern1 = Pattern(synAssign, var, ExpressionType::underscore, queryExpression1);

		QueryResult result1 = pattern1.execute(pkb, false, map);

		vector<string> expectedResult1 = { "1", "2", "3" };

		REQUIRE(result1.getResult());
		auto result1vec1 = result1.getSynonymResult("a");
		std::sort(result1vec1.begin(), result1vec1.end());
		auto result1vec2 = result1.getSynonymResult("var");
		std::sort(result1vec2.begin(), result1vec2.end());
		REQUIRE(result1vec1 == expectedResult1);
		REQUIRE(result1vec1 == result1vec2);
	}

	SECTION("synonym, expr") {
		Pattern pattern1 = Pattern(synAssign, var, ExpressionType::expression, queryExpression1);
		Pattern pattern2 = Pattern(synAssign, var, ExpressionType::expression, queryExpression2);
		Pattern pattern3 = Pattern(synAssign, var, ExpressionType::expression, queryExpression3);
		QueryResult result1 = pattern1.execute(pkb, false, map);
		QueryResult result2 = pattern2.execute(pkb, false, map);
		QueryResult result3 = pattern3.execute(pkb, false, map);
		vector<string> expectedResult1 = { "1" };
		vector<string> expectedResult4 = { "2" };
		vector<string> expectedResult6 = { "2", "3" };

		REQUIRE(result1.getResult());
		auto result1vec = result1.getSynonymResult("a");
		std::sort(result1vec.begin(), result1vec.end());
		REQUIRE(result1vec == expectedResult1);
		REQUIRE(!result2.getResult());
		REQUIRE(!result3.getResult());

	}

	SECTION("synonym, _expr_") {
		Pattern pattern1 = Pattern(synAssign, var, ExpressionType::expressionUnderscore, queryExpression1);
		Pattern pattern2 = Pattern(synAssign, var, ExpressionType::expressionUnderscore, queryExpression2);
		Pattern pattern3 = Pattern(synAssign, var, ExpressionType::expressionUnderscore, queryExpression3);

		QueryResult result1 = pattern1.execute(pkb, false, map);
		QueryResult result2 = pattern2.execute(pkb, false, map);
		QueryResult result3 = pattern3.execute(pkb, false, map);

		vector<string> expectedResult1 = { "1", "2" };
		vector<string> expectedResult3 = { "2", "3" };

		REQUIRE(result1.getResult());
		auto result1vec = result1.getSynonymResult("a");
		std::sort(result1vec.begin(), result1vec.end());
		REQUIRE(result1vec == expectedResult1);
		REQUIRE(!result2.getResult());
		REQUIRE(result3.getResult());
		auto result3vec = result3.getSynonymResult("a");
		std::sort(result3vec.begin(), result3vec.end());
	}

}