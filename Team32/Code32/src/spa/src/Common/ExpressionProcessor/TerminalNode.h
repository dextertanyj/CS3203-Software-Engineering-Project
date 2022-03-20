#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_TERMINALNODE_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_TERMINALNODE_H

#include <memory>

#include "Common/ExpressionProcessor/AtomicNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"

using std::shared_ptr;

template <typename T>
class Common::ExpressionProcessor::TerminalNode : public Common::ExpressionProcessor::AtomicNode {
public:
	explicit TerminalNode(T value);
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_TERMINALNODE_H
