#include "SP/DesignExtractor.h"

#include "PKB/PKB.h"

using namespace std;

SP::DesignExtractor::DesignExtractor(PKB& pkb) : pkb(pkb) {}

void SP::DesignExtractor::extract(unique_ptr<Node::ProgramNode> ast) {
	ast->extract(pkb);
	pkb.populateComplexRelations();
}
