#include "SP/Node/ExpressionNode.h"

#include <utility>

#include "Common/ExpressionProcessor/ExpressionParser.h"
#include "SP/SP.h"

using namespace std;

SP::Node::ExpressionNode::ExpressionNode(Common::EP::Expression expression) : expression(move(expression)) {}

Common::EP::Expression SP::Node::ExpressionNode::extract() const { return expression; }

bool SP::Node::ExpressionNode::equals(const shared_ptr<ExpressionNode>& object) const { return this->expression == object->expression; }

unique_ptr<SP::Node::ExpressionNode> SP::Node::ExpressionNode::parse(Lexer& lex, Common::EP::ExpressionType type) {
	auto parser = Common::EP::ExpressionParser{lex, type};
	try {
		auto expression = parser.parse();
		return make_unique<ExpressionNode>(expression);
	} catch (const Common::EP::ExpressionProcessorException& e) {
		throw ParseException(e.what());
	}
}
