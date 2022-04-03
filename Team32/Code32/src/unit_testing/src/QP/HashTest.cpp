#include "QP/Hash.h"

#include "QP/QP.h"
#include "catch_tools.h"

using namespace QP;

TEST_CASE("QP Hash Test") {
	Types::Declaration dec_a = {Types::DesignEntity::Stmt, "a"};
	Types::Declaration dec_b = {Types::DesignEntity::Stmt, "a"};
	Types::Declaration dec_c = {Types::DesignEntity::Constant, "b"};
	Types::Attribute attr_a = {Types::AttributeType::NumberIdentifier, dec_a};
	Types::Attribute attr_b = {Types::AttributeType::NumberIdentifier, dec_b};
	Types::Attribute attr_c = {Types::AttributeType::VariableName, dec_a};
	Types::ReferenceArgument wildcard = Types::ReferenceArgument();
	Types::ReferenceArgument synonym = Types::ReferenceArgument(dec_a);
	Types::ReferenceArgument name1 = Types::ReferenceArgument("a");
	Types::ReferenceArgument name2 = Types::ReferenceArgument("a");
	Types::ExecutorSet exec = {};
	Relationship::Relation relation1 =
		Relationship::Relation(Types::ClauseType::Follows, vector<Types::ReferenceArgument>({name1, name1}), exec);
	Relationship::Relation relation2 =
		Relationship::Relation(Types::ClauseType::Follows, vector<Types::ReferenceArgument>({name2, name2}), exec);
	Relationship::Relation relation3 =
		Relationship::Relation(Types::ClauseType::Follows, vector<Types::ReferenceArgument>({name1, wildcard}), exec);
	Relationship::Relation relation4 =
		Relationship::Relation(Types::ClauseType::FollowsT, vector<Types::ReferenceArgument>({name2, name2}), exec);

	SECTION("Declaration") {
		REQUIRE_EQUALS(std::hash<Types::Declaration>()(dec_a), std::hash<Types::Declaration>()(dec_b));
		REQUIRE(std::hash<Types::Declaration>()(dec_a) != std::hash<Types::Declaration>()(dec_c));
	}

	SECTION("Attribute") {
		REQUIRE_EQUALS(std::hash<Types::Attribute>()(attr_a), std::hash<Types::Attribute>()(attr_b));
		REQUIRE(std::hash<Types::Attribute>()(attr_a) != std::hash<Types::Attribute>()(attr_c));
	}

	SECTION("ReferenceArgument") {
		REQUIRE_EQUALS(std::hash<Types::ReferenceArgument>()(name1), std::hash<Types::ReferenceArgument>()(name2));
		REQUIRE(std::hash<Types::ReferenceArgument>()(wildcard) != std::hash<Types::ReferenceArgument>()(synonym));
		REQUIRE(std::hash<Types::ReferenceArgument>()(name1) != std::hash<Types::ReferenceArgument>()(synonym));
	}

	SECTION("Relation") {
		REQUIRE_EQUALS(std::hash<Relationship::Relation>()(relation1), std::hash<Relationship::Relation>()(relation2));
		REQUIRE(std::hash<Relationship::Relation>()(relation2) != std::hash<Relationship::Relation>()(relation3));
		REQUIRE(std::hash<Relationship::Relation>()(relation2) != std::hash<Relationship::Relation>()(relation4));
	}
}
