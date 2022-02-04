#ifndef SPA_PRINTNODE_H
#define SPA_PRINTNODE_H

#include <string>

#include "SP/Node/StatementNode.h"
#include "SP/Node/VariableNode.h"

using namespace std;

class PrintNode : public StatementNode {
public:
	PrintNode(StmtRef stmtNo, unique_ptr<VariableNode> variable);
	StmtInfo extract(PKB& pkb) override;

    bool equals(shared_ptr<StatementNode> object) override;
private:
	unique_ptr<VariableNode> variable;
};

#endif  // SPA_PRINTNODE_H
