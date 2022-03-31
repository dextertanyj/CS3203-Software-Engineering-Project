#include "SP/Processor.h"

#include <iostream>
#include <memory>

#include "SP/Lexer.h"

SP::Processor::Processor(PKB::StorageUpdateInterface& pkb) : extractor(pkb) {}

void SP::Processor::process(string source) {
	try {
		unique_ptr<Node::ProgramNode> ast = parser.parse(move(source));
		extractor.extract(ast);
	} catch (const TokenizationException& e) {
		cout << e.what() << endl;
		exit(1);
	} catch (const ParseException& e) {
		cout << e.what() << endl;
		exit(2);
	}
}
