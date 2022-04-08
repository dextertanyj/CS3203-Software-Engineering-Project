#include "SP/DesignExtractor.h"

#include "PKB/PKB.h"

SP::DesignExtractor::DesignExtractor(PKB::StorageUpdateInterface& pkb) : pkb(pkb) {}

void SP::DesignExtractor::extract(const std::unique_ptr<Node::ProgramNode>& ast) {
	ast->extract(pkb);
	try {
		pkb.populateComplexRelations();
	} catch (const PKB::CallGraphException& e) {
		throw ExtractionException(e.what());
	}
}
