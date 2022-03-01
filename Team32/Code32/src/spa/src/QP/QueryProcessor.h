#ifndef SPA_SRC_QP_QUERYPROCESSOR_H
#define SPA_SRC_QP_QUERYPROCESSOR_H

#include <string>
#include <vector>

#include "QP/QueryEvaluator.h"
#include "QP/QueryFormatter.h"

using std::string;
using std::vector;

class QP::QueryProcessor {
public:
	explicit QueryProcessor(PKB::StorageAccessInterface& pkb);
	vector<string> processQuery(string query);

private:
	QueryEvaluator evaluator;
};

#endif  // SPA_SRC_QP_QUERYPROCESSOR_H