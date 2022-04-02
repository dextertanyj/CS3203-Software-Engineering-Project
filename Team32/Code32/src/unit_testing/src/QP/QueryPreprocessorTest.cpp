#include "QP/Preprocessor/QueryPreprocessor.h"

#include <string>

#include "QP/Evaluator/Clause.h"
#include "catch.hpp"
#include "catch_tools.h"

using namespace QP::Preprocessor;
using namespace QP::Types;

const string UnivDeclarations =
	"stmt s1, s2; read r1, r2; print p1, p2; "
	"call c1, c2; while w1, w2; if i1, i2; "
	"assign a1, a2; variable v1, v2; "
	"constant ct1, ct2; procedure pc1, pc2;";

TEST_CASE("QueryPreprocessor::tokenizeQuery Invalid tokenizer input") {
	QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery("!"), QP::QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("@"), QP::QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("$"), QP::QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("^"), QP::QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("&"), QP::QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("|"), QP::QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("~"), QP::QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("["), QP::QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("]"), QP::QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("{"), QP::QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("}"), QP::QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("`"), QP::QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("'"), QP::QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery(":"), QP::QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("\\"), QP::QueryTokenizationException);
}

TEST_CASE("QueryPreprocessor::parseQuery valid declarations") {
	// Test design entities
	QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery("if a, b, c; while Select; read such, that;assign pattern;variable CAP3;\nSelect a");
	auto declaration_list1 = qp1.getDeclarationList();
	REQUIRE(find(declaration_list1.begin(), declaration_list1.end(), Declaration({QP::Types::DesignEntity::If, "a"})) !=
	        declaration_list1.end());
	REQUIRE(find(declaration_list1.begin(), declaration_list1.end(), Declaration({QP::Types::DesignEntity::If, "b"})) !=
	        declaration_list1.end());
	REQUIRE(find(declaration_list1.begin(), declaration_list1.end(), Declaration({QP::Types::DesignEntity::If, "c"})) !=
	        declaration_list1.end());
	REQUIRE(find(declaration_list1.begin(), declaration_list1.end(), Declaration({QP::Types::DesignEntity::While, "Select"})) !=
	        declaration_list1.end());
	REQUIRE(find(declaration_list1.begin(), declaration_list1.end(), Declaration({QP::Types::DesignEntity::Read, "such"})) !=
	        declaration_list1.end());
	REQUIRE(find(declaration_list1.begin(), declaration_list1.end(), Declaration({QP::Types::DesignEntity::Read, "that"})) !=
	        declaration_list1.end());
	REQUIRE(find(declaration_list1.begin(), declaration_list1.end(), Declaration({QP::Types::DesignEntity::Assign, "pattern"})) !=
	        declaration_list1.end());
	REQUIRE(find(declaration_list1.begin(), declaration_list1.end(), Declaration({QP::Types::DesignEntity::Variable, "CAP3"})) !=
	        declaration_list1.end());
	REQUIRE(find(declaration_list1.begin(), declaration_list1.end(), Declaration({QP::Types::DesignEntity::If, "c"})) !=
	        declaration_list1.end());

	QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery("stmt stmt; read reads;print p1;procedure\nModifies; constant Uses; Select Modifies");
	auto declaration_list2 = qp2.getDeclarationList();
	REQUIRE(find(declaration_list2.begin(), declaration_list2.end(), Declaration({QP::Types::DesignEntity::Stmt, "stmt"})) !=
	        declaration_list2.end());
	REQUIRE(find(declaration_list2.begin(), declaration_list2.end(), Declaration({QP::Types::DesignEntity::Read, "reads"})) !=
	        declaration_list2.end());
	REQUIRE(find(declaration_list2.begin(), declaration_list2.end(), Declaration({QP::Types::DesignEntity::Print, "p1"})) !=
	        declaration_list2.end());
	REQUIRE(find(declaration_list2.begin(), declaration_list2.end(), Declaration({QP::Types::DesignEntity::Procedure, "Modifies"})) !=
	        declaration_list2.end());
	REQUIRE(find(declaration_list2.begin(), declaration_list2.end(), Declaration({QP::Types::DesignEntity::Constant, "Uses"})) !=
	        declaration_list2.end());

	// Multiple declaration clause with same design entity
	QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery("while stmt; if reads;while p1; while Modifies;Select Modifies");
	auto declaration_list3 = qp3.getDeclarationList();
	REQUIRE(find(declaration_list3.begin(), declaration_list3.end(), Declaration({QP::Types::DesignEntity::While, "stmt"})) !=
	        declaration_list3.end());
	REQUIRE(find(declaration_list3.begin(), declaration_list3.end(), Declaration({QP::Types::DesignEntity::If, "reads"})) !=
	        declaration_list3.end());
	REQUIRE(find(declaration_list3.begin(), declaration_list3.end(), Declaration({QP::Types::DesignEntity::While, "p1"})) !=
	        declaration_list3.end());
	REQUIRE(find(declaration_list3.begin(), declaration_list3.end(), Declaration({QP::Types::DesignEntity::While, "Modifies"})) !=
	        declaration_list3.end());
}

TEST_CASE("QueryPreprocessor::parseQuery invalid declarations") {
	// wrong spelling / case
	QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery("IF ifs; Select ifs"), QP::QuerySyntaxException);
	QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery("prints a; Select a"), QP::QuerySyntaxException);
	QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery("assssign a; Select a"), QP::QuerySyntaxException);

	// missing ; / ,
	QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery("while a Select a"), QP::QuerySyntaxException);
	QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery("variable a b c; Select a"), QP::QuerySyntaxException);

	// Duplicated synonym
	QueryPreprocessor qpp6;
	REQUIRE_THROWS_AS(qpp6.parseQuery("while a, a; Select a"), QP::QuerySemanticException);
	QueryPreprocessor qpp7;
	REQUIRE_THROWS_AS(qpp7.parseQuery("read a; print b; procedure a; Select a"), QP::QuerySemanticException);
}

TEST_CASE("QueryPreprocessor::parseQuery Select single") {
	QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery("print a; if b; Select a");
	REQUIRE(qp1.getSelectList()[0].getSynonymType() == QP::Types::DesignEntity::Print);
	REQUIRE(qp1.getSelectList()[0].getSynonymSymbol() == "a");

	QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery("print a; if b; Select b");
	REQUIRE(qp2.getSelectList()[0].getSynonymType() == QP::Types::DesignEntity::If);
	REQUIRE(qp2.getSelectList()[0].getSynonymSymbol() == "b");

	// select BOOLEAN synonym
	QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery("if BOOLEAN; Select BOOLEAN");
	REQUIRE(qp3.getSelectList()[0].getSynonymType() == QP::Types::DesignEntity::If);
	REQUIRE(qp3.getSelectList()[0].getSynonymSymbol() == "BOOLEAN");

	// undeclared synonym
	QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery("print a; if b; Select unknownSynonym"), QP::QuerySemanticException);
	// more than 1 synonym
	QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery("print a; if b; Select a,b"), QP::QuerySyntaxException);
	// more than 1 select
	QueryPreprocessor qpp6;
	REQUIRE_THROWS_AS(qpp6.parseQuery("print a; if b; Select a Select b"), QP::QuerySyntaxException);
}

