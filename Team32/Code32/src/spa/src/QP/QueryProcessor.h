#ifndef SPA_SRC_QP_QUERYPROCESSOR_H
#define SPA_SRC_QP_QUERYPROCESSOR_H

#include <string>
#include <vector>

#include "QP/QueryEvaluator.h"
#include "QP/QueryPostProcessor.h"
#include "QP/QueryPreprocessor.h"
#include "QP/StorageAdapter.h"

using std::string;
using std::vector;

class QP::QueryProcessor {
public:
	explicit QueryProcessor(PKB::StorageAccessInterface& pkb);
	vector<string> processQuery(string query);

private:
	StorageAdapter store;
	QueryPreprocessor pre_processor;
	QueryEvaluator evaluator;
	QueryPostProcessor post_processor;
};

#endif  // SPA_SRC_QP_QUERYPROCESSOR_H