#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_TERMINALNODE_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_TERMINALNODE_H

#include "Common/ExpressionProcessor/AtomicNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"

template <typename T>
class Common::ExpressionProcessor::TerminalNode : public AtomicNode {
public:
	explicit TerminalNode(T value);
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_TERMINALNODE_H
