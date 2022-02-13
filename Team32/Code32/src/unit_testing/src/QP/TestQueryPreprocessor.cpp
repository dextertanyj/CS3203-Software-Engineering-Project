#include "QP/QueryPreprocessor.h"

#include <string>

#include "catch.hpp"
#include "catch_tools.h"

const string univDeclarations = "stmt s1, s2; read r1, r2; print p1, p2; "
								"call c1, c2; while w1, w2; if i1, i2; "
								"assign a1, a2; variable v1, v2; "
								"constant ct1, ct2; procedure pc1, pc2;";

TEST_CASE("QP::QueryPreprocessor::tokenizeQuery Invalid tokenizer input") {
	QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery("!"), QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("@"), QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("#"), QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("$"), QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("^"), QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("&"), QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("|"), QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("~"), QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("["), QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("]"), QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("{"), QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("}"), QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("`"), QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("'"), QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery(":"), QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("\\"), QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery(">"), QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("<"), QueryTokenizationException);
}


TEST_CASE("QP::QueryPreprocessor::parseQuery valid declarations") {

	// Test design entities 
	QueryPreprocessor qpp1;
	QueryProperties qp1 = qpp1.parseQuery(
		"if a, b,c; while Select; read such, that;assign pattern;variable CAP3;\nSelect a"
	);
	auto declarationList1 = qp1.getDeclarationList();
    REQUIRE(declarationList1[0].type == DesignEntity::if_);
    REQUIRE(declarationList1[0].symbol == "a");
	REQUIRE(declarationList1[1].type == DesignEntity::if_);
	REQUIRE(declarationList1[1].symbol == "b");
	REQUIRE(declarationList1[2].type == DesignEntity::if_);
	REQUIRE(declarationList1[2].symbol == "c");
	REQUIRE(declarationList1[3].type == DesignEntity::while_);
	REQUIRE(declarationList1[3].symbol == "Select");
	REQUIRE(declarationList1[4].type == DesignEntity::read);
	REQUIRE(declarationList1[4].symbol == "such");
	REQUIRE(declarationList1[5].type == DesignEntity::read);
	REQUIRE(declarationList1[5].symbol == "that");
	REQUIRE(declarationList1[6].type == DesignEntity::assign);
	REQUIRE(declarationList1[6].symbol == "pattern");
	REQUIRE(declarationList1[7].type == DesignEntity::variable);
	REQUIRE(declarationList1[7].symbol == "CAP3");
	QueryPreprocessor qpp2;
	QueryProperties qp2 = qpp2.parseQuery(
		"stmt stmt; read reads;print p1;procedure\nModifies; constant Uses; Select Modifies"
	);
	auto declarationList2 = qp2.getDeclarationList();
	REQUIRE(declarationList2[0].type == DesignEntity::stmt);
	REQUIRE(declarationList2[0].symbol == "stmt");
	REQUIRE(declarationList2[1].type == DesignEntity::read);
	REQUIRE(declarationList2[1].symbol == "reads");
	REQUIRE(declarationList2[2].type == DesignEntity::print);
	REQUIRE(declarationList2[2].symbol == "p1");
	REQUIRE(declarationList2[3].type == DesignEntity::procedure);
	REQUIRE(declarationList2[3].symbol == "Modifies");
	REQUIRE(declarationList2[4].type == DesignEntity::constant);
	REQUIRE(declarationList2[4].symbol == "Uses");

	// Multiple declaration clause with same design entity
	QueryPreprocessor qpp3;
	QueryProperties qp3 = qpp3.parseQuery(
		"while stmt; if reads;while p1; while Modifies;Select Modifies"
	);
	auto declarationList3 = qp3.getDeclarationList();
	REQUIRE(declarationList3[0].type == DesignEntity::while_);
	REQUIRE(declarationList3[0].symbol == "stmt");
	REQUIRE(declarationList3[1].type == DesignEntity::if_);
	REQUIRE(declarationList3[1].symbol == "reads");
	REQUIRE(declarationList3[2].type == DesignEntity::while_);
	REQUIRE(declarationList3[2].symbol == "p1");
	REQUIRE(declarationList3[3].type == DesignEntity::while_);
	REQUIRE(declarationList3[3].symbol == "Modifies");

}


TEST_CASE("QP::QueryPreprocessor::parseQuery invalid declarations") {
	// wrong spelling / case
	QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery("IF ifs; Select ifs"), QueryException);
	QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery("prints a; Select a"), QueryException);
	QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery("assssign a; Select a"), QueryException);

	// missing ; / ,
	QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery("while a Select a"), QueryException);
	QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery("variable a b c; Select a"), QueryException);

	// Duplicated synonym
	QueryPreprocessor qpp6;
	REQUIRE_THROWS_AS(qpp6.parseQuery("while a, a; Select a"), QueryException);
	QueryPreprocessor qpp7;
	REQUIRE_THROWS_AS(qpp7.parseQuery("read a; print b; procedure a; Select a"), QueryException);
}


TEST_CASE("QP::QueryPreprocessor::parseQuery Select") {
	QueryPreprocessor qpp1;
	QueryProperties qp1 = qpp1.parseQuery("print a; if b; Select a");
	REQUIRE(qp1.getSelect().type == DesignEntity::print);
	REQUIRE(qp1.getSelect().symbol == "a");

	QueryPreprocessor qpp2;
	QueryProperties qp2 = qpp2.parseQuery("print a; if b; Select b");
	REQUIRE(qp2.getSelect().type == DesignEntity::if_);
	REQUIRE(qp2.getSelect().symbol == "b");

	// undeclared synonym
	QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery("print a; if b; Select unknownSynonym"), QueryException);
	// more than 1 synonym
	QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery("print a; if b; Select a,b"), QueryException);
	// more than 1 select
	QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery("print a; if b; Select a Select b"), QueryException);
}


