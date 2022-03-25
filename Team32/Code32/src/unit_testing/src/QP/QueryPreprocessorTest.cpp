#include "QP/QueryPreprocessor.h"

#include <string>

#include "catch.hpp"
#include "catch_tools.h"

using namespace QP::Types;

const string UnivDeclarations =
	"stmt s1, s2; read r1, r2; print p1, p2; "
	"call c1, c2; while w1, w2; if i1, i2; "
	"assign a1, a2; variable v1, v2; "
	"constant ct1, ct2; procedure pc1, pc2;";

TEST_CASE("QP::QueryPreprocessor::tokenizeQuery Invalid tokenizer input") {
	QP::QueryPreprocessor qpp1;
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

TEST_CASE("QP::QueryPreprocessor::parseQuery valid declarations") {
	// Test design entities
	QP::QueryPreprocessor qpp1;
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

	QP::QueryPreprocessor qpp2;
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
	QP::QueryPreprocessor qpp3;
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

TEST_CASE("QP::QueryPreprocessor::parseQuery invalid declarations") {
	// wrong spelling / case
	QP::QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery("IF ifs; Select ifs"), QP::QueryException);
	QP::QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery("prints a; Select a"), QP::QueryException);
	QP::QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery("assssign a; Select a"), QP::QueryException);

	// missing ; / ,
	QP::QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery("while a Select a"), QP::QueryException);
	QP::QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery("variable a b c; Select a"), QP::QueryException);

	// Duplicated synonym
	QP::QueryPreprocessor qpp6;
	REQUIRE_THROWS_AS(qpp6.parseQuery("while a, a; Select a"), QP::QueryException);
	QP::QueryPreprocessor qpp7;
	REQUIRE_THROWS_AS(qpp7.parseQuery("read a; print b; procedure a; Select a"), QP::QueryException);
}

TEST_CASE("QP::QueryPreprocessor::parseQuery Select single") {
	QP::QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery("print a; if b; Select a");
	REQUIRE(qp1.getSelectList()[0].getSynonymType() == QP::Types::DesignEntity::Print);
	REQUIRE(qp1.getSelectList()[0].getSynonymSymbol() == "a");

	QP::QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery("print a; if b; Select b");
	REQUIRE(qp2.getSelectList()[0].getSynonymType() == QP::Types::DesignEntity::If);
	REQUIRE(qp2.getSelectList()[0].getSynonymSymbol() == "b");

	// select BOOLEAN synonym
	QP::QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery("if BOOLEAN; Select BOOLEAN");
	REQUIRE(qp3.getSelectList()[0].getSynonymType() == QP::Types::DesignEntity::If);
	REQUIRE(qp3.getSelectList()[0].getSynonymSymbol() == "BOOLEAN");

	// undeclared synonym
	QP::QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery("print a; if b; Select unknownSynonym"), QP::QueryException);
	// more than 1 synonym
	QP::QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery("print a; if b; Select a,b"), QP::QueryException);
	// more than 1 select
	QP::QueryPreprocessor qpp6;
	REQUIRE_THROWS_AS(qpp6.parseQuery("print a; if b; Select a Select b"), QP::QueryException);
}

TEST_CASE("QP::QueryPreprocessor::parseQuery Select tuple") {
	QP::QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery("print a; if b; Select <a>");
	REQUIRE(qp1.getSelectList()[0].getSynonymType() == QP::Types::DesignEntity::Print);
	REQUIRE(qp1.getSelectList()[0].getSynonymSymbol() == "a");

	QP::QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery("print a; if b; Select <a,b>");
	REQUIRE(qp2.getSelectList()[0].getSynonymType() == QP::Types::DesignEntity::Print);
	REQUIRE(qp2.getSelectList()[0].getSynonymSymbol() == "a");
	REQUIRE(qp2.getSelectList()[1].getSynonymType() == QP::Types::DesignEntity::If);
	REQUIRE(qp2.getSelectList()[1].getSynonymSymbol() == "b");
	// Same synonym
	QP::QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery("print a; if b; Select <b,b>");
	REQUIRE(qp3.getSelectList()[0].getSynonymType() == QP::Types::DesignEntity::If);
	REQUIRE(qp3.getSelectList()[0].getSynonymSymbol() == "b");
	REQUIRE(qp3.getSelectList()[1].getSynonymType() == QP::Types::DesignEntity::If);
	REQUIRE(qp3.getSelectList()[1].getSynonymSymbol() == "b");
	// many synonym
	QP::QueryPreprocessor qpp4;
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
	QP::QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery("print a; if b; Select <>"), QP::QueryException);
	// Missing <>
	QP::QueryPreprocessor qpp6;
	REQUIRE_THROWS_AS(qpp6.parseQuery("print a; if b; Select a,b"), QP::QueryException);
	// undeclared synonym
	QP::QueryPreprocessor qpp7;
	REQUIRE_THROWS_AS(qpp7.parseQuery("print a; if b; Select <a,b,undeclaredSynonym>"), QP::QueryException);
}

