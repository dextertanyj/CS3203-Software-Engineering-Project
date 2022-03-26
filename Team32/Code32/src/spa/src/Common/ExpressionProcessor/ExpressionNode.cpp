#include "Common/ExpressionProcessor/ExpressionNode.h"

#include <utility>

#include "Common/Converter.h"

#define OPEN_PARENTHESES "("
#define CLOSE_PARENTHESES ")"

Common::ExpressionProcessor::ExpressionNode::ExpressionNode(
	vector<variant<shared_ptr<ExpressionNode>, MathematicalOperator, VarRef, ConstVal>> tokens)
	: tokens(move(tokens)) {}

string Common::ExpressionProcessor::ExpressionNode::traversal() {
	string traversal;
	for (const auto& token : tokens) {
		traversal += OPEN_PARENTHESES;
		visit(Visitor{[&](const VarRef& name) { traversal.append(name); }, [&](ConstVal value) { traversal.append(to_string(value)); },
		              [&](const MathematicalOperator& opr) { traversal.append(Converter::mathematicalToString(opr)); },
		              [&](const shared_ptr<ExpressionNode>& node) { traversal.append(node->traversal()); }},
		      token);
		traversal += CLOSE_PARENTHESES;
	}
	return traversal;
}