#include "QP/QueryResult.h"

#include "catch.hpp"

TEST_CASE("QP::QueryResult::addRow should set result to true") {
	QP::QueryResult result = QP::QueryResult(vector<string>({"a"}));

	result.addRow({"5"});

	REQUIRE(result.getResult());
}

TEST_CASE("QP::QueryResult::joinResult should set result to false") {
	QP::QueryResult result_one = QP::QueryResult(vector<string>({"a"}));
	QP::QueryResult result_two = QP::QueryResult(vector<string>({"a", "b"}));
	result_one.addRow({"5"});
	result_one.addRow({"6"});
	result_two.addRow({"1", "5"});
	result_two.addRow({"2", "5"});
	result_two.addRow({"2", "6"});
	result_two.addRow({"3", "7"});
	vector<QP::QueryResult> results = {result_one, result_two};

	QP::QueryResult final_result = QP::QueryResult::joinResults(results);

	REQUIRE(!final_result.getResult());
}

TEST_CASE("QP::QueryResult::joinResult should set result to true") {
	QP::QueryResult result_one = QP::QueryResult(vector<string>({"a"}));
	QP::QueryResult result_two = QP::QueryResult(vector<string>({"a", "b"}));
	result_one.addRow({"1"});
	result_one.addRow({"2"});
	result_two.addRow({"1", "5"});
	result_two.addRow({"2", "5"});
	result_two.addRow({"2", "6"});
	result_two.addRow({"3", "7"});
	vector<QP::QueryResult> results = {result_one, result_two};

	QP::QueryResult final_result = QP::QueryResult::joinResults(results);

	REQUIRE(final_result.getResult());
	REQUIRE(final_result.getNumberOfRows() == 3);
}

TEST_CASE("QP::QueryResult::joinResult multiple results, should return true") {
	QP::QueryResult result_one = QP::QueryResult(vector<string>({"a"}));
	QP::QueryResult result_two = QP::QueryResult(vector<string>({"a", "b"}));
	QP::QueryResult result_three = QP::QueryResult(vector<string>({"c", "d", "a", "e"}));
	QP::QueryResult result_four = QP::QueryResult(vector<string>({"c", "b"}));
	result_one.addRow({"5"});
	result_one.addRow({"6"});
	result_two.addRow({"1", "5"});
	result_two.addRow({"5", "5"});
	result_two.addRow({"2", "6"});
	result_two.addRow({"6", "7"});
	result_three.addRow({"x", "50", "5", "aaa"});
	result_three.addRow({"y", "30", "1", "bbb"});
	result_three.addRow({"w", "60", "6", "ccc"});
	result_three.addRow({"s", "70", "7", "ddd"});
	result_four.addRow({"x", "5"});
	result_four.addRow({"y", "3"});
	vector<QP::QueryResult> results = {result_one, result_two, result_three, result_four};

	QP::QueryResult final_result = QP::QueryResult::joinResults(results);

	REQUIRE(final_result.getResult());
	REQUIRE(final_result.getNumberOfRows() == 1);
	REQUIRE(final_result.getTable().getRowWithOrder({"a", "b", "c", "d", "e"}, 0) == vector<string>({"5", "5", "x", "50", "aaa"}));
}

TEST_CASE("QP::QueryResult::joinResult multiple results, should return false") {
	QP::QueryResult result_one = QP::QueryResult(vector<string>({"a"}));
	QP::QueryResult result_two = QP::QueryResult(vector<string>({"a", "b"}));
	QP::QueryResult result_three = QP::QueryResult(vector<string>({"c", "d", "a", "e"}));
	QP::QueryResult result_four = QP::QueryResult(vector<string>({"c", "b"}));
	result_one.addRow({"5"});
	result_one.addRow({"6"});
	result_two.addRow({"1", "5"});
	result_two.addRow({"5", "5"});
	result_two.addRow({"2", "6"});
	result_two.addRow({"6", "7"});
	result_three.addRow({"x", "50", "9", "aaa"});
	result_three.addRow({"y", "30", "1", "bbb"});
	result_three.addRow({"w", "60", "6", "ccc"});
	result_three.addRow({"s", "70", "7", "ddd"});
	result_four.addRow({"x", "5"});
	result_four.addRow({"y", "3"});
	vector<QP::QueryResult> results = {result_one, result_two, result_three, result_four};

	QP::QueryResult final_result = QP::QueryResult::joinResults(results);

	REQUIRE(!final_result.getResult());
}

TEST_CASE("QP::QueryResult::filterBySelect empty select list should remove table") {
	QP::QueryResult result = QP::QueryResult(vector<string>({"a"}));
	result.addRow({"1"});
	result.addRow({"2"});

	result.filterBySelect({});

	REQUIRE(result.getResult());
	REQUIRE(result.getNumberOfRows() == 0);
}
