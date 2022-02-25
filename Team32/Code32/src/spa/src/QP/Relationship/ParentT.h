#ifndef SPA_SRC_QP_RELATIONSHIP_PARENTT_H
#define SPA_SRC_QP_RELATIONSHIP_PARENTT_H

#include "QP/QueryTypes.h"
#include "QP/Relationship/Parent.h"

class QP::Relationship::ParentT : public Parent {
public:
	using Parent::Parent;

	QueryResult execute(PKB::Storage& pkb, bool is_trivial, unordered_map<string, DesignEntity>& map) override;

private:
	QueryResult executeTrivial(PKB::Storage& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB::Storage& pkb, unordered_map<string, DesignEntity>& map) override;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_PARENTT_H