TEST_CASE("QP::QueryPreprocessor::parseQuery Select BOOLEAN") {
	QP::QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery("if i; Select BOOLEAN");
	REQUIRE(qp1.getSelectList().size() == 0);

	// lower case
	QP::QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery("if i; Select boolean"), QP::QueryException);
	// tuple
	QP::QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery("if i; Select <BOOLEAN>"), QP::QueryException);
}

TEST_CASE("QP::QueryPreprocessor::parseQuery Select attribute") {
	QP::QueryPreprocessor qpp;
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

TEST_CASE("QP::QueryPreprocessor::parseQuery invalid such that Parent(*)") {
	// disjoint *
	QP::QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery(UnivDeclarations + "Select s1 such that Parent *(s1, s2)"), QP::QueryException);
	// non-statement synonyms
	QP::QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery(UnivDeclarations + "Select v1 such that Parent(v1, s2)"), QP::QueryException);
	QP::QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery(UnivDeclarations + "Select s1 such that Parent(pc1, s2)"), QP::QueryException);
	// misspelt word
	QP::QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery(UnivDeclarations + "Select s1 such that Parents(s1, s2)"), QP::QueryException);
	// disallowed statement reference
	QP::QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery(UnivDeclarations + "Select s1 such that Parents(s1, \"x\")"), QP::QueryException);
}

TEST_CASE("QP::QueryPreprocessor::parseQuery valid such that Follows(*)") {
	shared_ptr<QP::Relationship::Relation> clause;
	// Combinations of stmtRef : synonym | '_' | INTEGER for Follows
	QP::QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select s1 such that Follows(s1, s2)");
	clause = qp1.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1", "s2"}));

	QP::QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select i1 such that Follows(i1, _)");
	clause = qp2.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1"}));

	QP::QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select r1 such that Follows(r1, 20)");
	clause = qp3.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"r1"}));

	QP::QueryPreprocessor qpp4;
	QP::QueryProperties qp4 = qpp4.parseQuery(UnivDeclarations + "Select a1 such that Follows(1, a1)");
	clause = qp4.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));

	QP::QueryPreprocessor qpp5;
	QP::QueryProperties qp5 = qpp5.parseQuery(UnivDeclarations + "Select i1 such that Follows(32, 3)");
	clause = qp5.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>());

	QP::QueryPreprocessor qpp6;
	QP::QueryProperties qp6 = qpp6.parseQuery(UnivDeclarations + "Select w2 such that Follows(1010, _)");
	clause = qp6.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>());

	QP::QueryPreprocessor qpp7;
	QP::QueryProperties qp7 = qpp7.parseQuery(UnivDeclarations + "Select s1 such that Follows(_, p2)");
	clause = qp7.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"p2"}));

	QP::QueryPreprocessor qpp8;
	QP::QueryProperties qp8 = qpp8.parseQuery(UnivDeclarations + "Select i1 such that Follows(_, 1)");
	clause = qp8.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>());

	QP::QueryPreprocessor qpp9;
	QP::QueryProperties qp9 = qpp9.parseQuery(UnivDeclarations + "Select w2 such that Follows(_, _)");
	clause = qp9.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>());

	// Follows*
	QP::QueryPreprocessor qpp10;
	QP::QueryProperties qp10 = qpp10.parseQuery(UnivDeclarations + "Select c2 such that Follows*(w1, c2)");
	clause = qp10.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::FollowsT);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1", "c2"}));
	QP::QueryPreprocessor qpp11;
	QP::QueryProperties qp11 = qpp11.parseQuery(UnivDeclarations + "Select c2 such that Follows*      (   w1  , c2  )");
	clause = qp11.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::FollowsT);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1", "c2"}));

	// Multiple Follows(*) clauses
	QP::QueryPreprocessor qpp12;
	QP::QueryProperties qp12 = qpp12.parseQuery(UnivDeclarations + "Select c2 such that Follows(i1, _) and Follows(w1, c2)");
	clause = qp12.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1"}));
	clause = qp12.getClauseList()[1].relation;
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1", "c2"}));
	QP::QueryPreprocessor qpp13;
	QP::QueryProperties qp13 = qpp13.parseQuery(UnivDeclarations + "Select c2 such that Follows*(i1, _) and Follows(w1, c2)");
	clause = qp13.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::FollowsT);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1"}));
	clause = qp13.getClauseList()[1].relation;
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1", "c2"}));
}

