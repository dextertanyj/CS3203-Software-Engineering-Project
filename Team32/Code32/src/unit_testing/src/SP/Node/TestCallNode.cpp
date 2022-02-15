#include "SP/Node/CallNode.h"
#include "SP/Node/PrintNode.h"

#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::CallNode::equals Same Object Test") {
    shared_ptr<CallNode> node = make_shared<CallNode>(1, "test");
    REQUIRE(node->equals(node));
}

TEST_CASE("SP::Node::CallNode::equals Same Node Test") {
    shared_ptr<CallNode> node = make_shared<CallNode>(1, "test");
    shared_ptr<CallNode> other = make_shared<CallNode>(1, "test");
    REQUIRE(node->equals(move(other)));
}

TEST_CASE("SP::Node::CallNode::equals Different Node Test") {
    shared_ptr<CallNode> node = make_shared<CallNode>(1, "test");
    shared_ptr<CallNode> other = make_shared<CallNode>(1, "test2");
    REQUIRE_FALSE(node->equals(move(other)));
    node = make_shared<CallNode>(1, "test");
    other = make_shared<CallNode>(2, "test");
    REQUIRE_FALSE(node->equals(move(other)));
    node = make_shared<CallNode>(1, "test");
    other = make_shared<CallNode>(2, "test2");
    REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("SP::Node::CallNode::equals Case-Sensitivity Test") {
    unique_ptr<CallNode> node = make_unique<CallNode>(1, "A");
    unique_ptr<CallNode> other = make_unique<CallNode>(1, "a");
    REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("SP::Node::CallNode::equals Different Node Type Test") {
    shared_ptr<CallNode> node = make_shared<CallNode>(1, "test");
    shared_ptr<PrintNode> other = make_shared<PrintNode>(1, make_unique<VariableNode>("a"));
    REQUIRE_FALSE(node->equals(move(other)));
}

TEST_CASE("CallNode::extract Test") {
	PKB pkb;
	CallNode node = CallNode(1, "P");
	StmtRef result = node.extract(pkb);
	REQUIRE_EQUALS(result, 1);
}

TEST_CASE("SP::Node::CallNode::parseCallStatement Valid Token Test") {
	SP::Lexer lex;
    lex.initialize("readPoint;");
    int statement_count = 1;
    unique_ptr<CallNode> node = CallNode::parseCallStatement(lex, statement_count);
    shared_ptr<CallNode> expected = make_shared<CallNode>(1, "readPoint");
    REQUIRE(node->equals(move(expected)));
    REQUIRE_EQUALS(statement_count, 2);
}