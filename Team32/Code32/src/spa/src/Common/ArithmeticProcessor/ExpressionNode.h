#ifndef SPA_EXPRESSIONNODE_H
#define SPA_EXPRESSIONNODE_H

#include <memory>

#include "Common/ArithmeticProcessor/ArithmeticProcessor.h"
#include "Common/TypeDefs.h"

using namespace std;

class Common::ArithmeticProcessor::ExpressionNode {
public:
	virtual bool equals(shared_ptr<ExpressionNode> object) = 0;
	virtual bool contains(shared_ptr<ExpressionNode> object) = 0;
};

#endif  // SPA_EXPRESSIONNODE_H
