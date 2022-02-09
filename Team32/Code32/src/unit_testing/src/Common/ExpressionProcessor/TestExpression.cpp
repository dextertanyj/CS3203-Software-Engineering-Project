#include <unordered_set>

#include "Common/Converter.h"
#include "Common/ExpressionProcessor/ArithmeticNode.h"
#include "Common/ExpressionProcessor/BinaryLogicalNode.h"
#include "Common/ExpressionProcessor/ConstantNode.h"
#include "Common/ExpressionProcessor/Expression.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/ExpressionProcessor/OperatorAcceptor.h"
#include "Common/ExpressionProcessor/RelationalNode.h"
#include "Common/ExpressionProcessor/UnaryLogicalNode.h"
#include "Common/ExpressionProcessor/VariableNode.h"
#include "MockLexer.h"
#include "catch.hpp"
#include "catch_tools.h"

using namespace std;
using namespace Common::ExpressionProcessor;

TEST_CASE("Common::ExpressionProcessor::Expression::parse Arithmetic Basic Test") {
	MockLexer lex = MockLexer(vector<string>({"1", "+", "2", ";"}));
	Expression expression = Expression::parse(lex, Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic);
	shared_ptr<ExpressionNode> root =
		make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_shared<ConstantNode>("1"), make_shared<ConstantNode>("2"));
	unordered_set<VarRef> variables = unordered_set<VarRef>();
	unordered_set<int> constants = unordered_set<int>({1, 2});
	Expression expected = Expression(root, variables, constants);
	REQUIRE(expression.equals(expected));
	REQUIRE_EQUALS(expression.getConstants(), constants);
	REQUIRE_EQUALS(expression.getVariables(), variables);
}

TEST_CASE("Common::ExpressionProcessor::Expression::parse Arithmetic Bracketing Condition Test") {
	MockLexer lex = MockLexer(vector<string>({"(", "(", "1", ")", ")", ")"}));
	Expression expression = Expression::parse(lex, Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic);
	shared_ptr<ExpressionNode> root = make_shared<ConstantNode>("1");
	unordered_set<VarRef> variables = unordered_set<VarRef>();
	unordered_set<int> constants = unordered_set<int>({1});
	Expression expected = Expression(root, variables, constants);
	REQUIRE(expression.equals(expected));
	REQUIRE_EQUALS(expression.getConstants(), constants);
	REQUIRE_EQUALS(expression.getVariables(), variables);
	REQUIRE_EQUALS(lex.readToken(), ")");
}

TEST_CASE("Common::ExpressionProcessor::Expression::parse Arithmetic Precedence Test") {
	MockLexer lex = MockLexer(vector<string>({"(", "A", "+", "1", ")", "*", "B", "+", "2", "/", "3", ")"}));
	Expression expression = Expression::parse(lex, Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic);
	shared_ptr<ArithmeticNode> bracketed =
		make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_shared<VariableNode>("A"), make_shared<ConstantNode>("1"));
	shared_ptr<ArithmeticNode> times = make_shared<ArithmeticNode>(MathematicalOperator::Times, bracketed, make_shared<VariableNode>("B"));
	shared_ptr<ArithmeticNode> divide =
		make_shared<ArithmeticNode>(MathematicalOperator::Divide, make_shared<ConstantNode>("2"), make_shared<ConstantNode>("3"));
	shared_ptr<ExpressionNode> root = make_shared<ArithmeticNode>(MathematicalOperator::Plus, times, divide);
	unordered_set<VarRef> variables = unordered_set<VarRef>({"A", "B"});
	unordered_set<int> constants = unordered_set<int>({1, 2, 3});
	Expression expected = Expression(root, variables, constants);
	REQUIRE(expression.equals(expected));
	REQUIRE_EQUALS(expression.getConstants(), constants);
	REQUIRE_EQUALS(expression.getVariables(), variables);
	REQUIRE_EQUALS(lex.readToken(), ")");
}

TEST_CASE("Common::ExpressionProcessor::Expression::parse Relational Basic Test") {
	MockLexer lex = MockLexer(vector<string>({"1", "<", "2", ")"}));
	Expression expression = Expression::parse(lex, Common::ExpressionProcessor::OperatorAcceptor::acceptLogical);
	shared_ptr<ExpressionNode> root =
		make_shared<RelationalNode>(MathematicalOperator::LT, make_shared<ConstantNode>("1"), make_shared<ConstantNode>("2"));
	unordered_set<VarRef> variables = unordered_set<VarRef>();
	unordered_set<int> constants = unordered_set<int>({1, 2});
	Expression expected = Expression(root, variables, constants);
	REQUIRE(expression.equals(expected));
	REQUIRE_EQUALS(expression.getConstants(), constants);
	REQUIRE_EQUALS(expression.getVariables(), variables);
	REQUIRE_EQUALS(lex.readToken(), ")");
}

