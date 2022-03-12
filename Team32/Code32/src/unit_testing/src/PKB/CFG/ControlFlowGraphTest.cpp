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
	/* SIMPLE Code:
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

	CHECK(cfg.getNextNodes(if_stmt->getIdentifier()) == StmtRefSet{2, 4});
	CHECK(cfg.getNextNodes(while_stmt->getIdentifier()) == StmtRefSet{5, 3});
	CHECK(cfg.getNextNodes(print_stmt->getIdentifier()) == StmtRefSet{2});
	CHECK(cfg.getNextNodes(read_stmt->getIdentifier()) == StmtRefSet{5});
	CHECK(cfg.getNextNodes(assign_stmt->getIdentifier()).empty());
}

TEST_CASE("PKB::ControlFlowGraph::getPreviousNodes Test") {
	/* Example SIMPLE Code:
	 * 1. if (x==0) then {
	 * 2.   while (y<2) {
	 * 3.     print(y);
	 *      }
	 *    } else {
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
	CHECK(cfg.getPreviousNodes(assign_stmt->getIdentifier()) == StmtRefSet{2, 4});
}

TEST_CASE("PKB::ControlFlowGraph::checkNext Test") {
	/* SIMPLE Code:
	 * 1. if (x==0) then {
	 * 2.   while (y<2) {
	 * 3.     print(y);
	 *      }
	 *    } else {
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

TEST_CASE("PKB::ControlFlowGraph Overall Complicated CFG Stress Test") {
	/* SIMPLE Code 1: Many Nested Ifs.
	 * 1. if (x==0) then {
	 * 2.   if (y>0) then {
	 * 3.     print y;
	 *      } else {
	 * 4.     if (z<5) then {
	 * 5.       read z;
	 *        } else {
	 * 6.       x = y + 1;
	 *        }
	 *      }
	 * } else {
	 * 7. if (a==b) then {
	 * 8.   b = c - 1;
	 *    } else {
	 * 9.   a = b;
	 *    }
	 * }
	 * 10. call Monkey;
	 * */
	PKB::ControlFlowGraph cfg = PKB::ControlFlowGraph();
	shared_ptr<StmtInfo> if_stmt_1 = TestUtilities::createStmtInfo(1, StmtType::IfStmt);
	shared_ptr<StmtInfo> if_stmt_2 = TestUtilities::createStmtInfo(2, StmtType::IfStmt);
	shared_ptr<StmtInfo> print_stmt_3 = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> if_stmt_4 = TestUtilities::createStmtInfo(4, StmtType::IfStmt);
	shared_ptr<StmtInfo> read_stmt_5 = TestUtilities::createStmtInfo(5, StmtType::Read);
	shared_ptr<StmtInfo> assign_stmt_6 = TestUtilities::createStmtInfo(6, StmtType::Assign);
	shared_ptr<StmtInfo> if_stmt_7 = TestUtilities::createStmtInfo(7, StmtType::IfStmt);
	shared_ptr<StmtInfo> assign_stmt_8 = TestUtilities::createStmtInfo(8, StmtType::Assign);
	shared_ptr<StmtInfo> assign_stmt_9 = TestUtilities::createStmtInfo(9, StmtType::Assign);
	shared_ptr<StmtInfo> call_stmt_10 = TestUtilities::createStmtInfo(10, StmtType::Call);

	cfg.createNode(if_stmt_1);
	cfg.createNode(if_stmt_2);
	cfg.createNode(print_stmt_3);
	cfg.createNode(if_stmt_4);
	cfg.createNode(read_stmt_5);
	cfg.createNode(assign_stmt_6);
	cfg.createNode(if_stmt_7);
	cfg.createNode(assign_stmt_8);
	cfg.createNode(assign_stmt_9);
	cfg.createNode(call_stmt_10);

	cfg.setIfNext(if_stmt_1->getIdentifier(), if_stmt_2->getIdentifier(), if_stmt_7->getIdentifier());
	cfg.setIfNext(if_stmt_2->getIdentifier(), print_stmt_3->getIdentifier(), if_stmt_4->getIdentifier());
	cfg.setIfNext(if_stmt_4->getIdentifier(), read_stmt_5->getIdentifier(), assign_stmt_6->getIdentifier());
	cfg.setIfNext(if_stmt_7->getIdentifier(), assign_stmt_8->getIdentifier(), assign_stmt_9->getIdentifier());
	cfg.setIfExit(if_stmt_2->getIdentifier(), if_stmt_7->getIdentifier(), if_stmt_1->getIdentifier());
	cfg.setIfExit(print_stmt_3->getIdentifier(), if_stmt_4->getIdentifier(), if_stmt_2->getIdentifier());
	cfg.setIfExit(read_stmt_5->getIdentifier(), assign_stmt_6->getIdentifier(), if_stmt_4->getIdentifier());
	cfg.setIfExit(assign_stmt_8->getIdentifier(), assign_stmt_9->getIdentifier(), if_stmt_7->getIdentifier());
	cfg.setNext(if_stmt_1->getIdentifier(), call_stmt_10->getIdentifier());

	CHECK(cfg.getNextNodes(if_stmt_1->getIdentifier()) == StmtRefSet{2, 7});
	CHECK(cfg.getNextNodes(if_stmt_2->getIdentifier()) == StmtRefSet{3, 4});
	CHECK(cfg.getNextNodes(print_stmt_3->getIdentifier()) == StmtRefSet{10});
	CHECK(cfg.getNextNodes(if_stmt_4->getIdentifier()) == StmtRefSet{5, 6});
	CHECK(cfg.getNextNodes(read_stmt_5->getIdentifier()) == StmtRefSet{10});
	CHECK(cfg.getNextNodes(assign_stmt_6->getIdentifier()) == StmtRefSet{10});
	CHECK(cfg.getNextNodes(if_stmt_7->getIdentifier()) == StmtRefSet{8, 9});
	CHECK(cfg.getNextNodes(assign_stmt_8->getIdentifier()) == StmtRefSet{10});
	CHECK(cfg.getNextNodes(assign_stmt_9->getIdentifier()) == StmtRefSet{10});
	CHECK(cfg.getNextNodes(call_stmt_10->getIdentifier()).empty());

	CHECK(cfg.getPreviousNodes(if_stmt_1->getIdentifier()).empty());
	CHECK(cfg.getPreviousNodes(if_stmt_2->getIdentifier()) == StmtRefSet{1});
	CHECK(cfg.getPreviousNodes(print_stmt_3->getIdentifier()) == StmtRefSet{2});
	CHECK(cfg.getPreviousNodes(if_stmt_4->getIdentifier()) == StmtRefSet{2});
	CHECK(cfg.getPreviousNodes(read_stmt_5->getIdentifier()) == StmtRefSet{4});
	CHECK(cfg.getPreviousNodes(assign_stmt_6->getIdentifier()) == StmtRefSet{4});
	CHECK(cfg.getPreviousNodes(if_stmt_7->getIdentifier()) == StmtRefSet{1});
	CHECK(cfg.getPreviousNodes(assign_stmt_8->getIdentifier()) == StmtRefSet{7});
	CHECK(cfg.getPreviousNodes(assign_stmt_9->getIdentifier()) == StmtRefSet{7});
	CHECK(cfg.getPreviousNodes(call_stmt_10->getIdentifier()) == StmtRefSet{3, 5, 6, 8, 9});
}

// TODO: When Next* has been implemented.
// TEST_CASE("PKB::ControlFlowGraph::checkNextStar Test") {}

// TODO: When optimize has been implemented.
// TEST_CASE("PKB::ControlFlowGraph::optimize Test") {}