TEST_CASE("QP::QueryPreprocessor::parseQuery invalid such that Follows(*)") {
	// Missing (
	QP::QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery(UnivDeclarations + "Select s1 such that Follows s1, s2)"), QP::QueryException);
	// disjoint *
	QP::QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery(UnivDeclarations + "Select s1 such that Follows *(s1, s2)"), QP::QueryException);
	// non-statement synonyms
	QP::QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery(UnivDeclarations + "Select s1 such that Follows(pc1, s2)"), QP::QueryException);
	// misspelt word
	QP::QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery(UnivDeclarations + "Select s1 such that Follow(s1, s2)"), QP::QueryException);
	// disallowed statement reference
	QP::QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery(UnivDeclarations + "Select s1 such that Follows(s1, \"x\")"), QP::QueryException);
	// statement number with leading 0
	QP::QueryPreprocessor qpp6;
	REQUIRE_THROWS_AS(qpp6.parseQuery(UnivDeclarations + "Select s1 such that Follows(s1, 007)"), QP::QueryException);
}

TEST_CASE("QP::QueryPreprocessor::parseQuery valid such that Next(*)") {
	shared_ptr<QP::Relationship::Relation> clause;
	// Combinations of stmtRef : synonym | '_' | INTEGER for Next
	QP::QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select s1 such that Next(s1, s2)");
	clause = qp1.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Next);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1", "s2"}));

	QP::QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select i1 such that Next(i1, _)");
	clause = qp2.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Next);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1"}));

	QP::QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select r1 such that Next(r1, 20)");
	clause = qp3.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Next);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"r1"}));

	QP::QueryPreprocessor qpp4;
	QP::QueryProperties qp4 = qpp4.parseQuery(UnivDeclarations + "Select a1 such that Next(1, a1)");
	clause = qp4.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Next);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));

	QP::QueryPreprocessor qpp5;
	QP::QueryProperties qp5 = qpp5.parseQuery(UnivDeclarations + "Select i1 such that Next(32, 3)");
	clause = qp5.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Next);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>());

	QP::QueryPreprocessor qpp6;
	QP::QueryProperties qp6 = qpp6.parseQuery(UnivDeclarations + "Select w2 such that Next(1010, _)");
	clause = qp6.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Next);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>());

	QP::QueryPreprocessor qpp7;
	QP::QueryProperties qp7 = qpp7.parseQuery(UnivDeclarations + "Select s1 such that Next(_, p2)");
	clause = qp7.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Next);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"p2"}));

	QP::QueryPreprocessor qpp8;
	QP::QueryProperties qp8 = qpp8.parseQuery(UnivDeclarations + "Select i1 such that Next(_, 1)");
	clause = qp8.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Next);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>());

	QP::QueryPreprocessor qpp9;
	QP::QueryProperties qp9 = qpp9.parseQuery(UnivDeclarations + "Select w2 such that Next(_, _)");
	clause = qp9.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Next);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>());

	// Follows*
	QP::QueryPreprocessor qpp10;
	QP::QueryProperties qp10 = qpp10.parseQuery(UnivDeclarations + "Select c2 such that Next*(w1, c2)");
	clause = qp10.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::NextT);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1", "c2"}));
	QP::QueryPreprocessor qpp11;
	QP::QueryProperties qp11 = qpp11.parseQuery(UnivDeclarations + "Select c2 such that Next*      (   w1  , c2  )");
	clause = qp11.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::NextT);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1", "c2"}));

	// Multiple Follows(*) clauses
	QP::QueryPreprocessor qpp12;
	QP::QueryProperties qp12 = qpp12.parseQuery(UnivDeclarations + "Select c2 such that Next(i1, _) and Next(w1, c2)");
	clause = qp12.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Next);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1"}));
	clause = qp12.getClauseList()[1].relation;
	REQUIRE(clause->getType() == ClauseType::Next);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1", "c2"}));
	QP::QueryPreprocessor qpp13;
	QP::QueryProperties qp13 = qpp13.parseQuery(UnivDeclarations + "Select c2 such that Next*(i1, _) and Next(w1, c2)");
	clause = qp13.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::NextT);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1"}));
	clause = qp13.getClauseList()[1].relation;
	REQUIRE(clause->getType() == ClauseType::Next);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1", "c2"}));
}

TEST_CASE("QP::QueryPreprocessor::parseQuery invalid such that Next(*)") {
	// Missing (
	QP::QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery(UnivDeclarations + "Select s1 such that Next s1, s2)"), QP::QueryException);
	// disjoint *
	QP::QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery(UnivDeclarations + "Select s1 such that Next *(s1, s2)"), QP::QueryException);
	// non-statement synonyms
	QP::QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery(UnivDeclarations + "Select s1 such that Next(pc1, s2)"), QP::QueryException);
	// misspelt word
	QP::QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery(UnivDeclarations + "Select s1 such that Nexts(s1, s2)"), QP::QueryException);
	// disallowed statement reference
	QP::QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery(UnivDeclarations + "Select s1 such that Next(s1, \"x\")"), QP::QueryException);
	// statement number with leading 0
	QP::QueryPreprocessor qpp6;
	REQUIRE_THROWS_AS(qpp6.parseQuery(UnivDeclarations + "Select s1 such that Next(s1, 007)"), QP::QueryException);
}