TEST_CASE("QueryPreprocessor::parseQuery Select tuple") {
	QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery("print a; if b; Select <a>");
	REQUIRE(qp1.getSelectList()[0].getSynonymType() == QP::Types::DesignEntity::Print);
	REQUIRE(qp1.getSelectList()[0].getSynonymSymbol() == "a");

	QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery("print a; if b; Select <a,b>");
	REQUIRE(qp2.getSelectList()[0].getSynonymType() == QP::Types::DesignEntity::Print);
	REQUIRE(qp2.getSelectList()[0].getSynonymSymbol() == "a");
	REQUIRE(qp2.getSelectList()[1].getSynonymType() == QP::Types::DesignEntity::If);
	REQUIRE(qp2.getSelectList()[1].getSynonymSymbol() == "b");
	// Same synonym
	QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery("print a; if b; Select <b,b>");
	REQUIRE(qp3.getSelectList()[0].getSynonymType() == QP::Types::DesignEntity::If);
	REQUIRE(qp3.getSelectList()[0].getSynonymSymbol() == "b");
	REQUIRE(qp3.getSelectList()[1].getSynonymType() == QP::Types::DesignEntity::If);
	REQUIRE(qp3.getSelectList()[1].getSynonymSymbol() == "b");
	// many synonym
	QueryPreprocessor qpp4;
	QP::QueryProperties qp4 = qpp4.parseQuery(UnivDeclarations + "Select <s1, s2, p1, p2, c1, c2, w1,w2,i1,i2,a1,a2>");
	REQUIRE(qp4.getSelectList()[0].getSynonymType() == QP::Types::DesignEntity::Stmt);
	REQUIRE(qp4.getSelectList()[0].getSynonymSymbol() == "s1");
	REQUIRE(qp4.getSelectList()[1].getSynonymType() == QP::Types::DesignEntity::Stmt);
	REQUIRE(qp4.getSelectList()[1].getSynonymSymbol() == "s2");
	REQUIRE(qp4.getSelectList()[7].getSynonymType() == QP::Types::DesignEntity::While);
	REQUIRE(qp4.getSelectList()[7].getSynonymSymbol() == "w2");
	REQUIRE(qp4.getSelectList()[11].getSynonymType() == QP::Types::DesignEntity::Assign);
	REQUIRE(qp4.getSelectList()[11].getSynonymSymbol() == "a2");

	// empty tuple
	QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery("print a; if b; Select <>"), QP::QuerySyntaxException);
	// Missing <>
	QueryPreprocessor qpp6;
	REQUIRE_THROWS_AS(qpp6.parseQuery("print a; if b; Select a,b"), QP::QuerySyntaxException);
	// undeclared synonym
	QueryPreprocessor qpp7;
	REQUIRE_THROWS_AS(qpp7.parseQuery("print a; if b; Select <a,b,undeclaredSynonym>"), QP::QuerySemanticException);
}

TEST_CASE("QueryPreprocessor::parseQuery Select BOOLEAN") {
	QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery("if i; Select BOOLEAN");
	REQUIRE(qp1.getSelectList().size() == 0);

	// lower case
	QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery("if i; Select boolean"), QP::QuerySemanticException);
	// tuple
	QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery("if i; Select <BOOLEAN>"), QP::QuerySemanticException);
}

TEST_CASE("QueryPreprocessor::parseQuery Select attribute") {
	QueryPreprocessor qpp;
	QP::QueryProperties qp = {{}, {}, {}};

	SECTION("Single attribute") {
		qp = qpp.parseQuery(UnivDeclarations + "Select s1.stmt#");
		REQUIRE(qp.getSelectList()[0].getType() == QP::Types::ReferenceType::Attribute);
		REQUIRE(qp.getSelectList()[0].getAttributeType() == QP::Types::AttributeType::NumberIdentifier);
		REQUIRE(qp.getSelectList()[0].getSynonymSymbol() == "s1");
		REQUIRE(qp.getSelectList()[0].getSynonymType() == QP::Types::DesignEntity::Stmt);
	}

	SECTION("Attribute and synonym") {
		qp = qpp.parseQuery(UnivDeclarations + "Select <s1, s1.stmt#>");
		REQUIRE(qp.getSelectList()[0].getType() == QP::Types::ReferenceType::Synonym);
		REQUIRE(qp.getSelectList()[0].getSynonymSymbol() == "s1");
		REQUIRE(qp.getSelectList()[0].getSynonymType() == QP::Types::DesignEntity::Stmt);
		REQUIRE(qp.getSelectList()[1].getType() == QP::Types::ReferenceType::Attribute);
		REQUIRE(qp.getSelectList()[1].getAttributeType() == QP::Types::AttributeType::NumberIdentifier);
		REQUIRE(qp.getSelectList()[1].getSynonymSymbol() == "s1");
		REQUIRE(qp.getSelectList()[1].getSynonymType() == QP::Types::DesignEntity::Stmt);
	}

	SECTION("Multiple attributes") {
		qp = qpp.parseQuery(UnivDeclarations + "Select <s1.stmt#, s2.stmt#, p1.varName>");
		REQUIRE(qp.getSelectList()[0].getType() == QP::Types::ReferenceType::Attribute);
		REQUIRE(qp.getSelectList()[0].getAttributeType() == QP::Types::AttributeType::NumberIdentifier);
		REQUIRE(qp.getSelectList()[0].getSynonymSymbol() == "s1");
		REQUIRE(qp.getSelectList()[0].getSynonymType() == QP::Types::DesignEntity::Stmt);
		REQUIRE(qp.getSelectList()[1].getType() == QP::Types::ReferenceType::Attribute);
		REQUIRE(qp.getSelectList()[1].getAttributeType() == QP::Types::AttributeType::NumberIdentifier);
		REQUIRE(qp.getSelectList()[1].getSynonymSymbol() == "s2");
		REQUIRE(qp.getSelectList()[1].getSynonymType() == QP::Types::DesignEntity::Stmt);
		REQUIRE(qp.getSelectList()[2].getType() == QP::Types::ReferenceType::Attribute);
		REQUIRE(qp.getSelectList()[2].getAttributeType() == QP::Types::AttributeType::VariableName);
		REQUIRE(qp.getSelectList()[2].getSynonymSymbol() == "p1");
		REQUIRE(qp.getSelectList()[2].getSynonymType() == QP::Types::DesignEntity::Print);
	}
}

