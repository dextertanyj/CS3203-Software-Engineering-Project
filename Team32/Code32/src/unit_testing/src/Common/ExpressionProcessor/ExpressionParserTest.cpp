#include "Common/ExpressionProcessor/ExpressionParser.h"

#include <unordered_set>

#include "Common/ExpressionProcessor/ArithmeticNode.h"
#include "Common/ExpressionProcessor/BinaryLogicalNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/ExpressionProcessor/OperatorAcceptor.h"
#include "Common/ExpressionProcessor/RelationalNode.h"
#include "Common/ExpressionProcessor/TerminalNode.tpp"
#include "Common/ExpressionProcessor/UnaryLogicalNode.h"
#include "MockLexer.h"
#include "catch_tools.h"

#define variable(name) make_shared<TerminalNode<string>>(name)
#define constant(value) make_shared<TerminalNode<ConstVal>>(value)
#define arithmetic(op, lhs, rhs) make_shared<ArithmeticNode>(op, lhs, rhs)
#define relational(op, lhs, rhs) make_shared<RelationalNode>(op, lhs, rhs)
#define unarylogical(op, exp) make_shared<UnaryLogicalNode>(op, exp)
#define binarylogical(op, lhs, rhs) make_shared<BinaryLogicalNode>(op, lhs, rhs)

using namespace std;
using namespace Common::ExpressionProcessor;

TEST_CASE("Common::ExpressionProcessor::ExpressionParser::parse Acceptor Tests") {
	SECTION("Artihmetic Test") {
		MockLexer lex = MockLexer(vector<string>({"x", "<", "y"}));
		ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
		REQUIRE_EQUALS(lex.peekToken(), "<");
	}
	SECTION("Relational Test") {
		MockLexer lex = MockLexer(vector<string>({"x", "+", "2", "<", "3", "||", "(", "x", "<", "4", ")"}));
		ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Relational}.parse();
		REQUIRE_EQUALS(lex.peekToken(), "||");
	}
}

