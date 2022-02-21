#ifndef SPA_DESIGNEXTRACTOR_H
#define SPA_DESIGNEXTRACTOR_H

#include <memory>

#include "PKB/Storage.h"
#include "SP/Node/ProgramNode.h"
#include "SP/SP.h"

using namespace std;

class SP::DesignExtractor {
public:
	explicit DesignExtractor(PKB::Storage& pkb);
	void extract(unique_ptr<Node::ProgramNode> ast);

private:
	PKB::Storage& pkb;
};

#endif  // SPA_DESIGNEXTRACTOR_H
