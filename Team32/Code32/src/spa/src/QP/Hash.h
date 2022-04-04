#ifndef SPA_SRC_QP_HASH_H
#define SPA_SRC_QP_HASH_H

#include "Common/ExpressionProcessor/Hash.h"
#include "Common/Hash.h"
#include "QP/Evaluator/Clause.h"
#include "QP/Types.h"

namespace std {
template <>
struct hash<QP::Types::Declaration> {
	std::size_t operator()(const QP::Types::Declaration& key) const {
		std::size_t seed = 0;
		combineHash(seed, key.symbol);
		combineHash(seed, key.type);
		return seed;
	}
};

template <>
struct hash<QP::Types::Attribute> {
	std::size_t operator()(const QP::Types::Attribute& key) const {
		std::size_t seed = 0;
		combineHash(seed, key.synonym);
		combineHash(seed, static_cast<int>(key.attribute));
		return seed;
	}
};

template <>
struct hash<QP::ReferenceArgument> {
	std::size_t operator()(const QP::ReferenceArgument& key) const { return hash<QP::Types::ArgumentValue>()(key.value); }
};

template <>
struct hash<QP::Evaluator::Clause> {
	std::size_t operator()(const QP::Evaluator::Clause& key) const {
		std::size_t seed = 0;
		combineHash(seed, static_cast<int>(key.getType()));
		combineHash(seed, key.arguments);
		return seed;
	}
};
}

#endif  // SPA_SRC_QP_HASH_H
