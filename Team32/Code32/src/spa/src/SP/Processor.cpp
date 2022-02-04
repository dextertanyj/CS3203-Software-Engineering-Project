#include "SP/Processor.h"
#include <memory>
#include <utility>
using namespace std;
Processor::Processor(SP::Parser parser, SP::DesignExtractor extractor) : parser(std::move(parser)), extractor(extractor) {}

void Processor::process(PKB& pkb, string source) {
	unique_ptr<ProgramNode> ast = parser.parse(std::move(source));
	extractor.extract(pkb, std::move(ast));
}
