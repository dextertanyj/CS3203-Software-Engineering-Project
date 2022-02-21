#ifndef SPA_SOURCEPROCESSOR_H
#define SPA_SOURCEPROCESSOR_H

#include <string>

#include "PKB/Storage.h"
#include "SP/DesignExtractor.h"
#include "SP/Parser.h"
#include "SP/SP.h"

class SP::Processor {
public:
	explicit Processor(PKB::Storage& pkb);
	void process(string source);

private:
	SP::Parser parser;
	SP::DesignExtractor extractor;
};

#endif  // SPA_SOURCEPROCESSOR_H
