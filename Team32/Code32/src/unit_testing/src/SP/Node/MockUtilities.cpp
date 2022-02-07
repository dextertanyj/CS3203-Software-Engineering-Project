#include "MockUtilities.h"

#include "../../Common/ArithmeticProcessor/MockLexer.h"

using namespace std;

Common::ArithmeticProcessor::ArithmeticExpression createArithmeticExpression(vector<string> str_list) {
    MockLexer lex = MockLexer(move(str_list));
	Common::ArithmeticProcessor::ArithmeticExpression expression = Common::ArithmeticProcessor::ArithmeticExpression::parse(lex);
    return expression;
}

unique_ptr<SP::Node::RelationalExpressionNode> createRelationalExpression(string str) {
    SP::Lexer lex;
    lex.initialize(move(str));
    unique_ptr<SP::Node::RelationalExpressionNode> node = SP::Node::RelationalExpressionNode::parseRelationalExpression(lex);
    return node;
}

unique_ptr<SP::Node::StatementListNode> createStatementList(string str, int statement_count) {
    SP::Lexer lex;
    lex.initialize(move(str));
    unique_ptr<SP::Node::StatementListNode> node = SP::Node::StatementListNode::parseStatementList(lex, statement_count);
    return node;
}
