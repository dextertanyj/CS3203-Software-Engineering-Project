#ifndef SPA_SRC_SP_NODE_ASSIGNMENTNODE_H
#define SPA_SRC_SP_NODE_ASSIGNMENTNODE_H

#include <memory>
#include <string>

#include "Common/TypeDefs.h"
#include "SP/Lexer.h"
#include "SP/Node/ExpressionNode.h"
#include "SP/Node/Node.h"
#include "SP/Node/StatementNode.h"
#include "SP/Node/VariableNode.h"

using namespace std;

class SP::Node::AssignmentNode : public StatementNode {
public:
	AssignmentNode(StmtRef stmt_no, unique_ptr<VariableNode> assignee, unique_ptr<ExpressionNode> expression);
	static unique_ptr<AssignmentNode> parseAssignmentStatement(Lexer& lex, StmtRef& statement_count, string token);
	StmtRef extract(PKB::Storage& pkb) override;
	bool equals(const shared_ptr<StatementNode>& object) override;

private:
	shared_ptr<VariableNode> assignee;
	shared_ptr<ExpressionNode> expression;
};

#endif  // SPA_SRC_SP_NODE_ASSIGNMENTNODE_H
