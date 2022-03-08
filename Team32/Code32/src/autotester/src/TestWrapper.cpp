#include "TestWrapper.h"

#include <fstream>
#include <string>

#include "AbstractWrapper.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
	if (wrapper == 0) wrapper = new TestWrapper;
	return wrapper;
}

// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;
AbstractWrapper::~AbstractWrapper() = default;

// a default constructor
TestWrapper::TestWrapper() : source_processor(pkb), query_processor(pkb) {}

TestWrapper::~TestWrapper() = default;

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	std::ifstream file(filename);
	std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	source_processor.process(source);
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results) {
	std::vector<std::string> internal_results = query_processor.processQuery(query);
	results.insert(results.end(), internal_results.begin(), internal_results.end());
}
