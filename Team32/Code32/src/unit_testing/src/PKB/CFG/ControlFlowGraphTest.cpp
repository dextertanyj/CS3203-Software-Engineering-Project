#include "PKB/CFG/ControlFlowGraph.h"

#include "../TestUtilities.h"
#include "PKB/CFG/NodeInterface.h"
#include "catch.hpp"

TEST_CASE("PKB::ControlFlowGraph::createNode Test") {
	PKB::ControlFlowGraph cfg = PKB::ControlFlowGraph();
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::IfStmt);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::WhileStmt);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	CHECK_NOTHROW(cfg.createNode(if_stmt));
	CHECK_THROWS(cfg.createNode(if_stmt));
	CHECK_NOTHROW(cfg.createNode(while_stmt));
	CHECK_THROWS(cfg.createNode(while_stmt));
	CHECK_NOTHROW(cfg.createNode(print_stmt));
	CHECK_THROWS(cfg.createNode(print_stmt));
}

// TODO
// TEST_CASE("PKB::ControlFlowGraph::optimize Test") {}