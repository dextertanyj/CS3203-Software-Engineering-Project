#ifndef SPA_DESIGNEXTRACTOR_H
#define SPA_DESIGNEXTRACTOR_H

#include <memory>

#include "PKB/PKB.h"
#include "SP/Node/ProgramNode.h"
#include "SP/SP.h"

using namespace std;

class SP::DesignExtractor {
public:
	explicit DesignExtractor(PKB &pkb);
	bool extract(unique_ptr<Node::ProgramNode> ast);

private:
	PKB &pkb;
};

#endif  // SPA_DESIGNEXTRACTOR_H
