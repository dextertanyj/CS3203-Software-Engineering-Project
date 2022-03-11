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

TEST_CASE("QP::QueryPreprocessor::parseQuery Select") {
	QP::QueryPreprocessor qpp1;
	QP::QueryProperties qp1 = qpp1.parseQuery("print a; if b; Select a");
	REQUIRE(qp1.getSelectList()[0].type == QP::Types::DesignEntity::Print);
	REQUIRE(qp1.getSelectList()[0].symbol == "a");

	QP::QueryPreprocessor qpp2;
	QP::QueryProperties qp2 = qpp2.parseQuery("print a; if b; Select b");
	REQUIRE(qp2.getSelectList()[0].type == QP::Types::DesignEntity::If);
	REQUIRE(qp2.getSelectList()[0].symbol == "b");

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
/*
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
    clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp1.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "s1");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getEnt().getSynonym().symbol == "v1");

    QP::QueryPreprocessor qpp2;
    QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select s1 such that Uses(s1, _)");
    clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp2.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "s1");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Wildcard);

    QP::QueryPreprocessor qpp3;
    QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select s1 such that Uses(s1, \"x\")");
    clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp3.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "s1");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Name);
    REQUIRE((*clause).getEnt().getName() == "x");

    QP::QueryPreprocessor qpp4;
    QP::QueryProperties qp4 = qpp4.parseQuery(UnivDeclarations + "Select v1 such that Uses(i1, v1)");
    clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp4.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "i1");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getEnt().getSynonym().symbol == "v1");

    QP::QueryPreprocessor qpp5;
    QP::QueryProperties qp5 = qpp5.parseQuery(UnivDeclarations + "Select p1 such that Uses(p1, _)");
    clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp5.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "p1");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Wildcard);

    QP::QueryPreprocessor qpp6;
    QP::QueryProperties qp6 = qpp6.parseQuery(UnivDeclarations + "Select a1 such that Uses(a1, \"y\")");
    clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp6.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "a1");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Name);
    REQUIRE((*clause).getEnt().getName() == "y");

    QP::QueryPreprocessor qpp7;
    QP::QueryProperties qp7 = qpp7.parseQuery(UnivDeclarations + "Select v1 such that Uses(w2, v1)");
    clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp7.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "w2");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getEnt().getSynonym().symbol == "v1");

    QP::QueryPreprocessor qpp8;
    QP::QueryProperties qp8 = qpp8.parseQuery(UnivDeclarations + "Select i1 such that Uses(i1, _)");
    clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp8.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "i1");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Wildcard);

    QP::QueryPreprocessor qpp9;
    QP::QueryProperties qp9 = qpp9.parseQuery(UnivDeclarations + "Select w1 such that Uses(w1, \"z0\")");
    clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp9.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "w1");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Name);
    REQUIRE((*clause).getEnt().getName() == "z0");

    QP::QueryPreprocessor qpp10;
    QP::QueryProperties qp10 = qpp10.parseQuery(UnivDeclarations + "Select v1 such that Uses(1, v1)");
    clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp10.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::StatementIndex);
    REQUIRE((*clause).getStmt().getStatementIndex() == 1);
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getEnt().getSynonym().symbol == "v1");

    QP::QueryPreprocessor qpp11;
    QP::QueryProperties qp11 = qpp11.parseQuery(UnivDeclarations + "Select s1 such that Uses(c2, _)");
    clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp11.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "c2");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Wildcard);

    QP::QueryPreprocessor qpp12;
    QP::QueryProperties qp12 = qpp12.parseQuery(UnivDeclarations + "Select s1 such that Uses(w2, \"var\")");
    clause = dynamic_pointer_cast<QP::Relationship::UsesS>(qp12.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "w2");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Name);
    REQUIRE((*clause).getEnt().getName() == "var");

    // UsesP
    QP::Relationship::UsesP* clause2;
    QP::QueryPreprocessor qpp13;
    QP::QueryProperties qp13 = qpp13.parseQuery(UnivDeclarations + "Select v1 such that Uses(pc1, v1)");
    clause2 = dynamic_pointer_cast<QP::Relationship::UsesP>(qp13.getClauseList()[0].relation).get();
    REQUIRE((*clause2).getLeftEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause2).getLeftEnt().getSynonym().symbol == "pc1");
    REQUIRE((*clause2).getRightEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause2).getRightEnt().getSynonym().symbol == "v1");

    QP::QueryPreprocessor qpp14;
    QP::QueryProperties qp14 = qpp14.parseQuery(UnivDeclarations + "Select v1 such that Uses(pc1, _)");
    clause2 = dynamic_pointer_cast<QP::Relationship::UsesP>(qp14.getClauseList()[0].relation).get();
    REQUIRE((*clause2).getLeftEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause2).getLeftEnt().getSynonym().symbol == "pc1");
    REQUIRE((*clause2).getRightEnt().getType() == QP::Types::ReferenceType::Wildcard);

    QP::QueryPreprocessor qpp15;
    QP::QueryProperties qp15 = qpp15.parseQuery(UnivDeclarations + R"(Select s2 such that Uses("procedureName", "varName"))");
    clause2 = dynamic_pointer_cast<QP::Relationship::UsesP>(qp15.getClauseList()[0].relation).get();
    REQUIRE((*clause2).getLeftEnt().getType() == QP::Types::ReferenceType::Name);
    REQUIRE((*clause2).getLeftEnt().getName() == "procedureName");
    REQUIRE((*clause2).getRightEnt().getType() == QP::Types::ReferenceType::Name);
    REQUIRE((*clause2).getRightEnt().getName() == "varName");
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
    clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp1.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "s1");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getEnt().getSynonym().symbol == "v1");

    QP::QueryPreprocessor qpp2;
    QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select s1 such that Modifies(s1, _)");
    clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp2.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "s1");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Wildcard);

    QP::QueryPreprocessor qpp3;
    QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select s1 such that Modifies(s1, \"x\")");
    clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp3.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "s1");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Name);
    REQUIRE((*clause).getEnt().getName() == "x");

    QP::QueryPreprocessor qpp4;
    QP::QueryProperties qp4 = qpp4.parseQuery(UnivDeclarations + "Select v1 such that Modifies(i1, v1)");
    clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp4.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "i1");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getEnt().getSynonym().symbol == "v1");

    QP::QueryPreprocessor qpp5;
    QP::QueryProperties qp5 = qpp5.parseQuery(UnivDeclarations + "Select p1 such that Modifies(r1, _)");
    clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp5.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "r1");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Wildcard);

    QP::QueryPreprocessor qpp6;
    QP::QueryProperties qp6 = qpp6.parseQuery(UnivDeclarations + "Select a1 such that Modifies(a1, \"y\")");
    clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp6.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "a1");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Name);
    REQUIRE((*clause).getEnt().getName() == "y");

    QP::QueryPreprocessor qpp7;
    QP::QueryProperties qp7 = qpp7.parseQuery(UnivDeclarations + "Select v1 such that Modifies(w2, v1)");
    clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp7.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "w2");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getEnt().getSynonym().symbol == "v1");

    QP::QueryPreprocessor qpp8;
    QP::QueryProperties qp8 = qpp8.parseQuery(UnivDeclarations + "Select i1 such that Modifies(i1, _)");
    clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp8.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "i1");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Wildcard);

    QP::QueryPreprocessor qpp9;
    QP::QueryProperties qp9 = qpp9.parseQuery(UnivDeclarations + "Select w1 such that Modifies(w1, \"z0\")");
    clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp9.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "w1");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Name);
    REQUIRE((*clause).getEnt().getName() == "z0");

    QP::QueryPreprocessor qpp10;
    QP::QueryProperties qp10 = qpp10.parseQuery(UnivDeclarations + "Select v1 such that Modifies(1, v1)");
    clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp10.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::StatementIndex);
    REQUIRE((*clause).getStmt().getStatementIndex() == 1);
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getEnt().getSynonym().symbol == "v1");

    QP::QueryPreprocessor qpp11;
    QP::QueryProperties qp11 = qpp11.parseQuery(UnivDeclarations + "Select s1 such that Modifies(w2, _)");
    clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp11.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "w2");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Wildcard);

    QP::QueryPreprocessor qpp12;
    QP::QueryProperties qp12 = qpp12.parseQuery(UnivDeclarations + "Select s1 such that Modifies(c2, \"var\")");
    clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp12.getClauseList()[0].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "c2");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Name);
    REQUIRE((*clause).getEnt().getName() == "var");

    // ModifiesP
    QP::Relationship::ModifiesP* clause2;
    QP::QueryPreprocessor qpp13;
    QP::QueryProperties qp13 = qpp13.parseQuery(UnivDeclarations + "Select v1 such that Modifies(pc1, v1)");
    clause2 = dynamic_pointer_cast<QP::Relationship::ModifiesP>(qp13.getClauseList()[0].relation).get();
    REQUIRE((*clause2).getLeftEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause2).getLeftEnt().getSynonym().symbol == "pc1");
    REQUIRE((*clause2).getRightEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause2).getRightEnt().getSynonym().symbol == "v1");

    QP::QueryPreprocessor qpp14;
    QP::QueryProperties qp14 = qpp14.parseQuery(UnivDeclarations + "Select v1 such that Modifies(pc1, _)");
    clause2 = dynamic_pointer_cast<QP::Relationship::ModifiesP>(qp14.getClauseList()[0].relation).get();
    REQUIRE((*clause2).getLeftEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause2).getLeftEnt().getSynonym().symbol == "pc1");
    REQUIRE((*clause2).getRightEnt().getType() == QP::Types::ReferenceType::Wildcard);

    QP::QueryPreprocessor qpp15;
    QP::QueryProperties qp15 = qpp15.parseQuery(UnivDeclarations + R"(Select s2 such that Modifies("procedureName", "varName"))");
    clause2 = dynamic_pointer_cast<QP::Relationship::ModifiesP>(qp15.getClauseList()[0].relation).get();
    REQUIRE((*clause2).getLeftEnt().getType() == QP::Types::ReferenceType::Name);
    REQUIRE((*clause2).getLeftEnt().getName() == "procedureName");
    REQUIRE((*clause2).getRightEnt().getType() == QP::Types::ReferenceType::Name);
    REQUIRE((*clause2).getRightEnt().getName() == "varName");
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
    QP::Relationship::Calls* clause;
    QP::Relationship::CallsT* clause_t;

    QP::QueryPreprocessor qpp1;
    QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select pc1 such that Calls(pc1, pc2)");
    clause = dynamic_pointer_cast<QP::Relationship::Calls>(qp1.getClauseList()[0].relation).get();
    REQUIRE((*clause).getCallerEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getCallerEnt().getSynonym().symbol == "pc1");
    REQUIRE((*clause).getCalleeEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getCalleeEnt().getSynonym().symbol == "pc2");

    QP::QueryPreprocessor qpp2;
    QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select pc1 such that Calls(pc1, _)");
    clause = dynamic_pointer_cast<QP::Relationship::Calls>(qp2.getClauseList()[0].relation).get();
    REQUIRE((*clause).getCallerEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getCallerEnt().getSynonym().symbol == "pc1");
    REQUIRE((*clause).getCalleeEnt().getType() == QP::Types::ReferenceType::Wildcard);

    QP::QueryPreprocessor qpp3;
    QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select pc1 such that Calls(pc1, \"procedure1\")");
    clause = dynamic_pointer_cast<QP::Relationship::Calls>(qp3.getClauseList()[0].relation).get();
    REQUIRE((*clause).getCallerEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getCallerEnt().getSynonym().symbol == "pc1");
    REQUIRE((*clause).getCalleeEnt().getType() == QP::Types::ReferenceType::Name);
    REQUIRE((*clause).getCalleeEnt().getName() == "procedure1");

    QP::QueryPreprocessor qpp4;
    QP::QueryProperties qp4 = qpp4.parseQuery(UnivDeclarations + "Select pc1 such that Calls(\"procedure1\", pc1)");
    clause = dynamic_pointer_cast<QP::Relationship::Calls>(qp4.getClauseList()[0].relation).get();
    REQUIRE((*clause).getCallerEnt().getType() == QP::Types::ReferenceType::Name);
    REQUIRE((*clause).getCallerEnt().getName() == "procedure1");
    REQUIRE((*clause).getCalleeEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getCalleeEnt().getSynonym().symbol == "pc1");

    QP::QueryPreprocessor qpp5;
    QP::QueryProperties qp5 = qpp5.parseQuery(UnivDeclarations + "Select i1 such that Calls(\"procedure1\", \"procedure2\")");
    clause = dynamic_pointer_cast<QP::Relationship::Calls>(qp5.getClauseList()[0].relation).get();
    REQUIRE((*clause).getCallerEnt().getType() == QP::Types::ReferenceType::Name);
    REQUIRE((*clause).getCallerEnt().getName() == "procedure1");
    REQUIRE((*clause).getCalleeEnt().getType() == QP::Types::ReferenceType::Name);
    REQUIRE((*clause).getCalleeEnt().getName() == "procedure2");

    QP::QueryPreprocessor qpp6;
    QP::QueryProperties qp6 = qpp6.parseQuery(UnivDeclarations + "Select w2 such that Calls(\"procedure1\", _)");
    clause = dynamic_pointer_cast<QP::Relationship::Calls>(qp6.getClauseList()[0].relation).get();
    REQUIRE((*clause).getCallerEnt().getType() == QP::Types::ReferenceType::Name);
    REQUIRE((*clause).getCallerEnt().getName() == "procedure1");
    REQUIRE((*clause).getCalleeEnt().getType() == QP::Types::ReferenceType::Wildcard);

    QP::QueryPreprocessor qpp7;
    QP::QueryProperties qp7 = qpp7.parseQuery(UnivDeclarations + "Select s1 such that Calls(_, pc2)");
    clause = dynamic_pointer_cast<QP::Relationship::Calls>(qp7.getClauseList()[0].relation).get();
    REQUIRE((*clause).getCallerEnt().getType() == QP::Types::ReferenceType::Wildcard);
    REQUIRE((*clause).getCalleeEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getCalleeEnt().getSynonym().symbol == "pc2");

    QP::QueryPreprocessor qpp8;
    QP::QueryProperties qp8 = qpp8.parseQuery(UnivDeclarations + "Select i1 such that Calls*(_, \"procedure1\")");
    clause = dynamic_pointer_cast<QP::Relationship::Calls>(qp8.getClauseList()[0].relation).get();
    REQUIRE((*clause).getCallerEnt().getType() == QP::Types::ReferenceType::Wildcard);
    REQUIRE((*clause).getCalleeEnt().getType() == QP::Types::ReferenceType::Name);
    REQUIRE((*clause).getCalleeEnt().getName() == "procedure1");

    QP::QueryPreprocessor qpp9;
    QP::QueryProperties qp9 = qpp9.parseQuery(UnivDeclarations + "Select w2 such that Calls(_, _)");
    clause = dynamic_pointer_cast<QP::Relationship::Calls>(qp9.getClauseList()[0].relation).get();
    REQUIRE((*clause).getCallerEnt().getType() == QP::Types::ReferenceType::Wildcard);
    REQUIRE((*clause).getCalleeEnt().getType() == QP::Types::ReferenceType::Wildcard);

    QP::QueryPreprocessor qpp10;
    QP::QueryProperties qp10 = qpp10.parseQuery(UnivDeclarations + "Select pc2 such that Calls*(pc1, pc2)");
    clause_t = dynamic_pointer_cast<QP::Relationship::CallsT>(qp10.getClauseList()[0].relation).get();
    REQUIRE((*clause_t).getCallerEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause_t).getCallerEnt().getSynonym().symbol == "pc1");
    REQUIRE((*clause_t).getCalleeEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause_t).getCalleeEnt().getSynonym().symbol == "pc2");
}

TEST_CASE("QP::QueryPreprocessor::parseQuery Multiple such that") {
    QP::QueryPreprocessor qpp1;
    QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select c2 such that Modifies(c2, _) and Follows(w1, c2)");
    QP::Relationship::ModifiesS* clause = dynamic_pointer_cast<QP::Relationship::ModifiesS>(qp1.getClauseList()[0].relation).get();
    QP::Relationship::Follows* clause1 = dynamic_pointer_cast<QP::Relationship::Follows>(qp1.getClauseList()[1].relation).get();
    REQUIRE((*clause).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getStmt().getSynonym().symbol == "c2");
    REQUIRE((*clause).getEnt().getType() == QP::Types::ReferenceType::Wildcard);
    REQUIRE((*clause1).getLeftStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause1).getLeftStmt().getSynonym().symbol == "w1");
    REQUIRE((*clause1).getRightStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause1).getRightStmt().getSynonym().symbol == "c2");

    QP::QueryPreprocessor qpp2;
    QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select w2 such that Parent*(7, _) such that Uses(w2, v1)");
    QP::Relationship::ParentT* clause2 = dynamic_pointer_cast<QP::Relationship::ParentT>(qp2.getClauseList()[0].relation).get();
    QP::Relationship::UsesS* clause3 = dynamic_pointer_cast<QP::Relationship::UsesS>(qp2.getClauseList()[1].relation).get();
    REQUIRE((*clause2).getParentStmt().getType() == QP::Types::ReferenceType::StatementIndex);
    REQUIRE((*clause2).getParentStmt().getStatementIndex() == 7);
    REQUIRE((*clause2).getChildStmt().getType() == QP::Types::ReferenceType::Wildcard);
    REQUIRE((*clause3).getStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause3).getStmt().getSynonym().symbol == "w2");
    REQUIRE((*clause3).getEnt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause3).getEnt().getSynonym().symbol == "v1");
}

TEST_CASE("QP::QueryPreprocessor::parseQuery valid pattern") {
    QP::Relationship::Pattern* clause1;
    QP::QueryPreprocessor qpp1;
    QP::QueryProperties qp1 = qpp1.parseQuery(UnivDeclarations + "Select a1 pattern a1(v1, _)");
    clause1 = dynamic_pointer_cast<QP::Relationship::Pattern>(qp1.getClauseList()[0].relation).get();
    REQUIRE((*clause1).getSynAssign().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause1).getSynAssign().getSynonym().type == QP::Types::DesignEntity::Assign);
    REQUIRE((*clause1).getSynAssign().getSynonym().symbol == "a1");
    REQUIRE((*clause1).getEntRef().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause1).getEntRef().getSynonym().symbol == "v1");
    REQUIRE((*clause1).getExpressionType() == QP::Types::ReferenceType::Wildcard);
    QP::QueryPreprocessor qpp2;
    QP::QueryProperties qp2 = qpp2.parseQuery(UnivDeclarations + "Select v1 pattern a1(v1, \"x\")");
    clause1 = dynamic_pointer_cast<QP::Relationship::Pattern>(qp2.getClauseList()[0].relation).get();
    REQUIRE((*clause1).getSynAssign().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause1).getSynAssign().getSynonym().type == QP::Types::DesignEntity::Assign);
    REQUIRE((*clause1).getSynAssign().getSynonym().symbol == "a1");
    REQUIRE((*clause1).getEntRef().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause1).getEntRef().getSynonym().symbol == "v1");
    REQUIRE((*clause1).getExpressionType() == QP::Types::ReferenceType::ExactExpression);
    QP::QueryPreprocessor qpp3;
    QP::QueryProperties qp3 = qpp3.parseQuery(UnivDeclarations + "Select v1 pattern a2  (v1, _\"x\"_)");
    clause1 = dynamic_pointer_cast<QP::Relationship::Pattern>(qp3.getClauseList()[0].relation).get();
    REQUIRE((*clause1).getSynAssign().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause1).getSynAssign().getSynonym().type == QP::Types::DesignEntity::Assign);
    REQUIRE((*clause1).getSynAssign().getSynonym().symbol == "a2");
    REQUIRE((*clause1).getEntRef().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause1).getEntRef().getSynonym().symbol == "v1");
    REQUIRE((*clause1).getExpressionType() == QP::Types::ReferenceType::SubExpression);
    QP::QueryPreprocessor qpp4;
    QP::QueryProperties qp4 = qpp4.parseQuery(UnivDeclarations + "Select a1 pattern a1(_, _)");
    clause1 = dynamic_pointer_cast<QP::Relationship::Pattern>(qp4.getClauseList()[0].relation).get();
    REQUIRE((*clause1).getSynAssign().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause1).getSynAssign().getSynonym().type == QP::Types::DesignEntity::Assign);
    REQUIRE((*clause1).getSynAssign().getSynonym().symbol == "a1");
    REQUIRE((*clause1).getEntRef().getType() == QP::Types::ReferenceType::Wildcard);
    REQUIRE((*clause1).getExpressionType() == QP::Types::ReferenceType::Wildcard);
    QP::QueryPreprocessor qpp5;
    QP::QueryProperties qp5 = qpp5.parseQuery(UnivDeclarations + "Select a2 pattern a1(_, \"x\")");
    clause1 = dynamic_pointer_cast<QP::Relationship::Pattern>(qp5.getClauseList()[0].relation).get();
    REQUIRE((*clause1).getSynAssign().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause1).getSynAssign().getSynonym().type == QP::Types::DesignEntity::Assign);
    REQUIRE((*clause1).getSynAssign().getSynonym().symbol == "a1");
    REQUIRE((*clause1).getEntRef().getType() == QP::Types::ReferenceType::Wildcard);
    ;
    REQUIRE((*clause1).getExpressionType() == QP::Types::ReferenceType::ExactExpression);
    QP::QueryPreprocessor qpp6;
    QP::QueryProperties qp6 = qpp6.parseQuery(UnivDeclarations + "Select a1 pattern a1(_, _\"x + 2\"_)");
    clause1 = dynamic_pointer_cast<QP::Relationship::Pattern>(qp6.getClauseList()[0].relation).get();
    REQUIRE((*clause1).getSynAssign().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause1).getSynAssign().getSynonym().type == QP::Types::DesignEntity::Assign);
    REQUIRE((*clause1).getSynAssign().getSynonym().symbol == "a1");
    REQUIRE((*clause1).getEntRef().getType() == QP::Types::ReferenceType::Wildcard);
    REQUIRE((*clause1).getExpressionType() == QP::Types::ReferenceType::SubExpression);
    QP::QueryPreprocessor qpp7;
    QP::QueryProperties qp7 = qpp7.parseQuery(UnivDeclarations + R"(Select a1 pattern a1("foo", "(x+5)*3- y"))");
    clause1 = dynamic_pointer_cast<QP::Relationship::Pattern>(qp7.getClauseList()[0].relation).get();
    REQUIRE((*clause1).getSynAssign().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause1).getSynAssign().getSynonym().type == QP::Types::DesignEntity::Assign);
    REQUIRE((*clause1).getSynAssign().getSynonym().symbol == "a1");
    REQUIRE((*clause1).getEntRef().getType() == QP::Types::ReferenceType::Name);
    REQUIRE((*clause1).getEntRef().getName() == "foo");
    REQUIRE((*clause1).getExpressionType() == QP::Types::ReferenceType::ExactExpression);
    QP::QueryPreprocessor qpp8;
    QP::QueryProperties qp8 = qpp8.parseQuery(UnivDeclarations + "Select a1 pattern a1(\"foo\", _)");
    clause1 = dynamic_pointer_cast<QP::Relationship::Pattern>(qp8.getClauseList()[0].relation).get();
    REQUIRE((*clause1).getSynAssign().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause1).getSynAssign().getSynonym().type == QP::Types::DesignEntity::Assign);
    REQUIRE((*clause1).getSynAssign().getSynonym().symbol == "a1");
    REQUIRE((*clause1).getEntRef().getType() == QP::Types::ReferenceType::Name);
    REQUIRE((*clause1).getEntRef().getName() == "foo");
    REQUIRE((*clause1).getExpressionType() == QP::Types::ReferenceType::Wildcard);
    QP::QueryPreprocessor qpp9;
    QP::QueryProperties qp9 = qpp9.parseQuery(UnivDeclarations + R"(Select a2 pattern a1("foo", _"x-2"_))");
    clause1 = dynamic_pointer_cast<QP::Relationship::Pattern>(qp9.getClauseList()[0].relation).get();
    REQUIRE((*clause1).getSynAssign().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause1).getSynAssign().getSynonym().type == QP::Types::DesignEntity::Assign);
    REQUIRE((*clause1).getSynAssign().getSynonym().symbol == "a1");
    REQUIRE((*clause1).getEntRef().getType() == QP::Types::ReferenceType::Name);
    REQUIRE((*clause1).getEntRef().getName() == "foo");
    REQUIRE((*clause1).getExpressionType() == QP::Types::ReferenceType::SubExpression);
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
    QP::Relationship::Follows* clause = dynamic_pointer_cast<QP::Relationship::Follows>(qp12.getClauseList()[0].relation).get();
    QP::Relationship::Pattern* clause1 = dynamic_pointer_cast<QP::Relationship::Pattern>(qp12.getClauseList()[1].relation).get();
    REQUIRE((*clause1).getSynAssign().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause1).getSynAssign().getSynonym().type == QP::Types::DesignEntity::Assign);
    REQUIRE((*clause1).getSynAssign().getSynonym().symbol == "a1");
    REQUIRE((*clause1).getEntRef().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause1).getEntRef().getSynonym().symbol == "v1");
    REQUIRE((*clause1).getExpressionType() == QP::Types::ReferenceType::SubExpression);
    REQUIRE((*clause).getLeftStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getLeftStmt().getSynonym().symbol == "w1");
    REQUIRE((*clause).getRightStmt().getType() == QP::Types::ReferenceType::Synonym);
    REQUIRE((*clause).getRightStmt().getSynonym().symbol == "a1");
}
*/