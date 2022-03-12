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

TEST_CASE("PKB::ControlFlowGraph::setNext Test") {
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

TEST_CASE("PKB::ControlFlowGraph::setIfNext Test") {
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

TEST_CASE("PKB::ControlFlowGraph::setIfExit Test") {
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

TEST_CASE("PKB::ControlFlowGraph::getNextNodes Test") {
	/* Example SIMPLE Code:
	 * 1. if (x==0) {
	 * 2.   while (y<2) {
	 * 3.     print(y);
	 *      }
	 *    } then else {
	 * 4.   read x;
	 *    }
	 * 5. z = 5;
	 * */
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

	cfg.setIfNext(if_stmt->getIdentifier(), while_stmt->getIdentifier(), read_stmt->getIdentifier());
	cfg.setIfExit(while_stmt->getIdentifier(), read_stmt->getIdentifier(), if_stmt->getIdentifier());
	cfg.setNext(while_stmt->getIdentifier(), print_stmt->getIdentifier());
	cfg.setNext(print_stmt->getIdentifier(), while_stmt->getIdentifier());
	cfg.setNext(if_stmt->getIdentifier(), assign_stmt->getIdentifier());

	CHECK(cfg.getNextNodes(if_stmt->getIdentifier()) == StmtRefSet{5, 2, 4});
	CHECK(cfg.getNextNodes(while_stmt->getIdentifier()) == StmtRefSet{5, 3});
	CHECK(cfg.getNextNodes(print_stmt->getIdentifier()) == StmtRefSet{2});
	CHECK(cfg.getNextNodes(read_stmt->getIdentifier()) == StmtRefSet{5});
	CHECK(cfg.getNextNodes(assign_stmt->getIdentifier()).empty());
}

TEST_CASE("PKB::ControlFlowGraph::getPreviousNodes Test") {
	/* Example SIMPLE Code:
	 * 1. if (x==0) {
	 * 2.   while (y<2) {
	 * 3.     print(y);
	 *      }
	 *    } then else {
	 * 4.   read x;
	 *    }
	 * 5. z = 5;
	 * */
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

	cfg.setIfNext(if_stmt->getIdentifier(), while_stmt->getIdentifier(), read_stmt->getIdentifier());
	cfg.setIfExit(while_stmt->getIdentifier(), read_stmt->getIdentifier(), if_stmt->getIdentifier());
	cfg.setNext(while_stmt->getIdentifier(), print_stmt->getIdentifier());
	cfg.setNext(print_stmt->getIdentifier(), while_stmt->getIdentifier());
	cfg.setNext(if_stmt->getIdentifier(), assign_stmt->getIdentifier());

	CHECK(cfg.getPreviousNodes(if_stmt->getIdentifier()).empty());
	CHECK(cfg.getPreviousNodes(while_stmt->getIdentifier()) == StmtRefSet{1, 3});
	CHECK(cfg.getPreviousNodes(print_stmt->getIdentifier()) == StmtRefSet{2});
	CHECK(cfg.getPreviousNodes(read_stmt->getIdentifier()) == StmtRefSet{1});
	CHECK(cfg.getPreviousNodes(assign_stmt->getIdentifier()) == StmtRefSet{1, 2, 4});
}

TEST_CASE("PKB::ControlFlowGraph::checkNext Test") {
	/* Example SIMPLE Code:
	 * 1. if (x==0) {
	 * 2.   while (y<2) {
	 * 3.     print(y);
	 *      }
	 *    } then else {
	 * 4.   read x;
	 *    }
	 * 5. z = 5;
	 * */
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

	cfg.setIfNext(if_stmt->getIdentifier(), while_stmt->getIdentifier(), read_stmt->getIdentifier());
	cfg.setIfExit(while_stmt->getIdentifier(), read_stmt->getIdentifier(), if_stmt->getIdentifier());
	cfg.setNext(while_stmt->getIdentifier(), print_stmt->getIdentifier());
	cfg.setNext(print_stmt->getIdentifier(), while_stmt->getIdentifier());
	cfg.setNext(if_stmt->getIdentifier(), assign_stmt->getIdentifier());

	CHECK(cfg.checkNext(1, 2));
	CHECK(cfg.checkNext(1, 4));
	CHECK(cfg.checkNext(2, 3));
	CHECK(cfg.checkNext(3, 2));
	CHECK(cfg.checkNext(2, 5));
	CHECK(cfg.checkNext(4, 5));
}

// TODO: When Next* has been implemented.
// TEST_CASE("PKB::ControlFlowGraph::checkNextStar Test") {}

// TODO: When optimize has been implemented.
// TEST_CASE("PKB::ControlFlowGraph::optimize Test") {}