#include "QP/QueryPreprocessor.h"

#include <string>

#include "catch.hpp"
#include "catch_tools.h"

const string UnivDeclarations =
	"stmt s1, s2; read r1, r2; print p1, p2; "
	"call c1, c2; while w1, w2; if i1, i2; "
	"assign a1, a2; variable v1, v2; "
	"constant ct1, ct2; procedure pc1, pc2;";

TEST_CASE("QP::QueryPreprocessor::tokenizeQuery Invalid tokenizer input") {
	QP::QueryPreprocessor qpp1;
	REQUIRE_THROWS_AS(qpp1.parseQuery("!"), QP::QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("@"), QP::QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("#"), QP::QueryTokenizationException);
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
	REQUIRE_THROWS_AS(qpp1.parseQuery(">"), QP::QueryTokenizationException);
	REQUIRE_THROWS_AS(qpp1.parseQuery("<"), QP::QueryTokenizationException);
}

TEST_CASE("QP::QueryPreprocessor::parseQuery valid declarations") {
	// Test design entities
	QP::QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery("if a, b,c; while Select; read such, that;assign pattern;variable CAP3;\nSelect a");
	auto declaration_list1 = qp1.getDeclarationList();
	REQUIRE(declaration_list1[0].type == DesignEntity::If);
	REQUIRE(declaration_list1[0].symbol == "a");
	REQUIRE(declaration_list1[1].type == DesignEntity::If);
	REQUIRE(declaration_list1[1].symbol == "b");
	REQUIRE(declaration_list1[2].type == DesignEntity::If);
	REQUIRE(declaration_list1[2].symbol == "c");
	REQUIRE(declaration_list1[3].type == DesignEntity::While);
	REQUIRE(declaration_list1[3].symbol == "Select");
	REQUIRE(declaration_list1[4].type == DesignEntity::Read);
	REQUIRE(declaration_list1[4].symbol == "such");
	REQUIRE(declaration_list1[5].type == DesignEntity::Read);
	REQUIRE(declaration_list1[5].symbol == "that");
	REQUIRE(declaration_list1[6].type == DesignEntity::Assign);
	REQUIRE(declaration_list1[6].symbol == "pattern");
	REQUIRE(declaration_list1[7].type == DesignEntity::Variable);
	REQUIRE(declaration_list1[7].symbol == "CAP3");
	QP::QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery("stmt stmt; read reads;print p1;procedure\nModifies; constant Uses; Select Modifies");
	auto declaration_list2 = qp2.getDeclarationList();
	REQUIRE(declaration_list2[0].type == DesignEntity::Stmt);
	REQUIRE(declaration_list2[0].symbol == "stmt");
	REQUIRE(declaration_list2[1].type == DesignEntity::Read);
	REQUIRE(declaration_list2[1].symbol == "reads");
	REQUIRE(declaration_list2[2].type == DesignEntity::Print);
	REQUIRE(declaration_list2[2].symbol == "p1");
	REQUIRE(declaration_list2[3].type == DesignEntity::Procedure);
	REQUIRE(declaration_list2[3].symbol == "Modifies");
	REQUIRE(declaration_list2[4].type == DesignEntity::Constant);
	REQUIRE(declaration_list2[4].symbol == "Uses");

	// Multiple declaration clause with same design entity
	QP::QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery("while stmt; if reads;while p1; while Modifies;Select Modifies");
	auto declaration_list3 = qp3.getDeclarationList();
	REQUIRE(declaration_list3[0].type == DesignEntity::While);
	REQUIRE(declaration_list3[0].symbol == "stmt");
	REQUIRE(declaration_list3[1].type == DesignEntity::If);
	REQUIRE(declaration_list3[1].symbol == "reads");
	REQUIRE(declaration_list3[2].type == DesignEntity::While);
	REQUIRE(declaration_list3[2].symbol == "p1");
	REQUIRE(declaration_list3[3].type == DesignEntity::While);
	REQUIRE(declaration_list3[3].symbol == "Modifies");
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

TEST_CASE("QP::QueryPreprocessor::parseQuery Select") {
	QP::QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery("print a; if b; Select a");
	REQUIRE(qp1.getSelect().type == DesignEntity::Print);
	REQUIRE(qp1.getSelect().symbol == "a");

	QP::QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery("print a; if b; Select b");
	REQUIRE(qp2.getSelect().type == DesignEntity::If);
	REQUIRE(qp2.getSelect().symbol == "b");

	// undeclared synonym
	QP::QueryPreprocessor qpp3;
	REQUIRE_THROWS_AS(qpp3.parseQuery("print a; if b; Select unknownSynonym"), QP::QueryException);
	// more than 1 synonym
	QP::QueryPreprocessor qpp4;
	REQUIRE_THROWS_AS(qpp4.parseQuery("print a; if b; Select a,b"), QP::QueryException);
	// more than 1 select
	QP::QueryPreprocessor qpp5;
	REQUIRE_THROWS_AS(qpp5.parseQuery("print a; if b; Select a Select b"), QP::QueryException);
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
	QP::Relationship::Follows* clause;
	QP::Relationship::Follows* clause1;
	QP::Relationship::FollowsT* clause_t;
	// Combinations of stmtRef : synonym | '_' | INTEGER for Follows
	QP::QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select s1 such that Follows(s1, s2)");
	clause = dynamic_pointer_cast<QP::Relationship::Follows>(qp1.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getLeftStmt().stmt_ref == "s1");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getRightStmt().stmt_ref == "s2");

	QP::QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select i1 such that Follows(i1, _)");
	clause = dynamic_pointer_cast<QP::Relationship::Follows>(qp2.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getLeftStmt().stmt_ref == "i1");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::Underscore);
	REQUIRE((*clause).getRightStmt().stmt_ref == "_");

	QP::QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select r1 such that Follows(r1, 20)");
	clause = dynamic_pointer_cast<QP::Relationship::Follows>(qp3.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getLeftStmt().stmt_ref == "r1");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::StmtNumber);
	REQUIRE((*clause).getRightStmt().stmt_ref == "20");

	QP::QueryPreprocessor qpp4;
	QP::QueryProperties qp4 = qpp4.parseQuery(UnivDeclarations + "Select a1 such that Follows(01, a1)");
	clause = dynamic_pointer_cast<QP::Relationship::Follows>(qp4.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::StmtNumber);
	REQUIRE((*clause).getLeftStmt().stmt_ref == "01");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getRightStmt().stmt_ref == "a1");

	QP::QueryPreprocessor qpp5;
	QP::QueryProperties qp5 = qpp5.parseQuery(UnivDeclarations + "Select i1 such that Follows(32, 03)");
	clause = dynamic_pointer_cast<QP::Relationship::Follows>(qp5.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::StmtNumber);
	REQUIRE((*clause).getLeftStmt().stmt_ref == "32");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::StmtNumber);
	REQUIRE((*clause).getRightStmt().stmt_ref == "03");

	QP::QueryPreprocessor qpp6;
	QP::QueryProperties qp6 = qpp6.parseQuery(UnivDeclarations + "Select w2 such that Follows(1010, _)");
	clause = dynamic_pointer_cast<QP::Relationship::Follows>(qp6.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::StmtNumber);
	REQUIRE((*clause).getLeftStmt().stmt_ref == "1010");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::Underscore);
	REQUIRE((*clause).getRightStmt().stmt_ref == "_");

	QP::QueryPreprocessor qpp7;
	QP::QueryProperties qp7 = qpp7.parseQuery(UnivDeclarations + "Select s1 such that Follows(_, p2)");
	clause = dynamic_pointer_cast<QP::Relationship::Follows>(qp7.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::Underscore);
	REQUIRE((*clause).getLeftStmt().stmt_ref == "_");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getRightStmt().stmt_ref == "p2");

	QP::QueryPreprocessor qpp8;
	QP::QueryProperties qp8 = qpp8.parseQuery(UnivDeclarations + "Select i1 such that Follows(_, 1)");
	clause = dynamic_pointer_cast<QP::Relationship::Follows>(qp8.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::Underscore);
	REQUIRE((*clause).getLeftStmt().stmt_ref == "_");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::StmtNumber);
	REQUIRE((*clause).getRightStmt().stmt_ref == "1");

	QP::QueryPreprocessor qpp9;
	QP::QueryProperties qp9 = qpp9.parseQuery(UnivDeclarations + "Select w2 such that Follows(_, _)");
	clause = dynamic_pointer_cast<QP::Relationship::Follows>(qp9.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::Underscore);
	REQUIRE((*clause).getLeftStmt().stmt_ref == "_");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::Underscore);
	REQUIRE((*clause).getRightStmt().stmt_ref == "_");

	// Follows*
	QP::QueryPreprocessor qpp10;
	QP::QueryProperties qp10 = qpp10.parseQuery(UnivDeclarations + "Select c2 such that Follows*(w1, c2)");
	clause_t = dynamic_pointer_cast<QP::Relationship::FollowsT>(qp10.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause_t).getLeftStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause_t).getLeftStmt().stmt_ref == "w1");
	REQUIRE((*clause_t).getRightStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause_t).getRightStmt().stmt_ref == "c2");
	QP::QueryPreprocessor qpp11;
	QP::QueryProperties qp11 = qpp11.parseQuery(UnivDeclarations + "Select c2 such that Follows*      (   w1  , c2  )");
	clause_t = dynamic_pointer_cast<QP::Relationship::FollowsT>(qp11.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause_t).getLeftStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause_t).getLeftStmt().stmt_ref == "w1");
	REQUIRE((*clause_t).getRightStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause_t).getRightStmt().stmt_ref == "c2");

	// Multiple Follows(*) clauses
	QP::QueryPreprocessor qpp12;
	QP::QueryProperties qp12 = qpp12.parseQuery(UnivDeclarations + "Select c2 such that Follows(i1, _) and Follows(w1, c2)");
	clause = dynamic_pointer_cast<QP::Relationship::Follows>(qp12.getSuchThatClauseList()[0].relation).get();
	clause1 = dynamic_pointer_cast<QP::Relationship::Follows>(qp12.getSuchThatClauseList()[1].relation).get();
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getLeftStmt().stmt_ref == "i1");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::Underscore);
	REQUIRE((*clause).getRightStmt().stmt_ref == "_");
	REQUIRE((*clause1).getLeftStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause1).getLeftStmt().stmt_ref == "w1");
	REQUIRE((*clause1).getRightStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause1).getRightStmt().stmt_ref == "c2");
	QP::QueryPreprocessor qpp13;
	QP::QueryProperties qp13 = qpp13.parseQuery(UnivDeclarations + "Select c2 such that Follows*(i1, _) and Follows(w1, c2)");
	clause_t = dynamic_pointer_cast<QP::Relationship::FollowsT>(qp13.getSuchThatClauseList()[0].relation).get();
	clause1 = dynamic_pointer_cast<QP::Relationship::Follows>(qp13.getSuchThatClauseList()[1].relation).get();
	REQUIRE((*clause_t).getLeftStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause_t).getLeftStmt().stmt_ref == "i1");
	REQUIRE((*clause_t).getRightStmt().type == StmtRefType::Underscore);
	REQUIRE((*clause_t).getRightStmt().stmt_ref == "_");
	REQUIRE((*clause1).getLeftStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause1).getLeftStmt().stmt_ref == "w1");
	REQUIRE((*clause1).getRightStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause1).getRightStmt().stmt_ref == "c2");
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
}

