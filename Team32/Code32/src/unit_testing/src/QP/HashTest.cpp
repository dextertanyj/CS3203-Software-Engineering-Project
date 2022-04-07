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
	ClauseArgument wildcard = ClauseArgument();
	ClauseArgument synonym = ClauseArgument(dec_a);
	ClauseArgument name1 = ClauseArgument("a");
	ClauseArgument name2 = ClauseArgument("a");
	Types::ExecutorSet exec = {};
	Evaluator::Clause clause_1 = Evaluator::Clause(Types::ClauseType::Follows, vector<ClauseArgument>({name1, name1}), exec);
	Evaluator::Clause clause_2 = Evaluator::Clause(Types::ClauseType::Follows, vector<ClauseArgument>({name2, name2}), exec);
	Evaluator::Clause clause_3 = Evaluator::Clause(Types::ClauseType::Follows, vector<ClauseArgument>({name1, wildcard}), exec);
	Evaluator::Clause clause_4 = Evaluator::Clause(Types::ClauseType::FollowsT, vector<ClauseArgument>({name2, name2}), exec);

	SECTION("Declaration") {
		REQUIRE_EQUALS(std::hash<Types::Declaration>()(dec_a), std::hash<Types::Declaration>()(dec_b));
		REQUIRE(std::hash<Types::Declaration>()(dec_a) != std::hash<Types::Declaration>()(dec_c));
	}

	SECTION("Attribute") {
		REQUIRE_EQUALS(std::hash<Types::Attribute>()(attr_a), std::hash<Types::Attribute>()(attr_b));
		REQUIRE(std::hash<Types::Attribute>()(attr_a) != std::hash<Types::Attribute>()(attr_c));
	}

	SECTION("ClauseArgument") {
		REQUIRE_EQUALS(std::hash<ClauseArgument>()(name1), std::hash<ClauseArgument>()(name2));
		REQUIRE(std::hash<ClauseArgument>()(wildcard) != std::hash<ClauseArgument>()(synonym));
		REQUIRE(std::hash<ClauseArgument>()(name1) != std::hash<ClauseArgument>()(synonym));
	}

	SECTION("Relation") {
		REQUIRE_EQUALS(std::hash<Evaluator::Clause>()(clause_1), std::hash<Evaluator::Clause>()(clause_2));
		REQUIRE(std::hash<Evaluator::Clause>()(clause_2) != std::hash<Evaluator::Clause>()(clause_3));
		REQUIRE(std::hash<Evaluator::Clause>()(clause_2) != std::hash<Evaluator::Clause>()(clause_4));
	}
}
