#ifndef SPA_PARENTHESESWRAPPER_H
#define SPA_PARENTHESESWRAPPER_H

#include <memory>

#include "Common/ExpressionProcessor/ExpressionNode.h"

class Common::ExpressionProcessor::ParenthesesWrapper : public ExpressionNode {
public:
	explicit ParenthesesWrapper(shared_ptr<ExpressionNode> expression);
	bool equals(shared_ptr<ExpressionNode> object) override;
	bool contains(shared_ptr<ExpressionNode> object) override;
	shared_ptr<ExpressionNode> getExpression();

private:
	shared_ptr<ExpressionNode> expression;
};

#endif  // SPA_PARENTHESESWRAPPER_H
