#include "SP/Processor.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>

#include "SP/Node/ProgramNode.h"

#define INVALID_INPUT_EXIT (1)
#define EXCEPTIONAL_EXIT (255)

SP::Processor::Processor(PKB::StorageUpdateInterface& pkb) : extractor(pkb) {}

void SP::Processor::process(std::string source) {
	try {
		std::unique_ptr<Node::ProgramNode> ast = parser.parse(std::move(source));
		extractor.extract(ast);
	} catch (const TokenizationException& e) {
		std::cout << e.what() << std::endl;
		exit(INVALID_INPUT_EXIT);
	} catch (const ParseException& e) {
		std::cout << e.what() << std::endl;
		exit(INVALID_INPUT_EXIT);
	} catch (const ExtractionException& e) {
		std::cout << e.what() << std::endl;
		exit(INVALID_INPUT_EXIT);
	} catch (const std::exception& e) {
		exit(EXCEPTIONAL_EXIT);
	}
}
