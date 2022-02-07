#include "SP/Node/CallNode.h"
#include "SP/Node/ReadNode.h"

#include "catch.hpp"
#include "catch_tools.h"

using namespace std;

TEST_CASE("SP::Node::ReadNode::equals Same Object Test") {
    shared_ptr<ReadNode> node = make_shared<ReadNode>(1, make_unique<VariableNode>("a"));
    REQUIRE(node->equals(move(node)));
}

TEST_CASE("SP::Node::ReadNode::equals Same Node Test") {
    shared_ptr<ReadNode> node = make_shared<ReadNode>(1, make_unique<VariableNode>("a"));
    shared_ptr<ReadNode> other = make_shared<ReadNode>(1, make_unique<VariableNode>("a"));
    REQUIRE(node->equals(move(other)));
}

TEST_CASE("SP::Node::ReadNode::equals Different Node Test") {
    shared_ptr<ReadNode> node = make_shared<ReadNode>(1, make_unique<VariableNode>("a"));
    shared_ptr<ReadNode> other = make_shared<ReadNode>(1, make_unique<VariableNode>("b"));
    REQUIRE_FALSE(node->equals(move(other)));
    node = make_shared<ReadNode>(1, make_unique<VariableNode>("a"));
    other = make_shared<ReadNode>(2, make_unique<VariableNode>("a"));
    REQUIRE_FALSE(node->equals(move(other)));
    node = make_shared<ReadNode>(1, make_unique<VariableNode>("a"));
    other = make_shared<ReadNode>(2, make_unique<VariableNode>("b"));
    REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("SP::Node::ReadNode::equals Case-Sensitivity Test") {
    unique_ptr<ReadNode> node = make_unique<ReadNode>(1, make_unique<VariableNode>("A"));
    unique_ptr<ReadNode> other = make_unique<ReadNode>(1, make_unique<VariableNode>("a"));
    REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("SP::Node::ReadNode::equals Different Node Type Test") {
    unique_ptr<ReadNode> node = make_unique<ReadNode>(1, make_unique<VariableNode>("A"));
    shared_ptr<CallNode> other = make_shared<CallNode>(1, "test");
    REQUIRE_FALSE(node->equals(move(other)));
}