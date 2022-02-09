#include "SP/DesignExtractor.h"

#include "PKB/PKB.h"

using namespace std;

SP::DesignExtractor::DesignExtractor(PKB &pkb) : pkb(pkb) {}

bool SP::DesignExtractor::extract(unique_ptr<Node::ProgramNode> ast) {
	ast->extract(pkb);
	return true;
}
