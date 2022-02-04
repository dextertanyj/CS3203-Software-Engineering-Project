#ifndef SPA_DESIGNEXTRACTOR_H
#define SPA_DESIGNEXTRACTOR_H

#include <memory>

#include "PKB/PKB.h"
#include "SP/Node/ProgramNode.h"
#include "SP/SP.h"

using namespace std;

class SP::DesignExtractor {
public:
	bool extract(PKB& pkb, unique_ptr<ProgramNode> ast);
};

#endif  // SPA_DESIGNEXTRACTOR_H
