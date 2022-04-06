#include "PKB/CFG/NodeInterface.h"

#include "../TestUtilities.h"
#include "PKB/CFG/DummyNode.h"
#include "PKB/CFG/IfNode.h"
#include "PKB/CFG/StatementNode.h"
#include "PKB/CFG/WhileNode.h"
#include "PKB/PKB.h"
#include "PKB/Types.h"
#include "catch.hpp"
#include "climits"

TEST_CASE("PKB::NodeInterface Constructor Test") {
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::While);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);

	CHECK_NOTHROW(PKB::IfNode(if_stmt));
	CHECK_NOTHROW(PKB::WhileNode(while_stmt));
	CHECK_NOTHROW(PKB::StatementNode(print_stmt));

	CHECK_NOTHROW(PKB::DummyNode(1));
	CHECK_NOTHROW(PKB::DummyNode(INT_MAX));
}

TEST_CASE("PKB::NodeInterface::setNext Test") {
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::While);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);

	shared_ptr<PKB::IfNode> if_node = make_shared<PKB::IfNode>(PKB::IfNode(if_stmt));
	shared_ptr<PKB::NodeInterface> while_node = make_shared<PKB::WhileNode>(PKB::WhileNode(while_stmt));
	shared_ptr<PKB::NodeInterface> print_node = make_shared<PKB::StatementNode>(PKB::StatementNode(print_stmt));
	shared_ptr<PKB::NodeInterface> read_node = make_shared<PKB::StatementNode>(PKB::StatementNode(read_stmt));

	CHECK_NOTHROW(if_node->setNext(print_node));
	CHECK_NOTHROW(print_node->setNext(while_node));
}

TEST_CASE("PKB::IfNode::insertIfNext Test") {
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::While);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);

	shared_ptr<PKB::IfNode> if_node = make_shared<PKB::IfNode>(PKB::IfNode(if_stmt));
	shared_ptr<PKB::StatementNode> while_node = make_shared<PKB::WhileNode>(PKB::WhileNode(while_stmt));
	shared_ptr<PKB::StatementNode> read_node = make_shared<PKB::StatementNode>(PKB::StatementNode(read_stmt));

	CHECK_NOTHROW(if_node->insertIfNext(while_node, read_node));
}

TEST_CASE("PKB::IfNode::insertIfExit Test") {
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::While);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);

	shared_ptr<PKB::IfNode> if_node = make_shared<PKB::IfNode>(PKB::IfNode(if_stmt));
	shared_ptr<PKB::StatementNode> while_node = make_shared<PKB::WhileNode>(PKB::WhileNode(while_stmt));
	shared_ptr<PKB::StatementNode> read_node = make_shared<PKB::StatementNode>(PKB::StatementNode(read_stmt));

	CHECK_NOTHROW(if_node->insertIfExit(while_node, read_node));
}

TEST_CASE("PKB::NodeInterface::getNextNodes Test") {
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::While);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);

	shared_ptr<PKB::IfNode> if_node = make_shared<PKB::IfNode>(PKB::IfNode(if_stmt));
	shared_ptr<PKB::StatementNode> while_node = make_shared<PKB::WhileNode>(PKB::WhileNode(while_stmt));
	shared_ptr<PKB::StatementNode> print_node = make_shared<PKB::StatementNode>(PKB::StatementNode(print_stmt));
	shared_ptr<PKB::StatementNode> read_node = make_shared<PKB::StatementNode>(PKB::StatementNode(read_stmt));

	if_node->insertIfNext(while_node, read_node);
	while_node->setNext(print_node);
	print_node->setNext(while_node);

	REQUIRE(if_node->getNextNodes() == unordered_set<shared_ptr<PKB::NodeInterface>>({while_node, read_node}));
	REQUIRE(while_node->getNextNodes() == unordered_set<shared_ptr<PKB::NodeInterface>>({print_node}));
	REQUIRE(print_node->getNextNodes() == unordered_set<shared_ptr<PKB::NodeInterface>>({while_node}));
	REQUIRE(read_node->getNextNodes().empty());
}

TEST_CASE("PKB::NodeInterface::setPrevious Test") {
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::While);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);

	shared_ptr<PKB::NodeInterface> if_node = make_shared<PKB::IfNode>(PKB::IfNode(if_stmt));
	shared_ptr<PKB::NodeInterface> while_node = make_shared<PKB::WhileNode>(PKB::WhileNode(while_stmt));
	shared_ptr<PKB::NodeInterface> print_node = make_shared<PKB::StatementNode>(PKB::StatementNode(print_stmt));
	shared_ptr<PKB::NodeInterface> read_node = make_shared<PKB::StatementNode>(PKB::StatementNode(read_stmt));

	CHECK_NOTHROW(while_node->setPrevious(if_node));
	CHECK_NOTHROW(read_node->setPrevious(if_node));
	CHECK_NOTHROW(while_node->setPrevious(print_node));
	CHECK_NOTHROW(print_node->setPrevious(while_node));
}