TEST_CASE("Common::ExpressionProcessor::ExpressionParser{}.parse Arithmetic Tests") {
	SECTION("Single Token Test") {
		MockLexer lex = MockLexer(vector<string>({"1", ")"}));
		Expression expression = ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
		shared_ptr<ExpressionNode> root = constant(1);
		unordered_set<VarRef> variables = unordered_set<VarRef>();
		unordered_set<ConstVal> constants = unordered_set<ConstVal>({1});
		Expression expected = Expression(root, variables, constants);
		REQUIRE(expression == expected);
		REQUIRE_EQUALS(expression.getConstants(), constants);
		REQUIRE_EQUALS(expression.getVariables(), variables);
		REQUIRE_EQUALS(lex.readToken(), ")");
		REQUIRE_EQUALS(lex.readToken(), "");
	}

	SECTION("Basic Expression Test") {
		MockLexer lex = MockLexer(vector<string>({"1", "+", "2", ";"}));
		Expression expression = ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
		shared_ptr<ExpressionNode> root = arithmetic(MathematicalOperator::Plus, constant(1), constant(2));
		unordered_set<VarRef> variables = unordered_set<VarRef>();
		unordered_set<ConstVal> constants = unordered_set<ConstVal>({1, 2});
		Expression expected = Expression(root, variables, constants);
		REQUIRE(expression == expected);
		REQUIRE_EQUALS(expression.getConstants(), constants);
		REQUIRE_EQUALS(expression.getVariables(), variables);
		REQUIRE_EQUALS(lex.readToken(), ";");
	}

	SECTION("Parentheses Test") {
		MockLexer lex = MockLexer(vector<string>({"(", "(", "1", ")", ")", ")"}));
		Expression expression = ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
		shared_ptr<ExpressionNode> root = constant(1);
		unordered_set<VarRef> variables = unordered_set<VarRef>();
		unordered_set<ConstVal> constants = unordered_set<ConstVal>({1});
		Expression expected = Expression(root, variables, constants);
		REQUIRE(expression == expected);
		REQUIRE_EQUALS(expression.getConstants(), constants);
		REQUIRE_EQUALS(expression.getVariables(), variables);
		REQUIRE_EQUALS(lex.readToken(), ")");
		REQUIRE_EQUALS(lex.readToken(), "");
	}

	SECTION("Precedence Test") {
		MockLexer lex = MockLexer(vector<string>({"(", "A", "-", "1", ")", "*", "B", "+", "2", "/", "3", ")"}));
		Expression expression = ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
		shared_ptr<ArithmeticNode> bracketed = arithmetic(MathematicalOperator::Minus, variable("A"), constant(1));
		shared_ptr<ArithmeticNode> times = arithmetic(MathematicalOperator::Times, bracketed, variable("B"));
		shared_ptr<ArithmeticNode> divide = arithmetic(MathematicalOperator::Divide, constant(2), constant(3));
		shared_ptr<ExpressionNode> root = arithmetic(MathematicalOperator::Plus, times, divide);
		unordered_set<VarRef> variables = unordered_set<VarRef>({"A", "B"});
		unordered_set<ConstVal> constants = unordered_set<ConstVal>({1, 2, 3});
		Expression expected = Expression(root, variables, constants);
		REQUIRE(expression == expected);
		REQUIRE_EQUALS(expression.getConstants(), constants);
		REQUIRE_EQUALS(expression.getVariables(), variables);
		REQUIRE_EQUALS(lex.readToken(), ")");
		REQUIRE_EQUALS(lex.readToken(), "");
	}

	SECTION("Missing Operator Test") {
		MockLexer lex = MockLexer(vector<string>({"1", "2", "+", "3"}));
		Expression expression = ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
		REQUIRE_EQUALS(lex.readToken(), "2");
	}

	SECTION("Missing Operand Test") {
		MockLexer lex = MockLexer(vector<string>({"1", "+", "*", "3"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse()),
		                  ExpressionProcessorException);
	}

	SECTION("Early Termination Test") {
		MockLexer lex = MockLexer(vector<string>({"1", "+"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse()),
		                  ExpressionProcessorException);
	}
}

TEST_CASE("Common::ExpressionProcessor::ExpressionParser{}.parse Relational Tests") {
	SECTION("Basic Test") {
		MockLexer lex = MockLexer(vector<string>({"1", "<", "2", ";"}));
		Expression expression = ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Relational}.parse();
		shared_ptr<ExpressionNode> root = relational(MathematicalOperator::LT, constant(1), constant(2));
		unordered_set<VarRef> variables = unordered_set<VarRef>();
		unordered_set<ConstVal> constants = unordered_set<ConstVal>({1, 2});
		Expression expected = Expression(root, variables, constants);
		REQUIRE(expression == expected);
		REQUIRE_EQUALS(expression.getConstants(), constants);
		REQUIRE_EQUALS(expression.getVariables(), variables);
		REQUIRE_EQUALS(lex.readToken(), ";");
	}

	SECTION("Complex Test") {
		MockLexer lex = MockLexer(vector<string>({"(", "1", "+", "2", ")", "*", "3", "==", "2", ";"}));
		Expression expression = ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Relational}.parse();
		shared_ptr<ArithmeticNode> bracketed = arithmetic(MathematicalOperator::Plus, constant(1), constant(2));
		shared_ptr<ArithmeticNode> times = arithmetic(MathematicalOperator::Times, bracketed, constant(3));
		shared_ptr<ExpressionNode> root = relational(MathematicalOperator::EQ, times, constant(2));
		unordered_set<VarRef> variables = unordered_set<VarRef>();
		unordered_set<ConstVal> constants = unordered_set<ConstVal>({1, 2, 3});
		Expression expected = Expression(root, variables, constants);
		REQUIRE(expression == expected);
		REQUIRE_EQUALS(expression.getConstants(), constants);
		REQUIRE_EQUALS(expression.getVariables(), variables);
		REQUIRE_EQUALS(lex.readToken(), ";");
	}

	SECTION("Missing Operator Test") {
		MockLexer lex = MockLexer(vector<string>({"1", "2", "<", "3"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Relational}.parse()),
		                  ExpressionProcessorException);
		REQUIRE_EQUALS(lex.readToken(), "2");
	}

	SECTION("Missing Operand Test") {
		MockLexer lex = MockLexer(vector<string>({"1", "<=", "==", "3"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Relational}.parse()),
		                  ExpressionProcessorException);
	}

	SECTION("Early Termination Test") {
		MockLexer lex = MockLexer(vector<string>({"1", "<"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Relational}.parse()),
		                  ExpressionProcessorException);
	}

	SECTION("Excess Parentheses Test") {
		MockLexer lex = MockLexer(vector<string>({"(", "1", "<", "3", ")"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Relational}.parse()),
		                  ExpressionProcessorException);
	}

	SECTION("Invalid Expression Test") {
		MockLexer lex = MockLexer(vector<string>({"(", "1", ")"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Relational}.parse()),
		                  ExpressionProcessorException);
	}

	SECTION("Invalid Chaining Test") {
		MockLexer lex = MockLexer(vector<string>({"1", "<", "(", "2", "<", "3", ")"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Relational}.parse()),
		                  ExpressionProcessorException);
		lex = MockLexer(vector<string>({"1", "<", "2", "<", "3"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Relational}.parse()),
		                  ExpressionProcessorException);
		lex = MockLexer(vector<string>({"1", "<", "2", "+", "3", "<", "4"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Relational}.parse()),
		                  ExpressionProcessorException);
		lex = MockLexer(vector<string>({"(", "1", "<", "2", ")", "+", "(", "3", "<", "4", ")"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Relational}.parse()),
		                  ExpressionProcessorException);
	}
}