TEST_CASE("QP::QueryPreprocessor::parseQuery valid such that Parent(*)") {
	Parent *clause, *clause1;
	// Combinations of stmtRef : synonym | '_' | INTEGER for Parent
	QueryPreprocessor qpp1;
	QueryProperties qp1 = qpp1.parseQuery(univDeclarations + "Select s1 such that Parent(s1, s2)");
	clause = dynamic_cast<Parent*>(qp1.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getIsStar() == false);
	REQUIRE((*clause).getParentStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getParentStmt().stmtRef == "s1");
	REQUIRE((*clause).getChildStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getChildStmt().stmtRef == "s2");

	QueryPreprocessor qpp2;
	QueryProperties qp2 = qpp2.parseQuery(univDeclarations + "Select i1 such that Parent(i1, _)");
	clause = dynamic_cast<Parent*>(qp2.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getIsStar() == false);
	REQUIRE((*clause).getParentStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getParentStmt().stmtRef == "i1");
	REQUIRE((*clause).getChildStmt().type == StmtRefType::underscore);
	REQUIRE((*clause).getChildStmt().stmtRef == "_");

	QueryPreprocessor qpp3;
	QueryProperties qp3 = qpp3.parseQuery(univDeclarations + "Select r1 such that Parent(r1, 20)");
	clause = dynamic_cast<Parent*>(qp3.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getIsStar() == false);
	REQUIRE((*clause).getParentStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getParentStmt().stmtRef == "r1");
	REQUIRE((*clause).getChildStmt().type == StmtRefType::stmtNumber);
	REQUIRE((*clause).getChildStmt().stmtRef == "20");

	QueryPreprocessor qpp4;
	QueryProperties qp4 = qpp4.parseQuery(univDeclarations + "Select a1 such that Parent(01, a1)");
	clause = dynamic_cast<Parent*>(qp4.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getIsStar() == false);
	REQUIRE((*clause).getParentStmt().type == StmtRefType::stmtNumber);
	REQUIRE((*clause).getParentStmt().stmtRef == "01");
	REQUIRE((*clause).getChildStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getChildStmt().stmtRef == "a1");

	QueryPreprocessor qpp5;
	QueryProperties qp5 = qpp5.parseQuery(univDeclarations + "Select i1 such that Parent(32, 03)");
	clause = dynamic_cast<Parent*>(qp5.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getIsStar() == false);
	REQUIRE((*clause).getParentStmt().type == StmtRefType::stmtNumber);
	REQUIRE((*clause).getParentStmt().stmtRef == "32");
	REQUIRE((*clause).getChildStmt().type == StmtRefType::stmtNumber);
	REQUIRE((*clause).getChildStmt().stmtRef == "03");

	QueryPreprocessor qpp6;
	QueryProperties qp6 = qpp6.parseQuery(univDeclarations + "Select w2 such that Parent(1010, _)");
	clause = dynamic_cast<Parent*>(qp6.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getIsStar() == false);
	REQUIRE((*clause).getParentStmt().type == StmtRefType::stmtNumber);
	REQUIRE((*clause).getParentStmt().stmtRef == "1010");
	REQUIRE((*clause).getChildStmt().type == StmtRefType::underscore);
	REQUIRE((*clause).getChildStmt().stmtRef == "_");

	QueryPreprocessor qpp7;
	QueryProperties qp7 = qpp7.parseQuery(univDeclarations + "Select s1 such that Parent(_, p2)");
	clause = dynamic_cast<Parent*>(qp7.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getIsStar() == false);
	REQUIRE((*clause).getParentStmt().type == StmtRefType::underscore);
	REQUIRE((*clause).getParentStmt().stmtRef == "_");
	REQUIRE((*clause).getChildStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getChildStmt().stmtRef == "p2");

	QueryPreprocessor qpp8;
	QueryProperties qp8 = qpp8.parseQuery(univDeclarations + "Select i1 such that Parent(_, 1)");
	clause = dynamic_cast<Parent*>(qp8.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getIsStar() == false);
	REQUIRE((*clause).getParentStmt().type == StmtRefType::underscore);
	REQUIRE((*clause).getParentStmt().stmtRef == "_");
	REQUIRE((*clause).getChildStmt().type == StmtRefType::stmtNumber);
	REQUIRE((*clause).getChildStmt().stmtRef == "1");

	QueryPreprocessor qpp9;
	QueryProperties qp9 = qpp9.parseQuery(univDeclarations + "Select w2 such that Parent(_, _)");
	clause = dynamic_cast<Parent*>(qp9.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getIsStar() == false);
	REQUIRE((*clause).getParentStmt().type == StmtRefType::underscore);
	REQUIRE((*clause).getParentStmt().stmtRef == "_");
	REQUIRE((*clause).getChildStmt().type == StmtRefType::underscore);
	REQUIRE((*clause).getChildStmt().stmtRef == "_");

	// Parent*
	QueryPreprocessor qpp10;
	QueryProperties qp10 = qpp10.parseQuery(univDeclarations + "Select c2 such that Parent*(w1, c2)");
	clause = dynamic_cast<Parent*>(qp10.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getIsStar() == true);
	REQUIRE((*clause).getParentStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getParentStmt().stmtRef == "w1");
	REQUIRE((*clause).getChildStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getChildStmt().stmtRef == "c2");
	QueryPreprocessor qpp11;
	QueryProperties qp11 = qpp11.parseQuery(univDeclarations + "Select c2 such that Parent*      (   w1  , c2  )");
	clause = dynamic_cast<Parent*>(qp11.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getIsStar() == true);
	REQUIRE((*clause).getParentStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getParentStmt().stmtRef == "w1");
	REQUIRE((*clause).getChildStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getChildStmt().stmtRef == "c2");

	// Multiple Parent(*) clauses
	QueryPreprocessor qpp12;
	QueryProperties qp12 = qpp12.parseQuery(univDeclarations + "Select c2 such that Parent(i1, _) and Parent(w1, c2)");
	clause = dynamic_cast<Parent*>(qp12.getSuchThatClauseList()[0].relation);
	clause1 = dynamic_cast<Parent*>(qp12.getSuchThatClauseList()[1].relation);
	REQUIRE((*clause).getIsStar() == false);
	REQUIRE((*clause).getParentStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getParentStmt().stmtRef == "i1");
	REQUIRE((*clause).getChildStmt().type == StmtRefType::underscore);
	REQUIRE((*clause).getChildStmt().stmtRef == "_");
	REQUIRE((*clause1).getIsStar() == false);
	REQUIRE((*clause1).getParentStmt().type == StmtRefType::synonym);
	REQUIRE((*clause1).getParentStmt().stmtRef == "w1");
	REQUIRE((*clause1).getChildStmt().type == StmtRefType::synonym);
	REQUIRE((*clause1).getChildStmt().stmtRef == "c2");
	QueryPreprocessor qpp13;
	QueryProperties qp13 = qpp13.parseQuery(univDeclarations + "Select c2 such that Parent*(i1, _) and Parent(w1, c2)");
	clause = dynamic_cast<Parent*>(qp13.getSuchThatClauseList()[0].relation);
	clause1 = dynamic_cast<Parent*>(qp13.getSuchThatClauseList()[1].relation);
	REQUIRE((*clause).getIsStar() == true);
	REQUIRE((*clause).getParentStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getParentStmt().stmtRef == "i1");
	REQUIRE((*clause).getChildStmt().type == StmtRefType::underscore);
	REQUIRE((*clause).getChildStmt().stmtRef == "_");
	REQUIRE((*clause1).getIsStar() == false);
	REQUIRE((*clause1).getParentStmt().type == StmtRefType::synonym);
	REQUIRE((*clause1).getParentStmt().stmtRef == "w1");
	REQUIRE((*clause1).getChildStmt().type == StmtRefType::synonym);
	REQUIRE((*clause1).getChildStmt().stmtRef == "c2");
}

