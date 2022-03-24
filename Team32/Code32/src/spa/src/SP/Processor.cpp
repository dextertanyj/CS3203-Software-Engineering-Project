#include "SP/Processor.h"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>

#include "SP/Lexer.h"

using namespace std;

SP::Processor::Processor(PKB::StorageUpdateInterface& pkb) : parser(SP::Lexer()), extractor(pkb) {}

void SP::Processor::process(string source) {
	try {
		unique_ptr<Node::ProgramNode> ast = parser.parse(std::move(source));
		extractor.extract(std::move(ast));
	} catch (const TokenizationException& e) {
		cout << e.what() << endl;
		exit(1);
	} catch (const ParseException& e) {
		cout << e.what() << endl;
		exit(2);
	}
}
