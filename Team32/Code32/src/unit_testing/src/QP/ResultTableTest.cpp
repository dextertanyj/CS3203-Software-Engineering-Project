#include "QP/ResultTable.h"

#include "catch.hpp"

using namespace std;
using namespace QP;

TEST_CASE("QP::ResulTable::joinResult Should join tables where one is a subset of another") {
	ResultTable table_one = ResultTable(vector<string>({"a"}));
	ResultTable table_two = ResultTable(vector<string>({"a", "b"}));
	table_one.insertRow({"3"});
	table_one.insertRow({"1"});
	table_two.insertRow({"1", "5"});
	table_two.insertRow({"2", "5"});
	table_two.insertRow({"2", "6"});
	table_two.insertRow({"3", "7"});

	ResultTable joined_table = ResultTable::joinTables(table_one, table_two);

	REQUIRE(joined_table.getNumberOfColumns() == 2);
	REQUIRE(joined_table.getNumberOfRows() == 2);
	REQUIRE(joined_table.containsRow({"1", "5"}));
	REQUIRE(joined_table.containsRow({"3", "7"}));
}

TEST_CASE("QP::QueryResult::joinResult Should join tables with exact same synonyms") {
	ResultTable table_one = ResultTable(vector<string>({"a", "b"}));
	ResultTable table_two = ResultTable(vector<string>({"a", "b"}));
	table_one.insertRow({"3", "7"});
	table_one.insertRow({"1", "1"});
	table_two.insertRow({"1", "5"});
	table_two.insertRow({"2", "5"});
	table_two.insertRow({"2", "6"});
	table_two.insertRow({"3", "7"});

	ResultTable joined_table = ResultTable::joinTables(table_one, table_two);

	REQUIRE(joined_table.containsRow({"3", "7"}));
}

TEST_CASE("QP::ResulTable::joinResult Should join tables with different synonyms") {
	ResultTable table_one = ResultTable(vector<string>({"a", "b"}));
	ResultTable table_two = ResultTable(vector<string>({"b", "c"}));

	table_one.insertRow({"3", "7"});
	table_one.insertRow({"1", "1"});
	table_one.insertRow({"2", "9"});
	table_one.insertRow({"4", "3"});
	table_two.insertRow({"1", "5"});
	table_two.insertRow({"2", "5"});
	table_two.insertRow({"2", "6"});
	table_two.insertRow({"3", "7"});
	table_two.insertRow({"3", "4"});
	table_two.insertRow({"1", "9"});

	ResultTable joined_table = ResultTable::joinTables(table_one, table_two);

	REQUIRE(joined_table.getNumberOfColumns() == 3);
	REQUIRE(joined_table.getNumberOfRows() == 4);
	REQUIRE(joined_table.getSynonymsStored() == vector<string>({"b", "c", "a"}));
	REQUIRE(joined_table.containsRow({"1", "5", "1"}));
	REQUIRE(joined_table.containsRow({"3", "7", "4"}));
	REQUIRE(joined_table.containsRow({"3", "4", "4"}));
	REQUIRE(joined_table.containsRow({"1", "9", "1"}));
}

TEST_CASE("QP::ResulTable::joinResult Should join tables without any common synonym") {
	ResultTable table_one = ResultTable(vector<string>({"a", "b"}));
	ResultTable table_two = ResultTable(vector<string>({"c", "d", "e"}));

	table_one.insertRow({"3", "7"});
	table_one.insertRow({"1", "1"});
	table_two.insertRow({"1", "5", "9"});
	table_two.insertRow({"2", "5", "8"});
	table_two.insertRow({"2", "6", "7"});

	ResultTable joined_table = ResultTable::joinTables(table_one, table_two);

	REQUIRE(joined_table.getNumberOfColumns() == 5);
	REQUIRE(joined_table.getNumberOfRows() == 6);
	REQUIRE(joined_table.containsRow({"1", "5", "9", "3", "7"}));
	REQUIRE(joined_table.containsRow({"1", "5", "9", "1", "1"}));
	REQUIRE(joined_table.containsRow({"2", "5", "8", "3", "7"}));
	REQUIRE(joined_table.containsRow({"2", "5", "8", "1", "1"}));
	REQUIRE(joined_table.containsRow({"2", "6", "7", "3", "7"}));
	REQUIRE(joined_table.containsRow({"2", "6", "7", "1", "1"}));
	REQUIRE(joined_table.getSynonymsStored() == vector<string>({"c", "d", "e", "a", "b"}));
	REQUIRE(joined_table.getSynonymsStoredMap().at("c") == 0);
	REQUIRE(joined_table.getSynonymsStoredMap().at("d") == 1);
	REQUIRE(joined_table.getSynonymsStoredMap().at("e") == 2);
	REQUIRE(joined_table.getSynonymsStoredMap().at("a") == 3);
	REQUIRE(joined_table.getSynonymsStoredMap().at("b") == 4);
}

TEST_CASE("QP::ResultTable::filterBySelect Should filter table") {
	QP::Types::DeclarationList select_list = {
		{QP::Types::DesignEntity::Stmt, "a"},
		{QP::Types::DesignEntity::Stmt, "c"},
	};
	ResultTable table = ResultTable({"a", "b", "c"});
	table.insertRow({"1", "a", "2"});
	table.insertRow({"3", "b", "4"});
	table.insertRow({"5", "c", "6"});
	table.insertRow({"1", "d", "2"});
	table.insertRow({"7", "e", "8"});

	ResultTable filtered_table = table.filterBySelect(select_list);

	REQUIRE(filtered_table.getNumberOfColumns() == 2);
	REQUIRE(filtered_table.getNumberOfRows() == 4);
	REQUIRE(filtered_table.containsRow({"1", "2"}));
	REQUIRE(filtered_table.containsRow({"3", "4"}));
	REQUIRE(filtered_table.containsRow({"5", "6"}));
	REQUIRE(filtered_table.containsRow({"7", "8"}));
}

TEST_CASE("QP::ResultTable::getRowWithOrder Should get row in speecific order") {
	ResultTable table = ResultTable({"a", "b", "c"});
	table.insertRow({"1", "a", "2"});
	table.insertRow({"3", "b", "4"});
	table.insertRow({"5", "c", "6"});
	table.insertRow({"1", "d", "2"});
	table.insertRow({"7", "e", "8"});

	Types::ResultRow row = table.getRowWithOrder({"c", "b"}, 2);

	REQUIRE(row == vector<string>({"6", "c"}));
}

TEST_CASE("QP::ResultTable::containsRow Should check if row exists") {
	ResultTable table = ResultTable({"a", "b", "c"});
	table.insertRow({"1", "a", "2"});

	REQUIRE(table.containsRow(vector<string>({"1", "a", "2"})));
	REQUIRE(!table.containsRow(vector<string>({"1", "b", "2"})));
}
