#include "SP/Lexer.h"

#include "catch.hpp"
#include "catch_tools.h"

using namespace std;

TEST_CASE("Lexer::initialize Illegal Token Test") {
	Lexer lex;
	REQUIRE_THROWS_AS(lex.initialize("`"), TokenizationException);
	REQUIRE_THROWS_AS(lex.initialize("~"), TokenizationException);
	REQUIRE_THROWS_AS(lex.initialize("@"), TokenizationException);
	REQUIRE_THROWS_AS(lex.initialize("$"), TokenizationException);
	REQUIRE_THROWS_AS(lex.initialize("^"), TokenizationException);
	REQUIRE_THROWS_AS(lex.initialize("]"), TokenizationException);
	REQUIRE_THROWS_AS(lex.initialize("["), TokenizationException);
	REQUIRE_THROWS_AS(lex.initialize("."), TokenizationException);

	REQUIRE_THROWS_AS(lex.initialize("One [ Two"), TokenizationException);
	REQUIRE_THROWS_AS(lex.initialize("One[Two"), TokenizationException);
}

TEST_CASE("Lexer::get_token Arithmetic Token Test") {
	Lexer lex;
	lex.initialize("+-*/%=");
	REQUIRE_EQUALS(lex.get_token(), "+");
	REQUIRE(lex.next_token());
	REQUIRE_EQUALS(lex.get_token(), "-");
	REQUIRE(lex.next_token());
	REQUIRE_EQUALS(lex.get_token(), "*");
	REQUIRE(lex.next_token());
	REQUIRE_EQUALS(lex.get_token(), "/");
	REQUIRE(lex.next_token());
	REQUIRE_EQUALS(lex.get_token(), "%");
	REQUIRE(lex.next_token());
	REQUIRE_EQUALS(lex.get_token(), "=");
	REQUIRE_FALSE(lex.next_token());
}

TEST_CASE("Lexer::get_token Relational Token Test") {
	Lexer lex;
	lex.initialize("==!=>=<=<>");
	REQUIRE_EQUALS(lex.get_token(), "==");
	REQUIRE(lex.next_token());
	REQUIRE_EQUALS(lex.get_token(), "!=");
	REQUIRE(lex.next_token());
	REQUIRE_EQUALS(lex.get_token(), ">=");
	REQUIRE(lex.next_token());
	REQUIRE_EQUALS(lex.get_token(), "<=");
	REQUIRE(lex.next_token());
	REQUIRE_EQUALS(lex.get_token(), "<");
	REQUIRE(lex.next_token());
	REQUIRE_EQUALS(lex.get_token(), ">");
	REQUIRE_FALSE(lex.next_token());
}

TEST_CASE("Lexer::get_token Name Test") {
	Lexer lex;
	lex.initialize("word mixed0 0mixed");
	REQUIRE_EQUALS(lex.get_token(), "word");
	REQUIRE(lex.next_token());
	REQUIRE_EQUALS(lex.get_token(), "mixed0");
	REQUIRE(lex.next_token());
	REQUIRE_EQUALS(lex.get_token(), "0");
	REQUIRE(lex.next_token());
	REQUIRE_EQUALS(lex.get_token(), "mixed");
	REQUIRE_FALSE(lex.next_token());
}

TEST_CASE("Lexer::get_token Integer Test") {
	Lexer lex;
	lex.initialize("1 12 123");
	REQUIRE_EQUALS(lex.get_token(), "1");
	REQUIRE(lex.next_token());
	REQUIRE_EQUALS(lex.get_token(), "12");
	REQUIRE(lex.next_token());
	REQUIRE_EQUALS(lex.get_token(), "123");
	REQUIRE_FALSE(lex.next_token());
}

TEST_CASE("Lexer::get_token Whitespace Test") {
	Lexer lex;
	lex.initialize("One   Two\n\nThree\n   Four   \n\n\n");
	REQUIRE_EQUALS(lex.get_token(), "One");
	REQUIRE(lex.next_token());
	REQUIRE_EQUALS(lex.get_token(), "Two");
	REQUIRE(lex.next_token());
	REQUIRE_EQUALS(lex.get_token(), "Three");
	REQUIRE(lex.next_token());
	REQUIRE_EQUALS(lex.get_token(), "Four");
	REQUIRE_FALSE(lex.next_token());
}

TEST_CASE("Lexer::next_token Repeat Empty Test") {
	Lexer lex;
	lex.initialize("One");
	REQUIRE_EQUALS(lex.get_token(), "One");
	REQUIRE_FALSE(lex.next_token());
	REQUIRE_FALSE(lex.next_token());
	REQUIRE_FALSE(lex.next_token());
}
