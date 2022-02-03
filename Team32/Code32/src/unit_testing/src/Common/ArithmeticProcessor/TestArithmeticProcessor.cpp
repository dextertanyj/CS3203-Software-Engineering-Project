#include "Common/ArithmeticProcessor/ArithmeticExpression.h"
#include "Common/ArithmeticProcessor/ConstantNode.h"
#include "Common/ArithmeticProcessor/OperatorNode.h"
#include "Common/ArithmeticProcessor/VariableNode.h"
#include "Common/Converter.h"
#include "MockLexer.h"
#include "catch.hpp"
#include "catch_tools.h"

using namespace std;
using namespace ArithmeticProcessor;

TEST_CASE("ArithmeticExpression::parse Basic Test") {
	MockLexer lex = MockLexer(vector<string>({"1", "+", "2", ";"}));
	ArithmeticExpression expression = ArithmeticExpression::parse(lex);
	shared_ptr<ExpressionNode> root = make_shared<OperatorNode>(ArithmeticOperator::Plus, make_shared<ConstantNode>("1"), make_shared<ConstantNode>("2"));
	ArithmeticExpression expected = ArithmeticExpression(root);
	REQUIRE(expression.equals(expected));
}

TEST_CASE("ArithmeticExpression::parse Bracketing Condition Test") {
	MockLexer lex = MockLexer(vector<string>({"(", "(", "1", ")", ")", ")"}));
	ArithmeticExpression expression = ArithmeticExpression::parse(lex);
	shared_ptr<ExpressionNode> root = make_shared<ConstantNode>("1");
	ArithmeticExpression expected = ArithmeticExpression(root);
	REQUIRE(expression.equals(expected));
	REQUIRE_EQUALS(lex.readToken(), ")");
}

TEST_CASE("ArithmeticExpression::parse Precedence Test") {
	MockLexer lex = MockLexer(vector<string>({"(", "A", "+", "2", ")", "*", "B", "+", "4", "/", "5", ")"}));
	ArithmeticExpression expression = ArithmeticExpression::parse(lex);
	shared_ptr<ExpressionNode> bracketed = make_shared<OperatorNode>(ArithmeticOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("2"));
	shared_ptr<ExpressionNode> times = make_shared<OperatorNode>(ArithmeticOperator::Times, bracketed, make_shared<VariableNode>("B"));
	shared_ptr<ExpressionNode> divide = make_shared<OperatorNode>(ArithmeticOperator::Divide, make_shared<ConstantNode>("4"), make_shared<ConstantNode>("5"));
	shared_ptr<ExpressionNode> root = make_shared<OperatorNode>(ArithmeticOperator::Plus, times, divide);
	ArithmeticExpression expected = ArithmeticExpression(root);
	REQUIRE(expression.equals(expected));
	REQUIRE_EQUALS(lex.readToken(), ")");
}