TEST_CASE("QP::QueryPreprocessor::parseQuery valid such that UsesS/P") {
	// Valid UsesS
	shared_ptr<QP::Relationship::Relation> clause;
	QP::QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select v1 such that Uses(s1, v1)");
	clause = qp1.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1", "v1"}));

	QP::QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select s1 such that Uses(s1, _)");
	clause = qp2.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1"}));

	QP::QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select s1 such that Uses(s1, \"x\")");
	clause = qp3.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1"}));

	QP::QueryPreprocessor qpp4;
	QP::QueryProperties qp4 = qpp4.parseQuery(UnivDeclarations + "Select v1 such that Uses(i1, v1)");
	clause = qp4.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1", "v1"}));

	QP::QueryPreprocessor qpp5;
	QP::QueryProperties qp5 = qpp5.parseQuery(UnivDeclarations + "Select p1 such that Uses(p1, _)");
	clause = qp5.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"p1"}));

	QP::QueryPreprocessor qpp6;
	QP::QueryProperties qp6 = qpp6.parseQuery(UnivDeclarations + "Select a1 such that Uses(a1, \"y\")");
	clause = qp6.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));

	QP::QueryPreprocessor qpp7;
	QP::QueryProperties qp7 = qpp7.parseQuery(UnivDeclarations + "Select v1 such that Uses(w2, v1)");
	clause = qp7.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w2", "v1"}));

	QP::QueryPreprocessor qpp8;
	QP::QueryProperties qp8 = qpp8.parseQuery(UnivDeclarations + "Select i1 such that Uses(i1, _)");
	clause = qp8.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1"}));

	QP::QueryPreprocessor qpp9;
	QP::QueryProperties qp9 = qpp9.parseQuery(UnivDeclarations + "Select w1 such that Uses(w1, \"z0\")");
	clause = qp9.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1"}));

	QP::QueryPreprocessor qpp10;
	QP::QueryProperties qp10 = qpp10.parseQuery(UnivDeclarations + "Select v1 such that Uses(1, v1)");
	clause = qp10.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"v1"}));

	QP::QueryPreprocessor qpp11;
	QP::QueryProperties qp11 = qpp11.parseQuery(UnivDeclarations + "Select s1 such that Uses(c2, _)");
	clause = qp11.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"c2"}));

	QP::QueryPreprocessor qpp12;
	QP::QueryProperties qp12 = qpp12.parseQuery(UnivDeclarations + "Select s1 such that Uses(w2, \"var\")");
	clause = qp12.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w2"}));

	// UsesP
	QP::QueryPreprocessor qpp13;
	QP::QueryProperties qp13 = qpp13.parseQuery(UnivDeclarations + "Select v1 such that Uses(pc1, v1)");
	clause = qp13.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::UsesP);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc1", "v1"}));

	QP::QueryPreprocessor qpp14;
	QP::QueryProperties qp14 = qpp14.parseQuery(UnivDeclarations + "Select v1 such that Uses(pc1, _)");
	clause = qp14.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::UsesP);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc1"}));

	QP::QueryPreprocessor qpp15;
	QP::QueryProperties qp15 = qpp15.parseQuery(UnivDeclarations + R"(Select s2 such that Uses("procedureName", "varName"))");
	clause = qp15.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::UsesP);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>());
}

TEST_CASE("QP::QueryPreprocessor::parseQuery invalid such that UsesS/P") {
	// unexpected wildcard
	QP::QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery(UnivDeclarations + "Select v1 such that Uses(_, v1)"), QP::QueryException);
	// unexpected integer
	QP::QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery(UnivDeclarations + "Select pc1 such that Uses(pc1, 1)"), QP::QueryException);
	// invalid synonym
	QP::QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery(UnivDeclarations + "Select s1 such that Uses(1, s1)"), QP::QueryException);
	QP::QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery(UnivDeclarations + "Select r1 such that Uses(r1, _)"), QP::QueryException);
	// undeclared synonym
	QP::QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery(UnivDeclarations + "Select s1 such that Uses(s1, UNK)"), QP::QueryException);
	// misspelled keyword
	QP::QueryPreprocessor qpp6;
	REQUIRE_THROWS_AS(qpp6.parseQuery(UnivDeclarations + "Select s1 such that use(s1, a1)"), QP::QueryException);
}