TEST_CASE("QP::QueryPreprocessor::parseQuery invalid such that Parent(*)") {
	// disjoint *
	QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery(univDeclarations + "Select s1 such that Parent *(s1, s2)"), QueryException);
	// non-statement synonyms
	QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery(univDeclarations + "Select v1 such that Parent(v1, s2)"), QueryException);
	QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery(univDeclarations + "Select s1 such that Parent(pc1, s2)"), QueryException);
	// misspelt word
	QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery(univDeclarations + "Select s1 such that Parents(s1, s2)"), QueryException);
	// disallowed statement reference
	QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery(univDeclarations + "Select s1 such that Parents(s1, \"x\")"), QueryException);
}


TEST_CASE("QP::QueryPreprocessor::parseQuery valid such that Follows(*)") {
	Follows* clause, * clause1;
	// Combinations of stmtRef : synonym | '_' | INTEGER for Follows
	QueryPreprocessor qpp1;
	QueryProperties qp1 = qpp1.parseQuery(univDeclarations + "Select s1 such that Follows(s1, s2)");
	clause = dynamic_cast<Follows*>(qp1.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getIsStar() == false);
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getLeftStmt().stmtRef == "s1");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getRightStmt().stmtRef == "s2");

	QueryPreprocessor qpp2;
	QueryProperties qp2 = qpp2.parseQuery(univDeclarations + "Select i1 such that Follows(i1, _)");
	clause = dynamic_cast<Follows*>(qp2.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getIsStar() == false);
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getLeftStmt().stmtRef == "i1");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::underscore);
	REQUIRE((*clause).getRightStmt().stmtRef == "_");

	QueryPreprocessor qpp3;
	QueryProperties qp3 = qpp3.parseQuery(univDeclarations + "Select r1 such that Follows(r1, 20)");
	clause = dynamic_cast<Follows*>(qp3.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getIsStar() == false);
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getLeftStmt().stmtRef == "r1");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::stmtNumber);
	REQUIRE((*clause).getRightStmt().stmtRef == "20");

	QueryPreprocessor qpp4;
	QueryProperties qp4 = qpp4.parseQuery(univDeclarations + "Select a1 such that Follows(01, a1)");
	clause = dynamic_cast<Follows*>(qp4.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getIsStar() == false);
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::stmtNumber);
	REQUIRE((*clause).getLeftStmt().stmtRef == "01");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getRightStmt().stmtRef == "a1");

	QueryPreprocessor qpp5;
	QueryProperties qp5 = qpp5.parseQuery(univDeclarations + "Select i1 such that Follows(32, 03)");
	clause = dynamic_cast<Follows*>(qp5.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getIsStar() == false);
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::stmtNumber);
	REQUIRE((*clause).getLeftStmt().stmtRef == "32");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::stmtNumber);
	REQUIRE((*clause).getRightStmt().stmtRef == "03");

	QueryPreprocessor qpp6;
	QueryProperties qp6 = qpp6.parseQuery(univDeclarations + "Select w2 such that Follows(1010, _)");
	clause = dynamic_cast<Follows*>(qp6.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getIsStar() == false);
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::stmtNumber);
	REQUIRE((*clause).getLeftStmt().stmtRef == "1010");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::underscore);
	REQUIRE((*clause).getRightStmt().stmtRef == "_");

	QueryPreprocessor qpp7;
	QueryProperties qp7 = qpp7.parseQuery(univDeclarations + "Select s1 such that Follows(_, p2)");
	clause = dynamic_cast<Follows*>(qp7.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getIsStar() == false);
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::underscore);
	REQUIRE((*clause).getLeftStmt().stmtRef == "_");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getRightStmt().stmtRef == "p2");

	QueryPreprocessor qpp8;
	QueryProperties qp8 = qpp8.parseQuery(univDeclarations + "Select i1 such that Follows(_, 1)");
	clause = dynamic_cast<Follows*>(qp8.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getIsStar() == false);
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::underscore);
	REQUIRE((*clause).getLeftStmt().stmtRef == "_");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::stmtNumber);
	REQUIRE((*clause).getRightStmt().stmtRef == "1");

	QueryPreprocessor qpp9;
	QueryProperties qp9 = qpp9.parseQuery(univDeclarations + "Select w2 such that Follows(_, _)");
	clause = dynamic_cast<Follows*>(qp9.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getIsStar() == false);
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::underscore);
	REQUIRE((*clause).getLeftStmt().stmtRef == "_");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::underscore);
	REQUIRE((*clause).getRightStmt().stmtRef == "_");

	// Follows*
	QueryPreprocessor qpp10;
	QueryProperties qp10 = qpp10.parseQuery(univDeclarations + "Select c2 such that Follows*(w1, c2)");
	clause = dynamic_cast<Follows*>(qp10.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getIsStar() == true);
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getLeftStmt().stmtRef == "w1");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getRightStmt().stmtRef == "c2");
	QueryPreprocessor qpp11;
	QueryProperties qp11 = qpp11.parseQuery(univDeclarations + "Select c2 such that Follows*      (   w1  , c2  )");
	clause = dynamic_cast<Follows*>(qp11.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getIsStar() == true);
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getLeftStmt().stmtRef == "w1");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getRightStmt().stmtRef == "c2");

	// Multiple Follows(*) clauses
	QueryPreprocessor qpp12;
	QueryProperties qp12 = qpp12.parseQuery(univDeclarations + "Select c2 such that Follows(i1, _) and Follows(w1, c2)");
	clause = dynamic_cast<Follows*>(qp12.getSuchThatClauseList()[0].relation);
	clause1 = dynamic_cast<Follows*>(qp12.getSuchThatClauseList()[1].relation);
	REQUIRE((*clause).getIsStar() == false);
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getLeftStmt().stmtRef == "i1");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::underscore);
	REQUIRE((*clause).getRightStmt().stmtRef == "_");
	REQUIRE((*clause1).getIsStar() == false);
	REQUIRE((*clause1).getLeftStmt().type == StmtRefType::synonym);
	REQUIRE((*clause1).getLeftStmt().stmtRef == "w1");
	REQUIRE((*clause1).getRightStmt().type == StmtRefType::synonym);
	REQUIRE((*clause1).getRightStmt().stmtRef == "c2");
	QueryPreprocessor qpp13;
	QueryProperties qp13 = qpp13.parseQuery(univDeclarations + "Select c2 such that Follows*(i1, _) and Follows(w1, c2)");
	clause = dynamic_cast<Follows*>(qp13.getSuchThatClauseList()[0].relation);
	clause1 = dynamic_cast<Follows*>(qp13.getSuchThatClauseList()[1].relation);
	REQUIRE((*clause).getIsStar() == true);
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getLeftStmt().stmtRef == "i1");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::underscore);
	REQUIRE((*clause).getRightStmt().stmtRef == "_");
	REQUIRE((*clause1).getIsStar() == false);
	REQUIRE((*clause1).getLeftStmt().type == StmtRefType::synonym);
	REQUIRE((*clause1).getLeftStmt().stmtRef == "w1");
	REQUIRE((*clause1).getRightStmt().type == StmtRefType::synonym);
	REQUIRE((*clause1).getRightStmt().stmtRef == "c2");
}

