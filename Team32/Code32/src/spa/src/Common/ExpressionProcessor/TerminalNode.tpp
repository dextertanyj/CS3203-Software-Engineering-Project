#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_TERMINALNODE_TPP
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_TERMINALNODE_TPP

#include "Common/ExpressionProcessor/TerminalNode.h"

#include <utility>

template <typename T>
Common::ExpressionProcessor::TerminalNode<T>::TerminalNode(T value) : AtomicNode({std::move(value)}) {}

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_TERMINALNODE_TPP