TEST_CASE("QueryPreprocessor::parseQuery invalid such that Parent(*)") {
	// disjoint *
	QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery(UnivDeclarations + "Select s1 such that Parent *(s1, s2)"), QP::QuerySyntaxException);
	// non-statement synonyms
	QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery(UnivDeclarations + "Select v1 such that Parent(v1, s2)"), QP::QuerySemanticException);
	QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery(UnivDeclarations + "Select s1 such that Parent(pc1, s2)"), QP::QuerySemanticException);
	// misspelt word
	QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery(UnivDeclarations + "Select s1 such that Parents(s1, s2)"), QP::QuerySyntaxException);
	// disallowed statement reference
	QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery(UnivDeclarations + "Select s1 such that Parents(s1, \"x\")"), QP::QuerySyntaxException);
}

TEST_CASE("QueryPreprocessor::parseQuery valid such that Follows(*)") {
	shared_ptr<QP::Evaluator::Clause> clause;
	// Combinations of stmtRef : synonym | '_' | INTEGER for Follows
	QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select s1 such that Follows(s1, s2)");
	clause = qp1.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1", "s2"}));

	QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select i1 such that Follows(i1, _)");
	clause = qp2.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1"}));

	QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select r1 such that Follows(r1, 20)");
	clause = qp3.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"r1"}));

	QueryPreprocessor qpp4;
	QP::QueryProperties qp4 = qpp4.parseQuery(UnivDeclarations + "Select a1 such that Follows(1, a1)");
	clause = qp4.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));

	QueryPreprocessor qpp5;
	QP::QueryProperties qp5 = qpp5.parseQuery(UnivDeclarations + "Select i1 such that Follows(32, 3)");
	clause = qp5.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>());

	QueryPreprocessor qpp6;
	QP::QueryProperties qp6 = qpp6.parseQuery(UnivDeclarations + "Select w2 such that Follows(1010, _)");
	clause = qp6.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>());

	QueryPreprocessor qpp7;
	QP::QueryProperties qp7 = qpp7.parseQuery(UnivDeclarations + "Select s1 such that Follows(_, p2)");
	clause = qp7.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"p2"}));

	QueryPreprocessor qpp8;
	QP::QueryProperties qp8 = qpp8.parseQuery(UnivDeclarations + "Select i1 such that Follows(_, 1)");
	clause = qp8.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>());

	QueryPreprocessor qpp9;
	QP::QueryProperties qp9 = qpp9.parseQuery(UnivDeclarations + "Select w2 such that Follows(_, _)");
	clause = qp9.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>());

	// Follows*
	QueryPreprocessor qpp10;
	QP::QueryProperties qp10 = qpp10.parseQuery(UnivDeclarations + "Select c2 such that Follows*(w1, c2)");
	clause = qp10.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::FollowsT);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1", "c2"}));
	QueryPreprocessor qpp11;
	QP::QueryProperties qp11 = qpp11.parseQuery(UnivDeclarations + "Select c2 such that Follows*      (   w1  , c2  )");
	clause = qp11.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::FollowsT);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1", "c2"}));

	// Multiple Follows(*) clauses
	QueryPreprocessor qpp12;
	QP::QueryProperties qp12 = qpp12.parseQuery(UnivDeclarations + "Select c2 such that Follows(i1, _) and Follows(w1, c2)");
	clause = qp12.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1"}));
	clause = qp12.getClauseList()[1];
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1", "c2"}));
	QueryPreprocessor qpp13;
	QP::QueryProperties qp13 = qpp13.parseQuery(UnivDeclarations + "Select c2 such that Follows*(i1, _) and Follows(w1, c2)");
	clause = qp13.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::FollowsT);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1"}));
	clause = qp13.getClauseList()[1];
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1", "c2"}));
}

TEST_CASE("QueryPreprocessor::parseQuery invalid such that Follows(*)") {
	// Missing (
	QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery(UnivDeclarations + "Select s1 such that Follows s1, s2)"), QP::QuerySyntaxException);
	// disjoint *
	QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery(UnivDeclarations + "Select s1 such that Follows *(s1, s2)"), QP::QuerySyntaxException);
	// non-statement synonyms
	QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery(UnivDeclarations + "Select s1 such that Follows(pc1, s2)"), QP::QuerySemanticException);
	// misspelt word
	QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery(UnivDeclarations + "Select s1 such that Follow(s1, s2)"), QP::QuerySyntaxException);
	// disallowed statement reference
	QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery(UnivDeclarations + "Select s1 such that Follows(s1, \"x\")"), QP::QuerySyntaxException);
	// statement number with leading 0
	QueryPreprocessor qpp6;
	REQUIRE_THROWS_AS(qpp6.parseQuery(UnivDeclarations + "Select s1 such that Follows(s1, 007)"), QP::QuerySyntaxException);
}

TEST_CASE("QP::QueryPreprocessor::parseQuery valid such that UsesS/P") {
	// Valid UsesS
	shared_ptr<QP::Evaluator::Clause> clause;
	QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select v1 such that Uses(s1, v1)");
	clause = qp1.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1", "v1"}));

	QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select s1 such that Uses(s1, _)");
	clause = qp2.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1"}));

	QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select s1 such that Uses(s1, \"x\")");
	clause = qp3.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1"}));

	QueryPreprocessor qpp4;
	QP::QueryProperties qp4 = qpp4.parseQuery(UnivDeclarations + "Select v1 such that Uses(i1, v1)");
	clause = qp4.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1", "v1"}));

	QueryPreprocessor qpp5;
	QP::QueryProperties qp5 = qpp5.parseQuery(UnivDeclarations + "Select p1 such that Uses(p1, _)");
	clause = qp5.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"p1"}));

	QueryPreprocessor qpp6;
	QP::QueryProperties qp6 = qpp6.parseQuery(UnivDeclarations + "Select a1 such that Uses(a1, \"y\")");
	clause = qp6.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));

	QueryPreprocessor qpp7;
	QP::QueryProperties qp7 = qpp7.parseQuery(UnivDeclarations + "Select v1 such that Uses(w2, v1)");
	clause = qp7.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w2", "v1"}));

	QueryPreprocessor qpp8;
	QP::QueryProperties qp8 = qpp8.parseQuery(UnivDeclarations + "Select i1 such that Uses(i1, _)");
	clause = qp8.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1"}));

	QueryPreprocessor qpp9;
	QP::QueryProperties qp9 = qpp9.parseQuery(UnivDeclarations + "Select w1 such that Uses(w1, \"z0\")");
	clause = qp9.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1"}));

	QueryPreprocessor qpp10;
	QP::QueryProperties qp10 = qpp10.parseQuery(UnivDeclarations + "Select v1 such that Uses(1, v1)");
	clause = qp10.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"v1"}));

	QueryPreprocessor qpp11;
	QP::QueryProperties qp11 = qpp11.parseQuery(UnivDeclarations + "Select s1 such that Uses(c2, _)");
	clause = qp11.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"c2"}));

	QueryPreprocessor qpp12;
	QP::QueryProperties qp12 = qpp12.parseQuery(UnivDeclarations + "Select s1 such that Uses(w2, \"var\")");
	clause = qp12.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w2"}));

	// UsesP
	QueryPreprocessor qpp13;
	QP::QueryProperties qp13 = qpp13.parseQuery(UnivDeclarations + "Select v1 such that Uses(pc1, v1)");
	clause = qp13.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::UsesP);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc1", "v1"}));

	QueryPreprocessor qpp14;
	QP::QueryProperties qp14 = qpp14.parseQuery(UnivDeclarations + "Select v1 such that Uses(pc1, _)");
	clause = qp14.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::UsesP);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc1"}));

	QueryPreprocessor qpp15;
	QP::QueryProperties qp15 = qpp15.parseQuery(UnivDeclarations + R"(Select s2 such that Uses("procedureName", "varName"))");
	clause = qp15.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::UsesP);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>());
}