TEST_CASE("QP::QueryPreprocessor::parseQuery valid such that ModifiesS/P") {
	shared_ptr<QP::Relationship::Relation> clause;
	// Valid ModifiesS
	QP::QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select v1 such that Modifies(s1, v1)");
	clause = qp1.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1", "v1"}));

	QP::QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select s1 such that Modifies(s1, _)");
	clause = qp2.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1"}));

	QP::QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select s1 such that Modifies(s1, \"x\")");
	clause = qp3.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1"}));

	QP::QueryPreprocessor qpp4;
	QP::QueryProperties qp4 = qpp4.parseQuery(UnivDeclarations + "Select v1 such that Modifies(i1, v1)");
	clause = qp4.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1", "v1"}));

	QP::QueryPreprocessor qpp5;
	QP::QueryProperties qp5 = qpp5.parseQuery(UnivDeclarations + "Select p1 such that Modifies(r1, _)");
	clause = qp5.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"r1"}));

	QP::QueryPreprocessor qpp6;
	QP::QueryProperties qp6 = qpp6.parseQuery(UnivDeclarations + "Select a1 such that Modifies(a1, \"y\")");
	clause = qp6.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));

	QP::QueryPreprocessor qpp7;
	QP::QueryProperties qp7 = qpp7.parseQuery(UnivDeclarations + "Select v1 such that Modifies(w2, v1)");
	clause = qp7.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w2", "v1"}));

	QP::QueryPreprocessor qpp8;
	QP::QueryProperties qp8 = qpp8.parseQuery(UnivDeclarations + "Select i1 such that Modifies(i1, _)");
	clause = qp8.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1"}));

	QP::QueryPreprocessor qpp9;
	QP::QueryProperties qp9 = qpp9.parseQuery(UnivDeclarations + "Select w1 such that Modifies(w1, \"z0\")");
	clause = qp9.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1"}));

	QP::QueryPreprocessor qpp10;
	QP::QueryProperties qp10 = qpp10.parseQuery(UnivDeclarations + "Select v1 such that Modifies(1, v1)");
	clause = qp10.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"v1"}));

	QP::QueryPreprocessor qpp11;
	QP::QueryProperties qp11 = qpp11.parseQuery(UnivDeclarations + "Select s1 such that Modifies(w2, _)");
	clause = qp11.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w2"}));

	QP::QueryPreprocessor qpp12;
	QP::QueryProperties qp12 = qpp12.parseQuery(UnivDeclarations + "Select s1 such that Modifies(c2, \"var\")");
	clause = qp12.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"c2"}));

	// ModifiesP
	QP::QueryPreprocessor qpp13;
	QP::QueryProperties qp13 = qpp13.parseQuery(UnivDeclarations + "Select v1 such that Modifies(pc1, v1)");
	clause = qp13.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::ModifiesP);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc1", "v1"}));

	QP::QueryPreprocessor qpp14;
	QP::QueryProperties qp14 = qpp14.parseQuery(UnivDeclarations + "Select v1 such that Modifies(pc1, _)");
	clause = qp14.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::ModifiesP);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc1"}));

	QP::QueryPreprocessor qpp15;
	QP::QueryProperties qp15 = qpp15.parseQuery(UnivDeclarations + R"(Select s2 such that Modifies("procedureName", "varName"))");
	clause = qp15.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::ModifiesP);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({}));
}

TEST_CASE("QP::QueryPreprocessor::parseQuery invalid such that ModifiesS/P") {
	// unexpected wildcard
	QP::QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery(UnivDeclarations + "Select v1 such that Modifies(_, v1)"), QP::QueryException);
	// unexpected integer
	QP::QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery(UnivDeclarations + "Select pc1 such that Modifies(pc1, 1)"), QP::QueryException);
	// invalid synonym
	QP::QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery(UnivDeclarations + "Select s1 such that Modifies(1, 0s)"), QP::QueryException);
	QP::QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery(UnivDeclarations + "Select s1 such that Modifies(1, s1)"), QP::QueryException);
	QP::QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery(UnivDeclarations + "Select p1 such that Modifies(p1, v1)"), QP::QueryException);
	// undeclared synonym
	QP::QueryPreprocessor qpp6;
	REQUIRE_THROWS_AS(qpp6.parseQuery(UnivDeclarations + "Select s1 such that Modifies(s1, UNK)"), QP::QueryException);
	// misspelled keyword
	QP::QueryPreprocessor qpp7;
	REQUIRE_THROWS_AS(qpp7.parseQuery(UnivDeclarations + "Select s1 such that modifies(s1, a1)"), QP::QueryException);
}

