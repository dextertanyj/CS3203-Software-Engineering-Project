#include "SP/DesignExtractor.h"

#include "PKB/PKB.h"

using namespace std;
using namespace SP;

bool DesignExtractor::extract(PKB& pkb, unique_ptr<ProgramNode> ast) {
	ast->extract(pkb);
	return true;
}