TEST_CASE("QP::QueryPreprocessor::parseQuery invalid such that Follows(*)") {
	// Missing (
	QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery(univDeclarations + "Select s1 such that Follows s1, s2)"), QueryException);
	// disjoint *
	QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery(univDeclarations + "Select s1 such that Follows *(s1, s2)"), QueryException);
	// non-statement synonyms
	QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery(univDeclarations + "Select s1 such that Follows(pc1, s2)"), QueryException);
	// misspelt word
	QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery(univDeclarations + "Select s1 such that Follow(s1, s2)"), QueryException);
	// disallowed statement reference
	QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery(univDeclarations + "Select s1 such that Follows(s1, \"x\")"), QueryException);
}

TEST_CASE("QP::QueryPreprocessor::parseQuery valid such that UsesS/P") {

	// Valid UsesS
	UsesS* clause, * clause1;
	QueryPreprocessor qpp1;
	QueryProperties qp1 = qpp1.parseQuery(univDeclarations + "Select v1 such that Uses(s1, v1)");
	clause = dynamic_cast<UsesS*>(qp1.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getStmt().stmtRef == "s1");
	REQUIRE((*clause).getEnt().type == EntRefType::synonym);
	REQUIRE((*clause).getEnt().entRef == "v1");

	QueryPreprocessor qpp2;
	QueryProperties qp2 = qpp2.parseQuery(univDeclarations + "Select s1 such that Uses(s1, _)");
	clause = dynamic_cast<UsesS*>(qp2.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getStmt().stmtRef == "s1");
	REQUIRE((*clause).getEnt().type == EntRefType::underscore);
	REQUIRE((*clause).getEnt().entRef == "_");

	QueryPreprocessor qpp3;
	QueryProperties qp3 = qpp3.parseQuery(univDeclarations + "Select s1 such that Uses(s1, \"x\")");
	clause = dynamic_cast<UsesS*>(qp3.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getStmt().stmtRef == "s1");
	REQUIRE((*clause).getEnt().type == EntRefType::varName);
	REQUIRE((*clause).getEnt().entRef == "x");

	QueryPreprocessor qpp4;
	QueryProperties qp4 = qpp4.parseQuery(univDeclarations + "Select v1 such that Uses(i1, v1)");
	clause = dynamic_cast<UsesS*>(qp4.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getStmt().stmtRef == "i1");
	REQUIRE((*clause).getEnt().type == EntRefType::synonym);
	REQUIRE((*clause).getEnt().entRef == "v1");

	QueryPreprocessor qpp5;
	QueryProperties qp5 = qpp5.parseQuery(univDeclarations + "Select p1 such that Uses(p1, _)");
	clause = dynamic_cast<UsesS*>(qp5.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getStmt().stmtRef == "p1");
	REQUIRE((*clause).getEnt().type == EntRefType::underscore);
	REQUIRE((*clause).getEnt().entRef == "_");

	QueryPreprocessor qpp6;
	QueryProperties qp6 = qpp6.parseQuery(univDeclarations + "Select a1 such that Uses(a1, \"y\")");
	clause = dynamic_cast<UsesS*>(qp6.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getStmt().stmtRef == "a1");
	REQUIRE((*clause).getEnt().type == EntRefType::varName);
	REQUIRE((*clause).getEnt().entRef == "y");

	QueryPreprocessor qpp7;
	QueryProperties qp7 = qpp7.parseQuery(univDeclarations + "Select v1 such that Uses(w2, v1)");
	clause = dynamic_cast<UsesS*>(qp7.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getStmt().stmtRef == "w2");
	REQUIRE((*clause).getEnt().type == EntRefType::synonym);
	REQUIRE((*clause).getEnt().entRef == "v1");

	QueryPreprocessor qpp8;
	QueryProperties qp8 = qpp8.parseQuery(univDeclarations + "Select i1 such that Uses(i1, _)");
	clause = dynamic_cast<UsesS*>(qp8.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getStmt().stmtRef == "i1");
	REQUIRE((*clause).getEnt().type == EntRefType::underscore);
	REQUIRE((*clause).getEnt().entRef == "_");

	QueryPreprocessor qpp9;
	QueryProperties qp9 = qpp9.parseQuery(univDeclarations + "Select w1 such that Uses(w1, \"z0\")");
	clause = dynamic_cast<UsesS*>(qp9.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getStmt().stmtRef == "w1");
	REQUIRE((*clause).getEnt().type == EntRefType::varName);
	REQUIRE((*clause).getEnt().entRef == "z0");

	QueryPreprocessor qpp10;
	QueryProperties qp10 = qpp10.parseQuery(univDeclarations + "Select v1 such that Uses(1, v1)");
	clause = dynamic_cast<UsesS*>(qp10.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::stmtNumber);
	REQUIRE((*clause).getStmt().stmtRef == "1");
	REQUIRE((*clause).getEnt().type == EntRefType::synonym);
	REQUIRE((*clause).getEnt().entRef == "v1");

	QueryPreprocessor qpp11;
	QueryProperties qp11 = qpp11.parseQuery(univDeclarations + "Select s1 such that Uses(w2, _)");
	clause = dynamic_cast<UsesS*>(qp11.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getStmt().stmtRef == "w2");
	REQUIRE((*clause).getEnt().type == EntRefType::underscore);
	REQUIRE((*clause).getEnt().entRef == "_");

	QueryPreprocessor qpp12;
	QueryProperties qp12 = qpp12.parseQuery(univDeclarations + "Select s1 such that Uses(w2, \"var\")");
	clause = dynamic_cast<UsesS*>(qp12.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getStmt().stmtRef == "w2");
	REQUIRE((*clause).getEnt().type == EntRefType::varName);
	REQUIRE((*clause).getEnt().entRef == "var");

	// UsesP
	UsesP* clause2, * clause3;
	QueryPreprocessor qpp13;
	QueryProperties qp13 = qpp13.parseQuery(univDeclarations + "Select v1 such that Uses(pc1, v1)");
	clause2 = dynamic_cast<UsesP*>(qp13.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause2).getLeftEnt().type == EntRefType::synonym);
	REQUIRE((*clause2).getLeftEnt().entRef == "pc1");
	REQUIRE((*clause2).getRightEnt().type == EntRefType::synonym);
	REQUIRE((*clause2).getRightEnt().entRef == "v1");

	QueryPreprocessor qpp14;
	QueryProperties qp14 = qpp14.parseQuery(univDeclarations + "Select v1 such that Uses(c1, _)");
	clause2 = dynamic_cast<UsesP*>(qp14.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause2).getLeftEnt().type == EntRefType::synonym);
	REQUIRE((*clause2).getLeftEnt().entRef == "c1");
	REQUIRE((*clause2).getRightEnt().type == EntRefType::underscore);
	REQUIRE((*clause2).getRightEnt().entRef == "_");

	QueryPreprocessor qpp15;
	QueryProperties qp15 = qpp15.parseQuery(univDeclarations + "Select s2 such that Uses(\"procedureName\", \"varName\")");
	clause2 = dynamic_cast<UsesP*>(qp15.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause2).getLeftEnt().type == EntRefType::varName);
	REQUIRE((*clause2).getLeftEnt().entRef == "procedureName");
	REQUIRE((*clause2).getRightEnt().type == EntRefType::varName);
	REQUIRE((*clause2).getRightEnt().entRef == "varName");

}

