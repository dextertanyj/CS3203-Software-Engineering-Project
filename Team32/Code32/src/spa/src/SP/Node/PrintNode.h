#ifndef SPA_PRINTNODE_H
#define SPA_PRINTNODE_H

#include <memory>
#include <string>

#include "Common/TypeDefs.h"
#include "PKB/PKB.h"
#include "SP/Lexer.h"
#include "SP/Node/StatementNode.h"
#include "SP/Node/VariableNode.h"

using namespace std;

class SP::Node::PrintNode : public StatementNode {
public:
	PrintNode(StmtRef stmt_no, unique_ptr<VariableNode> variable);
	StmtRef extract(PKB& pkb) override;
	bool equals(const shared_ptr<StatementNode>& object) override;
	static unique_ptr<PrintNode> parsePrintStatement(Lexer& lex, StmtRef& statement_count);

private:
	shared_ptr<VariableNode> variable;
};

#endif  // SPA_PRINTNODE_H
