#include <unordered_set>

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
#include "catch_tools.h"

#define variable(name) make_shared<VariableNode>(name)
#define constant(value) make_shared<ConstantNode>(value)
#define arithmetic(op, lhs, rhs) make_shared<ArithmeticNode>(op, lhs, rhs)
#define relational(op, lhs, rhs) make_shared<RelationalNode>(op, lhs, rhs)
#define unarylogical(op, exp) make_shared<UnaryLogicalNode>(op, exp)
#define binarylogical(op, lhs, rhs) make_shared<BinaryLogicalNode>(op, lhs, rhs)

using namespace std;
using namespace Common::ExpressionProcessor;

TEST_CASE("Common::ExpressionProcessor::Expression::parse Arithmetic Basic Test") {
	MockLexer lex = MockLexer(vector<string>({"1", "+", "2", ";"}));
	Expression expression = Expression::parse(lex, Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic);
	shared_ptr<ExpressionNode> root = arithmetic(MathematicalOperator::Plus, constant("1"), constant("2"));
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
	shared_ptr<ExpressionNode> root = constant("1");
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
	shared_ptr<ArithmeticNode> bracketed = arithmetic(MathematicalOperator::Plus, variable("A"), constant("1"));
	shared_ptr<ArithmeticNode> times = arithmetic(MathematicalOperator::Times, bracketed, variable("B"));
	shared_ptr<ArithmeticNode> divide = arithmetic(MathematicalOperator::Divide, constant("2"), constant("3"));
	shared_ptr<ExpressionNode> root = arithmetic(MathematicalOperator::Plus, times, divide);
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
	shared_ptr<ExpressionNode> root = relational(MathematicalOperator::LT, constant("1"), constant("2"));
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
	shared_ptr<ArithmeticNode> bracketed = arithmetic(MathematicalOperator::Plus, constant("1"), constant("2"));
	shared_ptr<ArithmeticNode> times = arithmetic(MathematicalOperator::Times, bracketed, constant("3"));
	shared_ptr<ExpressionNode> root = relational(MathematicalOperator::LT, times, constant("2"));
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
	shared_ptr<RelationalNode> relation = relational(MathematicalOperator::LT, variable("x"), constant("1"));
	shared_ptr<LogicalNode> root = unarylogical(MathematicalOperator::Not, relation);
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
	shared_ptr<RelationalNode> lhs_relation = relational(MathematicalOperator::LT, variable("x"), constant("1"));
	shared_ptr<RelationalNode> rhs_relation = relational(MathematicalOperator::NEQ, variable("x"), variable("y"));
	shared_ptr<LogicalNode> root = binarylogical(MathematicalOperator::Or, lhs_relation, rhs_relation);
	unordered_set<VarRef> variables = unordered_set<VarRef>({"x", "y"});
	unordered_set<int> constants = unordered_set<int>({1});
	Expression expected = Expression(root, variables, constants);
	REQUIRE(expression.equals(expected));
	REQUIRE_EQUALS(expression.getConstants(), constants);
	REQUIRE_EQUALS(expression.getVariables(), variables);
}

TEST_CASE("Common::ExpressionProcessor::Expression::parse Logical Complex Test") {
	MockLexer lex = MockLexer(vector<string>({"(", "!", "(", "(",  "1", "+", "2",  ")", "<", "(", "(", "3",  ")", ")", ")", ")", "||",
	                                          "(", "(", "4", "<=", "5", ")", "&&", "(", "!", "(", "6", "!=", "7", ")", ")", ")"}));
	Expression expression = Expression::parse(lex, Common::ExpressionProcessor::OperatorAcceptor::acceptLogical);
	shared_ptr<AtomicNode> addition = arithmetic(MathematicalOperator::Plus, constant("1"), constant("2"));
	shared_ptr<RelationalNode> less_than = relational(MathematicalOperator::LT, addition, constant("3"));
	shared_ptr<LogicalNode> lhs = unarylogical(MathematicalOperator::Not, less_than);
	shared_ptr<RelationalNode> less_than_equal = relational(MathematicalOperator::LTE, constant("4"), constant("5"));
	shared_ptr<RelationalNode> not_equal = relational(MathematicalOperator::NEQ, constant("6"), constant("7"));
	shared_ptr<LogicalNode> not_node = unarylogical(MathematicalOperator::Not, not_equal);
	shared_ptr<LogicalNode> rhs = binarylogical(MathematicalOperator::And, less_than_equal, not_node);
	shared_ptr<LogicalNode> root = binarylogical(MathematicalOperator::Or, lhs, rhs);
	unordered_set<VarRef> variables = unordered_set<VarRef>();
	unordered_set<int> constants = unordered_set<int>({1, 2, 3, 4, 5, 6, 7});
	Expression expected = Expression(root, variables, constants);
	REQUIRE(expression.equals(expected));
	REQUIRE_EQUALS(expression.getConstants(), constants);
	REQUIRE_EQUALS(expression.getVariables(), variables);
}

TEST_CASE("Common::ExpressionProcessor::Expression::parse Logical Missing Before Bracket Test") {
	MockLexer lex = MockLexer(vector<string>({"x", "<", "y", "&&", "(", "z", ">", "3", ")"}));
	Expression::parse(lex, Common::ExpressionProcessor::OperatorAcceptor::acceptLogical);
	REQUIRE(lex.peekToken() == "&&");
}

TEST_CASE("Common::ExpressionProcessor::Expression::parse Logical Missing After Bracket Test") {
	MockLexer lex = MockLexer(vector<string>({"(", "x", "<", "y", ")", "&&", "z", ">", "3"}));
	REQUIRE_THROWS_AS(Expression::parse(lex, Common::ExpressionProcessor::OperatorAcceptor::acceptLogical), ExpressionProcessorException);
}


TEST_CASE("Common::ExpressionProcessor::Expression::parse Acceptor Test") {
	MockLexer lex = MockLexer(vector<string>({"x", "<", "y"}));
	Expression::parse(lex, Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic);
	REQUIRE_EQUALS(lex.peekToken(), "<");
	lex = MockLexer(vector<string>({"(", "x", "<", "3", ")", "||", "(", "x", "<", "4", ")"}));
	Expression::parse(lex, Common::ExpressionProcessor::OperatorAcceptor::acceptRelational);
	REQUIRE_EQUALS(lex.peekToken(), "||");
}

TEST_CASE("Common::ExpressionProcessor::Expression::parse Relational Subexpression Test") {
	MockLexer lex = MockLexer(vector<string>({"(", "x", "||", "y", ")", "<", "(", "z", "+", "3", ")"}));
	REQUIRE_THROWS_AS(Expression::parse(lex, Common::ExpressionProcessor::OperatorAcceptor::acceptLogical), ExpressionProcessorException);
	lex = MockLexer(vector<string>({"(", "(", "x", "<", "y", ")", ")"}));
	REQUIRE_THROWS_AS(Expression::parse(lex, Common::ExpressionProcessor::OperatorAcceptor::acceptLogical), ExpressionProcessorException);
}