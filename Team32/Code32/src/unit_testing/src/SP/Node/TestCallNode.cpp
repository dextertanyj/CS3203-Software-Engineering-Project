#include "SP/Node/CallNode.h"

#include "catch.hpp"
#include "catch_tools.h"

using namespace std;

TEST_CASE("SP::Node::CallNode::equals Same Object Test") {
    shared_ptr<CallNode> call_node = make_shared<CallNode>(1, "test");
    REQUIRE(call_node->equals(move(call_node)));
}

TEST_CASE("SP::Node::CallNode::equals Same Node Test") {
    shared_ptr<CallNode> call_node = make_shared<CallNode>(1, "test");
    shared_ptr<CallNode> other = make_shared<CallNode>(1, "test");
    REQUIRE(call_node->equals(move(other)));
}

TEST_CASE("SP::Node::CallNode::equals Different Node Test") {
    shared_ptr<CallNode> call_node = make_shared<CallNode>(1, "test");
    shared_ptr<CallNode> other = make_shared<CallNode>(1, "test2");
    REQUIRE_FALSE(call_node->equals(move(other)));
    call_node = make_shared<CallNode>(1, "test");
    other = make_shared<CallNode>(2, "test");
    REQUIRE_FALSE(call_node->equals(move(other)));
    call_node = make_shared<CallNode>(1, "test");
    other = make_shared<CallNode>(2, "test2");
    REQUIRE_FALSE(call_node->equals(move(other)));
}

TEST_CASE("SP::Node::CallNode::equals Case-Sensitivity Test") {
    unique_ptr<CallNode> call_node = make_unique<CallNode>(1, "A");
    unique_ptr<CallNode> other = make_unique<CallNode>(1, "a");
    REQUIRE_FALSE(call_node->equals(move(other)));
}

