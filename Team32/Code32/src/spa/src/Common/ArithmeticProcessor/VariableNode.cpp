#include "VariableNode.h"

#include <memory>
#include <utility>

using namespace std;

ArithmeticProcessor::VariableNode::VariableNode(VarRef name) : name(std::move(name)) {}

bool ArithmeticProcessor::VariableNode::equals(shared_ptr<ExpressionNode> object) {
	std::shared_ptr<VariableNode> other = dynamic_pointer_cast<VariableNode>(object);
	if (other == nullptr) {
		return false;
	}
	return other->name == this->name;
}

bool ArithmeticProcessor::VariableNode::contains(shared_ptr<ExpressionNode> object) { return equals(object); }