TEST_CASE("QueryPreprocessor::parseQuery invalid such that UsesS/P") {
	// unexpected wildcard
	QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery(UnivDeclarations + "Select v1 such that Uses(_, v1)"), QP::QuerySemanticException);
	// unexpected integer
	QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery(UnivDeclarations + "Select pc1 such that Uses(pc1, 1)"), QP::QuerySyntaxException);
	// invalid synonym
	QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery(UnivDeclarations + "Select s1 such that Uses(1, s1)"), QP::QuerySemanticException);
	QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery(UnivDeclarations + "Select r1 such that Uses(r1, _)"), QP::QuerySemanticException);
	// undeclared synonym
	QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery(UnivDeclarations + "Select s1 such that Uses(s1, UNK)"), QP::QuerySemanticException);
	// misspelled keyword
	QueryPreprocessor qpp6;
	REQUIRE_THROWS_AS(qpp6.parseQuery(UnivDeclarations + "Select s1 such that use(s1, a1)"), QP::QuerySyntaxException);
}

TEST_CASE("QueryPreprocessor::parseQuery valid such that ModifiesS/P") {
	shared_ptr<QP::Evaluator::Clause> clause;
	// Valid ModifiesS
	QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select v1 such that Modifies(s1, v1)");
	clause = qp1.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1", "v1"}));

	QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select s1 such that Modifies(s1, _)");
	clause = qp2.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1"}));

	QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select s1 such that Modifies(s1, \"x\")");
	clause = qp3.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1"}));

	QueryPreprocessor qpp4;
	QP::QueryProperties qp4 = qpp4.parseQuery(UnivDeclarations + "Select v1 such that Modifies(i1, v1)");
	clause = qp4.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1", "v1"}));

	QueryPreprocessor qpp5;
	QP::QueryProperties qp5 = qpp5.parseQuery(UnivDeclarations + "Select p1 such that Modifies(r1, _)");
	clause = qp5.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"r1"}));

	QueryPreprocessor qpp6;
	QP::QueryProperties qp6 = qpp6.parseQuery(UnivDeclarations + "Select a1 such that Modifies(a1, \"y\")");
	clause = qp6.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));

	QueryPreprocessor qpp7;
	QP::QueryProperties qp7 = qpp7.parseQuery(UnivDeclarations + "Select v1 such that Modifies(w2, v1)");
	clause = qp7.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w2", "v1"}));

	QueryPreprocessor qpp8;
	QP::QueryProperties qp8 = qpp8.parseQuery(UnivDeclarations + "Select i1 such that Modifies(i1, _)");
	clause = qp8.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1"}));

	QueryPreprocessor qpp9;
	QP::QueryProperties qp9 = qpp9.parseQuery(UnivDeclarations + "Select w1 such that Modifies(w1, \"z0\")");
	clause = qp9.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1"}));

	QueryPreprocessor qpp10;
	QP::QueryProperties qp10 = qpp10.parseQuery(UnivDeclarations + "Select v1 such that Modifies(1, v1)");
	clause = qp10.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"v1"}));

	QueryPreprocessor qpp11;
	QP::QueryProperties qp11 = qpp11.parseQuery(UnivDeclarations + "Select s1 such that Modifies(w2, _)");
	clause = qp11.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w2"}));

	QueryPreprocessor qpp12;
	QP::QueryProperties qp12 = qpp12.parseQuery(UnivDeclarations + "Select s1 such that Modifies(c2, \"var\")");
	clause = qp12.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"c2"}));

	// ModifiesP
	QueryPreprocessor qpp13;
	QP::QueryProperties qp13 = qpp13.parseQuery(UnivDeclarations + "Select v1 such that Modifies(pc1, v1)");
	clause = qp13.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::ModifiesP);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc1", "v1"}));

	QueryPreprocessor qpp14;
	QP::QueryProperties qp14 = qpp14.parseQuery(UnivDeclarations + "Select v1 such that Modifies(pc1, _)");
	clause = qp14.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::ModifiesP);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc1"}));

	QueryPreprocessor qpp15;
	QP::QueryProperties qp15 = qpp15.parseQuery(UnivDeclarations + R"(Select s2 such that Modifies("procedureName", "varName"))");
	clause = qp15.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::ModifiesP);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({}));
}

TEST_CASE("QueryPreprocessor::parseQuery invalid such that ModifiesS/P") {
	// unexpected wildcard
	QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery(UnivDeclarations + "Select v1 such that Modifies(_, v1)"), QP::QuerySemanticException);
	// unexpected integer
	QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery(UnivDeclarations + "Select pc1 such that Modifies(pc1, 1)"), QP::QuerySyntaxException);
	// invalid synonym
	QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery(UnivDeclarations + "Select s1 such that Modifies(1, 0s)"), QP::QuerySyntaxException);
	QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery(UnivDeclarations + "Select s1 such that Modifies(1, s1)"), QP::QuerySemanticException);
	QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery(UnivDeclarations + "Select p1 such that Modifies(p1, v1)"), QP::QuerySemanticException);
	// undeclared synonym
	QueryPreprocessor qpp6;
	REQUIRE_THROWS_AS(qpp6.parseQuery(UnivDeclarations + "Select s1 such that Modifies(s1, UNK)"), QP::QuerySemanticException);
	// misspelled keyword
	QueryPreprocessor qpp7;
	REQUIRE_THROWS_AS(qpp7.parseQuery(UnivDeclarations + "Select s1 such that modifies(s1, a1)"), QP::QuerySyntaxException);
}

