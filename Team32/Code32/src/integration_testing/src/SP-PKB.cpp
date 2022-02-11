#include "catch_tools.h"

#include "PKB/PKB.h"
#include "SP/Processor.h"

#include <string>

using namespace std;

TEST_CASE("SP::Processor::process Basic Test") {
	PKB pkb;
	SP::Processor source_processor = SP::Processor(pkb);
	string source = "procedure main { print x; }";
	source_processor.process(source);
	StmtInfoList result = pkb.getStatements();
	REQUIRE_EQUALS(result.size(), 1);
}

TEST_CASE("SP::Processor::process Two Line Test") {
	PKB pkb;
	SP::Processor source_processor = SP::Processor(pkb);
	string source = "procedure main { print x; read y; }";
	source_processor.process(source);
	StmtInfoList result = pkb.getStatements();
	REQUIRE_EQUALS(result.size(), 2);
}

TEST_CASE("SP::Processor::process Basic If Test") {
	PKB pkb;
	SP::Processor source_processor = SP::Processor(pkb);
	string source = "procedure main { if (x < 3) then { read x; } else { read y; x = y + 1; print x; } }";
	source_processor.process(source);
	REQUIRE_EQUALS(pkb.getStatements().size(), 5);
	REQUIRE_EQUALS(pkb.getParent(1)->reference, -1);
	REQUIRE_EQUALS(pkb.getParent(2)->reference, 1);
	REQUIRE_EQUALS(pkb.getParent(3)->reference, 1);
	REQUIRE_EQUALS(pkb.getParent(4)->reference, 1);
	REQUIRE_EQUALS(pkb.getParent(5)->reference, 1);
	std::unordered_set<shared_ptr<StmtInfo>> expected_set = {};
	for (shared_ptr<StmtInfo> ptr : pkb.getStatements()){
		if (ptr->reference != 1) expected_set.insert(ptr);
	}
	REQUIRE_EQUALS(pkb.getChildren(1), expected_set);
}