TEST_CASE("QP::QueryPreprocessor::parseQuery invalid such that UsesS/P") {
	// unexpected wildcard
	QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery(univDeclarations + "Select v1 such that Uses(_, v1)"), QueryException);
	// unexpected integer
	QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery(univDeclarations + "Select pc1 such that Uses(pc1, 1)"), QueryException);
	// invalid synonym
	QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery(univDeclarations + "Select s1 such that Uses(1, s1)"), QueryException);
	QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery(univDeclarations + "Select r1 such that Uses(r1, _)"), QueryException);
	// undeclared synonym
	QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery(univDeclarations + "Select s1 such that Uses(s1, UNK)"), QueryException);
	// misspelled keyword
	QueryPreprocessor qpp6;
	REQUIRE_THROWS_AS(qpp6.parseQuery(univDeclarations + "Select s1 such that use(s1, a1)"), QueryException);
}

TEST_CASE("QP::QueryPreprocessor::parseQuery valid such that ModifiesS/P") {
	// Valid ModifiesS
	ModifiesS* clause, * clause1;
	QueryPreprocessor qpp1;
	QueryProperties qp1 = qpp1.parseQuery(univDeclarations + "Select v1 such that Modifies(s1, v1)");
	clause = dynamic_cast<ModifiesS*>(qp1.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getStmt().stmtRef == "s1");
	REQUIRE((*clause).getEnt().type == EntRefType::synonym);
	REQUIRE((*clause).getEnt().entRef == "v1");

	QueryPreprocessor qpp2;
	QueryProperties qp2 = qpp2.parseQuery(univDeclarations + "Select s1 such that Modifies(s1, _)");
	clause = dynamic_cast<ModifiesS*>(qp2.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getStmt().stmtRef == "s1");
	REQUIRE((*clause).getEnt().type == EntRefType::underscore);
	REQUIRE((*clause).getEnt().entRef == "_");

	QueryPreprocessor qpp3;
	QueryProperties qp3 = qpp3.parseQuery(univDeclarations + "Select s1 such that Modifies(s1, \"x\")");
	clause = dynamic_cast<ModifiesS*>(qp3.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getStmt().stmtRef == "s1");
	REQUIRE((*clause).getEnt().type == EntRefType::varName);
	REQUIRE((*clause).getEnt().entRef == "x");

	QueryPreprocessor qpp4;
	QueryProperties qp4 = qpp4.parseQuery(univDeclarations + "Select v1 such that Modifies(i1, v1)");
	clause = dynamic_cast<ModifiesS*>(qp4.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getStmt().stmtRef == "i1");
	REQUIRE((*clause).getEnt().type == EntRefType::synonym);
	REQUIRE((*clause).getEnt().entRef == "v1");

	QueryPreprocessor qpp5;
	QueryProperties qp5 = qpp5.parseQuery(univDeclarations + "Select p1 such that Modifies(r1, _)");
	clause = dynamic_cast<ModifiesS*>(qp5.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getStmt().stmtRef == "r1");
	REQUIRE((*clause).getEnt().type == EntRefType::underscore);
	REQUIRE((*clause).getEnt().entRef == "_");

	QueryPreprocessor qpp6;
	QueryProperties qp6 = qpp6.parseQuery(univDeclarations + "Select a1 such that Modifies(a1, \"y\")");
	clause = dynamic_cast<ModifiesS*>(qp6.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getStmt().stmtRef == "a1");
	REQUIRE((*clause).getEnt().type == EntRefType::varName);
	REQUIRE((*clause).getEnt().entRef == "y");

	QueryPreprocessor qpp7;
	QueryProperties qp7 = qpp7.parseQuery(univDeclarations + "Select v1 such that Modifies(w2, v1)");
	clause = dynamic_cast<ModifiesS*>(qp7.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getStmt().stmtRef == "w2");
	REQUIRE((*clause).getEnt().type == EntRefType::synonym);
	REQUIRE((*clause).getEnt().entRef == "v1");

	QueryPreprocessor qpp8;
	QueryProperties qp8 = qpp8.parseQuery(univDeclarations + "Select i1 such that Modifies(i1, _)");
	clause = dynamic_cast<ModifiesS*>(qp8.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getStmt().stmtRef == "i1");
	REQUIRE((*clause).getEnt().type == EntRefType::underscore);
	REQUIRE((*clause).getEnt().entRef == "_");

	QueryPreprocessor qpp9;
	QueryProperties qp9 = qpp9.parseQuery(univDeclarations + "Select w1 such that Modifies(w1, \"z0\")");
	clause = dynamic_cast<ModifiesS*>(qp9.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getStmt().stmtRef == "w1");
	REQUIRE((*clause).getEnt().type == EntRefType::varName);
	REQUIRE((*clause).getEnt().entRef == "z0");

	QueryPreprocessor qpp10;
	QueryProperties qp10 = qpp10.parseQuery(univDeclarations + "Select v1 such that Modifies(1, v1)");
	clause = dynamic_cast<ModifiesS*>(qp10.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::stmtNumber);
	REQUIRE((*clause).getStmt().stmtRef == "1");
	REQUIRE((*clause).getEnt().type == EntRefType::synonym);
	REQUIRE((*clause).getEnt().entRef == "v1");

	QueryPreprocessor qpp11;
	QueryProperties qp11 = qpp11.parseQuery(univDeclarations + "Select s1 such that Modifies(w2, _)");
	clause = dynamic_cast<ModifiesS*>(qp11.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getStmt().stmtRef == "w2");
	REQUIRE((*clause).getEnt().type == EntRefType::underscore);
	REQUIRE((*clause).getEnt().entRef == "_");

	QueryPreprocessor qpp12;
	QueryProperties qp12 = qpp12.parseQuery(univDeclarations + "Select s1 such that Modifies(w2, \"var\")");
	clause = dynamic_cast<ModifiesS*>(qp12.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getStmt().stmtRef == "w2");
	REQUIRE((*clause).getEnt().type == EntRefType::varName);
	REQUIRE((*clause).getEnt().entRef == "var");

	// ModifiesP
	ModifiesP* clause2, * clause3;
	QueryPreprocessor qpp13;
	QueryProperties qp13 = qpp13.parseQuery(univDeclarations + "Select v1 such that Modifies(pc1, v1)");
	clause2 = dynamic_cast<ModifiesP*>(qp13.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause2).getLeftEnt().type == EntRefType::synonym);
	REQUIRE((*clause2).getLeftEnt().entRef == "pc1");
	REQUIRE((*clause2).getRightEnt().type == EntRefType::synonym);
	REQUIRE((*clause2).getRightEnt().entRef == "v1");

	QueryPreprocessor qpp14;
	QueryProperties qp14 = qpp14.parseQuery(univDeclarations + "Select v1 such that Modifies(c1, _)");
	clause2 = dynamic_cast<ModifiesP*>(qp14.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause2).getLeftEnt().type == EntRefType::synonym);
	REQUIRE((*clause2).getLeftEnt().entRef == "c1");
	REQUIRE((*clause2).getRightEnt().type == EntRefType::underscore);
	REQUIRE((*clause2).getRightEnt().entRef == "_");

	QueryPreprocessor qpp15;
	QueryProperties qp15 = qpp15.parseQuery(univDeclarations + "Select s2 such that Modifies(\"procedureName\", \"varName\")");
	clause2 = dynamic_cast<ModifiesP*>(qp15.getSuchThatClauseList()[0].relation);
	REQUIRE((*clause2).getLeftEnt().type == EntRefType::varName);
	REQUIRE((*clause2).getLeftEnt().entRef == "procedureName");
	REQUIRE((*clause2).getRightEnt().type == EntRefType::varName);
	REQUIRE((*clause2).getRightEnt().entRef == "varName");

}

