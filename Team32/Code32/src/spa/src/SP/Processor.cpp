#include "SP/Processor.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>

#include "SP/Node/ProgramNode.h"

SP::Processor::Processor(PKB::StorageUpdateInterface& pkb) : extractor(pkb) {}

void SP::Processor::process(std::string source) {
	try {
		std::unique_ptr<Node::ProgramNode> ast = parser.parse(std::move(source));
		extractor.extract(ast);
	} catch (const TokenizationException& e) {
		std::cout << e.what() << std::endl;
		exit(1);
	} catch (const ParseException& e) {
		std::cout << e.what() << std::endl;
		exit(2);
	} catch (const ExtractionException& e) {
		std::cout << e.what() << std::endl;
		exit(3);
	}
}
