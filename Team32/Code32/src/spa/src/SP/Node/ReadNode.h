#ifndef SPA_READNODE_H
#define SPA_READNODE_H

#include <string>

#include "SP/Node/StatementNode.h"
#include "SP/Node/VariableNode.h"

using namespace std;

class ReadNode : public StatementNode {
public:
	ReadNode(StmtRef stmtNo, unique_ptr<VariableNode> variable);
	StmtInfo extract(PKB& pkb) override;
    bool equals(shared_ptr<StatementNode> object) override;
    static unique_ptr<ReadNode> parseReadStatement(Lexer& lex, int& statement_count);

private:
	unique_ptr<VariableNode> variable;
};

#endif  // SPA_READNODE_H
