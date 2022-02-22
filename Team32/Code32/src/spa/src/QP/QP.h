#ifndef SPA_QP_H
#define SPA_QP_H

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
namespace Utilities {};
namespace Types {};

struct QueryException : public runtime_error {
	using runtime_error::runtime_error;
};

struct QueryTokenizationException : public runtime_error {
	using runtime_error::runtime_error;
};

// struct QuerySyntaxException : public runtime_error {
//     using runtime_error::runtime_error;
// };
}  // namespace QP

#endif  // SPA_QP_H
