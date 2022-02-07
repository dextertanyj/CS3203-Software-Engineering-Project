#include "QP/Relationship/Parent.h"

#include "catch.hpp"

TEST_CASE("QP::Relation::Parent::isTrivialCase Trivial relation") {
	Parent parent = Parent(false, { StmtRefType::stmtNumber, "8" }, { StmtRefType::stmtNumber, "9" });
	REQUIRE(parent.isTrivialCase());
}

TEST_CASE("QP::Relation::Parent::isTrivialCase Non trivial relation") {
	Parent parent = Parent(false, { StmtRefType::synonym, "s" }, { StmtRefType::stmtNumber, "9" });
	REQUIRE(!parent.isTrivialCase());
}
