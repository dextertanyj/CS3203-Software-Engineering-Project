#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_TERMINALNODE_TPP
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_TERMINALNODE_TPP

#include "TerminalNode.h"

template <typename T>
Common::ExpressionProcessor::TerminalNode<T>::TerminalNode(T value) : value(value) {}

template <typename T>
bool Common::ExpressionProcessor::TerminalNode<T>::equals(shared_ptr<ExpressionNode> object) {
	shared_ptr<TerminalNode<T>> other = dynamic_pointer_cast<TerminalNode<T>>(object);
	if (other == nullptr) {
		return false;
	}
	return other->value == this->value;
}
template <typename T>
bool Common::ExpressionProcessor::TerminalNode<T>::contains(shared_ptr<ExpressionNode> object) {
	return equals(object);
}

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_TERMINALNODE_TPP