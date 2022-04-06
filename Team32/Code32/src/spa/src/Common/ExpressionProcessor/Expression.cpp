#include "Common/ExpressionProcessor/Expression.h"

using namespace Common::ExpressionProcessor;

Expression::Expression(std::shared_ptr<ExpressionNode> root, VarRefSet variables, ConstValSet constants)
	: root(std::move(root)), variables(std::move(variables)), constants(std::move(constants)) {
	if (this->root != nullptr) {
		traversal = this->root->traversal();
	}
}

ConstValSet Expression::getConstants() const { return constants; }

VarRefSet Expression::getVariables() const { return variables; }

bool Expression::contains(const Expression& other) const { return traversal.find(other.traversal) != std::string::npos; }

bool Expression::operator==(const Expression& other) const { return traversal == other.traversal; }
