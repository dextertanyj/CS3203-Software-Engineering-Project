#include "QP/QueryFormatter.h"

#include "catch.hpp"

TEST_CASE("QP::QueryFormatter::formatResult No results") {
	QP::QueryProperties properties = QP::QueryProperties({}, {{QP::Types::DesignEntity::Stmt, "s"}}, {});
	QP::QueryResult result = QP::QueryResult();
	REQUIRE(QP::QueryFormatter::formatResult(properties, result).empty());
}

TEST_CASE("QP::QueryFormatter::formatResult List of statement number") {
	QP::QueryProperties properties = QP::QueryProperties({}, {{QP::Types::DesignEntity::Stmt, "s"}}, {});
	QP::QueryResult result = QP::QueryResult(vector<string>{"s"});
	result.addRow({"1"});
	result.addRow({"2"});
	result.addRow({"3"});

	vector<string> result_string = QP::QueryFormatter::formatResult(properties, result);

	sort(result_string.begin(), result_string.end());
	REQUIRE(result_string == vector<string>({"1", "2", "3"}));
}

TEST_CASE("QP::QueryFormatter::formatResult Boolean") {
	QP::QueryProperties properties = QP::QueryProperties({}, {}, {});
	QP::QueryResult result = QP::QueryResult(true);

	vector<string> result_string = QP::QueryFormatter::formatResult(properties, result);

	sort(result_string.begin(), result_string.end());
	REQUIRE(result_string == vector<string>({"TRUE"}));
}

TEST_CASE("QP::QueryFormatter::formatResult Tuple") {
	QP::Types::DeclarationList select_list = {
		{QP::Types::DesignEntity::Stmt, "s"},
		{QP::Types::DesignEntity::Stmt, "s1"},
	};
	QP::QueryProperties properties = QP::QueryProperties({}, select_list, {});
	QP::QueryResult result = QP::QueryResult(vector<string>{"s", "s1"});
	result.addRow({"1", "5"});
	result.addRow({"2", "6"});
	result.addRow({"3", "7"});

	vector<string> result_string = QP::QueryFormatter::formatResult(properties, result);

	sort(result_string.begin(), result_string.end());
	REQUIRE(result_string == vector<string>({"1 5", "2 6", "3 7"}));
}
