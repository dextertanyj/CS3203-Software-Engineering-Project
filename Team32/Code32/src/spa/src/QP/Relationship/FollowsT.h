#ifndef SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H
#define SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H

#include "QP/QueryTypes.h"
#include "QP/Relationship/Follows.h"

class QP::Relationship::FollowsT : public Follows {
public:
	using Follows::Follows;

	QueryResult execute(PKB::StorageAccessInterface& pkb, bool is_trivial, unordered_map<string, DesignEntity>& map) override;

private:
	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) override;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_FOLLOWST_H
