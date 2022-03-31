#ifndef SPA_SRC_QP_RELATIONSHIP_RELATIONSHIP_H
#define SPA_SRC_QP_RELATIONSHIP_RELATIONSHIP_H

#include "Common/Hash.h"

namespace QP::Relationship {
class Relation;

struct RelationHash {
	std::size_t operator()(const Relation& key) const {
		std::size_t seed = 0;
		combineHash(seed, key.getType());
		combineHash(seed, key.getArguments());
		return seed;
	}
};
}

#endif  // SPA_SRC_QP_RELATIONSHIP_RELATIONSHIP_H
