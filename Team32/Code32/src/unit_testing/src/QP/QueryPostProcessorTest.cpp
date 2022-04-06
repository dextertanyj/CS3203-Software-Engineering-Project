#include "QP/QueryPostProcessor.h"

#include "PKB/Storage.h"
#include "QP/ClauseArgument.h"
#include "catch.hpp"

using namespace QP;
using namespace Types;

TEST_CASE("QueryPostProcessor::processResult No results") {
	PKB::Storage pkb;
	StorageAdapter store(pkb);
	QueryPostProcessor post_processor(store);

	QueryProperties properties = QueryProperties({}, {ClauseArgument{{DesignEntity::Stmt, "s"}}}, {});
	QueryResult result = QueryResult();
	REQUIRE(post_processor.processResult(properties, result).empty());
}

TEST_CASE("QueryPostProcessor::processResult List of statement number") {
	PKB::Storage pkb;
	StorageAdapter store(pkb);
	QueryPostProcessor post_processor(store);

	QueryProperties properties = QueryProperties({}, {ClauseArgument{{DesignEntity::Stmt, "s"}}}, {});
	QueryResult result = QueryResult(vector<string>{"s"});
	result.addRow({"1"});
	result.addRow({"2"});
	result.addRow({"3"});

	vector<string> result_string = post_processor.processResult(properties, result);

	sort(result_string.begin(), result_string.end());
	REQUIRE(result_string == vector<string>({"1", "2", "3"}));
}

TEST_CASE("QueryPostProcessor::processResult Boolean") {
	PKB::Storage pkb;
	StorageAdapter store(pkb);
	QueryPostProcessor post_processor(store);

	QueryProperties properties = QueryProperties({}, {}, {});
	QueryResult result = QueryResult(true);

	vector<string> result_string = post_processor.processResult(properties, result);

	sort(result_string.begin(), result_string.end());
	REQUIRE(result_string == vector<string>({"TRUE"}));
}

TEST_CASE("QueryPostProcessor::processResult Tuple") {
	PKB::Storage pkb;
	StorageAdapter store(pkb);
	QueryPostProcessor post_processor(store);

	SelectList select_list = {
		ClauseArgument{{DesignEntity::Stmt, "s"}},
		ClauseArgument{{DesignEntity::Stmt, "s1"}},
	};
	QueryProperties properties = QueryProperties({}, select_list, {});
	QueryResult result = QueryResult(vector<string>{"s", "s1"});
	result.addRow({"1", "5"});
	result.addRow({"2", "6"});
	result.addRow({"3", "7"});

	vector<string> result_string = post_processor.processResult(properties, result);

	sort(result_string.begin(), result_string.end());
	REQUIRE(result_string == vector<string>({"1 5", "2 6", "3 7"}));
}

TEST_CASE("QueryPostProcessor::processResult Tuple repeated") {
	PKB::Storage pkb;
	StorageAdapter store(pkb);
	QueryPostProcessor post_processor(store);

	SelectList select_list = {
		ClauseArgument{{DesignEntity::Stmt, "s"}},
		ClauseArgument{{DesignEntity::Stmt, "s"}},
	};
	QueryProperties properties = QueryProperties({}, select_list, {});
	QueryResult result = QueryResult(vector<string>{"s", "s1"});
	result.addRow({"1", "5"});
	result.addRow({"2", "6"});
	result.addRow({"3", "7"});

	vector<string> result_string = post_processor.processResult(properties, result);

	sort(result_string.begin(), result_string.end());
	REQUIRE(result_string == vector<string>({"1 1", "2 2", "3 3"}));
}

TEST_CASE("QueryPostProcessor::processResult Trivial Attribute") {
	PKB::Storage pkb;
	StorageAdapter store(pkb);
	QueryPostProcessor post_processor(store);

	QueryProperties properties = QueryProperties({}, {ClauseArgument{{AttributeType::NumberIdentifier, {DesignEntity::Stmt, "s"}}}}, {});
	QueryResult result = QueryResult(vector<string>{"s"});
	result.addRow({"1"});
	result.addRow({"2"});
	result.addRow({"3"});

	vector<string> result_string = post_processor.processResult(properties, result);

	sort(result_string.begin(), result_string.end());
	REQUIRE(result_string == vector<string>({"1", "2", "3"}));
}

TEST_CASE("QueryPostProcessor::processResult Attribute") {
	PKB::Storage pkb;
	pkb.setStmtType(1, StmtType::Print);
	pkb.setStmtType(2, StmtType::Print);
	pkb.setStmtType(3, StmtType::Print);
	pkb.setUses(1, "x");
	pkb.setUses(2, "y");
	pkb.setUses(3, "z");
	StorageAdapter store(pkb);
	QueryPostProcessor post_processor(store);

	QueryProperties properties = QueryProperties({}, {ClauseArgument{{AttributeType::VariableName, {DesignEntity::Print, "p"}}}}, {});
	QueryResult result = QueryResult(vector<string>{"p"});
	result.addRow({"1"});
	result.addRow({"2"});
	result.addRow({"3"});

	vector<string> result_string = post_processor.processResult(properties, result);

	sort(result_string.begin(), result_string.end());
	REQUIRE(result_string == vector<string>({"x", "y", "z"}));
}
