#ifndef SPA_ASSIGNMENTNODE_H
#define SPA_ASSIGNMENTNODE_H

#include <memory>
#include <string>

#include "SP/Lexer.h"
#include "SP/Node/ArithmeticExpressionNode.h"
#include "SP/Node/StatementNode.h"
#include "SP/Node/VariableNode.h"

using namespace std;
using namespace SP;

class AssignmentNode : public StatementNode {
public:
	AssignmentNode(StmtRef stmtNo, unique_ptr<VariableNode> assignee, unique_ptr<ArithmeticExpressionNode> expression);
	static unique_ptr<AssignmentNode> parseAssignmentStatement(Lexer& lex, int& statement_count, string token);

private:
	unique_ptr<VariableNode> assignee;
	unique_ptr<ArithmeticExpressionNode> expression;
};

#endif  // SPA_ASSIGNMENTNODE_H
