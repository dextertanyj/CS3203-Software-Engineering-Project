#include "SP/Lexer.h"

#include "catch.hpp"
#include "catch_tools.h"
#include "SP/SP.h"

using namespace std;
using namespace SP;

TEST_CASE("Lexer::initialize Illegal Token Test") {
	Lexer lex;
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

TEST_CASE("Lexer::readToken Arithmetic Token Test") {
	Lexer lex;
	lex.initialize("+-*/%=");
	REQUIRE_EQUALS(lex.readToken(), "+");
	REQUIRE_EQUALS(lex.readToken(), "-");
	REQUIRE_EQUALS(lex.readToken(), "*");
	REQUIRE_EQUALS(lex.readToken(), "/");
	REQUIRE_EQUALS(lex.readToken(), "%");
	REQUIRE_EQUALS(lex.readToken(), "=");
	REQUIRE_FALSE(lex.nextToken());
}

TEST_CASE("Lexer::readToken Relational Token Test") {
	Lexer lex;
	lex.initialize("==!=>=<=<>");
	REQUIRE_EQUALS(lex.readToken(), "==");
	REQUIRE_EQUALS(lex.readToken(), "!=");
	REQUIRE_EQUALS(lex.readToken(), ">=");
	REQUIRE_EQUALS(lex.readToken(), "<=");
	REQUIRE_EQUALS(lex.readToken(), "<");
	REQUIRE_EQUALS(lex.readToken(), ">");
	REQUIRE_FALSE(lex.nextToken());
}

TEST_CASE("Lexer::readToken Name Test") {
	Lexer lex;
	lex.initialize("word mixed0 0mixed");
	REQUIRE_EQUALS(lex.readToken(), "word");
	REQUIRE_EQUALS(lex.readToken(), "mixed0");
	REQUIRE_EQUALS(lex.readToken(), "0");
	REQUIRE_EQUALS(lex.readToken(), "mixed");
	REQUIRE_FALSE(lex.nextToken());
}

TEST_CASE("Lexer::readToken Integer Test") {
	Lexer lex;
	lex.initialize("1 12 123");
	REQUIRE_EQUALS(lex.readToken(), "1");
	REQUIRE_EQUALS(lex.readToken(), "12");
	REQUIRE_EQUALS(lex.readToken(), "123");
	REQUIRE_FALSE(lex.nextToken());
}

TEST_CASE("Lexer::readToken Whitespace Test") {
	Lexer lex;
	lex.initialize("One   Two\n\nThree\n   Four   \n\n\n");
	REQUIRE_EQUALS(lex.readToken(), "One");
	REQUIRE_EQUALS(lex.readToken(), "Two");
	REQUIRE_EQUALS(lex.readToken(), "Three");
	REQUIRE_EQUALS(lex.readToken(), "Four");
	REQUIRE_FALSE(lex.nextToken());
}

TEST_CASE("Lexer::nextToken Repeat Empty Test") {
	Lexer lex;
	lex.initialize("One");
	REQUIRE_EQUALS(lex.readToken(), "One");
	REQUIRE_FALSE(lex.nextToken());
	REQUIRE_FALSE(lex.nextToken());
	REQUIRE_FALSE(lex.nextToken());
}

TEST_CASE("Lexer::peekToken Repeat Test") {
	Lexer lex;
	lex.initialize("One Two Three");
	REQUIRE_EQUALS(lex.peekToken(), "One");
	REQUIRE_EQUALS(lex.peekToken(), "One");
}

TEST_CASE("Lexer::nextIf Test") {
	Lexer lex;
	lex.initialize("{ One }");
	REQUIRE(lex.nextIf("{"));
	REQUIRE(lex.nextIf("One"));
	REQUIRE_THROWS_AS(lex.nextIf("Three"), SP::TokenizationException);
	REQUIRE_FALSE(lex.nextIf("}"));
}

TEST_CASE("Lexer::nextIf List Test") {
	Lexer lex;
	lex.initialize("{ One Two Three Four }");
	REQUIRE(lex.nextIf({"{"}));
	REQUIRE(lex.nextIf({"One", "Two"}));
	REQUIRE_THROWS_AS(lex.nextIf({"Three", "Five"}), SP::TokenizationException);
	REQUIRE_FALSE(lex.nextIf({"Four", "}"}));
}
