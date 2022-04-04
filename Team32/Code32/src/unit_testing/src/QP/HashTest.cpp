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
	ReferenceArgument wildcard = ReferenceArgument();
	ReferenceArgument synonym = ReferenceArgument(dec_a);
	ReferenceArgument name1 = ReferenceArgument("a");
	ReferenceArgument name2 = ReferenceArgument("a");
	Types::ExecutorSet exec = {};
	Evaluator::Clause clause_1 = Evaluator::Clause(Types::ClauseType::Follows, vector<ReferenceArgument>({name1, name1}), exec);
	Evaluator::Clause clause_2 = Evaluator::Clause(Types::ClauseType::Follows, vector<ReferenceArgument>({name2, name2}), exec);
	Evaluator::Clause clause_3 = Evaluator::Clause(Types::ClauseType::Follows, vector<ReferenceArgument>({name1, wildcard}), exec);
	Evaluator::Clause clause_4 = Evaluator::Clause(Types::ClauseType::FollowsT, vector<ReferenceArgument>({name2, name2}), exec);

	SECTION("Declaration") {
		REQUIRE_EQUALS(std::hash<Types::Declaration>()(dec_a), std::hash<Types::Declaration>()(dec_b));
		REQUIRE(std::hash<Types::Declaration>()(dec_a) != std::hash<Types::Declaration>()(dec_c));
	}

	SECTION("Attribute") {
		REQUIRE_EQUALS(std::hash<Types::Attribute>()(attr_a), std::hash<Types::Attribute>()(attr_b));
		REQUIRE(std::hash<Types::Attribute>()(attr_a) != std::hash<Types::Attribute>()(attr_c));
	}

	SECTION("ReferenceArgument") {
		REQUIRE_EQUALS(std::hash<ReferenceArgument>()(name1), std::hash<ReferenceArgument>()(name2));
		REQUIRE(std::hash<ReferenceArgument>()(wildcard) != std::hash<ReferenceArgument>()(synonym));
		REQUIRE(std::hash<ReferenceArgument>()(name1) != std::hash<ReferenceArgument>()(synonym));
	}

	SECTION("Relation") {
		REQUIRE_EQUALS(std::hash<Evaluator::Clause>()(clause_1), std::hash<Evaluator::Clause>()(clause_2));
		REQUIRE(std::hash<Evaluator::Clause>()(clause_2) != std::hash<Evaluator::Clause>()(clause_3));
		REQUIRE(std::hash<Evaluator::Clause>()(clause_2) != std::hash<Evaluator::Clause>()(clause_4));
	}
}
