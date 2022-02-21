#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPROCESSOR_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPROCESSOR_H_

#include <string>
#include <vector>

#include "QP/QueryEvaluator.h"
#include "QP/QueryFormatter.h"

using std::string;
using std::vector;

class QP::QueryProcessor {
public:
	explicit QueryProcessor(PKB::Storage& pkb);
	vector<string> processQuery(string query);

private:
	QueryEvaluator evaluator;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPROCESSOR_H_