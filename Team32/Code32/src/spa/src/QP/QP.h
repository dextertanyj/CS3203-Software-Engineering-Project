#ifndef SPA_SRC_QP_QP_H
#define SPA_SRC_QP_QP_H

#include <stdexcept>

using namespace std;

namespace QP {
namespace Relationship {};
class QueryEvaluator;
class QueryExpressionLexer;
class QueryFormatter;
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

struct QueryException : public runtime_error {
	using runtime_error::runtime_error;
};

struct QueryTokenizationException : public runtime_error {
	using runtime_error::runtime_error;
};

struct ReferenceArgumentException : public logic_error {
	using logic_error::logic_error;
};

// struct QuerySyntaxException : public runtime_error {
//     using runtime_error::runtime_error;
// };
}

#endif  // SPA_SRC_QP_QP_H
