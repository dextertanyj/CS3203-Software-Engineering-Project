#ifndef STMTS_RELATION_H_
#define STMTS_RELATION_H_

#include "Relation.h"

class StmtsRelation : public Relation {
public:
	StmtsRelation(Relationship relationship, QueryStmtRef stmtOne, QueryStmtRef stmtTwo);

private:
	static Relationship validateRelationship(Relationship relationship);
	QueryStmtRef stmtOne;
	QueryStmtRef stmtTwo;
};

#endif // STMTS_RELATION_H_