TEST_CASE("QueryPreprocessor::parseQuery valid such that Calls(*)") {
	shared_ptr<QP::Evaluator::Clause> clause;

	QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select pc1 such that Calls(pc1, pc2)");
	clause = qp1.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::Calls);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc1", "pc2"}));

	QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select pc1 such that Calls(pc1, _)");
	clause = qp2.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::Calls);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc1"}));

	QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select pc1 such that Calls(pc1, \"procedure1\")");
	clause = qp3.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::Calls);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc1"}));

	QueryPreprocessor qpp4;
	QP::QueryProperties qp4 = qpp4.parseQuery(UnivDeclarations + "Select pc1 such that Calls(\"procedure1\", pc1)");
	clause = qp4.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::Calls);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc1"}));

	QueryPreprocessor qpp5;
	QP::QueryProperties qp5 = qpp5.parseQuery(UnivDeclarations + "Select i1 such that Calls(\"procedure1\", \"procedure2\")");
	clause = qp5.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::Calls);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({}));

	QueryPreprocessor qpp6;
	QP::QueryProperties qp6 = qpp6.parseQuery(UnivDeclarations + "Select w2 such that Calls(\"procedure1\", _)");
	clause = qp6.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::Calls);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({}));

	QueryPreprocessor qpp7;
	QP::QueryProperties qp7 = qpp7.parseQuery(UnivDeclarations + "Select s1 such that Calls(_, pc2)");
	clause = qp7.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::Calls);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc2"}));

	QueryPreprocessor qpp8;
	QP::QueryProperties qp8 = qpp8.parseQuery(UnivDeclarations + "Select i1 such that Calls*(_, \"procedure1\")");
	clause = qp8.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::CallsT);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({}));

	QueryPreprocessor qpp9;
	QP::QueryProperties qp9 = qpp9.parseQuery(UnivDeclarations + "Select w2 such that Calls(_, _)");
	clause = qp9.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::Calls);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({}));

	QueryPreprocessor qpp10;
	QP::QueryProperties qp10 = qpp10.parseQuery(UnivDeclarations + "Select pc2 such that Calls*(pc1, pc2)");
	clause = qp10.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::CallsT);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc1", "pc2"}));
}