TEST_CASE("QP::QueryPreprocessor::parseQuery valid such that Calls(*)") {
	shared_ptr<QP::Relationship::Relation> clause;

	QP::QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select pc1 such that Calls(pc1, pc2)");
	clause = qp1.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Calls);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc1", "pc2"}));

	QP::QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select pc1 such that Calls(pc1, _)");
	clause = qp2.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Calls);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc1"}));

	QP::QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select pc1 such that Calls(pc1, \"procedure1\")");
	clause = qp3.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Calls);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc1"}));

	QP::QueryPreprocessor qpp4;
	QP::QueryProperties qp4 = qpp4.parseQuery(UnivDeclarations + "Select pc1 such that Calls(\"procedure1\", pc1)");
	clause = qp4.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Calls);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc1"}));

	QP::QueryPreprocessor qpp5;
	QP::QueryProperties qp5 = qpp5.parseQuery(UnivDeclarations + "Select i1 such that Calls(\"procedure1\", \"procedure2\")");
	clause = qp5.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Calls);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({}));

	QP::QueryPreprocessor qpp6;
	QP::QueryProperties qp6 = qpp6.parseQuery(UnivDeclarations + "Select w2 such that Calls(\"procedure1\", _)");
	clause = qp6.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Calls);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({}));

	QP::QueryPreprocessor qpp7;
	QP::QueryProperties qp7 = qpp7.parseQuery(UnivDeclarations + "Select s1 such that Calls(_, pc2)");
	clause = qp7.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Calls);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc2"}));

	QP::QueryPreprocessor qpp8;
	QP::QueryProperties qp8 = qpp8.parseQuery(UnivDeclarations + "Select i1 such that Calls*(_, \"procedure1\")");
	clause = qp8.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::CallsT);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({}));

	QP::QueryPreprocessor qpp9;
	QP::QueryProperties qp9 = qpp9.parseQuery(UnivDeclarations + "Select w2 such that Calls(_, _)");
	clause = qp9.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Calls);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({}));

	QP::QueryPreprocessor qpp10;
	QP::QueryProperties qp10 = qpp10.parseQuery(UnivDeclarations + "Select pc2 such that Calls*(pc1, pc2)");
	clause = qp10.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::CallsT);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc1", "pc2"}));
}

TEST_CASE("QP::QueryPreprocessor::parseQuery valid while pattern") {
	shared_ptr<QP::Relationship::Relation> clause;

	QP::QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select w1 pattern w1(v1, _)");
	clause = qp1.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::PatternWhile);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1", "v1"}));

	QP::QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select w1 pattern w1(\"var\", _)");
	clause = qp2.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::PatternWhile);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1"}));

	QP::QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select w1 pattern w1(_, _)");
	clause = qp3.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::PatternWhile);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1"}));
}

TEST_CASE("QP::QueryPreprocessor::parseQuery invalid while pattern") {
	QP::QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery(UnivDeclarations + "Select w1 pattern w1(v1, v2)"), QP::QueryException);

	QP::QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery(UnivDeclarations + "Select w1 pattern w1(\"var\", p)"), QP::QueryException);

	QP::QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery(UnivDeclarations + "Select w1 pattern w1(_, \"s\")"), QP::QueryException);
}

TEST_CASE("QP::QueryPreprocessor::parseQuery valid if pattern") {
	shared_ptr<QP::Relationship::Relation> clause;

	QP::QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select i1 pattern i1(v1, _, _)");
	clause = qp1.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::PatternIf);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1", "v1"}));

	QP::QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select i1 pattern i1(\"var\", _, _)");
	clause = qp2.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::PatternIf);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1"}));

	QP::QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select w1 pattern i1(_, _, _)");
	clause = qp3.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::PatternIf);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"i1"}));
}

TEST_CASE("QP::QueryPreprocessor::parseQuery invalid if pattern") {
	QP::QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery(UnivDeclarations + "Select i1 pattern i1(v1, v2, _)"), QP::QueryException);

	QP::QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery(UnivDeclarations + "Select i1 pattern i1(\"var\", _)"), QP::QueryException);

	QP::QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery(UnivDeclarations + "Select i1 pattern i1(_, \"s\", _)"), QP::QueryException);
}

TEST_CASE("QP::QueryPreprocessor::parseQuery Multiple such that") {
	shared_ptr<QP::Relationship::Relation> clause;
	QP::QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select c2 such that Modifies(c2, _) and Follows(w1, c2)");
	clause = qp1.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::ModifiesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"c2"}));
	clause = qp1.getClauseList()[1].relation;
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1", "c2"}));

	QP::QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select w2 such that Parent*(7, _) such that Uses(w2, v1)");
	clause = qp2.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::ParentT);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>());
	clause = qp2.getClauseList()[1].relation;
	REQUIRE(clause->getType() == ClauseType::UsesS);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w2", "v1"}));
}

