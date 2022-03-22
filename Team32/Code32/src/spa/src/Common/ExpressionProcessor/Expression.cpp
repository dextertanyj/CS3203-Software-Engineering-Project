#include "Common/ExpressionProcessor/Expression.h"

using namespace Common::ExpressionProcessor;

Expression::Expression(shared_ptr<ExpressionNode> root, unordered_set<VarRef> variables, unordered_set<ConstVal> constants)
	: root(move(root)), variables(move(variables)), constants(move(constants)) {
	if (this->root != nullptr) {
		traversal = this->root->traversal();
	}
}

bool Expression::contains(const Expression& other) { return traversal.find(other.traversal) != std::string::npos; }

bool Expression::equals(const Expression& other) { return traversal == other.traversal; }

unordered_set<ConstVal> Expression::getConstants() { return constants; }

unordered_set<VarRef> Expression::getVariables() { return variables; }
