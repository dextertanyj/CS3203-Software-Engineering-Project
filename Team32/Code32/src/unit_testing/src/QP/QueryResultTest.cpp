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

	result_one.joinResult(result_two);

	REQUIRE(!result_one.getResult());
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

	result_one.joinResult(result_two);

	REQUIRE(result_one.getResult());
	REQUIRE(result_one.getNumberOfRows() == 3);
}
