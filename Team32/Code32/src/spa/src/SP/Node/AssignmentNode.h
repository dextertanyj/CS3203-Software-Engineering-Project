#ifndef SPA_SRC_SP_NODE_ASSIGNMENTNODE_H
#define SPA_SRC_SP_NODE_ASSIGNMENTNODE_H

#include <memory>
#include <string>

#include "Common/TypeDefs.h"
#include "PKB/StorageUpdateInterface.h"
#include "SP/Lexer.h"
#include "SP/Node/ExpressionNode.h"
#include "SP/Node/Node.h"
#include "SP/Node/StatementNode.h"
#include "SP/Node/VariableNode.h"

class SP::Node::AssignmentNode : public StatementNode {
public:
	AssignmentNode(StmtRef stmt_no, std::unique_ptr<VariableNode> assignee, std::unique_ptr<ExpressionNode> expression);
	StmtRef extract(PKB::StorageUpdateInterface& pkb) const override;
	[[nodiscard]] bool equals(const std::shared_ptr<StatementNode>& object) const override;

	static std::unique_ptr<AssignmentNode> parseAssignmentStatement(Lexer& lex, StmtRef& statement_count, std::string token);

private:
	std::shared_ptr<VariableNode> assignee;
	std::shared_ptr<ExpressionNode> expression;
};

#endif  // SPA_SRC_SP_NODE_ASSIGNMENTNODE_H
