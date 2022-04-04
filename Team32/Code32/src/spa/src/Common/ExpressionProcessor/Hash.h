#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_HASH_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_HASH_H

#include "Common/ExpressionProcessor/Expression.h"

namespace std {
template <>
struct hash<Common::ExpressionProcessor::Expression> {
	std::size_t operator()(const Common::ExpressionProcessor::Expression& key) const { return hash<std::string>()(key.traversal); }
};
}

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_HASH_H
