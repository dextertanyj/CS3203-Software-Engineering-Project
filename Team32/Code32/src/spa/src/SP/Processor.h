#ifndef SPA_SRC_SP_PROCESSOR_H
#define SPA_SRC_SP_PROCESSOR_H

#include <string>

#include "SP/DesignExtractor.h"
#include "SP/Parser.h"
#include "SP/SP.h"
#include "PKB/StorageUpdateInterface.h"

class SP::Processor {
public:
	explicit Processor(PKB::StorageUpdateInterface& pkb);
	void process(std::string source);

private:
	SP::Parser parser;
	SP::DesignExtractor extractor;
};

#endif  // SPA_SRC_SP_PROCESSOR_H
