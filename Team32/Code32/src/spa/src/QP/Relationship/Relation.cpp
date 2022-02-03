#include "Relation.h"

Relation::Relation(Relationship relationship) : relationship(relationship) {}

Relationship Relation::getRelationship() {
	return relationship;
}
