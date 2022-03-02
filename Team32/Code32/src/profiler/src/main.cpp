#include <algorithm>
#include <cctype>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>

#include "Wrapper.h"

#define MICRO_TO_MILLI 1000

static inline void printList(const std::string& prefix, const std::list<std::string>& result);
static inline void printSet(const std::string& prefix, const std::set<std::string>& result);
static inline void ltrim(std::string& str);
static inline void rtrim(std::string& str);
static inline void trim(std::string& str);

int main(int argc, char* argv[]) {
	if (argc != 3) {
		std::cout << "Usage: profiler source_file query_file" << std::endl;
		return 1;
	}

	std::string source_filename = std::string(argv[1]);
	std::string query_filename = std::string(argv[2]);
	std::ifstream query_file;
	try {
		query_file = std::ifstream(query_filename);
	} catch (const exception& e) {
		std::cout << e.what() << std::endl;
		return 1;
	}

	Wrapper application;
	try {
		std::cout << "Beginning to parse SIMPLE program." << std::endl;
		std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
		application.parse(source_filename);
		std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
		std::cout << "End of parsing SIMPLE program." << std::endl;
		double time_taken = (double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		time_taken /= MICRO_TO_MILLI;
		std::stringstream time_string;
		time_string << std::fixed << std::setprecision(3) << time_taken;
		std::cout << "Time taken: " + time_string.str() + "ms." << std::endl;
	} catch (exception& e) {
		std::cout << e.what() << std::endl;
		return 2;
	}

	try {
		std::string test;

		while (std::getline(query_file, test) && !test.empty()) {
			std::cout << test << std::endl;
			std::string declarations, query, expected, time_limit;
			std::list<std::string> expected_results;

			std::getline(query_file, declarations);
			std::getline(query_file, query);
			std::getline(query_file, expected);
			std::getline(query_file, time_limit);
			if (declarations.empty() || query.empty()) {
				throw invalid_argument("Test Error: Invalid query.");
			}

			std::cout << declarations << std::endl;
			std::cout << query << std::endl;
			std::cout << time_limit << std::endl;

			if (!expected.empty() && expected != "none") {
				size_t idx = expected.find(',');
				while (idx != std::string::npos) {
					std::string result = expected.substr(0, idx);
					trim(result);
					expected_results.push_back(result);
					expected.erase(0, idx + 1);
					idx = expected.find(',');
				}
				trim(expected);
				if (!expected.empty()) {
					expected_results.push_back(expected);
				}
			}

			std::list<std::string> results;
			std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
			application.evaluate(declarations.append(" ").append(query), results);
			std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
			printList("Your answer", results);
			printList("Correct answer", expected_results);
			double time_taken = (double)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			time_taken /= MICRO_TO_MILLI;
			std::set<std::string> actual_result_set = std::set(results.begin(), results.end());
			std::set<std::string> expected_result_set = std::set(expected_results.begin(), expected_results.end());

			if (actual_result_set != expected_result_set) {
				std::set<std::string> missing_set;
				std::set<std::string> additional_set;

				set_difference(expected_result_set.begin(), expected_result_set.end(), actual_result_set.begin(), actual_result_set.end(),
				               std::inserter(missing_set, missing_set.begin()));
				set_difference(actual_result_set.begin(), actual_result_set.end(), expected_result_set.begin(), expected_result_set.end(),
				               std::inserter(additional_set, additional_set.begin()));
				printSet("Missing", missing_set);
				printSet("Additional", additional_set);
			}
			std::stringstream time_string;
			time_string << std::fixed << std::setprecision(3) << time_taken;
			std::cout << "Time taken: " + time_string.str() + "ms." << std::endl;

			if (time_taken > stoi(time_limit)) {
				std::cout << "Time limit exceeded." << std::endl;
			}
		}
	} catch (const exception& e) {
		std::cout << e.what() << std::endl;
		return 3;
	}
	return 0;
};

static inline void printList(const std::string& prefix, const std::list<std::string>& result) {
	std::cout << prefix << ": ";
	for (const std::string& str : result) {
		std::cout << str << " ";
	}
	std::cout << std::endl;
}

static inline void printSet(const std::string& prefix, const std::set<std::string>& result) {
	std::cout << prefix << ": ";
	for (const std::string& str : result) {
		std::cout << str << " ";
	}
	std::cout << std::endl;
}

// Adapted from: https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
static inline void ltrim(std::string& str) {
	str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char chr) { return std::isspace(chr) == 0; }));
}

static inline void rtrim(std::string& str) {
	str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char chr) { return std::isspace(chr) == 0; }).base(), str.end());
}

static inline void trim(std::string& str) {
	ltrim(str);
	rtrim(str);
}
