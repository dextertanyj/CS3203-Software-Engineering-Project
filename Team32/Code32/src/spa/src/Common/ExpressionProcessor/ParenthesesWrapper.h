#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_PARENTHESESWRAPPER_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_PARENTHESESWRAPPER_H

#include <memory>

#include "Common/ExpressionProcessor/ExpressionNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"

class Common::ExpressionProcessor::ParenthesesWrapper {
public:
	explicit ParenthesesWrapper(const ParenthesizedExpression& expression);
	[[nodiscard]] std::shared_ptr<ExpressionNode> getExpression() const;

private:
	std::shared_ptr<ExpressionNode> expression;
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_PARENTHESESWRAPPER_H
