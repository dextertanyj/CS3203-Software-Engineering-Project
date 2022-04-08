#ifndef SPA_SRC_QP_QUERYPROCESSOR_H
#define SPA_SRC_QP_QUERYPROCESSOR_H

#include <string>
#include <vector>

#include "PKB/StorageAccessInterface.h"
#include "QP/Evaluator/QueryEvaluator.h"
#include "QP/Preprocessor/QueryPreprocessor.h"
#include "QP/QP.h"
#include "QP/QueryPostProcessor.h"
#include "QP/StorageAdapter.h"

class QP::QueryProcessor {
public:
	explicit QueryProcessor(PKB::StorageAccessInterface& pkb);
	std::vector<std::string> processQuery(std::string query);

private:
	StorageAdapter store;
	Preprocessor::QueryPreprocessor pre_processor;
	Evaluator::QueryEvaluator evaluator;
	QueryPostProcessor post_processor;
};

#endif  // SPA_SRC_QP_QUERYPROCESSOR_H