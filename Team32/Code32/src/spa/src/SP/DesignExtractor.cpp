#include "SP/DesignExtractor.h"

#include "PKB/PKB.h"

using namespace std;
using namespace SP;

DesignExtractor::DesignExtractor(PKB &pkb) : pkb(pkb) {}

bool DesignExtractor::extract(unique_ptr<ProgramNode> ast) {
	ast->extract(pkb);
	return true;
}
