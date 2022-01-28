#ifndef STMT_ENT_RELATION_H_
#define STMT_ENT_RELATION_H_

#include "Relation.h"

class StmtEntRelation : public Relation {
public:
	StmtEntRelation(Relationship relationship, QueryStmtRef stmt, QueryEntRef ent);

private:
	static Relationship validateRelationship(Relationship relationship);
	QueryStmtRef stmt;
	QueryEntRef ent;
};

#endif // STMT_ENT_RELATION_H_
