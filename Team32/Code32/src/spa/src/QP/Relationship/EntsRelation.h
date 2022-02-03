#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_ENTSRELATION_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_ENTSRELATION_H_

#include "Relation.h"

class EntsRelation : public Relation {
public:
	EntsRelation(Relationship relationship, QueryEntRef entOne, QueryEntRef entTwo);

private:
	static Relationship validateRelationship(Relationship relationship);
	QueryEntRef entOne;
	QueryEntRef entTwo;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_RELATIONSHIP_ENTSRELATION_H_
