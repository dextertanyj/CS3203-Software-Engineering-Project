#ifndef SPA_CONSTANTNODE_H
#define SPA_CONSTANTNODE_H

#include <memory>

#include "SP/Lexer.h"
#include "SP/Node/ReferenceNode.h"

class ConstantNode : public ReferenceNode {
public:
	explicit ConstantNode(int value);
	static unique_ptr<ConstantNode> parseConstant(Lexer& lex);

private:
	int value;
};

#endif  // SPA_CONSTANTNODE_H
