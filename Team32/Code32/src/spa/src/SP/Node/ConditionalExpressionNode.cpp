#include "SP/Node/ConditionalExpressionNode.h"

#include <string>

#include "SP/Node/AndNode.h"
#include "SP/Node/NotNode.h"
#include "SP/Node/OrNode.h"
#include "SP/Node/RelationalExpressionNode.h"
#include "SP/ParseException.h"

using namespace std;

unique_ptr<ConditionalExpressionNode> ConditionalExpressionNode::parseConditionalExpression(Lexer& lex) {
	string token = lex.peek_token();
	if (token == "!") {
		lex.next_if("(");
		unique_ptr<ConditionalExpressionNode> expression = ConditionalExpressionNode::parseConditionalExpression(lex);
		lex.next_if(")");
		return make_unique<NotNode>(move(expression));
	}
	if (token == "(") {
		unique_ptr<ConditionalExpressionNode> lhs = ConditionalExpressionNode::parseConditionalExpression(lex);
		lex.next_if(")");
		string op = lex.read_token();
		if (op != "&&" && op != "||") {
			throw ParseException("Invalid logical operator");
		}
		lex.next_if("(");
		unique_ptr<ConditionalExpressionNode> rhs = ConditionalExpressionNode::parseConditionalExpression(lex);
		lex.next_if(")");
		if (op == "&&") {
			return make_unique<AndNode>(move(lhs), move(rhs));
		}
		if (op == "||") {
			return make_unique<OrNode>(move(lhs), move(rhs));
		}
	}
	return RelationalExpressionNode::parseRelationalExpression(lex);
}