#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_TERMINALNODE_TPP
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_TERMINALNODE_TPP

#include "TerminalNode.h"

template <typename T>
Common::ExpressionProcessor::TerminalNode<T>::TerminalNode(T value) : AtomicNode({value}) {}

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_TERMINALNODE_TPP