TEST_CASE("QP::QueryPreprocessor::parseQuery valid such that UsesS/P") {
	// Valid UsesS
	QP::Relationship::UsesS* clause;
	QP::QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select v1 such that Uses(s1, v1)");
	clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp1.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getStmt().stmt_ref == "s1");
	REQUIRE((*clause).getEnt().type == EntRefType::Synonym);
	REQUIRE((*clause).getEnt().ent_ref == "v1");

	QP::QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select s1 such that Uses(s1, _)");
	clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp2.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getStmt().stmt_ref == "s1");
	REQUIRE((*clause).getEnt().type == EntRefType::Underscore);
	REQUIRE((*clause).getEnt().ent_ref == "_");

	QP::QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select s1 such that Uses(s1, \"x\")");
	clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp3.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getStmt().stmt_ref == "s1");
	REQUIRE((*clause).getEnt().type == EntRefType::VarName);
	REQUIRE((*clause).getEnt().ent_ref == "x");

	QP::QueryPreprocessor qpp4;
	QP::QueryProperties qp4 = qpp4.parseQuery(UnivDeclarations + "Select v1 such that Uses(i1, v1)");
	clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp4.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getStmt().stmt_ref == "i1");
	REQUIRE((*clause).getEnt().type == EntRefType::Synonym);
	REQUIRE((*clause).getEnt().ent_ref == "v1");

	QP::QueryPreprocessor qpp5;
	QP::QueryProperties qp5 = qpp5.parseQuery(UnivDeclarations + "Select p1 such that Uses(p1, _)");
	clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp5.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getStmt().stmt_ref == "p1");
	REQUIRE((*clause).getEnt().type == EntRefType::Underscore);
	REQUIRE((*clause).getEnt().ent_ref == "_");

	QP::QueryPreprocessor qpp6;
	QP::QueryProperties qp6 = qpp6.parseQuery(UnivDeclarations + "Select a1 such that Uses(a1, \"y\")");
	clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp6.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getStmt().stmt_ref == "a1");
	REQUIRE((*clause).getEnt().type == EntRefType::VarName);
	REQUIRE((*clause).getEnt().ent_ref == "y");

	QP::QueryPreprocessor qpp7;
	QP::QueryProperties qp7 = qpp7.parseQuery(UnivDeclarations + "Select v1 such that Uses(w2, v1)");
	clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp7.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getStmt().stmt_ref == "w2");
	REQUIRE((*clause).getEnt().type == EntRefType::Synonym);
	REQUIRE((*clause).getEnt().ent_ref == "v1");

	QP::QueryPreprocessor qpp8;
	QP::QueryProperties qp8 = qpp8.parseQuery(UnivDeclarations + "Select i1 such that Uses(i1, _)");
	clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp8.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getStmt().stmt_ref == "i1");
	REQUIRE((*clause).getEnt().type == EntRefType::Underscore);
	REQUIRE((*clause).getEnt().ent_ref == "_");

	QP::QueryPreprocessor qpp9;
	QP::QueryProperties qp9 = qpp9.parseQuery(UnivDeclarations + "Select w1 such that Uses(w1, \"z0\")");
	clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp9.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getStmt().stmt_ref == "w1");
	REQUIRE((*clause).getEnt().type == EntRefType::VarName);
	REQUIRE((*clause).getEnt().ent_ref == "z0");

	QP::QueryPreprocessor qpp10;
	QP::QueryProperties qp10 = qpp10.parseQuery(UnivDeclarations + "Select v1 such that Uses(1, v1)");
	clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp10.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::StmtNumber);
	REQUIRE((*clause).getStmt().stmt_ref == "1");
	REQUIRE((*clause).getEnt().type == EntRefType::Synonym);
	REQUIRE((*clause).getEnt().ent_ref == "v1");

	QP::QueryPreprocessor qpp11;
	QP::QueryProperties qp11 = qpp11.parseQuery(UnivDeclarations + "Select s1 such that Uses(w2, _)");
	clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp11.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getStmt().stmt_ref == "w2");
	REQUIRE((*clause).getEnt().type == EntRefType::Underscore);
	REQUIRE((*clause).getEnt().ent_ref == "_");

	QP::QueryPreprocessor qpp12;
	QP::QueryProperties qp12 = qpp12.parseQuery(UnivDeclarations + "Select s1 such that Uses(w2, \"var\")");
	clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp12.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getStmt().stmt_ref == "w2");
	REQUIRE((*clause).getEnt().type == EntRefType::VarName);
	REQUIRE((*clause).getEnt().ent_ref == "var");

	// UsesP
	QP::Relationship::UsesP* clause2;
	QP::QueryPreprocessor qpp13;
	QP::QueryProperties qp13 = qpp13.parseQuery(UnivDeclarations + "Select v1 such that Uses(pc1, v1)");
	clause2 = dynamic_pointer_cast<QP::Relationship::UsesP>(qp13.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause2).getLeftEnt().type == EntRefType::Synonym);
	REQUIRE((*clause2).getLeftEnt().ent_ref == "pc1");
	REQUIRE((*clause2).getRightEnt().type == EntRefType::Synonym);
	REQUIRE((*clause2).getRightEnt().ent_ref == "v1");

	QP::QueryPreprocessor qpp14;
	QP::QueryProperties qp14 = qpp14.parseQuery(UnivDeclarations + "Select v1 such that Uses(c1, _)");
	clause2 = dynamic_pointer_cast<QP::Relationship::UsesP>(qp14.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause2).getLeftEnt().type == EntRefType::Synonym);
	REQUIRE((*clause2).getLeftEnt().ent_ref == "c1");
	REQUIRE((*clause2).getRightEnt().type == EntRefType::Underscore);
	REQUIRE((*clause2).getRightEnt().ent_ref == "_");

	QP::QueryPreprocessor qpp15;
	QP::QueryProperties qp15 = qpp15.parseQuery(UnivDeclarations + R"(Select s2 such that Uses("procedureName", "varName"))");
	clause2 = dynamic_pointer_cast<QP::Relationship::UsesP>(qp15.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause2).getLeftEnt().type == EntRefType::VarName);
	REQUIRE((*clause2).getLeftEnt().ent_ref == "procedureName");
	REQUIRE((*clause2).getRightEnt().type == EntRefType::VarName);
	REQUIRE((*clause2).getRightEnt().ent_ref == "varName");
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
	// Valid ModifiesS
	QP::Relationship::ModifiesS* clause;
	QP::QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select v1 such that Modifies(s1, v1)");
	clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp1.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getStmt().stmt_ref == "s1");
	REQUIRE((*clause).getEnt().type == EntRefType::Synonym);
	REQUIRE((*clause).getEnt().ent_ref == "v1");

	QP::QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select s1 such that Modifies(s1, _)");
	clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp2.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getStmt().stmt_ref == "s1");
	REQUIRE((*clause).getEnt().type == EntRefType::Underscore);
	REQUIRE((*clause).getEnt().ent_ref == "_");

	QP::QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select s1 such that Modifies(s1, \"x\")");
	clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp3.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getStmt().stmt_ref == "s1");
	REQUIRE((*clause).getEnt().type == EntRefType::VarName);
	REQUIRE((*clause).getEnt().ent_ref == "x");

	QP::QueryPreprocessor qpp4;
	QP::QueryProperties qp4 = qpp4.parseQuery(UnivDeclarations + "Select v1 such that Modifies(i1, v1)");
	clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp4.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getStmt().stmt_ref == "i1");
	REQUIRE((*clause).getEnt().type == EntRefType::Synonym);
	REQUIRE((*clause).getEnt().ent_ref == "v1");

	QP::QueryPreprocessor qpp5;
	QP::QueryProperties qp5 = qpp5.parseQuery(UnivDeclarations + "Select p1 such that Modifies(r1, _)");
	clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp5.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getStmt().stmt_ref == "r1");
	REQUIRE((*clause).getEnt().type == EntRefType::Underscore);
	REQUIRE((*clause).getEnt().ent_ref == "_");

	QP::QueryPreprocessor qpp6;
	QP::QueryProperties qp6 = qpp6.parseQuery(UnivDeclarations + "Select a1 such that Modifies(a1, \"y\")");
	clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp6.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getStmt().stmt_ref == "a1");
	REQUIRE((*clause).getEnt().type == EntRefType::VarName);
	REQUIRE((*clause).getEnt().ent_ref == "y");

	QP::QueryPreprocessor qpp7;
	QP::QueryProperties qp7 = qpp7.parseQuery(UnivDeclarations + "Select v1 such that Modifies(w2, v1)");
	clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp7.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getStmt().stmt_ref == "w2");
	REQUIRE((*clause).getEnt().type == EntRefType::Synonym);
	REQUIRE((*clause).getEnt().ent_ref == "v1");

	QP::QueryPreprocessor qpp8;
	QP::QueryProperties qp8 = qpp8.parseQuery(UnivDeclarations + "Select i1 such that Modifies(i1, _)");
	clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp8.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getStmt().stmt_ref == "i1");
	REQUIRE((*clause).getEnt().type == EntRefType::Underscore);
	REQUIRE((*clause).getEnt().ent_ref == "_");

	QP::QueryPreprocessor qpp9;
	QP::QueryProperties qp9 = qpp9.parseQuery(UnivDeclarations + "Select w1 such that Modifies(w1, \"z0\")");
	clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp9.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getStmt().stmt_ref == "w1");
	REQUIRE((*clause).getEnt().type == EntRefType::VarName);
	REQUIRE((*clause).getEnt().ent_ref == "z0");

	QP::QueryPreprocessor qpp10;
	QP::QueryProperties qp10 = qpp10.parseQuery(UnivDeclarations + "Select v1 such that Modifies(1, v1)");
	clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp10.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::StmtNumber);
	REQUIRE((*clause).getStmt().stmt_ref == "1");
	REQUIRE((*clause).getEnt().type == EntRefType::Synonym);
	REQUIRE((*clause).getEnt().ent_ref == "v1");

	QP::QueryPreprocessor qpp11;
	QP::QueryProperties qp11 = qpp11.parseQuery(UnivDeclarations + "Select s1 such that Modifies(w2, _)");
	clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp11.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getStmt().stmt_ref == "w2");
	REQUIRE((*clause).getEnt().type == EntRefType::Underscore);
	REQUIRE((*clause).getEnt().ent_ref == "_");

	QP::QueryPreprocessor qpp12;
	QP::QueryProperties qp12 = qpp12.parseQuery(UnivDeclarations + "Select s1 such that Modifies(w2, \"var\")");
	clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp12.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getStmt().stmt_ref == "w2");
	REQUIRE((*clause).getEnt().type == EntRefType::VarName);
	REQUIRE((*clause).getEnt().ent_ref == "var");

	// ModifiesP
	QP::Relationship::ModifiesP* clause2;
	QP::QueryPreprocessor qpp13;
	QP::QueryProperties qp13 = qpp13.parseQuery(UnivDeclarations + "Select v1 such that Modifies(pc1, v1)");
	clause2 = dynamic_pointer_cast<QP::Relationship::ModifiesP>(qp13.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause2).getLeftEnt().type == EntRefType::Synonym);
	REQUIRE((*clause2).getLeftEnt().ent_ref == "pc1");
	REQUIRE((*clause2).getRightEnt().type == EntRefType::Synonym);
	REQUIRE((*clause2).getRightEnt().ent_ref == "v1");

	QP::QueryPreprocessor qpp14;
	QP::QueryProperties qp14 = qpp14.parseQuery(UnivDeclarations + "Select v1 such that Modifies(c1, _)");
	clause2 = dynamic_pointer_cast<QP::Relationship::ModifiesP>(qp14.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause2).getLeftEnt().type == EntRefType::Synonym);
	REQUIRE((*clause2).getLeftEnt().ent_ref == "c1");
	REQUIRE((*clause2).getRightEnt().type == EntRefType::Underscore);
	REQUIRE((*clause2).getRightEnt().ent_ref == "_");

	QP::QueryPreprocessor qpp15;
	QP::QueryProperties qp15 = qpp15.parseQuery(UnivDeclarations + R"(Select s2 such that Modifies("procedureName", "varName"))");
	clause2 = dynamic_pointer_cast<QP::Relationship::ModifiesP>(qp15.getSuchThatClauseList()[0].relation).get();
	REQUIRE((*clause2).getLeftEnt().type == EntRefType::VarName);
	REQUIRE((*clause2).getLeftEnt().ent_ref == "procedureName");
	REQUIRE((*clause2).getRightEnt().type == EntRefType::VarName);
	REQUIRE((*clause2).getRightEnt().ent_ref == "varName");
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

