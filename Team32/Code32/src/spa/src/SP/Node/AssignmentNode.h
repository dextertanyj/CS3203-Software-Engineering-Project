#ifndef SPA_ASSIGNMENTNODE_H
#define SPA_ASSIGNMENTNODE_H

#include <memory>
#include <string>

#include "SP/Lexer.h"
#include "SP/Node/ExpressionNode.h"
#include "SP/Node/Node.h"
#include "SP/Node/StatementNode.h"
#include "SP/Node/VariableNode.h"

using namespace std;

class SP::Node::AssignmentNode : public StatementNode {
public:
	AssignmentNode(StmtRef stmtNo, unique_ptr<VariableNode> assignee, unique_ptr<ExpressionNode> expression);
	static unique_ptr<AssignmentNode> parseAssignmentStatement(Lexer& lex, int& statement_count, string token);
	StmtInfo extract(PKB& pkb) override;
	bool equals(shared_ptr<StatementNode> object) override;

private:
	unique_ptr<VariableNode> assignee;
	unique_ptr<ExpressionNode> expression;
};

#endif  // SPA_ASSIGNMENTNODE_H
