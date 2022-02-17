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
    SP::Lexer lex_2;
    StmtRef statement_count = 1;

    SECTION("Assignment Valid Token Test") {
        lex.initialize("x = 10 + 3;");
        unique_ptr<StatementNode> node = StatementNode::parseStatement(lex, statement_count);
        unique_ptr<VariableNode> assignee = make_unique<VariableNode>("x");
        unique_ptr<ExpressionNode> expression =
                make_unique<ExpressionNode>(createArithmeticExpression(vector<string>({"10", "+", "3", ";"})));
        shared_ptr<AssignmentNode> expected = make_shared<AssignmentNode>(1, move(assignee), move(expression));
        REQUIRE(node->equals(move(expected)));
        REQUIRE_EQUALS(lex.peekToken(), "");
        lex_2.initialize("read = 10;");
        unique_ptr<StatementNode> node_2 = StatementNode::parseStatement(lex_2, statement_count);
        unique_ptr<VariableNode> assignee_2 = make_unique<VariableNode>("read");
        unique_ptr<ExpressionNode> expression_2 =
                make_unique<ExpressionNode>(createArithmeticExpression(vector<string>({"10", ";"})));
        shared_ptr<AssignmentNode> expected_2 = make_shared<AssignmentNode>(2, move(assignee_2), move(expression_2));
        REQUIRE(node_2->equals(move(expected_2)));
        REQUIRE_EQUALS(statement_count, 3);
        REQUIRE_EQUALS(lex_2.peekToken(), "");
    }

    SECTION("Read Valid Token Test") {
        lex.initialize("read x; ");
        unique_ptr<StatementNode> node = StatementNode::parseStatement(lex, statement_count);
        shared_ptr<ReadNode> expected = make_shared<ReadNode>(1, make_unique<VariableNode>("x"));
        REQUIRE(node->equals(move(expected)));
        REQUIRE_EQUALS(statement_count, 2);
        REQUIRE_EQUALS(lex.peekToken(), "");
        lex_2.initialize("read print; ");
        unique_ptr<StatementNode> node_2 = StatementNode::parseStatement(lex_2, statement_count);
        shared_ptr<ReadNode> expected_2 = make_shared<ReadNode>(2, make_unique<VariableNode>("print"));
        REQUIRE(node_2->equals(move(expected_2)));
        REQUIRE_EQUALS(statement_count, 3);
        REQUIRE_EQUALS(lex_2.peekToken(), "");
    }

    SECTION("Print Valid Token Test") {
        lex.initialize("print x;");
        unique_ptr<StatementNode> node = StatementNode::parseStatement(lex, statement_count);
        shared_ptr<PrintNode> expected = make_shared<PrintNode>(1, make_unique<VariableNode>("x"));
        REQUIRE(node->equals(move(expected)));
        REQUIRE_EQUALS(statement_count, 2);
        REQUIRE_EQUALS(lex.peekToken(), "");
        lex_2.initialize("print if;");
        unique_ptr<StatementNode> node_2 = StatementNode::parseStatement(lex_2, statement_count);
        shared_ptr<PrintNode> expected_2 = make_shared<PrintNode>(2, make_unique<VariableNode>("if"));
        REQUIRE(node_2->equals(move(expected_2)));
        REQUIRE_EQUALS(statement_count, 3);
        REQUIRE_EQUALS(lex.peekToken(), "");
    }

    SECTION("Call Valid Token Test") {
        lex.initialize("call readPoint;");
        unique_ptr<StatementNode> node = StatementNode::parseStatement(lex, statement_count);
        shared_ptr<CallNode> expected = make_shared<CallNode>(1, "readPoint");
        REQUIRE(node->equals(move(expected)));
        REQUIRE_EQUALS(statement_count, 2);
        REQUIRE_EQUALS(lex.peekToken(), "");
        lex_2.initialize("call while;");
        unique_ptr<StatementNode> node_2 = StatementNode::parseStatement(lex_2, statement_count);
        shared_ptr<CallNode> expected_2 = make_shared<CallNode>(2, "while");
        REQUIRE(node_2->equals(move(expected_2)));
        REQUIRE_EQUALS(statement_count, 3);
        REQUIRE_EQUALS(lex.peekToken(), "");
    }

    SECTION("While Valid Token Test") {
        lex.initialize("while (x == 0) { count = count + 1; call readPoint; }");
        unique_ptr<StatementNode> node = StatementNode::parseStatement(lex, statement_count);
        unique_ptr<ExpressionNode> cond_expr = make_unique<ExpressionNode>(createConditionalExpression(vector<string>({"x", "==", "0" ,")"})));
        unique_ptr<StatementListNode> stmt_lst = createStatementList("count = count + 1; call readPoint; }", 2);
        shared_ptr<WhileNode> expected = make_shared<WhileNode>(1, move(cond_expr), move(stmt_lst));
        REQUIRE(node->equals(move(expected)));
        REQUIRE_EQUALS(statement_count, 4);
        REQUIRE_EQUALS(lex.peekToken(), "");
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
        REQUIRE_EQUALS(lex.peekToken(), "");
    }

    SECTION("Invalid Keyword Test") {
        lex.initialize("for ( x > 0 ) { read y; }");
        REQUIRE_THROWS_AS(StatementNode::parseStatement(lex, statement_count), SP::ParseException);
        REQUIRE_EQUALS(statement_count, 1);
        REQUIRE_EQUALS(lex.peekToken(), "(");
    }
}