TEST_CASE("QP::QueryPreprocessor::parseQuery Multiple such that") {
	QP::QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select c2 such that Modifies(c2, _) and Follows(w1, c2)");
	QP::Relationship::ModifiesP* clause = dynamic_pointer_cast<QP::Relationship::ModifiesP>(qp1.getSuchThatClauseList()[0].relation).get();
	QP::Relationship::Follows* clause1 = dynamic_pointer_cast<QP::Relationship::Follows>(qp1.getSuchThatClauseList()[1].relation).get();
	REQUIRE((*clause).getLeftEnt().type == EntRefType::Synonym);
	REQUIRE((*clause).getLeftEnt().ent_ref == "c2");
	REQUIRE((*clause).getRightEnt().type == EntRefType::Underscore);
	REQUIRE((*clause).getRightEnt().ent_ref == "_");
	REQUIRE((*clause1).getLeftStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause1).getLeftStmt().stmt_ref == "w1");
	REQUIRE((*clause1).getRightStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause1).getRightStmt().stmt_ref == "c2");

	QP::QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select w2 such that Parent*(007, _) such that Uses(w2, v1)");
	QP::Relationship::ParentT* clause2 = dynamic_pointer_cast<QP::Relationship::ParentT>(qp2.getSuchThatClauseList()[0].relation).get();
	QP::Relationship::UsesS* clause3 = dynamic_pointer_cast<QP::Relationship::UsesS>(qp2.getSuchThatClauseList()[1].relation).get();
	REQUIRE((*clause2).getParentStmt().type == StmtRefType::StmtNumber);
	REQUIRE((*clause2).getParentStmt().stmt_ref == "007");
	REQUIRE((*clause2).getChildStmt().type == StmtRefType::Underscore);
	REQUIRE((*clause2).getChildStmt().stmt_ref == "_");
	REQUIRE((*clause3).getStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause3).getStmt().stmt_ref == "w2");
	REQUIRE((*clause3).getEnt().type == EntRefType::Synonym);
	REQUIRE((*clause3).getEnt().ent_ref == "v1");
}

