#include "QP/ResultTable.h"

#include "catch.hpp"

using QP::Types::ResultTable;

TEST_CASE("QP::ResulTable::joinResult Should join tables where one is a subset of another") {
	ResultTable table_one;
	ResultTable table_two;
	table_one.insertColumn("a", {"3", "1"});
	table_two.insertColumn("a", {"1", "2", "2", "3"});
	table_two.insertColumn("b", {"5", "5", "6", "7"});

	ResultTable joined_table = ResultTable::joinTables({table_one, table_two});

	REQUIRE(joined_table.getColumn("a") == vector<string>({"1", "3"}));
	REQUIRE(joined_table.getColumn("b") == vector<string>({"5", "7"}));
}

TEST_CASE("QP::QueryResult::joinResult Should join tables with exact same synonyms") {
	ResultTable table_one;
	ResultTable table_two;
	table_one.insertColumn("a", {"3", "1"});
	table_one.insertColumn("b", {"7", "1"});
	table_two.insertColumn("a", {"1", "2", "2", "3"});
	table_two.insertColumn("b", {"5", "5", "6", "7"});

	ResultTable joined_table = ResultTable::joinTables({table_one, table_two});

	REQUIRE(joined_table.getColumn("a") == vector<string>({"3"}));
	REQUIRE(joined_table.getColumn("b") == vector<string>({"7"}));
}

TEST_CASE("QP::ResulTable::joinResult Should join tables with different synonyms") {
	ResultTable table_one;
	ResultTable table_two;
	table_one.insertColumn("a", {"3", "1", "2", "4"});
	table_one.insertColumn("b", {"7", "1", "9", "3"});
	table_two.insertColumn("b", {"1", "2", "2", "3", "3", "1"});
	table_two.insertColumn("c", {"5", "5", "6", "7", "4", "9"});

	ResultTable joined_table = ResultTable::joinTables({table_one, table_two});

	REQUIRE(joined_table.getColumn("a") == vector<string>({"1", "1", "4", "4"}));
	REQUIRE(joined_table.getColumn("b") == vector<string>({"1", "1", "3", "3"}));
	REQUIRE(joined_table.getColumn("c") == vector<string>({"5", "9", "7", "4"}));
}

TEST_CASE("QP::ResulTable::joinResult Should join tables without any common synonym") {
	ResultTable table_one;
	ResultTable table_two;
	table_one.insertColumn("a", {"3", "1"});
	table_one.insertColumn("b", {"7", "1"});
	table_two.insertColumn("c", {"1", "2", "2"});
	table_two.insertColumn("d", {"5", "5", "6"});
	table_two.insertColumn("e", {"9", "8", "7"});

	ResultTable joined_table = ResultTable::joinTables({table_one, table_two});

	REQUIRE(joined_table.getColumn("a") == vector<string>({"3", "1", "3", "1", "3", "1"}));
	REQUIRE(joined_table.getColumn("b") == vector<string>({"7", "1", "7", "1", "7", "1"}));
	REQUIRE(joined_table.getColumn("c") == vector<string>({"1", "1", "2", "2", "2", "2"}));
	REQUIRE(joined_table.getColumn("d") == vector<string>({"5", "5", "5", "5", "6", "6"}));
	REQUIRE(joined_table.getColumn("e") == vector<string>({"9", "9", "8", "8", "7", "7"}));
	REQUIRE(joined_table.getNumberOfColumns() == 5);
}

TEST_CASE("QP::ResultTable::filterBySelect Should filter table") {
	QP::Types::DeclarationList select_list = {
		{QP::Types::DesignEntity::Stmt, "a"},
		{QP::Types::DesignEntity::Stmt, "c"},
	};
	ResultTable table;
	table.insertColumn("a", {"1", "3", "5", "1", "7"});
	table.insertColumn("b", {"a", "b", "c", "d", "e"});
	table.insertColumn("c", {"2", "4", "6", "2", "8"});

	table.filterBySelect(select_list);

	REQUIRE(table.getColumn("a") == vector<string>({"1", "3", "5", "7"}));
	REQUIRE(table.getColumn("c") == vector<string>({"2", "4", "6", "8"}));
	REQUIRE(table.getNumberOfColumns() == 2);
}
