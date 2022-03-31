#ifndef SPA_SRC_QP_QP_H
#define SPA_SRC_QP_QP_H

#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

namespace QP {
namespace Relationship {};
class QueryEvaluator;
class QueryExpressionLexer;
class QueryPostProcessor;
class QueryGraph;
class QueryPreprocessor;
class QueryProcessor;
class QueryProperties;
class QueryResult;
class StorageAdapter;
namespace Utilities {};
namespace Types {};

namespace Dispatcher {};
namespace Executor {};

namespace DispatchProcessors {};

struct QueryDispatchException : public runtime_error {
	using runtime_error::runtime_error;
};

struct QuerySyntaxException : public runtime_error {
	using runtime_error::runtime_error;
};

struct QueryException : public runtime_error {
	using runtime_error::runtime_error;
};

struct QueryTokenizationException : public runtime_error {
	using runtime_error::runtime_error;
};

struct ReferenceArgumentException : public logic_error {
	using logic_error::logic_error;
};

struct QuerySemanticException : public runtime_error {
	vector<string> result; // NOLINT(misc-non-private-member-variables-in-classes)
	explicit QuerySemanticException(const string& message) : runtime_error(message){};
	QuerySemanticException(vector<string> result, const string& message) : runtime_error(message), result(move(result)){};
};
}

#endif  // SPA_SRC_QP_QP_H
