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


TEST_CASE("PKB::NextManager::setNext Test") {
	PKB::ControlFlowGraph cfg = PKB::ControlFlowGraph();
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::IfStmt);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::WhileStmt);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);
	shared_ptr<StmtInfo> assign_stmt = TestUtilities::createStmtInfo(5, StmtType::Assign);

	cfg.createNode(if_stmt);
	cfg.createNode(while_stmt);
	cfg.createNode(print_stmt);
	cfg.createNode(read_stmt);
	cfg.createNode(assign_stmt);

	CHECK_THROWS(cfg.setNext(if_stmt->getIdentifier(), if_stmt->getIdentifier()));

	CHECK_NOTHROW(cfg.setNext(if_stmt->getIdentifier(), assign_stmt->getIdentifier()));
	CHECK_NOTHROW(cfg.setNext(while_stmt->getIdentifier(), print_stmt->getIdentifier()));
	CHECK_NOTHROW(cfg.setNext(print_stmt->getIdentifier(), while_stmt->getIdentifier()));
}

TEST_CASE("PKB::NextManager::setIfNext Test") {
	PKB::ControlFlowGraph cfg = PKB::ControlFlowGraph();
	shared_ptr<StmtInfo> if_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::IfStmt);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(2, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(3, StmtType::Read);
	shared_ptr<StmtInfo> if_stmt_2 = TestUtilities::createStmtInfo(4, StmtType::IfStmt);
	cfg.createNode(if_stmt_1);
	cfg.createNode(if_stmt_2);
	cfg.createNode(print_stmt);
	cfg.createNode(read_stmt);

	CHECK_THROWS(cfg.setIfNext(if_stmt_2->getIdentifier(), print_stmt->getIdentifier(), read_stmt->getIdentifier()));
	CHECK_THROWS(cfg.setIfNext(if_stmt_1->getIdentifier(), read_stmt->getIdentifier(), print_stmt->getIdentifier()));

	CHECK_NOTHROW(cfg.setIfNext(if_stmt_1->getIdentifier(), print_stmt->getIdentifier(), read_stmt->getIdentifier()));
}

TEST_CASE("PKB::NextManager::setIfExit Test") {
	PKB::ControlFlowGraph cfg = PKB::ControlFlowGraph();
	shared_ptr<StmtInfo> if_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::IfStmt);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(2, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(3, StmtType::Read);
	shared_ptr<StmtInfo> if_stmt_2 = TestUtilities::createStmtInfo(4, StmtType::IfStmt);
	cfg.createNode(if_stmt_1);
	cfg.createNode(if_stmt_2);
	cfg.createNode(print_stmt);
	cfg.createNode(read_stmt);

	CHECK_THROWS(cfg.setIfExit(print_stmt->getIdentifier(), read_stmt->getIdentifier(), if_stmt_2->getIdentifier()));
	CHECK_THROWS(cfg.setIfExit(read_stmt->getIdentifier(), print_stmt->getIdentifier(), if_stmt_1->getIdentifier()));

	CHECK_NOTHROW(cfg.setIfExit(print_stmt->getIdentifier(), read_stmt->getIdentifier(), if_stmt_1->getIdentifier()));
}

// TODO
// TEST_CASE("PKB::ControlFlowGraph::optimize Test") {}