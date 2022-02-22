#include "QP/Relationship/Pattern.h"

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/ExpressionProcessor/OperatorAcceptor.h"
#include "Common/TypeDefs.h"
#include "PKB/Storage.h"
#include "QP/QueryExpressionLexer.h"
#include "catch.hpp"

TEST_CASE("QP::Relationship::Pattern::getDeclarationSymbols") {
	Declaration synAssign = {DesignEntity::Assign, "a"};

	PKB::Storage pkb = PKB::Storage();
	QueryEntRef x = {EntRefType::VarName, "x"};
	QueryEntRef var = {EntRefType::Synonym, "var"};
	QueryEntRef varUnderscore = {EntRefType::Underscore, "_"};
	vector<string> queryToken = {"0"};
	QP::QueryExpressionLexer lexer = QP::QueryExpressionLexer(queryToken);
	auto queryExpression = Common::ExpressionProcessor::Expression::parse(lexer, Common::ExpressionProcessor::ExpressionType::Arithmetic);

	QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(synAssign, x, ExpressionType::Underscore, queryExpression);
	QP::Relationship::Pattern pattern2 = QP::Relationship::Pattern(synAssign, var, ExpressionType::Underscore, queryExpression);
	QP::Relationship::Pattern pattern3 = QP::Relationship::Pattern(synAssign, varUnderscore, ExpressionType::Underscore, queryExpression);

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

	vector<string> assignToken1 = {"1"};
	QP::QueryExpressionLexer lexer1 = QP::QueryExpressionLexer(assignToken1);
	auto expression1 = Common::ExpressionProcessor::Expression::parse(lexer1, Common::ExpressionProcessor::ExpressionType::Arithmetic);
	pkb.setAssign(1, "x", expression1);
	vector<string> assignToken2 = {"x", "+", "1"};
	QP::QueryExpressionLexer lexer2 = QP::QueryExpressionLexer(assignToken2);
	auto expression2 = Common::ExpressionProcessor::Expression::parse(lexer2, Common::ExpressionProcessor::ExpressionType::Arithmetic);
	pkb.setAssign(2, "y", expression2);
	vector<string> assignToken3 = {"x", "+", "y"};
	QP::QueryExpressionLexer lexer3 = QP::QueryExpressionLexer(assignToken3);
	auto expression3 = Common::ExpressionProcessor::Expression::parse(lexer3, Common::ExpressionProcessor::ExpressionType::Arithmetic);
	pkb.setAssign(3, "y", expression3);

	pkb.setModifies(1, "x");
	pkb.setModifies(2, "y");
	pkb.setModifies(3, "y");

	unordered_set<ConstVal> constants = {0, 1};
	pkb.setConstant(constants);

	unordered_map<string, DesignEntity> map;
	map.insert({"s", DesignEntity::Stmt});
	map.insert({"a", DesignEntity::Assign});
	map.insert({"var", DesignEntity::Variable});

	Declaration synAssign = {DesignEntity::Assign, "a"};

	QueryEntRef x = {EntRefType::VarName, "x"};
	QueryEntRef y = {EntRefType::VarName, "y"};
	QueryEntRef z = {EntRefType::VarName, "z"};
	QueryEntRef var = {EntRefType::Synonym, "var"};
	QueryEntRef varUnderscore = {EntRefType::Underscore, "x"};

	vector<string> queryToken1 = {"1"};
	QP::QueryExpressionLexer lexer4 = QP::QueryExpressionLexer(queryToken1);
	auto queryExpression1 = Common::ExpressionProcessor::Expression::parse(lexer4, Common::ExpressionProcessor::ExpressionType::Arithmetic);
	vector<string> queryToken2 = {"0"};
	QP::QueryExpressionLexer lexer5 = QP::QueryExpressionLexer(queryToken2);
	auto queryExpression2 = Common::ExpressionProcessor::Expression::parse(lexer5, Common::ExpressionProcessor::ExpressionType::Arithmetic);
	vector<string> queryToken3 = {"x"};
	QP::QueryExpressionLexer lexer6 = QP::QueryExpressionLexer(queryToken3);
	auto queryExpression3 = Common::ExpressionProcessor::Expression::parse(lexer6, Common::ExpressionProcessor::ExpressionType::Arithmetic);

	SECTION("trivial: _, _") {
		QP::Relationship::Pattern pattern1 =
			QP::Relationship::Pattern(synAssign, varUnderscore, ExpressionType::Underscore, queryExpression1);
		QP::QueryResult result1 = pattern1.execute(pkb, true, map);
		REQUIRE(result1.getResult());
	}

	SECTION("trivial: _, expr") {
		QP::Relationship::Pattern pattern1 =
			QP::Relationship::Pattern(synAssign, varUnderscore, ExpressionType::Expression, queryExpression1);
		QP::Relationship::Pattern pattern2 =
			QP::Relationship::Pattern(synAssign, varUnderscore, ExpressionType::Expression, queryExpression2);
		QP::Relationship::Pattern pattern3 =
			QP::Relationship::Pattern(synAssign, varUnderscore, ExpressionType::Expression, queryExpression3);
		QP::QueryResult result1 = pattern1.execute(pkb, true, map);
		QP::QueryResult result2 = pattern2.execute(pkb, true, map);
		QP::QueryResult result3 = pattern3.execute(pkb, true, map);
		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
		REQUIRE(!result3.getResult());
	}

	SECTION("trivial: _, _expr_") {
		QP::Relationship::Pattern pattern1 =
			QP::Relationship::Pattern(synAssign, varUnderscore, ExpressionType::ExpressionUnderscore, queryExpression1);
		QP::Relationship::Pattern pattern2 =
			QP::Relationship::Pattern(synAssign, varUnderscore, ExpressionType::ExpressionUnderscore, queryExpression2);
		QP::Relationship::Pattern pattern3 =
			QP::Relationship::Pattern(synAssign, varUnderscore, ExpressionType::ExpressionUnderscore, queryExpression3);
		QP::QueryResult result1 = pattern1.execute(pkb, true, map);
		QP::QueryResult result2 = pattern2.execute(pkb, true, map);
		QP::QueryResult result3 = pattern3.execute(pkb, true, map);
		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
		REQUIRE(result3.getResult());
	}

	SECTION("trivial: varName, _") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(synAssign, x, ExpressionType::Underscore, queryExpression1);
		QP::Relationship::Pattern pattern2 = QP::Relationship::Pattern(synAssign, y, ExpressionType::Underscore, queryExpression1);
		QP::Relationship::Pattern pattern3 = QP::Relationship::Pattern(synAssign, z, ExpressionType::Underscore, queryExpression1);
		QP::QueryResult result1 = pattern1.execute(pkb, true, map);
		QP::QueryResult result2 = pattern2.execute(pkb, true, map);
		QP::QueryResult result3 = pattern3.execute(pkb, true, map);
		REQUIRE(result1.getResult());
		REQUIRE(result2.getResult());
		REQUIRE(!result3.getResult());
	}

	SECTION("trivial: varName, expr") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(synAssign, x, ExpressionType::Expression, queryExpression1);
		QP::Relationship::Pattern pattern2 = QP::Relationship::Pattern(synAssign, x, ExpressionType::Expression, queryExpression2);
		QP::Relationship::Pattern pattern3 = QP::Relationship::Pattern(synAssign, x, ExpressionType::Expression, queryExpression3);
		QP::Relationship::Pattern pattern4 = QP::Relationship::Pattern(synAssign, y, ExpressionType::Expression, queryExpression1);
		QP::Relationship::Pattern pattern5 = QP::Relationship::Pattern(synAssign, y, ExpressionType::Expression, queryExpression2);
		QP::Relationship::Pattern pattern6 = QP::Relationship::Pattern(synAssign, y, ExpressionType::Expression, queryExpression3);
		QP::Relationship::Pattern pattern7 = QP::Relationship::Pattern(synAssign, z, ExpressionType::Expression, queryExpression1);
		QP::Relationship::Pattern pattern8 = QP::Relationship::Pattern(synAssign, z, ExpressionType::Expression, queryExpression2);
		QP::Relationship::Pattern pattern9 = QP::Relationship::Pattern(synAssign, z, ExpressionType::Expression, queryExpression3);
		QP::QueryResult result1 = pattern1.execute(pkb, true, map);
		QP::QueryResult result2 = pattern2.execute(pkb, true, map);
		QP::QueryResult result3 = pattern3.execute(pkb, true, map);
		QP::QueryResult result4 = pattern4.execute(pkb, true, map);
		QP::QueryResult result5 = pattern5.execute(pkb, true, map);
		QP::QueryResult result6 = pattern6.execute(pkb, true, map);
		QP::QueryResult result7 = pattern7.execute(pkb, true, map);
		QP::QueryResult result8 = pattern8.execute(pkb, true, map);
		QP::QueryResult result9 = pattern9.execute(pkb, true, map);
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
		QP::Relationship::Pattern pattern1 =
			QP::Relationship::Pattern(synAssign, x, ExpressionType::ExpressionUnderscore, queryExpression1);
		QP::Relationship::Pattern pattern2 =
			QP::Relationship::Pattern(synAssign, x, ExpressionType::ExpressionUnderscore, queryExpression2);
		QP::Relationship::Pattern pattern3 =
			QP::Relationship::Pattern(synAssign, x, ExpressionType::ExpressionUnderscore, queryExpression3);
		QP::Relationship::Pattern pattern4 =
			QP::Relationship::Pattern(synAssign, y, ExpressionType::ExpressionUnderscore, queryExpression1);
		QP::Relationship::Pattern pattern5 =
			QP::Relationship::Pattern(synAssign, y, ExpressionType::ExpressionUnderscore, queryExpression2);
		QP::Relationship::Pattern pattern6 =
			QP::Relationship::Pattern(synAssign, y, ExpressionType::ExpressionUnderscore, queryExpression3);
		QP::Relationship::Pattern pattern7 =
			QP::Relationship::Pattern(synAssign, z, ExpressionType::ExpressionUnderscore, queryExpression1);
		QP::Relationship::Pattern pattern8 =
			QP::Relationship::Pattern(synAssign, z, ExpressionType::ExpressionUnderscore, queryExpression2);
		QP::Relationship::Pattern pattern9 =
			QP::Relationship::Pattern(synAssign, z, ExpressionType::ExpressionUnderscore, queryExpression3);
		QP::QueryResult result1 = pattern1.execute(pkb, true, map);
		QP::QueryResult result2 = pattern2.execute(pkb, true, map);
		QP::QueryResult result3 = pattern3.execute(pkb, true, map);
		QP::QueryResult result4 = pattern4.execute(pkb, true, map);
		QP::QueryResult result5 = pattern5.execute(pkb, true, map);
		QP::QueryResult result6 = pattern6.execute(pkb, true, map);
		QP::QueryResult result7 = pattern7.execute(pkb, true, map);
		QP::QueryResult result8 = pattern8.execute(pkb, true, map);
		QP::QueryResult result9 = pattern9.execute(pkb, true, map);
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
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(synAssign, var, ExpressionType::Underscore, queryExpression1);
		QP::QueryResult result1 = pattern1.execute(pkb, true, map);
		REQUIRE(result1.getResult());
	}

	SECTION("trivial: synonym, expr") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(synAssign, var, ExpressionType::Expression, queryExpression1);
		QP::Relationship::Pattern pattern2 = QP::Relationship::Pattern(synAssign, var, ExpressionType::Expression, queryExpression2);
		QP::Relationship::Pattern pattern3 = QP::Relationship::Pattern(synAssign, var, ExpressionType::Expression, queryExpression3);
		QP::QueryResult result1 = pattern1.execute(pkb, true, map);
		QP::QueryResult result2 = pattern2.execute(pkb, true, map);
		QP::QueryResult result3 = pattern3.execute(pkb, true, map);
		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
		REQUIRE(!result3.getResult());
	}

	SECTION("trivial: synonym, _expr_") {
		QP::Relationship::Pattern pattern1 =
			QP::Relationship::Pattern(synAssign, var, ExpressionType::ExpressionUnderscore, queryExpression1);
		QP::Relationship::Pattern pattern2 =
			QP::Relationship::Pattern(synAssign, var, ExpressionType::ExpressionUnderscore, queryExpression2);
		QP::Relationship::Pattern pattern3 =
			QP::Relationship::Pattern(synAssign, var, ExpressionType::ExpressionUnderscore, queryExpression3);
		QP::QueryResult result1 = pattern1.execute(pkb, true, map);
		QP::QueryResult result2 = pattern2.execute(pkb, true, map);
		QP::QueryResult result3 = pattern3.execute(pkb, true, map);
		REQUIRE(result1.getResult());
		REQUIRE(!result2.getResult());
		REQUIRE(result3.getResult());
	}

	SECTION("non-trivial: _, _") {
		QP::Relationship::Pattern pattern1 =
			QP::Relationship::Pattern(synAssign, varUnderscore, ExpressionType::Underscore, queryExpression1);
		QP::QueryResult result1 = pattern1.execute(pkb, false, map);
		vector<string> expectedResult = {"1", "2", "3"};
		REQUIRE(result1.getResult());
		auto result1vec = result1.getSynonymResult("a");
		std::sort(result1vec.begin(), result1vec.end());
		REQUIRE(result1vec == expectedResult);
	}

	SECTION("non-trivial: _, expr") {
		QP::Relationship::Pattern pattern1 =
			QP::Relationship::Pattern(synAssign, varUnderscore, ExpressionType::Expression, queryExpression1);
		QP::Relationship::Pattern pattern2 =
			QP::Relationship::Pattern(synAssign, varUnderscore, ExpressionType::Expression, queryExpression2);
		QP::Relationship::Pattern pattern3 =
			QP::Relationship::Pattern(synAssign, varUnderscore, ExpressionType::Expression, queryExpression3);
		QP::QueryResult result1 = pattern1.execute(pkb, false, map);
		QP::QueryResult result2 = pattern2.execute(pkb, false, map);
		QP::QueryResult result3 = pattern3.execute(pkb, false, map);
		vector<string> expectedResult1 = {"1"};
		REQUIRE(result1.getResult());
		REQUIRE(result1.getSynonymResult("a") == expectedResult1);
		REQUIRE(!result2.getResult());
		REQUIRE(!result3.getResult());
	}

	SECTION("non-trivial: _, _expr_") {
		QP::Relationship::Pattern pattern1 =
			QP::Relationship::Pattern(synAssign, varUnderscore, ExpressionType::ExpressionUnderscore, queryExpression1);
		QP::Relationship::Pattern pattern2 =
			QP::Relationship::Pattern(synAssign, varUnderscore, ExpressionType::ExpressionUnderscore, queryExpression2);
		QP::Relationship::Pattern pattern3 =
			QP::Relationship::Pattern(synAssign, varUnderscore, ExpressionType::ExpressionUnderscore, queryExpression3);
		QP::QueryResult result1 = pattern1.execute(pkb, false, map);
		QP::QueryResult result2 = pattern2.execute(pkb, false, map);
		QP::QueryResult result3 = pattern3.execute(pkb, false, map);
		vector<string> expectedResult1 = {"1", "2"};
		vector<string> expectedResult3 = {"2", "3"};
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

	SECTION("non-trivial: varName, _") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(synAssign, x, ExpressionType::Underscore, queryExpression1);
		QP::Relationship::Pattern pattern2 = QP::Relationship::Pattern(synAssign, y, ExpressionType::Underscore, queryExpression1);
		QP::Relationship::Pattern pattern3 = QP::Relationship::Pattern(synAssign, z, ExpressionType::Underscore, queryExpression1);

		QP::QueryResult result1 = pattern1.execute(pkb, false, map);
		QP::QueryResult result2 = pattern2.execute(pkb, false, map);
		QP::QueryResult result3 = pattern3.execute(pkb, false, map);

		vector<string> expectedResult1 = {"1"};
		vector<string> expectedResult2 = {"2", "3"};
		REQUIRE(result1.getResult());
		REQUIRE(result1.getSynonymResult("a") == expectedResult1);
		REQUIRE(result2.getResult());
		auto result2vec = result2.getSynonymResult("a");
		std::sort(result2vec.begin(), result2vec.end());
		REQUIRE(result2vec == expectedResult2);
		REQUIRE(!result3.getResult());
	}

	SECTION("non-trivial: varName, expr") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(synAssign, x, ExpressionType::Expression, queryExpression1);
		QP::Relationship::Pattern pattern2 = QP::Relationship::Pattern(synAssign, x, ExpressionType::Expression, queryExpression2);
		QP::Relationship::Pattern pattern3 = QP::Relationship::Pattern(synAssign, x, ExpressionType::Expression, queryExpression3);
		QP::Relationship::Pattern pattern4 = QP::Relationship::Pattern(synAssign, y, ExpressionType::Expression, queryExpression1);
		QP::Relationship::Pattern pattern5 = QP::Relationship::Pattern(synAssign, y, ExpressionType::Expression, queryExpression2);
		QP::Relationship::Pattern pattern6 = QP::Relationship::Pattern(synAssign, y, ExpressionType::Expression, queryExpression3);
		QP::Relationship::Pattern pattern7 = QP::Relationship::Pattern(synAssign, z, ExpressionType::Expression, queryExpression1);
		QP::Relationship::Pattern pattern8 = QP::Relationship::Pattern(synAssign, z, ExpressionType::Expression, queryExpression2);
		QP::Relationship::Pattern pattern9 = QP::Relationship::Pattern(synAssign, z, ExpressionType::Expression, queryExpression3);
		QP::QueryResult result1 = pattern1.execute(pkb, false, map);
		QP::QueryResult result2 = pattern2.execute(pkb, false, map);
		QP::QueryResult result3 = pattern3.execute(pkb, false, map);
		QP::QueryResult result4 = pattern4.execute(pkb, false, map);
		QP::QueryResult result5 = pattern5.execute(pkb, false, map);
		QP::QueryResult result6 = pattern6.execute(pkb, false, map);
		QP::QueryResult result7 = pattern7.execute(pkb, false, map);
		QP::QueryResult result8 = pattern8.execute(pkb, false, map);
		QP::QueryResult result9 = pattern9.execute(pkb, false, map);
		vector<string> expectedResult1 = {"1"};
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

	SECTION("non-trivial: varName, _expr_") {
		QP::Relationship::Pattern pattern1 =
			QP::Relationship::Pattern(synAssign, x, ExpressionType::ExpressionUnderscore, queryExpression1);
		QP::Relationship::Pattern pattern2 =
			QP::Relationship::Pattern(synAssign, x, ExpressionType::ExpressionUnderscore, queryExpression2);
		QP::Relationship::Pattern pattern3 =
			QP::Relationship::Pattern(synAssign, x, ExpressionType::ExpressionUnderscore, queryExpression3);
		QP::Relationship::Pattern pattern4 =
			QP::Relationship::Pattern(synAssign, y, ExpressionType::ExpressionUnderscore, queryExpression1);
		QP::Relationship::Pattern pattern5 =
			QP::Relationship::Pattern(synAssign, y, ExpressionType::ExpressionUnderscore, queryExpression2);
		QP::Relationship::Pattern pattern6 =
			QP::Relationship::Pattern(synAssign, y, ExpressionType::ExpressionUnderscore, queryExpression3);
		QP::Relationship::Pattern pattern7 =
			QP::Relationship::Pattern(synAssign, z, ExpressionType::ExpressionUnderscore, queryExpression1);
		QP::Relationship::Pattern pattern8 =
			QP::Relationship::Pattern(synAssign, z, ExpressionType::ExpressionUnderscore, queryExpression2);
		QP::Relationship::Pattern pattern9 =
			QP::Relationship::Pattern(synAssign, z, ExpressionType::ExpressionUnderscore, queryExpression3);
		QP::QueryResult result1 = pattern1.execute(pkb, false, map);
		QP::QueryResult result2 = pattern2.execute(pkb, false, map);
		QP::QueryResult result3 = pattern3.execute(pkb, false, map);
		QP::QueryResult result4 = pattern4.execute(pkb, false, map);
		QP::QueryResult result5 = pattern5.execute(pkb, false, map);
		QP::QueryResult result6 = pattern6.execute(pkb, false, map);
		QP::QueryResult result7 = pattern7.execute(pkb, false, map);
		QP::QueryResult result8 = pattern8.execute(pkb, false, map);
		QP::QueryResult result9 = pattern9.execute(pkb, false, map);
		vector<string> expectedResult1 = {"1"};
		vector<string> expectedResult4 = {"2"};
		vector<string> expectedResult6 = {"2", "3"};
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

	SECTION("non-trivial: synonym, _") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(synAssign, var, ExpressionType::Underscore, queryExpression1);
		QP::QueryResult result1 = pattern1.execute(pkb, false, map);
		vector<string> expectedResult1 = {"1", "2", "3"};
		vector<string> expectedResultVar1 = {"x", "y", "y"};
		REQUIRE(result1.getResult());
		auto result1vec1 = result1.getSynonymResult("a");
		std::sort(result1vec1.begin(), result1vec1.end());
		auto result1vec2 = result1.getSynonymResult("var");
		std::sort(result1vec2.begin(), result1vec2.end());
		REQUIRE(result1vec1 == expectedResult1);
		REQUIRE(result1vec2 == expectedResultVar1);
	}

	SECTION("non-trivial: synonym, expr") {
		QP::Relationship::Pattern pattern1 = QP::Relationship::Pattern(synAssign, var, ExpressionType::Expression, queryExpression1);
		QP::Relationship::Pattern pattern2 = QP::Relationship::Pattern(synAssign, var, ExpressionType::Expression, queryExpression2);
		QP::Relationship::Pattern pattern3 = QP::Relationship::Pattern(synAssign, var, ExpressionType::Expression, queryExpression3);
		QP::QueryResult result1 = pattern1.execute(pkb, false, map);
		QP::QueryResult result2 = pattern2.execute(pkb, false, map);
		QP::QueryResult result3 = pattern3.execute(pkb, false, map);
		vector<string> expectedResult1 = {"1"};
		vector<string> expectedResultVar1 = {"x"};
		REQUIRE(result1.getResult());
		auto result1vec = result1.getSynonymResult("a");
		std::sort(result1vec.begin(), result1vec.end());
		REQUIRE(result1vec == expectedResult1);
		REQUIRE(result1.getSynonymResult("var") == expectedResultVar1);
		REQUIRE(!result2.getResult());
		REQUIRE(!result3.getResult());
	}

	SECTION("non-trivial: synonym, _expr_") {
		QP::Relationship::Pattern pattern1 =
			QP::Relationship::Pattern(synAssign, var, ExpressionType::ExpressionUnderscore, queryExpression1);
		QP::Relationship::Pattern pattern2 =
			QP::Relationship::Pattern(synAssign, var, ExpressionType::ExpressionUnderscore, queryExpression2);
		QP::Relationship::Pattern pattern3 =
			QP::Relationship::Pattern(synAssign, var, ExpressionType::ExpressionUnderscore, queryExpression3);

		QP::QueryResult result1 = pattern1.execute(pkb, false, map);
		QP::QueryResult result2 = pattern2.execute(pkb, false, map);
		QP::QueryResult result3 = pattern3.execute(pkb, false, map);

		vector<string> expectedResult1 = {"1", "2"};
		vector<string> expectedResultVar1 = {"x", "y"};
		vector<string> expectedResult3 = {"2", "3"};
		vector<string> expectedResultVar3 = {"y", "y"};

		REQUIRE(result1.getResult());
		auto result1vec = result1.getSynonymResult("a");
		std::sort(result1vec.begin(), result1vec.end());
		auto result1vec2 = result1.getSynonymResult("var");
		std::sort(result1vec2.begin(), result1vec2.end());
		REQUIRE(result1vec == expectedResult1);
		REQUIRE(result1vec2 == expectedResultVar1);
		REQUIRE(!result2.getResult());
		REQUIRE(result3.getResult());
		auto result3vec = result3.getSynonymResult("a");
		std::sort(result3vec.begin(), result3vec.end());
		REQUIRE(result3vec == expectedResult3);
		REQUIRE(result3.getSynonymResult("var") == expectedResultVar3);
	}
}