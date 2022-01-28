#include "StmtsRelation.h"

#include <stdexcept> 

using namespace std;

StmtsRelation::StmtsRelation(Relationship relationship, QueryStmtRef stmtOne, QueryStmtRef stmtTwo) :
	Relation(validateRelationship(relationship)),
	stmtOne(stmtOne),
	stmtTwo(stmtTwo) {
}

Relationship StmtsRelation::validateRelationship(Relationship relationship) {
	if (relationship == Relationship::Follows ||
		relationship == Relationship::FollowsT ||
		relationship == Relationship::Parent ||
		relationship == Relationship::ParentT) {
		return relationship;
	}

	throw invalid_argument("Invalid relationship");
}
