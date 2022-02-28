#include "QP/QueryResult.h"

#include "catch.hpp"

TEST_CASE("QP::QueryResult::joinResult Should join tables with same synonym") {
	QP::QueryResult result_one = QP::QueryResult();
	QP::QueryResult result_two = QP::QueryResult();
	result_one.addColumn("a", {"3", "1"});
	result_two.addColumn("a", {"1", "2", "2", "3"});
	result_two.addColumn("b", {"5", "5", "6", "7"});

	result_two.joinResult(result_one);

	unordered_map<string, vector<string>> table = result_two.getTable();
	REQUIRE(table.at("a") == vector<string>({"1", "3"}));
	REQUIRE(table.at("b") == vector<string>({"5", "7"}));
}

TEST_CASE("QP::QueryResult::joinResult Should join tables with exact same synonyms") {
	QP::QueryResult result_one = QP::QueryResult();
	QP::QueryResult result_two = QP::QueryResult();
	result_one.addColumn("a", {"3", "1"});
	result_one.addColumn("b", {"7", "1"});
	result_two.addColumn("a", {"1", "2", "2", "3"});
	result_two.addColumn("b", {"5", "5", "6", "7"});

	result_two.joinResult(result_one);

	unordered_map<string, vector<string>> table = result_two.getTable();
	REQUIRE(table.at("a") == vector<string>({"3"}));
	REQUIRE(table.at("b") == vector<string>({"7"}));
}

TEST_CASE("QP::QueryResult::joinResult Should join tables with different synonyms") {
	QP::QueryResult result_one = QP::QueryResult();
	QP::QueryResult result_two = QP::QueryResult();
	result_one.addColumn("a", {"3", "1", "2", "4"});
	result_one.addColumn("b", {"7", "1", "9", "3"});
	result_two.addColumn("b", {"1", "2", "2", "3", "3", "1"});
	result_two.addColumn("c", {"5", "5", "6", "7", "4", "9"});

	result_one.joinResult(result_two);

	unordered_map<string, vector<string>> table = result_one.getTable();
	REQUIRE(table.at("a") == vector<string>({"1", "1", "4", "4"}));
	REQUIRE(table.at("b") == vector<string>({"1", "1", "3", "3"}));
	REQUIRE(table.at("c") == vector<string>({"5", "9", "7", "4"}));
}

TEST_CASE("QP::QueryResult::joinResult Should join tables without any common synonym") {
	QP::QueryResult result_one = QP::QueryResult();
	QP::QueryResult result_two = QP::QueryResult();
	result_one.addColumn("a", {"3", "1"});
	result_one.addColumn("b", {"7", "1"});
	result_two.addColumn("c", {"1", "2", "2"});
	result_two.addColumn("d", {"5", "5", "6"});
	result_two.addColumn("e", {"9", "8", "7"});

	result_one.joinResult(result_two);

	unordered_map<string, vector<string>> table = result_one.getTable();
	REQUIRE(table.at("a") == vector<string>({"3", "3", "3", "1", "1", "1"}));
	REQUIRE(table.at("b") == vector<string>({"7", "7", "7", "1", "1", "1"}));
	REQUIRE(table.at("c") == vector<string>({"1", "2", "2", "1", "2", "2"}));
	REQUIRE(table.at("d") == vector<string>({"5", "5", "6", "5", "5", "6"}));
	REQUIRE(table.at("e") == vector<string>({"9", "8", "7", "9", "8", "7"}));
}

TEST_CASE("QP::QueryResult::joinResult Should set result to false when all rows are removed") {
	QP::QueryResult result_one = QP::QueryResult();
	QP::QueryResult result_two = QP::QueryResult();
	result_one.addColumn("a", {"3", "1"});
	result_one.addColumn("b", {"7", "1"});
	result_two.addColumn("b", {"9", "2", "2", "3"});
	result_two.addColumn("c", {"5", "5", "6", "7"});

	result_two.joinResult(result_one);

	REQUIRE(!result_two.getResult());
}