TEST_CASE("Common::ExpressionProcessor::Expression::parse Relational Complex Test") {
	MockLexer lex = MockLexer(vector<string>({"(", "1", "+", "2", ")", "*", "3", "<", "2", ")"}));
	Expression expression = Expression::parse(lex, Common::ExpressionProcessor::OperatorAcceptor::acceptRelational);
	shared_ptr<ArithmeticNode> bracketed =
		make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_shared<ConstantNode>("1"), make_shared<ConstantNode>("2"));
	shared_ptr<ArithmeticNode> times = make_shared<ArithmeticNode>(MathematicalOperator::Times, bracketed, make_shared<ConstantNode>("3"));
	shared_ptr<ExpressionNode> root = make_shared<RelationalNode>(MathematicalOperator::LT, times, make_shared<ConstantNode>("2"));
	unordered_set<VarRef> variables = unordered_set<VarRef>();
	unordered_set<int> constants = unordered_set<int>({1, 2, 3});
	Expression expected = Expression(root, variables, constants);
	REQUIRE(expression.equals(expected));
	REQUIRE_EQUALS(expression.getConstants(), constants);
	REQUIRE_EQUALS(expression.getVariables(), variables);
	REQUIRE_EQUALS(lex.readToken(), ")");
}

TEST_CASE("Common::ExpressionProcessor::Expression::parse Unary Logical Basic Test") {
	MockLexer lex = MockLexer(vector<string>({"!", "(", "x", "<", "1", ")"}));
	Expression expression = Expression::parse(lex, Common::ExpressionProcessor::OperatorAcceptor::acceptLogical);
	shared_ptr<RelationalNode> relation =
		make_shared<RelationalNode>(MathematicalOperator::LT, make_shared<VariableNode>("x"), make_shared<ConstantNode>("1"));
	shared_ptr<LogicalNode> root = make_shared<UnaryLogicalNode>(MathematicalOperator::Not, relation);
	unordered_set<VarRef> variables = unordered_set<VarRef>({"x"});
	unordered_set<int> constants = unordered_set<int>({1});
	Expression expected = Expression(root, variables, constants);
	REQUIRE(expression.equals(expected));
	REQUIRE_EQUALS(expression.getConstants(), constants);
	REQUIRE_EQUALS(expression.getVariables(), variables);
}

TEST_CASE("Common::ExpressionProcessor::Expression::parse Binary Logical Basic Test") {
	MockLexer lex = MockLexer(vector<string>({"(", "x", "<", "1", ")", "||", "(", "x", "!=", "y", ")"}));
	Expression expression = Expression::parse(lex, Common::ExpressionProcessor::OperatorAcceptor::acceptLogical);
	shared_ptr<RelationalNode> lhs_relation =
		make_shared<RelationalNode>(MathematicalOperator::LT, make_shared<VariableNode>("x"), make_shared<ConstantNode>("1"));
	shared_ptr<RelationalNode> rhs_relation =
		make_shared<RelationalNode>(MathematicalOperator::NEQ, make_shared<VariableNode>("x"), make_shared<VariableNode>("y"));
	shared_ptr<LogicalNode> root = make_shared<BinaryLogicalNode>(MathematicalOperator::Or, lhs_relation, rhs_relation);
	unordered_set<VarRef> variables = unordered_set<VarRef>({"x", "y"});
	unordered_set<int> constants = unordered_set<int>({1});
	Expression expected = Expression(root, variables, constants);
	REQUIRE(expression.equals(expected));
	REQUIRE_EQUALS(expression.getConstants(), constants);
	REQUIRE_EQUALS(expression.getVariables(), variables);
}

TEST_CASE("Common::ExpressionProcessor::Expression::parse Logical Complex Test") {
	MockLexer lex = MockLexer(vector<string>({"(", "!", "(", "(", "1", "+", "2",  ")", "<", "(", "(", "3", ")", ")", ")", ")", "||",
	                                          "(", "(", "4", "<=", "5", ")", "&&", "(", "!", "(", "6", "!=", "7", ")", ")", ")"}));
	Expression expression = Expression::parse(lex, Common::ExpressionProcessor::OperatorAcceptor::acceptLogical);
	shared_ptr<AtomicNode> addition = make_shared<ArithmeticNode>(MathematicalOperator::Plus, make_shared<ConstantNode>("1"), make_shared<ConstantNode>("2"));
	shared_ptr<RelationalNode> less_than = make_shared<RelationalNode>(MathematicalOperator::LT, addition, make_shared<ConstantNode>("3"));
	shared_ptr<LogicalNode> lhs = make_shared<UnaryLogicalNode>(MathematicalOperator::Not, less_than);
	shared_ptr<RelationalNode> less_than_equal = make_shared<RelationalNode>(MathematicalOperator::LTE, make_shared<ConstantNode>("4"), make_shared<ConstantNode>("5"));
	shared_ptr<RelationalNode> not_equal = make_shared<RelationalNode>(MathematicalOperator::NEQ, make_shared<ConstantNode>("6"), make_shared<ConstantNode>("7"));
	shared_ptr<LogicalNode> not_node = make_shared<UnaryLogicalNode>(MathematicalOperator::Not, not_equal);
	shared_ptr<LogicalNode> rhs = make_shared<BinaryLogicalNode>(MathematicalOperator::And, less_than_equal, not_node);
	shared_ptr<LogicalNode> root = make_shared<BinaryLogicalNode>(MathematicalOperator::Or, lhs, rhs);
	unordered_set<VarRef> variables = unordered_set<VarRef>();
	unordered_set<int> constants = unordered_set<int>({1, 2, 3, 4, 5, 6, 7});
	Expression expected = Expression(root, variables, constants);
	REQUIRE(expression.equals(expected));
	REQUIRE_EQUALS(expression.getConstants(), constants);
	REQUIRE_EQUALS(expression.getVariables(), variables);
}
