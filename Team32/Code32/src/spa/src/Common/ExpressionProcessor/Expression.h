#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSION_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSION_H

#include <memory>
#include <string>
#include <unordered_set>
#include <variant>

#include "Common/ExpressionProcessor/ExpressionNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/ExpressionProcessor/LexerInterface.h"
#include "Common/ExpressionProcessor/OperatorAcceptor.h"
#include "Common/TypeDefs.h"

using namespace std;

class Common::ExpressionProcessor::Expression {
public:
	Expression(shared_ptr<ExpressionNode> root, unordered_set<VarRef> variables, unordered_set<ConstVal> constants);
	[[nodiscard]] unordered_set<ConstVal> getConstants() const;
	[[nodiscard]] unordered_set<VarRef> getVariables() const;
	[[nodiscard]] bool operator==(const Expression& other) const;
	[[nodiscard]] bool contains(const Expression& other) const;

private:
	shared_ptr<ExpressionNode> root;
	string traversal;
	unordered_set<VarRef> variables;
	unordered_set<ConstVal> constants;
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSION_H