TEST_CASE("QP::QueryPreprocessor::parseQuery valid Next(*)") {
	shared_ptr<QP::Evaluator::Clause> clause;
	QueryPreprocessor qpp;
	QP::QueryProperties qp = {{}, {}, {}};

	SECTION("Next") {
		SECTION("Synonym Synonym") {
			qp = qpp.parseQuery(UnivDeclarations + "Select s1 such that Next(s1, s2)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::Next);
			REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1", "s2"}));
		}

		SECTION("Synonym Wildcard") {
			qp = qpp.parseQuery(UnivDeclarations + "Select i1 such that Next(i1, _)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::Next);
			REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1"}));
		}

		SECTION("Synonym Index") {
			qp = qpp.parseQuery(UnivDeclarations + "Select r1 such that Next(r1, 20)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::Next);
			REQUIRE(clause->getDeclarationSymbols() == vector<string>({"r1"}));
		}

		SECTION("Index Synonym") {
			qp = qpp.parseQuery(UnivDeclarations + "Select a1 such that Next(1, a1)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::Next);
			REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));
		}

		SECTION("Index Index") {
			qp = qpp.parseQuery(UnivDeclarations + "Select i1 such that Next(32, 3)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::Next);
			REQUIRE(clause->getDeclarationSymbols().empty());
		}

		SECTION("Index Wildcard") {
			qp = qpp.parseQuery(UnivDeclarations + "Select w2 such that Next(1010, _)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::Next);
			REQUIRE(clause->getDeclarationSymbols().empty());
		}

		SECTION("Wildcard Synonym") {
			qp = qpp.parseQuery(UnivDeclarations + "Select s1 such that Next(_, p2)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::Next);
			REQUIRE(clause->getDeclarationSymbols() == vector<string>({"p2"}));
		}

		SECTION("Wildcard Index") {
			qp = qpp.parseQuery(UnivDeclarations + "Select i1 such that Next(_, 1)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::Next);
			REQUIRE(clause->getDeclarationSymbols().empty());
		}

		SECTION("Wildcard Wildcard") {
			qp = qpp.parseQuery(UnivDeclarations + "Select w2 such that Next(_, _)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::Next);
			REQUIRE(clause->getDeclarationSymbols().empty());
		}
	}

	SECTION("Next*") {
		SECTION("Synonym Synonym") {
			qp = qpp.parseQuery(UnivDeclarations + "Select s1 such that Next*(s1, s2)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::NextT);
			REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1", "s2"}));
		}

		SECTION("Synonym Wildcard") {
			qp = qpp.parseQuery(UnivDeclarations + "Select i1 such that Next*(i1, _)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::NextT);
			REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1"}));
		}

		SECTION("Synonym Index") {
			qp = qpp.parseQuery(UnivDeclarations + "Select r1 such that Next*(r1, 20)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::NextT);
			REQUIRE(clause->getDeclarationSymbols() == vector<string>({"r1"}));
		}

		SECTION("Index Synonym") {
			qp = qpp.parseQuery(UnivDeclarations + "Select a1 such that Next*(1, a1)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::NextT);
			REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));
		}

		SECTION("Index Index") {
			qp = qpp.parseQuery(UnivDeclarations + "Select i1 such that Next*(32, 3)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::NextT);
			REQUIRE(clause->getDeclarationSymbols().empty());
		}

		SECTION("Index Wildcard") {
			qp = qpp.parseQuery(UnivDeclarations + "Select w2 such that Next*(1010, _)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::NextT);
			REQUIRE(clause->getDeclarationSymbols().empty());
		}

		SECTION("Wildcard Synonym") {
			qp = qpp.parseQuery(UnivDeclarations + "Select s1 such that Next*(_, p2)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::NextT);
			REQUIRE(clause->getDeclarationSymbols() == vector<string>({"p2"}));
		}

		SECTION("Wildcard Index") {
			qp = qpp.parseQuery(UnivDeclarations + "Select i1 such that Next*(_, 1)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::NextT);
			REQUIRE(clause->getDeclarationSymbols().empty());
		}

		SECTION("Wildcard Wildcard") {
			qp = qpp.parseQuery(UnivDeclarations + "Select w2 such that Next*(_, _)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::NextT);
			REQUIRE(clause->getDeclarationSymbols().empty());
		}
	}
}

TEST_CASE("QP::QueryPreprocessor::parseQuery invalid Next(*)") {
	QP::Preprocessor::QueryPreprocessor qpp;

	SECTION("Syntax Exceptions") {
		SECTION("Unexpected Name") {
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Next(\"name\", _)"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Next(_, \"name\")"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Next*(\"name\", _)"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Next*(_, \"name\")"), QP::QuerySyntaxException);
		}

		SECTION("Misspelled Keyword") {
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that next(1, 2)"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that next*(1, 2)"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Next *(1, 2)"), QP::QuerySyntaxException);
		}

		SECTION("Invalid Synonym Symbol") {
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Next(0s, _)"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Next(_, 0s)"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select p1 such that Next*(0s, _)"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select p1 such that Next*(_, 0s)"), QP::QuerySyntaxException);
		}

		SECTION("Invalid Index Value") {
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Next(01, _)"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Next(_, 03)"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select p1 such that Next*(01, _)"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select p1 such that Next*(_, 03)"), QP::QuerySyntaxException);
		}

		SECTION("Incorrect Argument Count") {
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Next(01, _, _)"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select p1 such that Next*(01, _, _)"), QP::QuerySyntaxException);
		}
	}

	SECTION("Semantic Exception") {
		SECTION("Undeclared Synonym") {
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Next(_, UND)"), QP::QuerySemanticException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Next(UND, _)"), QP::QuerySemanticException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Next*(_, UND)"), QP::QuerySemanticException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Next*(UND, _)"), QP::QuerySemanticException);
		}

		SECTION("Mismatched Synonym Type") {
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Next(_, v1)"), QP::QuerySemanticException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Next(pc1, _)"), QP::QuerySemanticException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Next*(_, v1)"), QP::QuerySemanticException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Next*(ct1, _)"), QP::QuerySemanticException);
		}
	}
}

TEST_CASE("QP::QueryPreprocessor::parseQuery valid Affects(*)") {
	shared_ptr<QP::Evaluator::Clause> clause;
	QueryPreprocessor qpp;
	QP::QueryProperties qp = {{}, {}, {}};

	SECTION("Affects") {
		SECTION("Synonym Synonym") {
			qp = qpp.parseQuery(UnivDeclarations + "Select s1 such that Affects(s1, s2)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::Affects);
			REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1", "s2"}));
		}

		SECTION("Synonym Wildcard") {
			qp = qpp.parseQuery(UnivDeclarations + "Select a1 such that Affects(a1, _)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::Affects);
			REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));
		}

		SECTION("Synonym Index") {
			qp = qpp.parseQuery(UnivDeclarations + "Select a1 such that Affects(a1, 20)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::Affects);
			REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));
		}

		SECTION("Index Synonym") {
			qp = qpp.parseQuery(UnivDeclarations + "Select a1 such that Affects(1, a2)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::Affects);
			REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a2"}));
		}

		SECTION("Index Index") {
			qp = qpp.parseQuery(UnivDeclarations + "Select a1 such that Affects(32, 3)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::Affects);
			REQUIRE(clause->getDeclarationSymbols().empty());
		}

		SECTION("Index Wildcard") {
			qp = qpp.parseQuery(UnivDeclarations + "Select a1 such that Affects(1010, _)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::Affects);
			REQUIRE(clause->getDeclarationSymbols().empty());
		}

		SECTION("Wildcard Synonym") {
			qp = qpp.parseQuery(UnivDeclarations + "Select a1 such that Affects(_, a2)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::Affects);
			REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a2"}));
		}

		SECTION("Wildcard Index") {
			qp = qpp.parseQuery(UnivDeclarations + "Select a1 such that Affects(_, 1)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::Affects);
			REQUIRE(clause->getDeclarationSymbols().empty());
		}

		SECTION("Wildcard Wildcard") {
			qp = qpp.parseQuery(UnivDeclarations + "Select a1 such that Affects(_, _)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::Affects);
			REQUIRE(clause->getDeclarationSymbols().empty());
		}
	}

	SECTION("Affects*") {
		SECTION("Synonym Synonym") {
			qp = qpp.parseQuery(UnivDeclarations + "Select s1 such that Affects*(s1, s2)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::AffectsT);
			REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1", "s2"}));
		}

		SECTION("Synonym Wildcard") {
			qp = qpp.parseQuery(UnivDeclarations + "Select a1 such that Affects*(a1, _)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::AffectsT);
			REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));
		}

		SECTION("Synonym Index") {
			qp = qpp.parseQuery(UnivDeclarations + "Select a1 such that Affects*(a1, 20)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::AffectsT);
			REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));
		}

		SECTION("Index Synonym") {
			qp = qpp.parseQuery(UnivDeclarations + "Select a2 such that Affects*(1, a2)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::AffectsT);
			REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a2"}));
		}

		SECTION("Index Index") {
			qp = qpp.parseQuery(UnivDeclarations + "Select a1 such that Affects*(32, 3)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::AffectsT);
			REQUIRE(clause->getDeclarationSymbols().empty());
		}

		SECTION("Index Wildcard") {
			qp = qpp.parseQuery(UnivDeclarations + "Select a1 such that Affects*(1010, _)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::AffectsT);
			REQUIRE(clause->getDeclarationSymbols().empty());
		}

		SECTION("Wildcard Synonym") {
			qp = qpp.parseQuery(UnivDeclarations + "Select a1 such that Affects*(_, a2)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::AffectsT);
			REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a2"}));
		}

		SECTION("Wildcard Index") {
			qp = qpp.parseQuery(UnivDeclarations + "Select a1 such that Affects*(_, 1)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::AffectsT);
			REQUIRE(clause->getDeclarationSymbols().empty());
		}

		SECTION("Wildcard Wildcard") {
			qp = qpp.parseQuery(UnivDeclarations + "Select a1 such that Affects*(_, _)");
			clause = qp.getClauseList()[0];
			REQUIRE(clause->getType() == ClauseType::AffectsT);
			REQUIRE(clause->getDeclarationSymbols().empty());
		}
	}
}


TEST_CASE("QP::QueryPreprocessor::parseQuery invalid Affects(*)") {
	QueryPreprocessor qpp;

	SECTION("Syntax Exceptions") {
		SECTION("Unexpected Name") {
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Affects(\"name\", _)"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Affects(_, \"name\")"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Affects*(\"name\", _)"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Affects*(_, \"name\")"), QP::QuerySyntaxException);
		}

		SECTION("Misspelled Keyword") {
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that affects(1, 2)"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that affects*(1, 2)"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Affects *(1, 2)"), QP::QuerySyntaxException);
		}

		SECTION("Invalid Synonym Symbol") {
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Affects(0s, _)"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Affects(_, 0s)"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select p1 such that Affects*(0s, _)"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select p1 such that Affects*(_, 0s)"), QP::QuerySyntaxException);
		}

		SECTION("Invalid Index Value") {
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Affects(01, _)"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Affects(_, 03)"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select p1 such that Affects*(01, _)"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select p1 such that Affects*(_, 03)"), QP::QuerySyntaxException);
		}

		SECTION("Incorrect Argument Count") {
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Affects(01, _, _)"), QP::QuerySyntaxException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select p1 such that Affects*(01, _, _)"), QP::QuerySyntaxException);
		}
	}

	SECTION("Semantic Exception") {
		SECTION("Undeclared Synonym") {
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Affects(_, UND)"), QP::QuerySemanticException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Affects(UND, _)"), QP::QuerySemanticException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Affects*(_, UND)"), QP::QuerySemanticException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Affects*(UND, _)"), QP::QuerySemanticException);
		}

		SECTION("Mismatched Synonym Type") {
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Affects(_, c1)"), QP::QuerySemanticException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Affects(i1, _)"), QP::QuerySemanticException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Affects*(_, w1)"), QP::QuerySemanticException);
			REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Affects*(r1, _)"), QP::QuerySemanticException);
		}
	}
}

TEST_CASE("QP::QueryPreprocessor::parseQuery valid while pattern") {
	shared_ptr<QP::Evaluator::Clause> clause;

	QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select w1 pattern w1(v1, _)");
	clause = qp1.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::PatternWhile);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1", "v1"}));

	QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select w1 pattern w1(\"var\", _)");
	clause = qp2.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::PatternWhile);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1"}));

	QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select w1 pattern w1(_, _)");
	clause = qp3.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::PatternWhile);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1"}));
}

TEST_CASE("QueryPreprocessor::parseQuery invalid while pattern") {
	QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery(UnivDeclarations + "Select w1 pattern w1(v1, v2)"), QP::QuerySyntaxException);

	QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery(UnivDeclarations + "Select w1 pattern w1(\"var\", p)"), QP::QuerySyntaxException);

	QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery(UnivDeclarations + "Select w1 pattern w1(_, \"s\")"), QP::QuerySyntaxException);
}

