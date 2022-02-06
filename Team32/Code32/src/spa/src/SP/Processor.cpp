#include "SP/Processor.h"

#include <memory>
#include <utility>
using namespace std;
Processor::Processor(PKB &pkb) : parser(SP::Lexer()), extractor(pkb) {}

bool Processor::process(string source) {
	unique_ptr<ProgramNode> ast = parser.parse(std::move(source));
	return extractor.extract(std::move(ast));
}
