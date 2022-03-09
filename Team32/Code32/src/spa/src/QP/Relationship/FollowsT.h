#ifndef SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H
#define SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H

#include "QP/QueryTypes.h"
#include "QP/Relationship/Follows.h"

class QP::Relationship::FollowsT : public Follows {
public:
	using Follows::Follows;

private:
	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb) override;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H
