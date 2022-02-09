#include "SP/Node/ReadNode.h"

SP::Node::ReadNode::ReadNode(StmtRef stmtNo, unique_ptr<VariableNode> variable) : StatementNode(stmtNo), variable(move(variable)) {}

StmtInfo SP::Node::ReadNode::extract(PKB& pkb) {
	StmtRef stmt_ref = getStmtRef();
	pkb.setModifies(stmt_ref, variable->extract());
	pkb.setStmtType(stmt_ref, StmtType::Read);
	return {stmt_ref, StmtType::Read};
}

bool SP::Node::ReadNode::equals(shared_ptr<StatementNode> object) {
    shared_ptr<ReadNode> other = dynamic_pointer_cast<ReadNode>(object);
    if (other == nullptr) {
        return false;
    }
    return other->getStmtRef() == this->getStmtRef() && other->variable->equals(move(this->variable));
}

unique_ptr<SP::Node::ReadNode> SP::Node::ReadNode::parseReadStatement(Lexer &lex, int &statement_count) {
    unique_ptr<VariableNode> variable = VariableNode::parseVariable(lex);
    lex.nextIf(";");
    return make_unique<ReadNode>(statement_count++, move(variable));
}
