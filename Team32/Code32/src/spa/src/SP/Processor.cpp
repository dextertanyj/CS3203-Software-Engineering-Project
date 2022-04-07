#include "SP/Processor.h"

#include <iostream>
#include <memory>
#include <utility>

SP::Processor::Processor(PKB::StorageUpdateInterface& pkb) : extractor(pkb) {}

void SP::Processor::process(std::string source) {
	try {
		std::unique_ptr<Node::ProgramNode> ast = parser.parse(std::move(source));
		extractor.extract(ast);
	} catch (const TokenizationException& e) {
		cout << e.what() << endl;
		exit(1);
	} catch (const ParseException& e) {
		cout << e.what() << endl;
		exit(2);
	} catch (const ExtractionException& e) {
		cout << e.what() << endl;
		exit(3);
	}
}
