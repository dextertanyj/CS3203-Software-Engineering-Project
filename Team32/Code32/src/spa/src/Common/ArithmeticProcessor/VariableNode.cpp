#include "VariableNode.h"

#include <memory>

using namespace std;

Common::ArithmeticProcessor::VariableNode::VariableNode(VarRef name) : name(std::move(name)) {}

bool Common::ArithmeticProcessor::VariableNode::equals(shared_ptr<ExpressionNode> object) {
	std::shared_ptr<VariableNode> other = dynamic_pointer_cast<VariableNode>(object);
	if (other == nullptr) {
		return false;
	}
	return other->name == this->name;
}

bool Common::ArithmeticProcessor::VariableNode::contains(shared_ptr<ExpressionNode> object) { return equals(object); }
