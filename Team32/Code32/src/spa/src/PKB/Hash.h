#ifndef SPA_SRC_PKB_HASH_H
#define SPA_SRC_PKB_HASH_H

#include "Common/Hash.h"
#include "PKB/PKB.h"

namespace std {
template <>
struct hash<PKB::AssignRelation> {
	size_t operator()(const PKB::AssignRelation& assign_relation) const {
		std::size_t seed = 0;
		combineHash(seed, assign_relation.node);
		combineHash(seed, assign_relation.variable);
		combineHash(seed, assign_relation.expression);
		return seed;
	}
};
}

#endif  // SPA_SRC_PKB_HASH_H
