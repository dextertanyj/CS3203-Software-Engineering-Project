#include "SP/DesignExtractor.h"

#include "PKB/Storage.h"

using namespace std;

SP::DesignExtractor::DesignExtractor(PKB::StorageUpdateInterface& pkb) : pkb(pkb) {}

void SP::DesignExtractor::extract(unique_ptr<Node::ProgramNode> ast) {
	ast->extract(pkb);
	pkb.populateComplexRelations();
}
