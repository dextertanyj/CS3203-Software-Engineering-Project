#ifndef SPA_WHILENODE_H
#define SPA_WHILENODE_H

#include <string>

#include "SP/Node/ConditionalExpressionNode.h"
#include "SP/Node/StatementListNode.h"
#include "SP/Node/StatementNode.h"

using namespace std;

class WhileNode : public StatementNode {
public:
	WhileNode(StmtRef stmtNo, unique_ptr<ConditionalExpressionNode> condExpr, unique_ptr<StatementListNode> stmtLst);
	static unique_ptr<WhileNode> parseWhileStatement(Lexer& lex, int& statement_count);
    bool equals(shared_ptr<StatementNode> object) override;

private:
	unique_ptr<ConditionalExpressionNode> condExpr;
	unique_ptr<StatementListNode> stmtLst;
};

#endif  // SPA_WHILENODE_H