TEST_CASE("QueryPreprocessor::parseQuery valid if pattern") {
	shared_ptr<QP::Evaluator::Clause> clause;

	QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select i1 pattern i1(v1, _, _)");
	clause = qp1.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::PatternIf);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1", "v1"}));

	QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select i1 pattern i1(\"var\", _, _)");
	clause = qp2.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::PatternIf);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1"}));

	QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select w1 pattern i1(_, _, _)");
	clause = qp3.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::PatternIf);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1"}));
}

TEST_CASE("QueryPreprocessor::parseQuery invalid if pattern") {
	QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery(UnivDeclarations + "Select i1 pattern i1(v1, v2, _)"), QP::QuerySyntaxException);

	QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery(UnivDeclarations + "Select i1 pattern i1(\"var\", _)"), QP::QuerySyntaxException);

	QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery(UnivDeclarations + "Select i1 pattern i1(_, \"s\", _)"), QP::QuerySyntaxException);
}

TEST_CASE("QueryPreprocessor::parseQuery Multiple such that") {
	shared_ptr<QP::Evaluator::Clause> clause;
	QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select c2 such that Modifies(c2, _) and Follows(w1, c2)");
	clause = qp1.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"c2"}));
	clause = qp1.getClauseList()[1];
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1", "c2"}));

	QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select w2 such that Parent*(7, _) such that Uses(w2, v1)");
	clause = qp2.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::ParentT);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>());
	clause = qp2.getClauseList()[1];
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w2", "v1"}));
}

TEST_CASE("QueryPreprocessor::parseQuery valid pattern") {
	shared_ptr<QP::Evaluator::Clause> clause;
	QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select a1 pattern a1(v1, _)");
	clause = qp1.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::PatternAssign);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1", "v1"}));
	QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select v1 pattern a1(v1, \"x\")");
	clause = qp2.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::PatternAssign);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1", "v1"}));
	QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select v1 pattern a2  (v1, _\"x\"_)");
	clause = qp3.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::PatternAssign);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a2", "v1"}));
	QueryPreprocessor qpp4;
	QP::QueryProperties qp4 = qpp4.parseQuery(UnivDeclarations + "Select a1 pattern a1(_, _)");
	clause = qp4.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::PatternAssign);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));
	QueryPreprocessor qpp5;
	QP::QueryProperties qp5 = qpp5.parseQuery(UnivDeclarations + "Select a2 pattern a1(_, \"x\")");
	clause = qp5.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::PatternAssign);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));
	QueryPreprocessor qpp6;
	QP::QueryProperties qp6 = qpp6.parseQuery(UnivDeclarations + "Select a1 pattern a1(_, _\"x + 2\"_)");
	clause = qp6.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::PatternAssign);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));
	QueryPreprocessor qpp7;
	QP::QueryProperties qp7 = qpp7.parseQuery(UnivDeclarations + R"(Select a1 pattern a1("foo", "(x+5)*3- y"))");
	clause = qp7.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::PatternAssign);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));
	QueryPreprocessor qpp8;
	QP::QueryProperties qp8 = qpp8.parseQuery(UnivDeclarations + "Select a1 pattern a1(\"foo\", _)");
	clause = qp8.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::PatternAssign);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));
	QueryPreprocessor qpp9;
	QP::QueryProperties qp9 = qpp9.parseQuery(UnivDeclarations + R"(Select a2 pattern a1("foo", _"x-2"_))");
	clause = qp9.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::PatternAssign);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));
}

TEST_CASE("QueryPreprocessor::parseQuery invalid pattern") {
	// invalid synonym
	QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery(UnivDeclarations + "Select a1 pattern s1(v1, \"x\")"), QP::QuerySemanticException);
	// undeclared synonym
	QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery(UnivDeclarations + "Select a1 pattern b1(v1, \"x\")"), QP::QuerySemanticException);
	// unexpected integer
	QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery(UnivDeclarations + "Select a1 pattern a1(1, \"x\") "), QP::QuerySyntaxException);
	// unexpected integer
	QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery(UnivDeclarations + "Select a1 pattern a1(v1, 1)"), QP::QuerySyntaxException);
	// unexpected synonym
	QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery(UnivDeclarations + "Select a1 pattern a1(v1, s1)"), QP::QuerySyntaxException);
	// invalid synonym
	QueryPreprocessor qpp6;
	REQUIRE_THROWS_AS(qpp6.parseQuery(UnivDeclarations + "Select a1 pattern a1(s1, \"x\")"), QP::QuerySemanticException);
	// missing synonym
	QueryPreprocessor qpp7;
	REQUIRE_THROWS_AS(qpp7.parseQuery(UnivDeclarations + "Select a1 pattern(s1, \"x\")"), QP::QuerySyntaxException);
	// incorrect expression
	QueryPreprocessor qpp8;
	REQUIRE_THROWS_AS(qpp8.parseQuery(UnivDeclarations + "Select a1 pattern(s1, \"x)\")"), QP::QuerySyntaxException);
}

