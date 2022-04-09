#ifndef SPA_SRC_SP_NODE_PRINTREADNODE_TPP
#define SPA_SRC_SP_NODE_PRINTREADNODE_TPP

#include "SP/Node/PrintReadNode.h"

#include <utility>

using namespace std;

template <StmtType T>
SP::Node::PrintReadNode<T>::PrintReadNode(StmtRef stmt_no, unique_ptr<VariableNode> variable)
	: StatementNode(stmt_no), variable(move(variable)) {}

template <StmtType T>
inline constexpr bool always_false = false;

template <StmtType T>
StmtRef SP::Node::PrintReadNode<T>::extract(PKB::StorageUpdateInterface& /*pkb*/) const {
	static_assert(always_false<T>);
}

template <>
inline StmtRef SP::Node::PrintReadNode<StmtType::Print>::extract(PKB::StorageUpdateInterface& pkb) const {
	StmtRef stmt_ref = getStmtRef();
	pkb.setStmtType(stmt_ref, StmtType::Print);
	pkb.setUses(stmt_ref, variable->extract());
	return stmt_ref;
}

template <>
inline StmtRef SP::Node::PrintReadNode<StmtType::Read>::extract(PKB::StorageUpdateInterface& pkb) const {
	StmtRef stmt_ref = getStmtRef();
	pkb.setStmtType(stmt_ref, StmtType::Read);
	pkb.setModifies(stmt_ref, variable->extract());
	return stmt_ref;
}

template <StmtType T>
bool SP::Node::PrintReadNode<T>::equals(const shared_ptr<StatementNode>& object) const {
	shared_ptr<PrintReadNode<T>> other = dynamic_pointer_cast<PrintReadNode<T>>(object);
	if (other == nullptr) {
		return false;
	}
	return other->getStmtRef() == this->getStmtRef() && other->variable->equals(this->variable);
}

template <StmtType T>
unique_ptr<SP::Node::PrintReadNode<T>> SP::Node::PrintReadNode<T>::parse(Lexer& lex, StmtRef& statement_count) {
	unique_ptr<VariableNode> variable = VariableNode::parse(lex);
	lex.nextIf(SEMICOLON);
	return make_unique<PrintReadNode<T>>(statement_count++, move(variable));
}

#endif  // SPA_SRC_SP_NODE_PRINTREADNODE_TPP
