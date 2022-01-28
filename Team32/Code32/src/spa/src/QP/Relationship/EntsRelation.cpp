#include "EntsRelation.h"

#include <stdexcept> 

using namespace std;

EntsRelation::EntsRelation(Relationship relationship, QueryEntRef entOne, QueryEntRef entTwo) :
	Relation(validateRelationship(relationship)),
	entOne(entOne),
	entTwo(entTwo) {
}

Relationship EntsRelation::validateRelationship(Relationship relationship) {
	if (relationship == Relationship::UsesP ||
		relationship == Relationship::ModifiesP) {
		return relationship;
	}

	throw invalid_argument("Invalid relationship");
}
