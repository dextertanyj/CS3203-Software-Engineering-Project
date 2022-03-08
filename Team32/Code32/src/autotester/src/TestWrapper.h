#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <iostream>
#include <list>
#include <string>

// include your other headers here
#include "AbstractWrapper.h"
#include "PKB/Storage.h"
#include "QP/QueryProcessor.h"
#include "SP/Processor.h"

class TestWrapper : public AbstractWrapper {
public:
	// default constructor
	TestWrapper();

	// destructor
	~TestWrapper();

	// method for parsing the SIMPLE source
	virtual void parse(std::string filename);

	// method for evaluating a query
	virtual void evaluate(std::string query, std::list<std::string>& results);

private:
	PKB::Storage pkb;
	SP::Processor source_processor;
	QP::QueryProcessor query_processor;
};

#endif
