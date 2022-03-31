#include "SP/DesignExtractor.h"

#include "PKB/Storage.h"

SP::DesignExtractor::DesignExtractor(PKB::StorageUpdateInterface& pkb) : pkb(pkb) {}

void SP::DesignExtractor::extract(const unique_ptr<Node::ProgramNode>& ast) {
	ast->extract(pkb);
	pkb.populateComplexRelations();
}
