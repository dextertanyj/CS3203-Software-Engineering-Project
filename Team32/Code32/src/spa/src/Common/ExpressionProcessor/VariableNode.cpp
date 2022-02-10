#include "Common/ExpressionProcessor/VariableNode.h"
#include "Common/Validator.h"

#include <memory>

using namespace std;

Common::ExpressionProcessor::VariableNode::VariableNode(VarRef name) {
	if (!Validator::validateName(name)) {
		throw ExpressionProcessorException("Invalid variable name");
	}
	this->name = std::move(name);
}

bool Common::ExpressionProcessor::VariableNode::equals(shared_ptr<ExpressionNode> object) {
	std::shared_ptr<VariableNode> other = dynamic_pointer_cast<VariableNode>(object);
	if (other == nullptr) {
		return false;
	}
	return other->name == this->name;
}

bool Common::ExpressionProcessor::VariableNode::contains(shared_ptr<ExpressionNode> object) { return equals(object); }