TEST_CASE("QueryPreprocessor::parseQuery valid with") {
	shared_ptr<QP::Evaluator::Clause> clause;
	QueryPreprocessor qpp;
	QP::QueryProperties qp = {{}, {}, {}};

	SECTION("Trivial") {
		qp = qpp.parseQuery(UnivDeclarations + "Select s1 with 2 = 1");
		clause = qp.getClauseList()[0];
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>());

		qp = qpp.parseQuery(UnivDeclarations + "Select s1 with \"this\" = \"other\"");
		clause = qp.getClauseList()[0];
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>());
	}

	SECTION("Statement indexes") {
		qp = qpp.parseQuery(UnivDeclarations + "Select s1 with s1.stmt# = 1");
		clause = qp.getClauseList()[0];
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1"}));

		qp = qpp.parseQuery(UnivDeclarations + "Select s1 with 1 = s1.stmt#");
		clause = qp.getClauseList()[0];
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1"}));

		qp = qpp.parseQuery(UnivDeclarations + "Select s1 with s1.stmt# = s2.stmt#");
		clause = qp.getClauseList()[0];
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1", "s2"}));
	}

	SECTION("Procedure name") {
		qp = qpp.parseQuery(UnivDeclarations + "Select pc1 with pc1.procName = \"name\"");
		clause = qp.getClauseList()[0];
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc1"}));

		qp = qpp.parseQuery(UnivDeclarations + "Select pc1 with \"name\" = pc1.procName");
		clause = qp.getClauseList()[0];
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc1"}));

		qp = qpp.parseQuery(UnivDeclarations + "Select pc1 with c1.procName = pc2.procName");
		clause = qp.getClauseList()[0];
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"c1", "pc2"}));
	}

	SECTION("Variable name") {
		qp = qpp.parseQuery(UnivDeclarations + "Select pc1 with r1.varName = \"name\"");
		clause = qp.getClauseList()[0];
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"r1"}));

		qp = qpp.parseQuery(UnivDeclarations + "Select pc1 with \"name\" = p1.varName");
		clause = qp.getClauseList()[0];
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"p1"}));

		qp = qpp.parseQuery(UnivDeclarations + "Select pc1 with v1.varName = v2.varName");
		clause = qp.getClauseList()[0];
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"v1", "v2"}));
	}

	SECTION("Value") {
		qp = qpp.parseQuery(UnivDeclarations + "Select ct1 with ct1.value = 1");
		clause = qp.getClauseList()[0];
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"ct1"}));

		qp = qpp.parseQuery(UnivDeclarations + "Select pc1 with 1 = ct1.value");
		clause = qp.getClauseList()[0];
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"ct1"}));

		qp = qpp.parseQuery(UnivDeclarations + "Select pc1 with ct1.value = ct2.value");
		clause = qp.getClauseList()[0];
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"ct1", "ct2"}));
	}
}

TEST_CASE("QueryPreprocessor::parseQuery Multiple with clauses") {
	shared_ptr<QP::Evaluator::Clause> clause;
	QueryPreprocessor qpp;
	QP::QueryProperties qp = {{}, {}, {}};

	qp = qpp.parseQuery(UnivDeclarations + "Select a1 with ct1.value = ct2.value and \"name\" = p1.varName");
	clause = qp.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::With);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"ct1", "ct2"}));
	clause = qp.getClauseList()[1];
	REQUIRE(clause->getType() == ClauseType::With);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"p1"}));

	qp = qpp.parseQuery(UnivDeclarations + "Select a1 with ct1.value = ct2.value with \"name\" = p1.varName");
	clause = qp.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::With);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"ct1", "ct2"}));
	clause = qp.getClauseList()[1];
	REQUIRE(clause->getType() == ClauseType::With);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"p1"}));
}

TEST_CASE("QueryPreprocessor::parseQuery invalid with") {
	QueryPreprocessor qpp;

	SECTION("Undeclared synonym") {
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with x.stmt# = 20"), QP::QuerySemanticException);
	}

	SECTION("Invalid attribute") {
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with s1.stmt = 20"), QP::QuerySyntaxException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with ct1.val = 20"), QP::QuerySyntaxException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with p1.varname = \"name\""), QP::QuerySyntaxException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with r1.varname = \"name\""), QP::QuerySyntaxException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with pc1.procname = \"name\""), QP::QuerySyntaxException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with c1.procname = \"name\""), QP::QuerySyntaxException);
	}

	SECTION("Type mismatch") {
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with 1 = \"name\""), QP::QuerySemanticException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with \"name\" = 1"), QP::QuerySemanticException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with c1.procName = 1"), QP::QuerySemanticException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with v1.varName = 1"), QP::QuerySemanticException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with p1.varName = 1"), QP::QuerySemanticException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with s1.stmt# = \"this\""), QP::QuerySemanticException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with ct1.value = \"this\""), QP::QuerySemanticException);
	}
}

TEST_CASE("QueryPreprocessor::parseQuery Multiple such that/pattern/with clauses") {
	shared_ptr<QP::Evaluator::Clause> clause;
	QueryPreprocessor qpp;
	QP::QueryProperties qp = {{}, {}, {}};

	qp = qpp.parseQuery(UnivDeclarations + "Select a1 such that Follows(w1, a1) pattern a1(v1, _\"2-4\"_) with a1.stmt# = ct1.value");
	clause = qp.getClauseList()[0];
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1", "a1"}));
	clause = qp.getClauseList()[1];
	REQUIRE(clause->getType() == ClauseType::PatternAssign);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1", "v1"}));
	clause = qp.getClauseList()[2];
	REQUIRE(clause->getType() == ClauseType::With);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1", "ct1"}));
}

TEST_CASE("QueryPreprocessor::parseQuery Semantic exceptions") {
	shared_ptr<QP::Evaluator::Clause> clause;
	QueryPreprocessor qpp;

	SECTION("Undeclared synonym") {
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select unknownSynonym"), QP::QuerySemanticException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 such that Follows(unknownSynonym, s1)"), QP::QuerySemanticException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 pattern a1(unknownSynonym, _)"), QP::QuerySemanticException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with unknownSynonym.procName = r1.varName"),
		                  QP::QuerySemanticException);
	}

	SECTION("Duplicate synonym declaration") {
		REQUIRE_THROWS_AS(qpp.parseQuery("stmt s1; assign s1; Select s1"), QP::QuerySemanticException);
		REQUIRE_THROWS_AS(qpp.parseQuery("stmt s1; stmt s1; Select s1"), QP::QuerySemanticException);
		REQUIRE_THROWS_AS(qpp.parseQuery("stmt s1, s1; Select s1"), QP::QuerySemanticException);
	}
}