TEST_CASE("Common::ExpressionProcessor::ExpressionParser{}.parse Logical Tests") {
	SECTION("Unary Test") {
		MockLexer lex = MockLexer(vector<string>({"!", "(", "x", "<", "1", ")"}));
		Expression expression = ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Logical}.parse();
		shared_ptr<RelationalNode> relation = relational(MathematicalOperator::LT, variable("x"), constant(1));
		shared_ptr<LogicalNode> root = unarylogical(MathematicalOperator::Not, relation);
		unordered_set<VarRef> variables = unordered_set<VarRef>({"x"});
		unordered_set<ConstVal> constants = unordered_set<ConstVal>({1});
		Expression expected = Expression(root, variables, constants);
		REQUIRE(expression == expected);
		REQUIRE_EQUALS(expression.getConstants(), constants);
		REQUIRE_EQUALS(expression.getVariables(), variables);
	}

	SECTION("Binary Test") {
		MockLexer lex = MockLexer(vector<string>({"(", "x", "<", "1", ")", "||", "(", "x", "!=", "y", ")"}));
		Expression expression = ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Logical}.parse();
		shared_ptr<RelationalNode> lhs_relation = relational(MathematicalOperator::LT, variable("x"), constant(1));
		shared_ptr<RelationalNode> rhs_relation = relational(MathematicalOperator::NEQ, variable("x"), variable("y"));
		shared_ptr<LogicalNode> root = binarylogical(MathematicalOperator::Or, lhs_relation, rhs_relation);
		unordered_set<VarRef> variables = unordered_set<VarRef>({"x", "y"});
		unordered_set<ConstVal> constants = unordered_set<ConstVal>({1});
		Expression expected = Expression(root, variables, constants);
		REQUIRE(expression == expected);
		REQUIRE_EQUALS(expression.getConstants(), constants);
		REQUIRE_EQUALS(expression.getVariables(), variables);
	}

	SECTION("Complex Test") {
		MockLexer lex = MockLexer(vector<string>({"(", "!", "(", "(",  "1", "+", "2",  ")", "<", "(", "(", "3",  ")", ")", ")", ")", "||",
		                                          "(", "(", "4", "<=", "5", ")", "&&", "(", "!", "(", "6", "!=", "7", ")", ")", ")"}));
		Expression expression = ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Logical}.parse();
		shared_ptr<AtomicNode> addition = arithmetic(MathematicalOperator::Plus, constant(1), constant(2));
		shared_ptr<RelationalNode> less_than = relational(MathematicalOperator::LT, addition, constant(3));
		shared_ptr<LogicalNode> lhs = unarylogical(MathematicalOperator::Not, less_than);
		shared_ptr<RelationalNode> less_than_equal = relational(MathematicalOperator::LTE, constant(4), constant(5));
		shared_ptr<RelationalNode> not_equal = relational(MathematicalOperator::NEQ, constant(6), constant(7));
		shared_ptr<LogicalNode> not_node = unarylogical(MathematicalOperator::Not, not_equal);
		shared_ptr<LogicalNode> rhs = binarylogical(MathematicalOperator::And, less_than_equal, not_node);
		shared_ptr<LogicalNode> root = binarylogical(MathematicalOperator::Or, lhs, rhs);
		unordered_set<VarRef> variables = unordered_set<VarRef>();
		unordered_set<ConstVal> constants = unordered_set<ConstVal>({1, 2, 3, 4, 5, 6, 7});
		Expression expected = Expression(root, variables, constants);
		REQUIRE(expression == expected);
		REQUIRE_EQUALS(expression.getConstants(), constants);
		REQUIRE_EQUALS(expression.getVariables(), variables);
	}

	SECTION("Missing Before Bracket Test") {
		MockLexer lex = MockLexer(vector<string>({"x", "<", "y", "&&", "(", "z", ">", "3", ")"}));
		ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Logical}.parse();
		REQUIRE(lex.peekToken() == "&&");
	}

	SECTION("Missing After Bracket Test") {
		MockLexer lex = MockLexer(vector<string>({"(", "x", "<", "y", ")", "&&", "z", ">", "3"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Logical}.parse()),
		                  ExpressionProcessorException);
	}

	SECTION("Missing Operator Test") {
		MockLexer lex = MockLexer(vector<string>({"1", "(", "2", "&&", "3", ")"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Logical}.parse()),
		                  ExpressionProcessorException);
		REQUIRE_EQUALS(lex.readToken(), "(");
	}

	SECTION("Missing Operand Test") {
		MockLexer lex = MockLexer(vector<string>({"(", "1", "<", "2", ")", "&&", "||", "(", "3", "==", "4", ")"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Logical}.parse()),
		                  ExpressionProcessorException);
	}

	SECTION("Early Termination Test") {
		MockLexer lex = MockLexer(vector<string>({"(", "1", "<", "2", ")", "&&"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Logical}.parse()),
		                  ExpressionProcessorException);
	}

	SECTION("Excess Parentheses Test") {
		MockLexer lex = MockLexer(vector<string>({"(", "(", "x", "<", "y", ")", "&&", "(", "z", ">", "3", ")", ")"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Logical}.parse()),
		                  ExpressionProcessorException);
		lex = MockLexer(vector<string>({"(", "1", "<", "2", ")"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Logical}.parse()),
		                  ExpressionProcessorException);
		lex = MockLexer(vector<string>({"!", "(", "(", "1", "<", "2", ")", ")"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Logical}.parse()),
		                  ExpressionProcessorException);
		lex = MockLexer(vector<string>({"(", "(", "x", "<", "y", ")", ")", "&&", "(", "z", ">", "3", ")"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Logical}.parse()),
		                  ExpressionProcessorException);
		lex = MockLexer(vector<string>({"(", "x", "<", "y", ")", "&&", "(", "(", "z", ">", "3", ")", ")"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Logical}.parse()),
		                  ExpressionProcessorException);
	}

	SECTION("Invalid Expression Test") {
		MockLexer lex = MockLexer(vector<string>({"(", "1", ")"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Logical}.parse()),
		                  ExpressionProcessorException);
	}

	SECTION("Invalid Chaining Test") {
		MockLexer lex = MockLexer(vector<string>({"(", "x", "<", "1", ")", "||", "(", "y", "==", "2", ")", "<", "(", "z", "+", "3", ")"}));
		ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Logical}.parse();
		REQUIRE_EQUALS(lex.readToken(), "<");
		lex = MockLexer(vector<string>({"(", "x", "<", "1", ")", "||", "(", "y", "==", "2", ")", "+", "(", "z", "+", "3", ")"}));
		ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Logical}.parse();
		REQUIRE_EQUALS(lex.readToken(), "+");
		lex = MockLexer(vector<string>({"2", "+", "(", "x", "<", "1", ")", "||", "(", "y", "==", "2", ")"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Logical}.parse()),
		                  ExpressionProcessorException);
		lex = MockLexer(vector<string>({"3", "<", "(", "x", "<", "1", ")", "||", "(", "y", "==", "2", ")"}));
		REQUIRE_THROWS_AS((ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Logical}.parse()),
		                  ExpressionProcessorException);
	}
}

