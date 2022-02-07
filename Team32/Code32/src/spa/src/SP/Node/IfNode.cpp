#include "SP/Node/IfNode.h"

using namespace std;
using namespace SP;

IfNode::IfNode(StmtRef stmtNo, unique_ptr<ConditionalExpressionNode> condExpr, unique_ptr<StatementListNode> ifStmtLst,
               unique_ptr<StatementListNode> elseStmtLst)
	: StatementNode(stmtNo), condExpr(move(condExpr)), ifStmtLst(move(ifStmtLst)), elseStmtLst(move(elseStmtLst)) {}

unique_ptr<IfNode> IfNode::parseIfStatement(Lexer& lex, int& statement_count) {
	StmtRef statement_index = statement_count++;
	lex.nextIf("(");
	unique_ptr<ConditionalExpressionNode> condition = ConditionalExpressionNode::parseConditionalExpression(lex);
    lex.nextIf(")");
    lex.nextIf("then");
	lex.nextIf("{");
	unique_ptr<StatementListNode> then_statements = StatementListNode::parseStatementList(lex, statement_count);
	lex.nextIf("}");
	lex.nextIf("else");
	lex.nextIf("{");
	unique_ptr<StatementListNode> else_statements = StatementListNode::parseStatementList(lex, statement_count);
	lex.nextIf("}");
	return make_unique<IfNode>(statement_index, move(condition), move(then_statements), move(else_statements));
}

bool IfNode::equals(shared_ptr<StatementNode> object) {
    shared_ptr<IfNode> other = dynamic_pointer_cast<IfNode>(object);
    if (other == nullptr) {
        return false;
    }
    return this->stmtNo == other->stmtNo && this->condExpr->equals(move(other->condExpr))
        && this->ifStmtLst->equals(move(other->ifStmtLst))
        && this->elseStmtLst->equals(move(other->elseStmtLst));
}
