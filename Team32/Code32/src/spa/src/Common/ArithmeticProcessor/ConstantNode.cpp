#include "ConstantNode.h"

#include "Common/Converter.h"

using namespace std;

ArithmeticProcessor::ConstantNode::ConstantNode(const string& value) : value(Common::Converter::convertInteger(value)) {}

bool ArithmeticProcessor::ConstantNode::equals(shared_ptr<ExpressionNode> object) {
	std::shared_ptr<ConstantNode> other = dynamic_pointer_cast<ConstantNode>(object);
	if (other == nullptr) {
		return false;
	}
	return other->value == this->value;
}

bool ArithmeticProcessor::ConstantNode::contains(shared_ptr<ExpressionNode> object) { return equals(object); }
