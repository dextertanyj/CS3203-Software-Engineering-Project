#include "QP/ClauseArgument.h"

#include "catch_tools.h"

using namespace QP;
using namespace Types;

TEST_CASE("QP::ClauseArgument Test") {
	SECTION("Wildcard") {
		ClauseArgument arg;
		REQUIRE_EQUALS(arg.getType(), ReferenceType::Wildcard);
	}

	SECTION("Statement Index") {
		ClauseArgument arg = ClauseArgument(1);
		REQUIRE_EQUALS(arg.getType(), ReferenceType::StatementIndex);
		REQUIRE_EQUALS(arg.getStatementIndex(), 1);
	}

	SECTION("Name") {
		ClauseArgument arg = ClauseArgument("Name");
		REQUIRE_EQUALS(arg.getType(), ReferenceType::Name);
		REQUIRE_EQUALS(arg.getName(), "Name");
	}

	SECTION("Synonym") {
		ClauseArgument arg = ClauseArgument(Declaration{DesignEntity::If, "ifs"});
		REQUIRE_EQUALS(arg.getType(), ReferenceType::Synonym);
		REQUIRE_EQUALS(arg.getSynonym().type, DesignEntity::If);
		REQUIRE_EQUALS(arg.getSynonym().symbol, "ifs");
	}

	SECTION("Attribute") {
		ClauseArgument arg = ClauseArgument({AttributeType::NameIdentifier, {DesignEntity::Read, "r"}});
		REQUIRE_EQUALS(arg.getType(), ReferenceType::Attribute);
		REQUIRE_EQUALS(arg.getAttribute().synonym.type, DesignEntity::Read);
		REQUIRE_EQUALS(arg.getAttribute().synonym.symbol, "r");
		REQUIRE_EQUALS(arg.getAttribute().attribute, AttributeType::NameIdentifier);
	}
}