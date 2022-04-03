#ifndef SPA_SRC_QP_HASH_H
#define SPA_SRC_QP_HASH_H

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/Hash.h"
#include "QP/Relationship/Relation.h"
#include "QP/Types.h"

namespace std {
template <>
struct hash<Common::ExpressionProcessor::Expression> {
	std::size_t operator()(const Common::ExpressionProcessor::Expression& key) const { return hash<std::string>()(key.getTraversal()); }
};

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
		combineHash(seed, (int)key.attribute);
		return seed;
	}
};

template <>
struct hash<QP::Types::ReferenceArgument> {
	std::size_t operator()(const QP::Types::ReferenceArgument& key) const { return hash<QP::Types::ArgumentValue>()(key.getValue()); }
};

template <>
struct hash<QP::Relationship::Relation> {
	std::size_t operator()(const QP::Relationship::Relation& key) const {
		std::size_t seed = 0;
		combineHash(seed, (int)key.getType());
		combineHash(seed, key.getArguments());
		return seed;
	}
};

static inline bool operator==(const QP::Types::Clause& clause1, const QP::Types::Clause& clause2) {
	return (*clause1.relation.get()) == (*clause2.relation.get());
}

template <>
struct hash<QP::Types::Clause> {
	std::size_t operator()(const QP::Types::Clause& key) const { return hash<QP::Relationship::Relation>()(*key.relation.get()); }
};
}

#endif  // SPA_SRC_QP_HASH_H
