#include "SP/Lexer.h"

#include "SP/SP.h"
#include "catch.hpp"
#include "catch_tools.h"

using namespace std;

TEST_CASE("SP::Lexer::initialize Illegal Token Test") {
	SP::Lexer lex;
	REQUIRE_NOTHROW(lex.initialize("x \t x"));
	REQUIRE_NOTHROW(lex.initialize("x \n x"));

	REQUIRE_THROWS_AS(lex.initialize("`"), SP::TokenizationException);
	REQUIRE_THROWS_AS(lex.initialize("~"), SP::TokenizationException);
	REQUIRE_THROWS_AS(lex.initialize("@"), SP::TokenizationException);
	REQUIRE_THROWS_AS(lex.initialize("$"), SP::TokenizationException);
	REQUIRE_THROWS_AS(lex.initialize("^"), SP::TokenizationException);
	REQUIRE_THROWS_AS(lex.initialize("]"), SP::TokenizationException);
	REQUIRE_THROWS_AS(lex.initialize("["), SP::TokenizationException);
	REQUIRE_THROWS_AS(lex.initialize("."), SP::TokenizationException);

	REQUIRE_THROWS_AS(lex.initialize("One [ Two"), SP::TokenizationException);
	REQUIRE_THROWS_AS(lex.initialize("One[Two"), SP::TokenizationException);
}

TEST_CASE("SP::Lexer::readToken Arithmetic Token Test") {
	SP::Lexer lex;
	lex.initialize("+-*/%=");
	REQUIRE_EQUALS(lex.readToken(), "+");
	REQUIRE_EQUALS(lex.readToken(), "-");
	REQUIRE_EQUALS(lex.readToken(), "*");
	REQUIRE_EQUALS(lex.readToken(), "/");
	REQUIRE_EQUALS(lex.readToken(), "%");
	REQUIRE_EQUALS(lex.readToken(), "=");
	REQUIRE_FALSE(lex.nextToken());
}

TEST_CASE("SP::Lexer::readToken Relational Token Test") {
	SP::Lexer lex;
	lex.initialize("==!=>=<=<>");
	REQUIRE_EQUALS(lex.readToken(), "==");
	REQUIRE_EQUALS(lex.readToken(), "!=");
	REQUIRE_EQUALS(lex.readToken(), ">=");
	REQUIRE_EQUALS(lex.readToken(), "<=");
	REQUIRE_EQUALS(lex.readToken(), "<");
	REQUIRE_EQUALS(lex.readToken(), ">");
	REQUIRE_FALSE(lex.nextToken());
}

TEST_CASE("SP::Lexer::readToken Name Test") {
	SP::Lexer lex;
	lex.initialize("word mixed0 0mixed");
	REQUIRE_EQUALS(lex.readToken(), "word");
	REQUIRE_EQUALS(lex.readToken(), "mixed0");
	REQUIRE_EQUALS(lex.readToken(), "0");
	REQUIRE_EQUALS(lex.readToken(), "mixed");
	REQUIRE_FALSE(lex.nextToken());
}

TEST_CASE("SP::Lexer::readToken Integer Test") {
	SP::Lexer lex;
	lex.initialize("1 12 123");
	REQUIRE_EQUALS(lex.readToken(), "1");
	REQUIRE_EQUALS(lex.readToken(), "12");
	REQUIRE_EQUALS(lex.readToken(), "123");
	REQUIRE_FALSE(lex.nextToken());
}

TEST_CASE("SP::Lexer::readToken Whitespace Test") {
	SP::Lexer lex;
	lex.initialize("One   Two\n\nThree\n   Four   \n\n\n");
	REQUIRE_EQUALS(lex.readToken(), "One");
	REQUIRE_EQUALS(lex.readToken(), "Two");
	REQUIRE_EQUALS(lex.readToken(), "Three");
	REQUIRE_EQUALS(lex.readToken(), "Four");
	REQUIRE_FALSE(lex.nextToken());
}

TEST_CASE("SP::Lexer::nextToken Repeat Empty Test") {
	SP::Lexer lex;
	lex.initialize("One");
	REQUIRE_EQUALS(lex.readToken(), "One");
	REQUIRE_FALSE(lex.nextToken());
	REQUIRE_FALSE(lex.nextToken());
	REQUIRE_FALSE(lex.nextToken());
}

TEST_CASE("SP::Lexer::peekToken Repeat Test") {
	SP::Lexer lex;
	lex.initialize("One Two Three");
	REQUIRE_EQUALS(lex.peekToken(), "One");
	REQUIRE_EQUALS(lex.peekToken(), "One");
}

TEST_CASE("SP::Lexer::nextIf Test") {
	SP::Lexer lex;
	lex.initialize("{ One }");
	REQUIRE(lex.nextIf("{"));
	REQUIRE(lex.nextIf("One"));
	REQUIRE_THROWS_AS(lex.nextIf("Three"), SP::TokenizationException);
	REQUIRE_FALSE(lex.nextIf("}"));
}

TEST_CASE("SP::Lexer::nextIf List Test") {
	SP::Lexer lex;
	lex.initialize("{ One Two Three Four }");
	REQUIRE(lex.nextIf({"{"}));
	REQUIRE(lex.nextIf({"One", "Two"}));
	REQUIRE_THROWS_AS(lex.nextIf({"Three", "Five"}), SP::TokenizationException);
	REQUIRE_FALSE(lex.nextIf({"Four", "}"}));
}
