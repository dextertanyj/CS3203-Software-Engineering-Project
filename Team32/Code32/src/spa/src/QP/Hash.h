#ifndef SPA_SRC_QP_HASH_H
#define SPA_SRC_QP_HASH_H

#include "QP/QP.h"

namespace std {
template <>
struct hash<QP::Types::ReferenceArgument> {
	std::size_t operator()(const QP::Types::ReferenceArgument& key) const {
		QP::Types::ReferenceType type = key.getType();
		switch (type) {
			case QP::Types::ReferenceType::StatementIndex:
				return hash<size_t>{}(key.getStatementIndex());
			case QP::Types::ReferenceType::Synonym:
				return hash<string>{}(key.getSynonymSymbol());
			case QP::Types::ReferenceType::Wildcard:
				return hash<string>{}("_");
			case QP::Types::ReferenceType::Name:
				return hash<string>{}(key.getName());
			case QP::Types::ReferenceType::ExactExpression:
			case QP::Types::ReferenceType::SubExpression:
				return hash<string>{}(key.getExpression().getTraversal());
			case QP::Types::ReferenceType::Attribute:
				return QP::Types::AttributeHash{}(key.getAttribute());
		}
	}
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

template <>
struct hash<QP::Types::Clause> {
	std::size_t operator()(const QP::Types::Clause& key) const {
		return hash<QP::Relationship::Relation>{}(key.relation.get());
	}
};
}

#endif  // SPA_SRC_QP_HASH_H
