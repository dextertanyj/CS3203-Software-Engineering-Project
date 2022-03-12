#include "PKB/CFG/Refactor/NodeInterface.h"

#include "../TestUtilities.h"
#include "PKB/CFG/Refactor/DummyNode.h"
#include "PKB/CFG/Refactor/IfNode.h"
#include "PKB/CFG/Refactor/NonConditionalNode.h"
#include "PKB/CFG/Refactor/WhileNode.h"
#include "catch.hpp"

TEST_CASE("PKB::NodeInterface Constructor Test") {
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::IfStmt);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::WhileStmt);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);

	CHECK_NOTHROW(PKB::IfNode(if_stmt));
	CHECK_THROWS(PKB::IfNode(while_stmt));
	CHECK_THROWS(PKB::IfNode(print_stmt));

	CHECK_NOTHROW(PKB::WhileNode(while_stmt));
	CHECK_THROWS(PKB::WhileNode(if_stmt));
	CHECK_THROWS(PKB::WhileNode(print_stmt));

	CHECK_NOTHROW(PKB::NonConditionalNode(print_stmt));
	CHECK_THROWS(PKB::NonConditionalNode(if_stmt));
	CHECK_THROWS(PKB::NonConditionalNode(while_stmt));

	CHECK_NOTHROW(PKB::DummyNode(1));
	CHECK_NOTHROW(PKB::DummyNode(INT_MAX));
}

TEST_CASE("PKB::NodeInterface::insertNext Test") {
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::IfStmt);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::WhileStmt);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);

	shared_ptr<PKB::NodeInterface> if_node = make_shared<PKB::NodeInterface>(PKB::IfNode(if_stmt));
	shared_ptr<PKB::NodeInterface> while_node = make_shared<PKB::NodeInterface>(PKB::WhileNode(while_stmt));
	shared_ptr<PKB::NodeInterface> print_node = make_shared<PKB::NodeInterface>(PKB::NonConditionalNode(print_stmt));
	shared_ptr<PKB::NodeInterface> read_node = make_shared<PKB::NodeInterface>(PKB::NonConditionalNode(read_stmt));

	CHECK_NOTHROW(if_node->insertNext(while_node));
	CHECK_NOTHROW(if_node->insertNext(read_node));
	CHECK_NOTHROW(while_node->insertNext(print_node));
	CHECK_NOTHROW(print_node->insertNext(while_node));
}

TEST_CASE("PKB::NodeInterface::getNextNodes Test") {
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::IfStmt);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::WhileStmt);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);

	shared_ptr<PKB::NodeInterface> if_node = make_shared<PKB::NodeInterface>(PKB::IfNode(if_stmt));
	shared_ptr<PKB::NodeInterface> while_node = make_shared<PKB::NodeInterface>(PKB::WhileNode(while_stmt));
	shared_ptr<PKB::NodeInterface> print_node = make_shared<PKB::NodeInterface>(PKB::NonConditionalNode(print_stmt));
	shared_ptr<PKB::NodeInterface> read_node = make_shared<PKB::NodeInterface>(PKB::NonConditionalNode(read_stmt));

	if_node->insertNext(while_node);
	if_node->insertNext(read_node);
	while_node->insertNext(print_node);
	print_node->insertNext(while_node);

	REQUIRE(if_node->getNextNodes() == set({while_node, read_node}));
	REQUIRE(while_node->getNextNodes() == set({print_node}));
	REQUIRE(print_node->getNextNodes() == set({while_node}));
	REQUIRE(read_node->getNextNodes().empty());
}

TEST_CASE("PKB::NodeInterface::insertPrevious Test") {
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::IfStmt);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::WhileStmt);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);

	shared_ptr<PKB::NodeInterface> if_node = make_shared<PKB::NodeInterface>(PKB::IfNode(if_stmt));
	shared_ptr<PKB::NodeInterface> while_node = make_shared<PKB::NodeInterface>(PKB::WhileNode(while_stmt));
	shared_ptr<PKB::NodeInterface> print_node = make_shared<PKB::NodeInterface>(PKB::NonConditionalNode(print_stmt));
	shared_ptr<PKB::NodeInterface> read_node = make_shared<PKB::NodeInterface>(PKB::NonConditionalNode(read_stmt));

	CHECK_NOTHROW(if_node->insertNext(while_node));
	CHECK_NOTHROW(if_node->insertNext(read_node));
	CHECK_NOTHROW(while_node->insertNext(print_node));
	CHECK_NOTHROW(print_node->insertNext(while_node));
}

TEST_CASE("PKB::NodeInterface::getPreviousNodes Test") {
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::IfStmt);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::WhileStmt);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);

	shared_ptr<PKB::NodeInterface> if_node = make_shared<PKB::NodeInterface>(PKB::IfNode(if_stmt));
	shared_ptr<PKB::NodeInterface> while_node = make_shared<PKB::NodeInterface>(PKB::WhileNode(while_stmt));
	shared_ptr<PKB::NodeInterface> print_node = make_shared<PKB::NodeInterface>(PKB::NonConditionalNode(print_stmt));
	shared_ptr<PKB::NodeInterface> read_node = make_shared<PKB::NodeInterface>(PKB::NonConditionalNode(read_stmt));

	while_node->insertPrevious(if_node);
	read_node->insertPrevious(if_node);
	while_node->insertPrevious(print_node);
	print_node->insertPrevious(while_node);

	REQUIRE(if_node->getPreviousNodes().empty());
	REQUIRE(while_node->getPreviousNodes() == set({if_node, print_node}));
	REQUIRE(print_node->getPreviousNodes() == set({while_node}));
	REQUIRE(read_node->getPreviousNodes() == set({if_node}));
}