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

TEST_CASE("Lexer::read_token Arithmetic Token Test") {
	Lexer lex;
	lex.initialize("+-*/%=");
	REQUIRE_EQUALS(lex.read_token(), "+");
	REQUIRE_EQUALS(lex.read_token(), "-");
	REQUIRE_EQUALS(lex.read_token(), "*");
	REQUIRE_EQUALS(lex.read_token(), "/");
	REQUIRE_EQUALS(lex.read_token(), "%");
	REQUIRE_EQUALS(lex.read_token(), "=");
	REQUIRE_FALSE(lex.next_token());
}

TEST_CASE("Lexer::read_token Relational Token Test") {
	Lexer lex;
	lex.initialize("==!=>=<=<>");
	REQUIRE_EQUALS(lex.read_token(), "==");
	REQUIRE_EQUALS(lex.read_token(), "!=");
	REQUIRE_EQUALS(lex.read_token(), ">=");
	REQUIRE_EQUALS(lex.read_token(), "<=");
	REQUIRE_EQUALS(lex.read_token(), "<");
	REQUIRE_EQUALS(lex.read_token(), ">");
	REQUIRE_FALSE(lex.next_token());
}

TEST_CASE("Lexer::read_token Name Test") {
	Lexer lex;
	lex.initialize("word mixed0 0mixed");
	REQUIRE_EQUALS(lex.read_token(), "word");
	REQUIRE_EQUALS(lex.read_token(), "mixed0");
	REQUIRE_EQUALS(lex.read_token(), "0");
	REQUIRE_EQUALS(lex.read_token(), "mixed");
	REQUIRE_FALSE(lex.next_token());
}

TEST_CASE("Lexer::read_token Integer Test") {
	Lexer lex;
	lex.initialize("1 12 123");
	REQUIRE_EQUALS(lex.read_token(), "1");
	REQUIRE_EQUALS(lex.read_token(), "12");
	REQUIRE_EQUALS(lex.read_token(), "123");
	REQUIRE_FALSE(lex.next_token());
}

TEST_CASE("Lexer::read_token Whitespace Test") {
	Lexer lex;
	lex.initialize("One   Two\n\nThree\n   Four   \n\n\n");
	REQUIRE_EQUALS(lex.read_token(), "One");
	REQUIRE_EQUALS(lex.read_token(), "Two");
	REQUIRE_EQUALS(lex.read_token(), "Three");
	REQUIRE_EQUALS(lex.read_token(), "Four");
	REQUIRE_FALSE(lex.next_token());
}

TEST_CASE("Lexer::next_token Repeat Empty Test") {
	Lexer lex;
	lex.initialize("One");
	REQUIRE_EQUALS(lex.read_token(), "One");
	REQUIRE_FALSE(lex.next_token());
	REQUIRE_FALSE(lex.next_token());
	REQUIRE_FALSE(lex.next_token());
}

TEST_CASE("Lexer::peek_token Repeat Test") {
	Lexer lex;
	lex.initialize("One Two Three");
	REQUIRE_EQUALS(lex.peek_token(), "One");
	REQUIRE_EQUALS(lex.peek_token(), "One");
}

TEST_CASE("Lexer::next_if Test") {
	Lexer lex;
	lex.initialize("{ One }");
	REQUIRE(lex.next_if("{"));
	REQUIRE(lex.next_if("One"));
	REQUIRE_THROWS_AS(lex.next_if("Three"), TokenizationException);
	REQUIRE_FALSE(lex.next_if("}"));
}

TEST_CASE("Lexer::next_if List Test") {
	Lexer lex;
	lex.initialize("{ One Two Three Four }");
	REQUIRE(lex.next_if({"{"}));
	REQUIRE(lex.next_if({"One", "Two"}));
	REQUIRE_THROWS_AS(lex.next_if({"Three", "Five"}), TokenizationException);
	REQUIRE_FALSE(lex.next_if({"Four", "}"}));
}