TEST_CASE("Common::ExpressionProcessor::Expression::contains Test") {
	MockLexer lex = MockLexer(vector<string>({"(", "A", "-", "1", ")", "*", "B", "+", "2", "/", "3", ")"}));
	Expression expression = ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();

	SECTION("Single Variable Test") {
		lex = MockLexer(vector<string>({"(", "A", ")"}));
		Expression target = ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
		REQUIRE(expression.contains(target));
	}

	SECTION("Single Constant Test") {
		lex = MockLexer(vector<string>({"(", "1", ")"}));
		Expression target = ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
		REQUIRE(expression.contains(target));
	}

	SECTION("Operator Test") {
		lex = MockLexer(vector<string>({"A", "-", "1"}));
		Expression target = ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
		REQUIRE(expression.contains(target));

		lex = MockLexer(vector<string>({"2", "/", "3"}));
		target = ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
		REQUIRE(expression.contains(target));
	}

	SECTION("Bracketed Test") {
		lex = MockLexer(vector<string>({"(", "A", "-", "1", ")", "*", "B"}));
		Expression target = ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
		REQUIRE(expression.contains(target));
	}

	SECTION("Precedence Test") {
		lex = MockLexer(vector<string>({"B", "+", "2"}));
		Expression target = ExpressionParser{lex, Common::ExpressionProcessor::ExpressionType::Arithmetic}.parse();
		REQUIRE_FALSE(expression.contains(target));
	}
}