TEST_CASE("QP::QueryPreprocessor::parseQuery valid pattern") {
	shared_ptr<QP::Relationship::Relation> clause;
	QP::QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select a1 pattern a1(v1, _)");
	clause = qp1.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::PatternAssign);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1", "v1"}));
	QP::QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select v1 pattern a1(v1, \"x\")");
	clause = qp2.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::PatternAssign);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1", "v1"}));
	QP::QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select v1 pattern a2  (v1, _\"x\"_)");
	clause = qp3.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::PatternAssign);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a2", "v1"}));
	QP::QueryPreprocessor qpp4;
	QP::QueryProperties qp4 = qpp4.parseQuery(UnivDeclarations + "Select a1 pattern a1(_, _)");
	clause = qp4.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::PatternAssign);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));
	QP::QueryPreprocessor qpp5;
	QP::QueryProperties qp5 = qpp5.parseQuery(UnivDeclarations + "Select a2 pattern a1(_, \"x\")");
	clause = qp5.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::PatternAssign);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));
	QP::QueryPreprocessor qpp6;
	QP::QueryProperties qp6 = qpp6.parseQuery(UnivDeclarations + "Select a1 pattern a1(_, _\"x + 2\"_)");
	clause = qp6.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::PatternAssign);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));
	QP::QueryPreprocessor qpp7;
	QP::QueryProperties qp7 = qpp7.parseQuery(UnivDeclarations + R"(Select a1 pattern a1("foo", "(x+5)*3- y"))");
	clause = qp7.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::PatternAssign);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));
	QP::QueryPreprocessor qpp8;
	QP::QueryProperties qp8 = qpp8.parseQuery(UnivDeclarations + "Select a1 pattern a1(\"foo\", _)");
	clause = qp8.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::PatternAssign);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));
	QP::QueryPreprocessor qpp9;
	QP::QueryProperties qp9 = qpp9.parseQuery(UnivDeclarations + R"(Select a2 pattern a1("foo", _"x-2"_))");
	clause = qp9.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::PatternAssign);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1"}));
}

TEST_CASE("QP::QueryPreprocessor::parseQuery invalid pattern") {
	// invalid synonym
	QP::QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery(UnivDeclarations + "Select a1 pattern s1(v1, \"x\")"), QP::QueryException);
	// undeclared synonym
	QP::QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery(UnivDeclarations + "Select a1 pattern b1(v1, \"x\")"), QP::QueryException);
	// unexpected integer
	QP::QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery(UnivDeclarations + "Select a1 pattern a1(1, \"x\") "), QP::QueryException);
	// unexpected integer
	QP::QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery(UnivDeclarations + "Select a1 pattern a1(v1, 1)"), QP::QueryException);
	// unexpected synonym
	QP::QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery(UnivDeclarations + "Select a1 pattern a1(v1, s1)"), QP::QueryException);
	// invalid synonym
	QP::QueryPreprocessor qpp6;
	REQUIRE_THROWS_AS(qpp6.parseQuery(UnivDeclarations + "Select a1 pattern a1(s1, \"x\")"), QP::QueryException);
	// missing synonym
	QP::QueryPreprocessor qpp7;
	REQUIRE_THROWS_AS(qpp7.parseQuery(UnivDeclarations + "Select a1 pattern(s1, \"x\")"), QP::QueryException);
	// incorrect expression
	QP::QueryPreprocessor qpp8;
	REQUIRE_THROWS_AS(qpp8.parseQuery(UnivDeclarations + "Select a1 pattern(s1, \"x)\")"), QP::QueryException);
}

