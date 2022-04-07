#include "SP/Node/ExpressionNode.h"

#include <utility>

#include "Common/ExpressionProcessor/ExpressionParser.h"
#include "Common/ExpressionProcessor/OperatorAcceptor.h"

using namespace std;

SP::Node::ExpressionNode::ExpressionNode(Common::ExpressionProcessor::Expression expression) : expression(move(expression)) {}

Common::ExpressionProcessor::Expression SP::Node::ExpressionNode::extract() const { return expression; }

bool SP::Node::ExpressionNode::equals(const shared_ptr<ExpressionNode>& object) const { return this->expression == object->expression; }

unique_ptr<SP::Node::ExpressionNode> SP::Node::ExpressionNode::parseExpression(Lexer& lex,
                                                                               Common::ExpressionProcessor::ExpressionType type) {
	auto parser = Common::ExpressionProcessor::ExpressionParser{lex, type};
	try {
		auto expression = parser.parse();
		return make_unique<ExpressionNode>(expression);
	} catch (const Common::ExpressionProcessor::ExpressionProcessorException& e) {
		throw ParseException(e.what());
	}
}
