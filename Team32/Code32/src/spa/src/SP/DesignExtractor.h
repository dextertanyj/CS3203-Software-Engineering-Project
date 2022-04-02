#ifndef SPA_SRC_SP_DESIGNEXTRACTOR_H
#define SPA_SRC_SP_DESIGNEXTRACTOR_H

#include <memory>

#include "SP/Node/ProgramNode.h"
#include "SP/SP.h"

class SP::DesignExtractor {
public:
	explicit DesignExtractor(PKB::StorageUpdateInterface& pkb);
	void extract(const std::unique_ptr<Node::ProgramNode>& ast);

private:
	PKB::StorageUpdateInterface& pkb;
};

#endif  // SPA_SRC_SP_DESIGNEXTRACTOR_H
