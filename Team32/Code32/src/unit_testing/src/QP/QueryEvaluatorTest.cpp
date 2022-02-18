#include "QP/QueryEvaluator.h"

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/ExpressionProcessor/OperatorAcceptor.h"
#include "Common/TypeDefs.h"
#include "QP/QueryExpressionLexer.h"
#include "QP/Relationship/Parent.h"
#include "QP/Relationship/ModifiesS.h"

#include "catch.hpp"

TEST_CASE("QP::QueryEvaluator::splitClauses Should split clauses into groups") {
	DeclarationList declarations = {
		{ DesignEntity::Stmt, "s1" },
		{ DesignEntity::Stmt, "s2" },
		{ DesignEntity::Variable, "v" },
		{ DesignEntity::Assign, "a" },
		{ DesignEntity::If, "i" },
	};
	Declaration select = { DesignEntity::Stmt, "s1" };
	QueryStmtRef s1 = { StmtRefType::Synonym, "s1" };
	QueryStmtRef s2 = { StmtRefType::Synonym, "s2" };
	QueryStmtRef a = { StmtRefType::Synonym, "a" };
	QueryStmtRef i = { StmtRefType::Synonym, "i" };
	QueryStmtRef stmtNo1 = { StmtRefType::StmtNumber, "1" };
	QueryStmtRef stmtNo2 = { StmtRefType::StmtNumber, "2" };
	QueryEntRef v = { EntRefType::Synonym, "v" };

	SuchThatClauseList suchThatClauses = {
		{ make_unique<Parent>(s1, s2) },
		{ make_unique<Parent>(a, i) },
		{ make_unique<Parent>(stmtNo1, stmtNo2) },
		{ make_unique<ModifiesS>(a, v) },
	};
	QueryProperties properties = QueryProperties(declarations, select, suchThatClauses, {});
	vector<unordered_set<string>> synonymsInGroup = {
		{"s1", "s2"},
		{"a", "v", "i"},
	};

	PKB pkb = PKB();
	vector<pair<SuchThatClauseList, PatternClauseList>> clausesInGroup =
		QueryEvaluator(pkb).splitClauses(properties, synonymsInGroup);

	REQUIRE(clausesInGroup.size() == 3);
	REQUIRE(clausesInGroup[0].first.size() == 1);
	REQUIRE(clausesInGroup[1].first.size() == 2);
	REQUIRE(clausesInGroup[2].first.size() == 1);
};