TEST_CASE("QP::QueryPreprocessor::parseQuery invalid such that ModifiesS/P") {
	// unexpected wildcard
	QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery(univDeclarations + "Select v1 such that Modifies(_, v1)"), QueryException);
	// unexpected integer
	QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery(univDeclarations + "Select pc1 such that Modifies(pc1, 1)"), QueryException);
	// invalid synonym
	QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery(univDeclarations + "Select s1 such that Modifies(1, 0s)"), QueryException);
	QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery(univDeclarations + "Select s1 such that Modifies(1, s1)"), QueryException);
	QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery(univDeclarations + "Select p1 such that Modifies(p1, v1)"), QueryException);
	// undeclared synonym
	QueryPreprocessor qpp6;
	REQUIRE_THROWS_AS(qpp6.parseQuery(univDeclarations + "Select s1 such that Modifies(s1, UNK)"), QueryException);
	// misspelled keyword
	QueryPreprocessor qpp7;
	REQUIRE_THROWS_AS(qpp7.parseQuery(univDeclarations + "Select s1 such that modifies(s1, a1)"), QueryException);
}

TEST_CASE("QP::QueryPreprocessor::parseQuery Multiple such that") {
	QueryPreprocessor qpp1;
	QueryProperties qp1 = qpp1.parseQuery(univDeclarations + "Select c2 such that Modifies(c2, _) and Follows(w1, c2)");
	auto clause = dynamic_cast<ModifiesP*>(qp1.getSuchThatClauseList()[0].relation);
	auto clause1 = dynamic_cast<Follows*>(qp1.getSuchThatClauseList()[1].relation);
	REQUIRE((*clause).getLeftEnt().type == EntRefType::synonym);
	REQUIRE((*clause).getLeftEnt().entRef == "c2");
	REQUIRE((*clause).getRightEnt().type == EntRefType::underscore);
	REQUIRE((*clause).getRightEnt().entRef == "_");
	REQUIRE((*clause1).getIsStar() == false);
	REQUIRE((*clause1).getLeftStmt().type == StmtRefType::synonym);
	REQUIRE((*clause1).getLeftStmt().stmtRef == "w1");
	REQUIRE((*clause1).getRightStmt().type == StmtRefType::synonym);
	REQUIRE((*clause1).getRightStmt().stmtRef == "c2");


	QueryPreprocessor qpp2;
	QueryProperties qp2 = qpp2.parseQuery(univDeclarations + "Select w2 such that Parent*(007, _) such that Uses(w2, v1)");
	auto clause2 = dynamic_cast<Parent*>(qp2.getSuchThatClauseList()[0].relation);
	auto clause3 = dynamic_cast<UsesS*>(qp2.getSuchThatClauseList()[1].relation);
	REQUIRE((*clause2).getIsStar() == true);
	REQUIRE((*clause2).getParentStmt().type == StmtRefType::stmtNumber);
	REQUIRE((*clause2).getParentStmt().stmtRef == "007");
	REQUIRE((*clause2).getChildStmt().type == StmtRefType::underscore);
	REQUIRE((*clause2).getChildStmt().stmtRef == "_");
	REQUIRE((*clause3).getStmt().type == StmtRefType::synonym);
	REQUIRE((*clause3).getStmt().stmtRef == "w2");
	REQUIRE((*clause3).getEnt().type == EntRefType::synonym);
	REQUIRE((*clause3).getEnt().entRef == "v1");
}

