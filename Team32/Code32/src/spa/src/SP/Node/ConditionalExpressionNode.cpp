#include "SP/Node/ConditionalExpressionNode.h"

#include <string>

#include "SP/Node/AndNode.h"
#include "SP/Node/NotNode.h"
#include "SP/Node/OrNode.h"
#include "SP/Node/RelationalExpressionNode.h"
#include "SP/SP.h"

using namespace std;
using namespace SP;

unique_ptr<ConditionalExpressionNode> ConditionalExpressionNode::parseConditionalExpression(Lexer& lex) {
	string token = lex.peekToken();
	if (token == "!") {
        lex.readToken();
		lex.nextIf("(");
		unique_ptr<ConditionalExpressionNode> expression = ConditionalExpressionNode::parseConditionalExpression(lex);
		lex.nextIf(")");
		return make_unique<NotNode>(move(expression));
	}
	if (token == "(") {
        lex.readToken();
		unique_ptr<ConditionalExpressionNode> lhs = ConditionalExpressionNode::parseConditionalExpression(lex);
		lex.nextIf(")");
		string op = lex.readToken();
		if (op != "&&" && op != "||") {
			throw ParseException("Invalid logical operator");
		}
		lex.nextIf("(");
		unique_ptr<ConditionalExpressionNode> rhs = ConditionalExpressionNode::parseConditionalExpression(lex);
		lex.nextIf(")");
		if (op == "&&") {
			return make_unique<AndNode>(move(lhs), move(rhs));
		}
		if (op == "||") {
			return make_unique<OrNode>(move(lhs), move(rhs));
		}
	}
	return RelationalExpressionNode::parseRelationalExpression(lex);
}
