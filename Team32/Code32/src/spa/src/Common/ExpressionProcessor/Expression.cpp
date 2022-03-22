#include "Common/ExpressionProcessor/Expression.h"

using namespace Common::ExpressionProcessor;

Expression::Expression(shared_ptr<ExpressionNode> root, unordered_set<VarRef> variables, unordered_set<ConstVal> constants)
	: root(move(root)), variables(move(variables)), constants(move(constants)) {
	if (this->root != nullptr) {
		traversal = this->root->traversal();
	}
}

unordered_set<ConstVal> Expression::getConstants() const { return constants; }

unordered_set<VarRef> Expression::getVariables() const { return variables; }

bool Expression::contains(const Expression& other) const { return traversal.find(other.traversal) != std::string::npos; }

bool Expression::operator==(const Expression& other) const { return traversal == other.traversal; }
