#include "SP/DesignExtractor.h"

SP::DesignExtractor::DesignExtractor(PKB::StorageUpdateInterface& pkb) : pkb(pkb) {}

void SP::DesignExtractor::extract(const std::unique_ptr<Node::ProgramNode>& ast) {
	ast->extract(pkb);
	pkb.populateComplexRelations();
}