TEST_CASE("PKB::NodeInterface::getPreviousNodes Test") {
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::While);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);

	shared_ptr<PKB::NodeInterface> if_node = make_shared<PKB::IfNode>(PKB::IfNode(if_stmt));
	shared_ptr<PKB::NodeInterface> while_node = make_shared<PKB::WhileNode>(PKB::WhileNode(while_stmt));
	shared_ptr<PKB::NodeInterface> print_node = make_shared<PKB::StatementNode>(PKB::StatementNode(print_stmt));
	shared_ptr<PKB::NodeInterface> read_node = make_shared<PKB::StatementNode>(PKB::StatementNode(read_stmt));

	while_node->setPrevious(if_node);
	read_node->setPrevious(if_node);
	while_node->setPrevious(print_node);
	print_node->setPrevious(while_node);

	REQUIRE(if_node->getPreviousNodes().empty());
	REQUIRE(while_node->getPreviousNodes() == unordered_set({if_node, print_node}));
	REQUIRE(print_node->getPreviousNodes() == unordered_set({while_node}));
	REQUIRE(read_node->getPreviousNodes() == unordered_set({if_node}));
}

TEST_CASE("PKB::NodeInterface::setGraphIndex Test") {
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<PKB::NodeInterface> if_node = make_shared<PKB::IfNode>(PKB::IfNode(if_stmt));
	CHECK_NOTHROW(if_node->setGraphIndex(5));
}

TEST_CASE("PKB::NodeInterface::getGraphIndex Test") {
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<PKB::NodeInterface> if_node = make_shared<PKB::IfNode>(PKB::IfNode(if_stmt));
	REQUIRE(if_node->getGraphIndex() == 0);
	if_node->setGraphIndex(5);
	REQUIRE(if_node->getGraphIndex() == 5);
}

TEST_CASE("PKB::NodeInterface::getNodeRef Test") {
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::While);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);

	shared_ptr<PKB::NodeInterface> if_node = make_shared<PKB::IfNode>(PKB::IfNode(if_stmt));
	shared_ptr<PKB::NodeInterface> while_node = make_shared<PKB::WhileNode>(PKB::WhileNode(while_stmt));
	shared_ptr<PKB::NodeInterface> print_node = make_shared<PKB::StatementNode>(PKB::StatementNode(print_stmt));
	shared_ptr<PKB::NodeInterface> read_node = make_shared<PKB::StatementNode>(PKB::StatementNode(read_stmt));
	shared_ptr<PKB::NodeInterface> dummy_node = make_shared<PKB::DummyNode>(PKB::DummyNode(1));

	REQUIRE(if_node->getNodeRef() == if_stmt->getIdentifier());
	REQUIRE(while_node->getNodeRef() == while_stmt->getIdentifier());
	REQUIRE(print_node->getNodeRef() == print_stmt->getIdentifier());
	REQUIRE(read_node->getNodeRef() == read_stmt->getIdentifier());
	REQUIRE(dummy_node->getNodeRef() == 1);
}

TEST_CASE("PKB::NodeInterface::getNodeType Test") {
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::While);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);

	shared_ptr<PKB::NodeInterface> if_node = make_shared<PKB::IfNode>(PKB::IfNode(if_stmt));
	shared_ptr<PKB::NodeInterface> while_node = make_shared<PKB::WhileNode>(PKB::WhileNode(while_stmt));
	shared_ptr<PKB::NodeInterface> print_node = make_shared<PKB::StatementNode>(PKB::StatementNode(print_stmt));
	shared_ptr<PKB::NodeInterface> read_node = make_shared<PKB::StatementNode>(PKB::StatementNode(read_stmt));

	REQUIRE(if_node->getNodeType() == PKB::Types::NodeType::If);
	REQUIRE(while_node->getNodeType() == PKB::Types::NodeType::While);
	REQUIRE(print_node->getNodeType() == PKB::Types::NodeType::Statement);
	REQUIRE(read_node->getNodeType() == PKB::Types::NodeType::Statement);
}

TEST_CASE("PKB::NodeInterface Comparator Test") {
	shared_ptr<StmtInfo> if_stmt = TestUtilities::createStmtInfo(1, StmtType::If);
	shared_ptr<StmtInfo> while_stmt = TestUtilities::createStmtInfo(2, StmtType::While);
	shared_ptr<StmtInfo> print_stmt = TestUtilities::createStmtInfo(3, StmtType::Print);
	shared_ptr<StmtInfo> read_stmt = TestUtilities::createStmtInfo(4, StmtType::Read);

	shared_ptr<PKB::NodeInterface> if_node = make_shared<PKB::IfNode>(PKB::IfNode(if_stmt));
	shared_ptr<PKB::NodeInterface> while_node = make_shared<PKB::WhileNode>(PKB::WhileNode(while_stmt));
	shared_ptr<PKB::NodeInterface> print_node = make_shared<PKB::StatementNode>(PKB::StatementNode(print_stmt));
	shared_ptr<PKB::NodeInterface> read_node = make_shared<PKB::StatementNode>(PKB::StatementNode(read_stmt));

	auto comparator = [](const shared_ptr<PKB::NodeInterface>& n1, const shared_ptr<PKB::NodeInterface>& n2) {
		return n1->getNodeRef() < n2->getNodeRef();
	};
	priority_queue<shared_ptr<PKB::NodeInterface>, vector<shared_ptr<PKB::NodeInterface>>, decltype(comparator)> pq(comparator);
	pq.push(if_node);
	pq.push(while_node);
	pq.push(print_node);
	pq.push(read_node);
	REQUIRE(pq.top() == read_node);
	pq.pop();
	REQUIRE(pq.top() == print_node);
	pq.pop();
	REQUIRE(pq.top() == while_node);
	pq.pop();
	REQUIRE(pq.top() == if_node);
}
