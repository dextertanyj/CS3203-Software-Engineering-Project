#include "SP/Node/CallNode.h"
#include "SP/Node/PrintNode.h"

#include "catch.hpp"
#include "catch_tools.h"

using namespace std;

TEST_CASE("SP::Node::PrintNode::equals Same Object Test") {
    shared_ptr<PrintNode> node = make_shared<PrintNode>(1, make_unique<VariableNode>("a"));
    REQUIRE(node->equals(move(node)));
}

TEST_CASE("SP::Node::PrintNode::equals Same Node Test") {
    shared_ptr<PrintNode> node = make_shared<PrintNode>(1, make_unique<VariableNode>("a"));
    shared_ptr<PrintNode> other = make_shared<PrintNode>(1, make_unique<VariableNode>("a"));
    REQUIRE(node->equals(move(other)));
}

TEST_CASE("SP::Node::PrintNode::equals Different Node Test") {
    shared_ptr<PrintNode> node = make_shared<PrintNode>(1, make_unique<VariableNode>("a"));
    shared_ptr<PrintNode> other = make_shared<PrintNode>(1, make_unique<VariableNode>("b"));
    REQUIRE_FALSE(node->equals(move(other)));
    node = make_shared<PrintNode>(1, make_unique<VariableNode>("a"));
    other = make_shared<PrintNode>(2, make_unique<VariableNode>("a"));
    REQUIRE_FALSE(node->equals(move(other)));
    node = make_shared<PrintNode>(1, make_unique<VariableNode>("a"));
    other = make_shared<PrintNode>(2, make_unique<VariableNode>("b"));
    REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("SP::Node::PrintNode::equals Case-Sensitivity Test") {
    unique_ptr<PrintNode> node = make_unique<PrintNode>(1, make_unique<VariableNode>("A"));
    unique_ptr<PrintNode> other = make_unique<PrintNode>(1, make_unique<VariableNode>("a"));
    REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("SP::Node::PrintNode::equals Different Node Type Test") {
    shared_ptr<PrintNode> node = make_shared<PrintNode>(1, make_unique<VariableNode>("a"));
    shared_ptr<CallNode> other = make_shared<CallNode>(1, "test");
    REQUIRE_FALSE(node->equals(move(other)));
}