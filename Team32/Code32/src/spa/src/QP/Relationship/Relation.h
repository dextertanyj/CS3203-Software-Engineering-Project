#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_RELATION_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_RELATION_H_

#include "QP/QueryTypeDefs.h"

enum class Relationship {
	Follows,
	FollowsT,
	Parent,
	ParentT,
	UsesS,
	UsesP,
	ModifiesS,
	ModifiesP,
};

class Relation {
 public:
	explicit Relation(Relationship relationship);
	Relationship getRelationship();

 private:
	Relationship relationship;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_RELATION_H_