TEST_CASE("QP::QueryPreprocessor::parseQuery valid with") {
	shared_ptr<QP::Relationship::Relation> clause;
	QP::QueryPreprocessor qpp;
	QP::QueryProperties qp = {{}, {}, {}};

	SECTION("Trivial") {
		qp = qpp.parseQuery(UnivDeclarations + "Select s1 with 2 = 1");
		clause = qp.getClauseList()[0].relation;
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>());

		qp = qpp.parseQuery(UnivDeclarations + "Select s1 with \"this\" = \"other\"");
		clause = qp.getClauseList()[0].relation;
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>());
	}

	SECTION("Statement indexes") {
		qp = qpp.parseQuery(UnivDeclarations + "Select s1 with s1.stmt# = 1");
		clause = qp.getClauseList()[0].relation;
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1"}));

		qp = qpp.parseQuery(UnivDeclarations + "Select s1 with 1 = s1.stmt#");
		clause = qp.getClauseList()[0].relation;
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1"}));

		qp = qpp.parseQuery(UnivDeclarations + "Select s1 with s1.stmt# = s2.stmt#");
		clause = qp.getClauseList()[0].relation;
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"s1", "s2"}));
	}

	SECTION("Procedure name") {
		qp = qpp.parseQuery(UnivDeclarations + "Select pc1 with pc1.procName = \"name\"");
		clause = qp.getClauseList()[0].relation;
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc1"}));

		qp = qpp.parseQuery(UnivDeclarations + "Select pc1 with \"name\" = pc1.procName");
		clause = qp.getClauseList()[0].relation;
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"pc1"}));

		qp = qpp.parseQuery(UnivDeclarations + "Select pc1 with c1.procName = pc2.procName");
		clause = qp.getClauseList()[0].relation;
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"c1", "pc2"}));
	}

	SECTION("Variable name") {
		qp = qpp.parseQuery(UnivDeclarations + "Select pc1 with r1.varName = \"name\"");
		clause = qp.getClauseList()[0].relation;
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"r1"}));

		qp = qpp.parseQuery(UnivDeclarations + "Select pc1 with \"name\" = p1.varName");
		clause = qp.getClauseList()[0].relation;
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"p1"}));

		qp = qpp.parseQuery(UnivDeclarations + "Select pc1 with v1.varName = v2.varName");
		clause = qp.getClauseList()[0].relation;
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"v1", "v2"}));
	}

	SECTION("Value") {
		qp = qpp.parseQuery(UnivDeclarations + "Select ct1 with ct1.value = 1");
		clause = qp.getClauseList()[0].relation;
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"ct1"}));

		qp = qpp.parseQuery(UnivDeclarations + "Select pc1 with 1 = ct1.value");
		clause = qp.getClauseList()[0].relation;
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"ct1"}));

		qp = qpp.parseQuery(UnivDeclarations + "Select pc1 with ct1.value = ct2.value");
		clause = qp.getClauseList()[0].relation;
		REQUIRE(clause->getType() == ClauseType::With);
		REQUIRE(clause->getDeclarationSymbols() == vector<string>({"ct1", "ct2"}));
	}
}

TEST_CASE("QP::QueryPreprocessor::parseQuery Multiple with clauses") {
	shared_ptr<QP::Relationship::Relation> clause;
	QP::QueryPreprocessor qpp;
	QP::QueryProperties qp = {{}, {}, {}};

	qp = qpp.parseQuery(UnivDeclarations + "Select a1 with ct1.value = ct2.value and \"name\" = p1.varName");
	clause = qp.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::With);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"ct1", "ct2"}));
	clause = qp.getClauseList()[1].relation;
	REQUIRE(clause->getType() == ClauseType::With);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"p1"}));

	qp = qpp.parseQuery(UnivDeclarations + "Select a1 with ct1.value = ct2.value with \"name\" = p1.varName");
	clause = qp.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::With);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"ct1", "ct2"}));
	clause = qp.getClauseList()[1].relation;
	REQUIRE(clause->getType() == ClauseType::With);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"p1"}));
}

TEST_CASE("QP::QueryPreprocessor::parseQuery invalid with") {
	QP::QueryPreprocessor qpp;

	SECTION("Undeclared synonym") {
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with x.stmt# = 20"), QP::QueryException);
	}

	SECTION("Invalid attribute") {
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with s1.stmt = 20"), QP::QueryException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with ct1.val = 20"), QP::QueryException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with p1.varname = \"name\""), QP::QueryException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with r1.varname = \"name\""), QP::QueryException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with pc1.procname = \"name\""), QP::QueryException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with c1.procname = \"name\""), QP::QueryException);
	}

	SECTION("Type mismatch") {
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with 1 = \"name\""), QP::QueryException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with \"name\" = 1"), QP::QueryException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with c1.procName = 1"), QP::QueryException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with v1.varName = 1"), QP::QueryException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with p1.varName = 1"), QP::QueryException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with s1.stmt# = \"this\""), QP::QueryException);
		REQUIRE_THROWS_AS(qpp.parseQuery(UnivDeclarations + "Select s1 with ct1.value = \"this\""), QP::QueryException);
	}
}

TEST_CASE("QP::QueryPreprocessor::parseQuery Multiple such that/pattern/with clauses") {
	shared_ptr<QP::Relationship::Relation> clause;
	QP::QueryPreprocessor qpp;
	QP::QueryProperties qp = {{}, {}, {}};

	qp = qpp.parseQuery(UnivDeclarations + "Select a1 such that Follows(w1, a1) pattern a1(v1, _\"2-4\"_) with a1.stmt# = ct1.value");
	clause = qp.getClauseList()[0].relation;
	REQUIRE(clause->getType() == ClauseType::Follows);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"w1", "a1"}));
	clause = qp.getClauseList()[1].relation;
	REQUIRE(clause->getType() == ClauseType::PatternAssign);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1", "v1"}));
	clause = qp.getClauseList()[2].relation;
	REQUIRE(clause->getType() == ClauseType::With);
	REQUIRE(clause->getDeclarationSymbols() == vector<string>({"a1", "ct1"}));
}