TEST_CASE("QP::QueryPreprocessor::parseQuery valid pattern") {
	QueryPreprocessor qpp1;
	QueryProperties qp1 = qpp1.parseQuery(univDeclarations + "Select a1 pattern a1(v1, _)");
	REQUIRE(qp1.getPatternClauseList()[0].synonym.type == DesignEntity::assign);
	REQUIRE(qp1.getPatternClauseList()[0].synonym.symbol == "a1");
	REQUIRE(qp1.getPatternClauseList()[0].entRef.type == EntRefType::synonym);
	REQUIRE(qp1.getPatternClauseList()[0].entRef.entRef == "v1");
	REQUIRE(qp1.getPatternClauseList()[0].expressionType == ExpressionType::underscore);
	QueryPreprocessor qpp2;
	QueryProperties qp2 = qpp2.parseQuery(univDeclarations + "Select v1 pattern a1(v1, \"x\")");
	REQUIRE(qp2.getPatternClauseList()[0].synonym.type == DesignEntity::assign);
	REQUIRE(qp2.getPatternClauseList()[0].synonym.symbol == "a1");
	REQUIRE(qp2.getPatternClauseList()[0].entRef.type == EntRefType::synonym);
	REQUIRE(qp2.getPatternClauseList()[0].entRef.entRef == "v1");
	REQUIRE(qp2.getPatternClauseList()[0].expressionType == ExpressionType::expression);
	QueryPreprocessor qpp3;
	QueryProperties qp3 = qpp3.parseQuery(univDeclarations + "Select v1 pattern a2  (v1, _\"x\"_)");
	REQUIRE(qp3.getPatternClauseList()[0].synonym.type == DesignEntity::assign);
	REQUIRE(qp3.getPatternClauseList()[0].synonym.symbol == "a2");
	REQUIRE(qp3.getPatternClauseList()[0].entRef.type == EntRefType::synonym);
	REQUIRE(qp3.getPatternClauseList()[0].entRef.entRef == "v1");
	REQUIRE(qp3.getPatternClauseList()[0].expressionType == ExpressionType::expressionUnderscore);
	QueryPreprocessor qpp4;
	QueryProperties qp4 = qpp4.parseQuery(univDeclarations + "Select a1 pattern a1(_, _)");
	REQUIRE(qp4.getPatternClauseList()[0].synonym.type == DesignEntity::assign);
	REQUIRE(qp4.getPatternClauseList()[0].synonym.symbol == "a1");
	REQUIRE(qp4.getPatternClauseList()[0].entRef.type == EntRefType::underscore);
	REQUIRE(qp4.getPatternClauseList()[0].entRef.entRef == "_");
	REQUIRE(qp4.getPatternClauseList()[0].expressionType == ExpressionType::underscore);
	QueryPreprocessor qpp5;
	QueryProperties qp5 = qpp5.parseQuery(univDeclarations + "Select a2 pattern a1(_, \"x\")");
	REQUIRE(qp5.getPatternClauseList()[0].synonym.type == DesignEntity::assign);
	REQUIRE(qp5.getPatternClauseList()[0].synonym.symbol == "a1");
	REQUIRE(qp5.getPatternClauseList()[0].entRef.type == EntRefType::underscore);
	REQUIRE(qp5.getPatternClauseList()[0].entRef.entRef == "_");
	REQUIRE(qp5.getPatternClauseList()[0].expressionType == ExpressionType::expression);
	QueryPreprocessor qpp6;
	QueryProperties qp6 = qpp6.parseQuery(univDeclarations + "Select a1 pattern a1(_, _\"x + 2\"_)");
	REQUIRE(qp6.getPatternClauseList()[0].synonym.type == DesignEntity::assign);
	REQUIRE(qp6.getPatternClauseList()[0].synonym.symbol == "a1");
	REQUIRE(qp6.getPatternClauseList()[0].entRef.type == EntRefType::underscore);
	REQUIRE(qp6.getPatternClauseList()[0].entRef.entRef == "_");
	REQUIRE(qp6.getPatternClauseList()[0].expressionType == ExpressionType::expressionUnderscore);
	QueryPreprocessor qpp7;
	QueryProperties qp7 = qpp7.parseQuery(univDeclarations + "Select a1 pattern a1(\"foo\", \"(x+5)*3- y\")");
	REQUIRE(qp7.getPatternClauseList()[0].synonym.type == DesignEntity::assign);
	REQUIRE(qp7.getPatternClauseList()[0].synonym.symbol == "a1");
	REQUIRE(qp7.getPatternClauseList()[0].entRef.type == EntRefType::varName);
	REQUIRE(qp7.getPatternClauseList()[0].entRef.entRef == "foo");
	REQUIRE(qp7.getPatternClauseList()[0].expressionType == ExpressionType::expression);
	QueryPreprocessor qpp8;
	QueryProperties qp8 = qpp8.parseQuery(univDeclarations + "Select a1 pattern a1(\"foo\", _)");
	REQUIRE(qp8.getPatternClauseList()[0].synonym.type == DesignEntity::assign);
	REQUIRE(qp8.getPatternClauseList()[0].synonym.symbol == "a1");
	REQUIRE(qp8.getPatternClauseList()[0].entRef.type == EntRefType::varName);
	REQUIRE(qp8.getPatternClauseList()[0].entRef.entRef == "foo");
	REQUIRE(qp8.getPatternClauseList()[0].expressionType == ExpressionType::underscore);
	QueryPreprocessor qpp9;
	QueryProperties qp9 = qpp9.parseQuery(univDeclarations + "Select a2 pattern a1(\"foo\", _\"x-2\"_)");
	REQUIRE(qp9.getPatternClauseList()[0].synonym.type == DesignEntity::assign);
	REQUIRE(qp9.getPatternClauseList()[0].synonym.symbol == "a1");
	REQUIRE(qp9.getPatternClauseList()[0].entRef.type == EntRefType::varName);
	REQUIRE(qp9.getPatternClauseList()[0].entRef.entRef == "foo");
	REQUIRE(qp9.getPatternClauseList()[0].expressionType == ExpressionType::expressionUnderscore);
}

