#ifndef SPA_SRC_QP_RELATIONSHIP_PARENTT_H
#define SPA_SRC_QP_RELATIONSHIP_PARENTT_H

#include "QP/QueryTypes.h"
#include "QP/Relationship/Parent.h"

class QP::Relationship::ParentT : public Parent {
public:
	using Parent::Parent;

private:
	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) override;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_PARENTT_H
