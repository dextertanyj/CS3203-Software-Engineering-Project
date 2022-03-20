#include "Common/ExpressionProcessor/ExpressionNode.h"

#include <utility>

#include "Common/Converter.h"

#define DELIMITER ","

Common::ExpressionProcessor::ExpressionNode::ExpressionNode(
	vector<variant<shared_ptr<ExpressionNode>, MathematicalOperator, VarRef, ConstVal>> tokens)
	: tokens(move(tokens)) {}

string Common::ExpressionProcessor::ExpressionNode::traversal() {
	string traversal;
	for (const auto& token : tokens) {
		visit(Visitor{[&](const VarRef& name) { traversal.append(name); }, [&](ConstVal value) { traversal.append(to_string(value)); },
		              [&](const MathematicalOperator& opr) { traversal.append(Converter::mathematicalToString(opr)); },
		              [&](const shared_ptr<ExpressionNode>& node) { traversal.append(node->traversal()); }},
		      token);
		traversal += DELIMITER;  // Prevent a + bc from matching ab + c in postfix notation.
	}
	return traversal;
}