#ifndef RELATION_H_
#define RELATION_H_

#include <string>

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
	Relation(Relationship relationship);
	Relationship getRelationship();

private:
	Relationship relationship;
};

#endif // RELATION_H_
