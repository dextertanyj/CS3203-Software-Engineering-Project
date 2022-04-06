#ifndef SPA_SRC_QP_QP_H
#define SPA_SRC_QP_QP_H

#include <stdexcept>
#include <string>
#include <vector>

namespace QP {
namespace Preprocessor {}
namespace Dispatcher {}
namespace Executor {}
namespace Evaluator {}
namespace Optimizer {}
namespace Utilities {}
namespace Types {}

class ClauseArgument;
class ResultTable;
class QueryResult;
class QueryProcessor;
class QueryPostProcessor;
class QueryProperties;
class StorageAdapter;

struct QueryDispatchException : public std::runtime_error {
	using runtime_error::runtime_error;
};

struct QuerySyntaxException : public std::runtime_error {
	using runtime_error::runtime_error;
};

struct QueryTokenizationException : public std::runtime_error {
	using runtime_error::runtime_error;
};

struct QuerySemanticException : public std::runtime_error {
	std::vector<std::string> result;  // NOLINT(misc-non-private-member-variables-in-classes)
	explicit QuerySemanticException(const std::string& message) : runtime_error(message){};
	QuerySemanticException(std::vector<std::string> result, const std::string& message) : runtime_error(message), result(move(result)){};
};
}

#endif  // SPA_SRC_QP_QP_H