TEST_CASE("QP::QueryEvaluator::execute") {
	PKB pkb = PKB();
	pkb.setStmtType(1, StmtType::Assign);
	pkb.setStmtType(2, StmtType::Read);
	pkb.setStmtType(3, StmtType::Assign);
	pkb.setStmtType(4, StmtType::IfStmt);
	pkb.setModifies(1, "x");
	
	unordered_set<ConstVal> constants = { 1 };
	pkb.setConstant(constants);
	
	QueryEvaluator evaluator = QueryEvaluator(pkb);
	
	DeclarationList declarations = {
		{ DesignEntity::Stmt, "s1" },
		{ DesignEntity::Stmt, "s2" },
		{ DesignEntity::Variable, "v" },
		{ DesignEntity::Assign, "a" },
	};
	Declaration assignSynonym = { DesignEntity::Assign, "a" };
	Declaration varSynonym = { DesignEntity::Variable, "v" };
	Declaration stmtSynonym = { DesignEntity::Stmt, "s1" };
	QueryEntRef v = { EntRefType::Synonym, "v" };
	QueryEntRef varUnderscore = { EntRefType::Underscore, "_" };
	QueryStmtRef a = { StmtRefType::Synonym, "a" };
	QueryStmtRef stmtNo1 = { StmtRefType::StmtNumber, "1" };

	vector<string> assignToken = { "x", "+", "1" };
	QueryExpressionLexer lexer = QueryExpressionLexer(assignToken);
	auto expression = Common::ExpressionProcessor::Expression::parse(lexer, Common::ExpressionProcessor::ExpressionType::Arithmetic);
	pkb.setAssign(1, "x", expression);

	vector<string> token = { "1" };
	QueryExpressionLexer queryLexer = QueryExpressionLexer(token);
	auto queryExpression = Common::ExpressionProcessor::Expression::parse(queryLexer, Common::ExpressionProcessor::ExpressionType::Arithmetic);

	SECTION("No clause, select assign statement") {
		QueryProperties properties = QueryProperties(declarations, assignSynonym, {}, {});
		QueryResult result = evaluator.executeQuery(properties);

		vector<string> expectedResult = { "1", "3" };
		vector<string> actualResult = result.getSynonymResult("a");
		sort(actualResult.begin(), actualResult.end());
		REQUIRE(actualResult == expectedResult);
	};

	SECTION("No clause, select variable") {
		QueryProperties properties = QueryProperties(declarations, varSynonym, {}, {});
		QueryResult result = evaluator.executeQuery(properties);

		vector<string> expectedResult = { "x" };
		vector<string> actualResult = result.getSynonymResult("v");
		REQUIRE(actualResult == expectedResult);
	};

	SECTION("One non-trivial such that clause") {
		SuchThatClauseList suchThatClauses = {{ make_unique<ModifiesS>(a, v) }};
		QueryProperties properties = QueryProperties(declarations, assignSynonym, suchThatClauses, {});

		QueryResult result = evaluator.executeQuery(properties);

		vector<string> expectedResult = { "1" };
		vector<string> actualResult = result.getSynonymResult("a");
		REQUIRE(actualResult == expectedResult);
	};

	SECTION("One trivial such that clause") {
		SuchThatClauseList suchThatClauses = {{ make_unique<ModifiesS>(stmtNo1, v)}};
		QueryProperties properties = QueryProperties(declarations, assignSynonym, suchThatClauses, {});

		QueryResult result = evaluator.executeQuery(properties);

		vector<string> expectedResult = { "1", "3" };
		vector<string> actualResult = result.getSynonymResult("a");
		sort(actualResult.begin(), actualResult.end());
		REQUIRE(actualResult == expectedResult);
	};

	SECTION("One trivial pattern clause") {
		PatternClauseList patternList = {
			{ make_unique<Pattern>(assignSynonym, varUnderscore, ExpressionType::ExpressionUnderscore, queryExpression) }
		};
		QueryProperties properties = QueryProperties(declarations, varSynonym, {}, patternList);

		QueryResult result = evaluator.executeQuery(properties);

		vector<string> expectedResult = { "x" };
		vector<string> actualResult = result.getSynonymResult("v");
		REQUIRE(actualResult == expectedResult);
	};

	SECTION("One non-trivial pattern clause") {
		PatternClauseList patternList = {
			{ make_unique<Pattern>(assignSynonym, v, ExpressionType::ExpressionUnderscore, queryExpression) }
		};
		QueryProperties properties = QueryProperties(declarations, varSynonym, {}, patternList);

		QueryResult result = evaluator.executeQuery(properties);

		vector<string> expectedResult = { "x" };
		vector<string> actualResult = result.getSynonymResult("v");
		REQUIRE(actualResult == expectedResult);
	};

	SECTION("Trivial pattern clause and trival such that clause") {
		SuchThatClauseList suchThatClauses = { { make_unique<ModifiesS>(stmtNo1, v)} };
		PatternClauseList patternList = {
			{ make_unique<Pattern>(assignSynonym, v, ExpressionType::ExpressionUnderscore, queryExpression) }
		};
		QueryProperties properties = QueryProperties(declarations, stmtSynonym, suchThatClauses, patternList);

		QueryResult result = evaluator.executeQuery(properties);

		vector<string> expectedResult = { "1", "2", "3", "4" };
		vector<string> actualResult = result.getSynonymResult("s1");
		sort(actualResult.begin(), actualResult.end());
		REQUIRE(actualResult == expectedResult);
	};

	SECTION("Non-trivial pattern clause and such that clause") {
		SuchThatClauseList suchThatClauses = { { make_unique<ModifiesS>(stmtNo1, v)} };
		PatternClauseList patternList = { {make_unique<Pattern>(assignSynonym, v, ExpressionType::ExpressionUnderscore, queryExpression)} };
		QueryProperties properties = QueryProperties(declarations, varSynonym, suchThatClauses, patternList);

		QueryResult result = evaluator.executeQuery(properties);

		vector<string> expectedResult = { "x" };
		vector<string> actualResult = result.getSynonymResult("v");
		REQUIRE(actualResult == expectedResult);
	};
};
