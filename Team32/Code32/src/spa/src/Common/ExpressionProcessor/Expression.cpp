#include "Common/ExpressionProcessor/Expression.h"

#include <utility>

using namespace Common::ExpressionProcessor;

Expression::Expression(std::shared_ptr<ExpressionNode> root, std::unordered_set<VarRef> variables, std::unordered_set<ConstVal> constants)
	: root(std::move(root)), variables(std::move(variables)), constants(std::move(constants)) {
	if (this->root != nullptr) {
		traversal = this->root->traversal();
	}
}

std::unordered_set<ConstVal> Expression::getConstants() const { return constants; }

std::unordered_set<VarRef> Expression::getVariables() const { return variables; }

bool Expression::contains(const Expression& other) const { return traversal.find(other.traversal) != std::string::npos; }

bool Expression::operator==(const Expression& other) const { return traversal == other.traversal; }
