#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_PARENTHESESWRAPPER_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_PARENTHESESWRAPPER_H

#include <memory>

#include "Common/ExpressionProcessor/ExpressionNode.h"

using namespace std;

class Common::ExpressionProcessor::ParenthesesWrapper {
public:
	explicit ParenthesesWrapper(ParenthesizedExpression expression);
	[[nodiscard]] shared_ptr<ExpressionNode> getExpression() const;

private:
	shared_ptr<ExpressionNode> expression;
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_PARENTHESESWRAPPER_H
