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
	StmtRefList result = pkb.getStatements();
	REQUIRE_EQUALS(result.size(), 1);
}

TEST_CASE("SP::Processor::process Two Line Test") {
	PKB pkb;
	SP::Processor source_processor = SP::Processor(pkb);
	string source = "procedure main { print x; read y; }";
	source_processor.process(source);
	StmtRefList result = pkb.getStatements();
	REQUIRE_EQUALS(result.size(), 2);
}

TEST_CASE("SP::Processor::process Basic If Test") {
	PKB pkb;
	SP::Processor source_processor = SP::Processor(pkb);
	string source = "procedure main { if (x < 3) then { read x; } else { read y; x = y + 1; print x; } }";
	source_processor.process(source);
	REQUIRE_EQUALS(pkb.getStatements().size(), 5);
	REQUIRE_EQUALS(pkb.getParent(1), -1);
	REQUIRE_EQUALS(pkb.getParent(2), 1);
	REQUIRE_EQUALS(pkb.getParent(3), 1);
	REQUIRE_EQUALS(pkb.getParent(4), 1);
	REQUIRE_EQUALS(pkb.getParent(5), 1);
	REQUIRE_EQUALS(pkb.getChildren(1), std::unordered_set<StmtRef>({2, 3, 4, 5}));
}