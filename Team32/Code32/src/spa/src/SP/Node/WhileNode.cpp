#include "SP/Node/WhileNode.h"

#include <utility>
#include <vector>

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"

using namespace std;

SP::Node::WhileNode::WhileNode(StmtRef stmt_no, unique_ptr<ExpressionNode> cond_expr, unique_ptr<StatementListNode> stmt_list)
	: StatementNode(stmt_no), cond_expr(move(cond_expr)), stmt_list(move(stmt_list)) {}

StmtRef SP::Node::WhileNode::extract(PKB::StorageUpdateInterface& pkb) const {
	StmtRef stmt_ref = getStmtRef();
	pkb.setStmtType(stmt_ref, StmtType::While);
	Common::EP::Expression expression = cond_expr->extract();
	pkb.setConstant(expression.getConstants());
	pkb.setUses(stmt_ref, expression.getVariables());
	pkb.setWhileControl(stmt_ref, expression.getVariables());
	vector<StmtRef> children = stmt_list->extract(pkb);
	for (auto iter = children.begin(); iter < children.end(); ++iter) {
		pkb.setParent(stmt_ref, *iter);
	}
	pkb.setNext(stmt_ref, children.front());
	pkb.setNext(children.back(), stmt_ref);
	return stmt_ref;
}

bool SP::Node::WhileNode::equals(const shared_ptr<StatementNode>& object) const {
	shared_ptr<WhileNode> other = dynamic_pointer_cast<WhileNode>(object);
	if (other == nullptr) {
		return false;
	}
	return this->getStmtRef() == other->getStmtRef() && this->cond_expr->equals(other->cond_expr) &&
	       this->stmt_list->equals(other->stmt_list);
}

unique_ptr<SP::Node::WhileNode> SP::Node::WhileNode::parse(Lexer& lex, StmtRef& statement_count) {
	StmtRef statement_index = statement_count++;
	lex.nextIf(OPEN_PARENTHESES);
	unique_ptr<ExpressionNode> condition = ExpressionNode::parse(lex, Common::EP::ExpressionType::Logical);
	lex.nextIf(CLOSE_PARENTHESES);
	lex.nextIf(OPEN_BRACES);
	unique_ptr<StatementListNode> statements = StatementListNode::parse(lex, statement_count);
	lex.nextIf(CLOSE_BRACES);
	return make_unique<WhileNode>(statement_index, move(condition), move(statements));
}
