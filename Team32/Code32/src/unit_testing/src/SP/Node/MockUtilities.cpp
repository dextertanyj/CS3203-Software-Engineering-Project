#include "MockUtilities.h"

#include "../../Common/ExpressionProcessor/MockLexer.h"

#include "Common/ExpressionProcessor/OperatorAcceptor.h"

using namespace std;

Common::ExpressionProcessor::Expression createArithmeticExpression(vector<string> str_list) {
    MockLexer lex = MockLexer(move(str_list));
	Common::ExpressionProcessor::Expression expression =
		Common::ExpressionProcessor::Expression::parse(lex, Common::ExpressionProcessor::ExpressionType::Arithmetic);
    return expression;
}

Common::ExpressionProcessor::Expression createRelationalExpression(vector<string> str_list) {
	MockLexer lex = MockLexer(move(str_list));
	Common::ExpressionProcessor::Expression expression =
		Common::ExpressionProcessor::Expression::parse(lex, Common::ExpressionProcessor::ExpressionType::Relational);
	return expression;
}

Common::ExpressionProcessor::Expression createConditionalExpression(vector<string> str_list) {
	MockLexer lex = MockLexer(move(str_list));
	Common::ExpressionProcessor::Expression expression =
		Common::ExpressionProcessor::Expression::parse(lex, Common::ExpressionProcessor::ExpressionType::Logical);
	return expression;
}

unique_ptr<SP::Node::StatementListNode> createStatementList(string str, StmtRef statement_count) {
    SP::Lexer lex;
    lex.initialize(move(str));
    unique_ptr<SP::Node::StatementListNode> node = SP::Node::StatementListNode::parseStatementList(lex, statement_count);
    return node;
}
