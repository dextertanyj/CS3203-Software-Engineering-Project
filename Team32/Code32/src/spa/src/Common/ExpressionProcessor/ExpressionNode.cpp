#include "Common/ExpressionProcessor/ExpressionNode.h"

#include <memory>
#include <utility>

#include "Common/Converter.h"

#define OPEN_PARENTHESES "("
#define CLOSE_PARENTHESES ")"

Common::ExpressionProcessor::ExpressionNode::ExpressionNode(std::vector<TokenType> tokens) : tokens(std::move(tokens)) {}

std::string Common::ExpressionProcessor::ExpressionNode::traversal() {
	std::string traversal;
	for (const auto& token : tokens) {
		traversal += OPEN_PARENTHESES;
		visit(Visitor{[&](const VarRef& name) { traversal.append(name); }, [&](ConstVal value) { traversal.append(std::to_string(value)); },
		              [&](const MathematicalOperator& opr) { traversal.append(Converter::mathematicalToString(opr)); },
		              [&](const std::shared_ptr<ExpressionNode>& node) { traversal.append(node->traversal()); }},
		      token);
		traversal += CLOSE_PARENTHESES;
	}
	return traversal;
}