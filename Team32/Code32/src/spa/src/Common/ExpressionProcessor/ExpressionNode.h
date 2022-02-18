#ifndef SPA_EXPRESSIONNODE_H
#define SPA_EXPRESSIONNODE_H

#include <memory>

#include "Common/ExpressionProcessor/ExpressionProcessor.h"

using namespace std;

class Common::ExpressionProcessor::ExpressionNode {
public:
	virtual bool equals(shared_ptr<ExpressionNode> object) = 0;
	virtual bool contains(shared_ptr<ExpressionNode> object) = 0;
	virtual ~ExpressionNode() = default;
};

#endif  // SPA_EXPRESSIONNODE_H
