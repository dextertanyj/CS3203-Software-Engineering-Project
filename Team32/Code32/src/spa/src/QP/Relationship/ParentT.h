#ifndef SPA_SRC_QP_RELATIONSHIP_PARENTT_H
#define SPA_SRC_QP_RELATIONSHIP_PARENTT_H

#include "QP/QueryTypes.h"
#include "QP/Relationship/Parent.h"

class QP::Relationship::ParentT : public Parent {
public:
	using Parent::Parent;

private:
	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb) override;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_PARENTT_H
