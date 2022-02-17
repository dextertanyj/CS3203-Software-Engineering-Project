#include "QP/QueryResult.h"

#include "catch.hpp"

TEST_CASE("QP::QueryResult::joinResult Should join tables with same synonym") {
	QueryResult resultOne = QueryResult();
	QueryResult resultTwo = QueryResult();
	resultOne.addColumn("a", { "3", "1" });
	resultTwo.addColumn("a", { "1", "2", "2", "3" });
	resultTwo.addColumn("b", { "5", "5", "6", "7" });
	
	resultTwo.joinResult(resultOne);

	unordered_map<string, vector<string>> table = resultTwo.getTable();
	REQUIRE(table.at("a") == vector<string>({ "1", "3" }));
	REQUIRE(table.at("b") == vector<string>({ "5", "7" }));
}

TEST_CASE("QP::QueryResult::joinResult Should join tables with exact same synonyms") {
	QueryResult resultOne = QueryResult();
	QueryResult resultTwo = QueryResult();
	resultOne.addColumn("a", { "3", "1" });
	resultOne.addColumn("b", { "7", "1" });
	resultTwo.addColumn("a", { "1", "2", "2", "3" });
	resultTwo.addColumn("b", { "5", "5", "6", "7" });
	
	resultTwo.joinResult(resultOne);

	unordered_map<string, vector<string>> table = resultTwo.getTable();
	REQUIRE(table.at("a") == vector<string>({ "3" }));
	REQUIRE(table.at("b") == vector<string>({ "7" }));
}

TEST_CASE("QP::QueryResult::joinResult Should join tables with different synonyms") {
	QueryResult resultOne = QueryResult();
	QueryResult resultTwo = QueryResult();
	resultOne.addColumn("a", { "3", "1" });
	resultOne.addColumn("b", { "7", "1" });
	resultTwo.addColumn("b", { "1", "2", "2", "3" });
	resultTwo.addColumn("c", { "5", "5", "6", "7" });
	
	resultTwo.joinResult(resultOne);

	unordered_map<string, vector<string>> table = resultTwo.getTable();
	REQUIRE(table.at("b") == vector<string>({ "1" }));
	REQUIRE(table.at("c") == vector<string>({ "5" }));
}

TEST_CASE("QP::QueryResult::joinResult Should set result to false when all rows are removed") {
	QueryResult resultOne = QueryResult();
	QueryResult resultTwo = QueryResult();
	resultOne.addColumn("a", { "3", "1" });
	resultOne.addColumn("b", { "7", "1" });
	resultTwo.addColumn("b", { "9", "2", "2", "3" });
	resultTwo.addColumn("c", { "5", "5", "6", "7" });
	
	resultTwo.joinResult(resultOne);

	REQUIRE(!resultTwo.getResult());
}