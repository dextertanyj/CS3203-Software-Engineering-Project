#ifndef ENTS_RELATION_H_
#define ENTS_RELATION_H_

#include "Relation.h"

class EntsRelation : public Relation {
public:
	EntsRelation(Relationship relationship, QueryEntRef entOne, QueryEntRef entTwo);

private:
	static Relationship validateRelationship(Relationship relationship);
	QueryEntRef entOne;
	QueryEntRef entTwo;
};

#endif // ENTS_RELATION_H_
