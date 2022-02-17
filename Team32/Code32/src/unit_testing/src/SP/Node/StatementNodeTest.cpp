#include "../Node/MockUtilities.h"
#include "SP/Node/ExpressionNode.h"
#include "SP/Node/AssignmentNode.h"
#include "SP/Node/CallNode.h"
#include "SP/Node/IfNode.h"
#include "SP/Node/PrintNode.h"
#include "SP/Node/ReadNode.h"
#include "SP/Node/StatementNode.h"
#include "SP/Node/VariableNode.h"
#include "SP/Node/WhileNode.h"
#include "SP/SP.h"
#include "catch_tools.h"

using namespace std;
using namespace SP::Node;

TEST_CASE("SP::Node::StatementNode::parseStatement") {
    SP::Lexer lex;
    StmtRef statement_count = 1;

    SECTION("Assignment Valid Token Test") {
        lex.initialize("x = 10 + 3;");
        unique_ptr<StatementNode> node = StatementNode::parseStatement(lex, statement_count);
        unique_ptr<VariableNode> assignee = make_unique<VariableNode>("x");
        unique_ptr<ExpressionNode> expression =
                make_unique<ExpressionNode>(createArithmeticExpression(vector<string>({"10", "+", "3", ";"})));
        shared_ptr<AssignmentNode> expected = make_shared<AssignmentNode>(1, move(assignee), move(expression));
        REQUIRE(node->equals(move(expected)));
    }

    SECTION("Read Valid Token Test") {
        lex.initialize("read x;");
        unique_ptr<StatementNode> node = StatementNode::parseStatement(lex, statement_count);
        shared_ptr<ReadNode> expected = make_shared<ReadNode>(1, make_unique<VariableNode>("x"));
        REQUIRE(node->equals(move(expected)));
        REQUIRE_EQUALS(statement_count, 2);
    }

    SECTION("Print Valid Token Test") {
        lex.initialize("print x;");
        unique_ptr<StatementNode> node = StatementNode::parseStatement(lex, statement_count);
        shared_ptr<PrintNode> expected = make_shared<PrintNode>(1, make_unique<VariableNode>("x"));
        REQUIRE(node->equals(move(expected)));
        REQUIRE_EQUALS(statement_count, 2);
    }

    SECTION("Call Valid Token Test") {
        lex.initialize("call readPoint;");
        unique_ptr<StatementNode> node = StatementNode::parseStatement(lex, statement_count);
        shared_ptr<CallNode> expected = make_shared<CallNode>(1, "readPoint");
        REQUIRE(node->equals(move(expected)));
        REQUIRE_EQUALS(statement_count, 2);
    }

    SECTION("While Valid Token Test") {
        lex.initialize("while (x == 0) { count = count + 1; call readPoint; }");
        unique_ptr<StatementNode> node = StatementNode::parseStatement(lex, statement_count);
        unique_ptr<ExpressionNode> cond_expr = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "==", "0" ,")"})));
        unique_ptr<StatementListNode> stmt_lst = createStatementList("count = count + 1; call readPoint; }", 2);
        shared_ptr<WhileNode> expected = make_shared<WhileNode>(1, move(cond_expr), move(stmt_lst));
        REQUIRE(node->equals(move(expected)));
        REQUIRE_EQUALS(statement_count, 4);
    }

    SECTION("If Valid Token Test") {
        lex.initialize("if ( x > 0 ) then { read y; } else { x = 10; }");
        unique_ptr<StatementNode> node = StatementNode::parseStatement(lex, statement_count);
        unique_ptr<ExpressionNode> cond_expr = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", ">", "0" ,")"})));
        unique_ptr<StatementListNode> if_stmt_lst = createStatementList("read y; }", 2);
        unique_ptr<StatementListNode> else_stmt_lst = createStatementList("x = 10; }", 3);
        shared_ptr<IfNode> expected = make_shared<IfNode>(1, move(cond_expr), move(if_stmt_lst), move(else_stmt_lst));
        REQUIRE(node->equals(expected));
        REQUIRE_EQUALS(statement_count, 4);
    }

    SECTION("Invalid Keyword Test") {
        lex.initialize("for ( x > 0 ) { read y; }");
        REQUIRE_THROWS_AS(StatementNode::parseStatement(lex, statement_count), SP::ParseException);
        REQUIRE_EQUALS(statement_count, 1);
    }
}