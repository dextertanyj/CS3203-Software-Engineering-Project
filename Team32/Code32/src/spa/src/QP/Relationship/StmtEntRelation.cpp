#include "StmtEntRelation.h"

#include <stdexcept> 

using namespace std;

StmtEntRelation::StmtEntRelation(Relationship relationship, QueryStmtRef stmt, QueryEntRef ent) :
	Relation(validateRelationship(relationship)),
	stmt(stmt),
	ent(ent) {
}

Relationship StmtEntRelation::validateRelationship(Relationship relationship) {
	if (relationship == Relationship::UsesS ||
		relationship == Relationship::ModifiesS) {
		return relationship;
	}

	throw invalid_argument("Invalid relationship");
}