TEST_CASE("QP::QueryPreprocessor::parseQuery invalid pattern") {
	// invalid synonym
	QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery(univDeclarations + "Select a1 pattern s1(v1, \"x\")"), QueryException);
	// undeclared synonym
	QueryPreprocessor qpp2;
	REQUIRE_THROWS_AS(qpp2.parseQuery(univDeclarations + "Select a1 pattern b1(v1, \"x\")"), QueryException);
	// unexpected integer
	QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery(univDeclarations + "Select a1 pattern a1(1, \"x\") "), QueryException);
	// unexpected integer
	QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery(univDeclarations + "Select a1 pattern a1(v1, 1)"), QueryException);
	// unexpected synonym
	QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery(univDeclarations + "Select a1 pattern a1(v1, s1)"), QueryException);
	// invalid synonym
	QueryPreprocessor qpp6;
	REQUIRE_THROWS_AS(qpp6.parseQuery(univDeclarations + "Select a1 pattern a1(s1, \"x\")"), QueryException);
	// missing synonym
	QueryPreprocessor qpp7;
	REQUIRE_THROWS_AS(qpp7.parseQuery(univDeclarations + "Select a1 pattern(s1, \"x\")"), QueryException);
}

TEST_CASE("QP::QueryPreprocessor::parseQuery Multiple such that/pattern clauses") {

	QueryPreprocessor qpp12;
	QueryProperties qp12 = qpp12.parseQuery(univDeclarations + "Select a1 such that Follows(w1, a1) pattern a1(v1, _\"2-4\"_)");
	auto clause = dynamic_cast<Follows*>(qp12.getSuchThatClauseList()[0].relation);
	REQUIRE(qp12.getPatternClauseList()[0].synonym.type == DesignEntity::assign);
	REQUIRE(qp12.getPatternClauseList()[0].synonym.symbol == "a1");
	REQUIRE(qp12.getPatternClauseList()[0].entRef.type == EntRefType::synonym);
	REQUIRE(qp12.getPatternClauseList()[0].entRef.entRef == "v1");
	REQUIRE(qp12.getPatternClauseList()[0].expressionType == ExpressionType::expressionUnderscore);
	REQUIRE((*clause).getIsStar() == false);
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getLeftStmt().stmtRef == "w1");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::synonym);
	REQUIRE((*clause).getRightStmt().stmtRef == "a1");


}