TEST_CASE("QP::QueryPreprocessor::parseQuery valid pattern") {
	QP::Relationship::Pattern* clause1;
	QP::QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select a1 pattern a1(v1, _)");
	clause1 = dynamic_pointer_cast<QP::Relationship::Pattern>(qp1.getPatternClauseList()[0].relation).get();
	REQUIRE((*clause1).getSynAssign().type == DesignEntity::Assign);
	REQUIRE((*clause1).getSynAssign().symbol == "a1");
	REQUIRE((*clause1).getEntRef().type == EntRefType::Synonym);
	REQUIRE((*clause1).getEntRef().ent_ref == "v1");
	REQUIRE((*clause1).getExpressionType() == ExpressionType::Underscore);
	QP::QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select v1 pattern a1(v1, \"x\")");
	clause1 = dynamic_pointer_cast<QP::Relationship::Pattern>(qp2.getPatternClauseList()[0].relation).get();
	REQUIRE((*clause1).getSynAssign().type == DesignEntity::Assign);
	REQUIRE((*clause1).getSynAssign().symbol == "a1");
	REQUIRE((*clause1).getEntRef().type == EntRefType::Synonym);
	REQUIRE((*clause1).getEntRef().ent_ref == "v1");
	REQUIRE((*clause1).getExpressionType() == ExpressionType::Expression);
	QP::QueryPreprocessor qpp3;
	QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select v1 pattern a2  (v1, _\"x\"_)");
	clause1 = dynamic_pointer_cast<QP::Relationship::Pattern>(qp3.getPatternClauseList()[0].relation).get();
	REQUIRE((*clause1).getSynAssign().type == DesignEntity::Assign);
	REQUIRE((*clause1).getSynAssign().symbol == "a2");
	REQUIRE((*clause1).getEntRef().type == EntRefType::Synonym);
	REQUIRE((*clause1).getEntRef().ent_ref == "v1");
	REQUIRE((*clause1).getExpressionType() == ExpressionType::ExpressionUnderscore);
	QP::QueryPreprocessor qpp4;
	QP::QueryProperties qp4 = qpp4.parseQuery(UnivDeclarations + "Select a1 pattern a1(_, _)");
	clause1 = dynamic_pointer_cast<QP::Relationship::Pattern>(qp4.getPatternClauseList()[0].relation).get();
	REQUIRE((*clause1).getSynAssign().type == DesignEntity::Assign);
	REQUIRE((*clause1).getSynAssign().symbol == "a1");
	REQUIRE((*clause1).getEntRef().type == EntRefType::Underscore);
	REQUIRE((*clause1).getEntRef().ent_ref == "_");
	REQUIRE((*clause1).getExpressionType() == ExpressionType::Underscore);
	QP::QueryPreprocessor qpp5;
	QP::QueryProperties qp5 = qpp5.parseQuery(UnivDeclarations + "Select a2 pattern a1(_, \"x\")");
	clause1 = dynamic_pointer_cast<QP::Relationship::Pattern>(qp5.getPatternClauseList()[0].relation).get();
	REQUIRE((*clause1).getSynAssign().type == DesignEntity::Assign);
	REQUIRE((*clause1).getSynAssign().symbol == "a1");
	REQUIRE((*clause1).getEntRef().type == EntRefType::Underscore);
	REQUIRE((*clause1).getEntRef().ent_ref == "_");
	REQUIRE((*clause1).getExpressionType() == ExpressionType::Expression);
	QP::QueryPreprocessor qpp6;
	QP::QueryProperties qp6 = qpp6.parseQuery(UnivDeclarations + "Select a1 pattern a1(_, _\"x + 2\"_)");
	clause1 = dynamic_pointer_cast<QP::Relationship::Pattern>(qp6.getPatternClauseList()[0].relation).get();
	REQUIRE((*clause1).getSynAssign().type == DesignEntity::Assign);
	REQUIRE((*clause1).getSynAssign().symbol == "a1");
	REQUIRE((*clause1).getEntRef().type == EntRefType::Underscore);
	REQUIRE((*clause1).getEntRef().ent_ref == "_");
	REQUIRE((*clause1).getExpressionType() == ExpressionType::ExpressionUnderscore);
	QP::QueryPreprocessor qpp7;
	QP::QueryProperties qp7 = qpp7.parseQuery(UnivDeclarations + R"(Select a1 pattern a1("foo", "(x+5)*3- y"))");
	clause1 = dynamic_pointer_cast<QP::Relationship::Pattern>(qp7.getPatternClauseList()[0].relation).get();
	REQUIRE((*clause1).getSynAssign().type == DesignEntity::Assign);
	REQUIRE((*clause1).getSynAssign().symbol == "a1");
	REQUIRE((*clause1).getEntRef().type == EntRefType::VarName);
	REQUIRE((*clause1).getEntRef().ent_ref == "foo");
	REQUIRE((*clause1).getExpressionType() == ExpressionType::Expression);
	QP::QueryPreprocessor qpp8;
	QP::QueryProperties qp8 = qpp8.parseQuery(UnivDeclarations + "Select a1 pattern a1(\"foo\", _)");
	clause1 = dynamic_pointer_cast<QP::Relationship::Pattern>(qp8.getPatternClauseList()[0].relation).get();
	REQUIRE((*clause1).getSynAssign().type == DesignEntity::Assign);
	REQUIRE((*clause1).getSynAssign().symbol == "a1");
	REQUIRE((*clause1).getEntRef().type == EntRefType::VarName);
	REQUIRE((*clause1).getEntRef().ent_ref == "foo");
	REQUIRE((*clause1).getExpressionType() == ExpressionType::Underscore);
	QP::QueryPreprocessor qpp9;
	QP::QueryProperties qp9 = qpp9.parseQuery(UnivDeclarations + R"(Select a2 pattern a1("foo", _"x-2"_))");
	clause1 = dynamic_pointer_cast<QP::Relationship::Pattern>(qp9.getPatternClauseList()[0].relation).get();
	REQUIRE((*clause1).getSynAssign().type == DesignEntity::Assign);
	REQUIRE((*clause1).getSynAssign().symbol == "a1");
	REQUIRE((*clause1).getEntRef().type == EntRefType::VarName);
	REQUIRE((*clause1).getEntRef().ent_ref == "foo");
	REQUIRE((*clause1).getExpressionType() == ExpressionType::ExpressionUnderscore);
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

TEST_CASE("QP::QueryPreprocessor::parseQuery Multiple such that/pattern clauses") {
	QP::QueryPreprocessor qpp12;
	QP::QueryProperties qp12 = qpp12.parseQuery(UnivDeclarations + "Select a1 such that Follows(w1, a1) pattern a1(v1, _\"2-4\"_)");
	QP::Relationship::Follows* clause = dynamic_pointer_cast<QP::Relationship::Follows>(qp12.getSuchThatClauseList()[0].relation).get();
	QP::Relationship::Pattern* clause1 = dynamic_pointer_cast<QP::Relationship::Pattern>(qp12.getPatternClauseList()[0].relation).get();
	REQUIRE((*clause1).getSynAssign().type == DesignEntity::Assign);
	REQUIRE((*clause1).getSynAssign().symbol == "a1");
	REQUIRE((*clause1).getEntRef().type == EntRefType::Synonym);
	REQUIRE((*clause1).getEntRef().ent_ref == "v1");
	REQUIRE((*clause1).getExpressionType() == ExpressionType::ExpressionUnderscore);
	REQUIRE((*clause).getLeftStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getLeftStmt().stmt_ref == "w1");
	REQUIRE((*clause).getRightStmt().type == StmtRefType::Synonym);
	REQUIRE((*clause).getRightStmt().stmt_ref == "a1");
}