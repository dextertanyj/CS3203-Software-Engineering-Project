#include "PKB/CFG/Refactor/ControlFlowGraph.h"

#include "../TestUtilities.h"
#include "PKB/CFG/Refactor/DummyNode.h"
#include "PKB/CFG/Refactor/IfNode.h"
#include "PKB/CFG/Refactor/NodeInterface.h"
#include "PKB/CFG/Refactor/NonConditionalNode.h"
#include "PKB/CFG/Refactor/WhileNode.h"
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

TEST_CASE("PKB::ControlFlowGraph::setNext Test") {}

TEST_CASE("PKB::ControlFlowGraph::setIfNext Test") {}

TEST_CASE("PKB::ControlFlowGraph::setIfExit Test") {}

TEST_CASE("PKB::ControlFlowGraph::checkNext Test") {}

// TODO: When Next* has been implemented.
TEST_CASE("PKB::ControlFlowGraph::checkNextStar Test") {}

TEST_CASE("PKB::ControlFlowGraph::getNextNodes Test") {}

TEST_CASE("PKB::ControlFlowGraph::getPreviousNodes Test") {}

// TODO: When optimize has been implemented.
TEST_CASE("PKB::ControlFlowGraph::optimize Test") {}