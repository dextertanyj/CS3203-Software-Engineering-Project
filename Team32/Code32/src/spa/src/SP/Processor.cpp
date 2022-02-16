#include "SP/Processor.h"

#include <memory>

#include "SP/Lexer.h"

using namespace std;

SP::Processor::Processor(PKB &pkb) : parser(SP::Lexer()), extractor(pkb) {}

void SP::Processor::process(string source) {
	unique_ptr<Node::ProgramNode> ast = parser.parse(std::move(source));
	extractor.extract(std::move(ast));
}
