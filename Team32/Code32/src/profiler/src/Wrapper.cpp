#include "Wrapper.h"

#include <fstream>
#include <utility>

Wrapper::Wrapper() : source_processor(pkb), query_processor(pkb) {}

void Wrapper::parse(const std::string& filename) {
	std::ifstream file(filename);
	std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	source_processor.process(source);
}

void Wrapper::evaluate(const std::string& query, list<std::string>& results) {
	std::vector<std::string> internal_results = query_processor.processQuery(query);
	results.insert(results.end(), internal_results.begin(), internal_results.end());
}
