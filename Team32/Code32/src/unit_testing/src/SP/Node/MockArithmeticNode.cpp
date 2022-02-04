#include "MockArithmeticNode.h"

#include "SP/Node/RelationalExpressionNode.h"
#include "../../Common/ArithmeticProcessor/MockLexer.h"

#include <utility>

using namespace std;

ArithmeticProcessor::ArithmeticExpression getArithmeticExpression(vector<string> str_list) {
    MockLexer lex = MockLexer(move(str_list));
    ArithmeticProcessor::ArithmeticExpression expression = ArithmeticProcessor::ArithmeticExpression::parse(lex);
    return expression;
}

unique_ptr<RelationalExpressionNode> getRelationalExpression(string str) {
    Lexer lex;
    lex.initialize(move(str));
    unique_ptr<RelationalExpressionNode> node = RelationalExpressionNode::parseRelationalExpression(lex);
    return node;
}

unique_ptr<StatementListNode> getStatementList(string str, int statement_count) {
    Lexer lex;
    lex.initialize(move(str));
    unique_ptr<StatementListNode> node = StatementListNode::parseStatementList(lex, statement_count);
    return node;
}
