#include "ConstantNode.h"

#include "Common/Converter.h"

using namespace std;

Common::ExpressionProcessor::ConstantNode::ConstantNode(const string& value) : value(Common::Converter::convertInteger(value)) {}

bool Common::ExpressionProcessor::ConstantNode::equals(shared_ptr<ExpressionNode> object) {
	shared_ptr<ConstantNode> other = dynamic_pointer_cast<ConstantNode>(object);
	if (other == nullptr) {
		return false;
	}
	return other->value == this->value;
}

bool Common::ExpressionProcessor::ConstantNode::contains(shared_ptr<ExpressionNode> object) { return equals(object); }
