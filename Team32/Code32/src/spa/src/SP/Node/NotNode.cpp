#include "SP/Node/NotNode.h"

NotNode::NotNode(unique_ptr<ConditionalExpressionNode> expression) : expression(move(expression)) {}
