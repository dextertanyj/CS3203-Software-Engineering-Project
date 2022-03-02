#ifndef SPA_WRAPPER_H
#define SPA_WRAPPER_H

#include <string>
#include <list>

#include "PKB/Storage.h"
#include "SP/Processor.h"
#include "QP/QueryProcessor.h"

class Wrapper {
public:
	Wrapper();
	void parse(const std::string& filename);
	void evaluate(const std::string& query, std::list<std::string>& results);

private:
	PKB::Storage pkb;
	SP::Processor source_processor;
	QP::QueryProcessor query_processor;
};

#endif  // SPA_WRAPPER_H
