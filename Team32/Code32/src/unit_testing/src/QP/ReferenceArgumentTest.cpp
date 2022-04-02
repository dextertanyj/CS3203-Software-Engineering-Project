#include "QP/ReferenceArgument.h"

#include "catch_tools.h"

using namespace QP;
using namespace Types;

TEST_CASE("QP::ReferenceArgument Test") {
	SECTION("Wildcard") {
		ReferenceArgument arg;
		REQUIRE_EQUALS(arg.getType(), ReferenceType::Wildcard);
	}

	SECTION("Statement Index") {
		ReferenceArgument arg = ReferenceArgument(1);
		REQUIRE_EQUALS(arg.getType(), ReferenceType::StatementIndex);
		REQUIRE_EQUALS(arg.getStatementIndex(), 1);
	}

	SECTION("Name") {
		ReferenceArgument arg = ReferenceArgument("Name");
		REQUIRE_EQUALS(arg.getType(), ReferenceType::Name);
		REQUIRE_EQUALS(arg.getName(), "Name");
	}

	SECTION("Synonym") {
		ReferenceArgument arg = ReferenceArgument(Declaration{DesignEntity::If, "ifs"});
		REQUIRE_EQUALS(arg.getType(), ReferenceType::Synonym);
		REQUIRE_EQUALS(arg.getSynonym().type, DesignEntity::If);
		REQUIRE_EQUALS(arg.getSynonym().symbol, "ifs");
	}

	SECTION("Attribute") {
		ReferenceArgument arg = ReferenceArgument({AttributeType::NameIdentifier, {DesignEntity::Read, "r"}});
		REQUIRE_EQUALS(arg.getType(), ReferenceType::Attribute);
		REQUIRE_EQUALS(arg.getAttribute().synonym.type, DesignEntity::Read);
		REQUIRE_EQUALS(arg.getAttribute().synonym.symbol, "r");
		REQUIRE_EQUALS(arg.getAttribute().attribute, AttributeType::NameIdentifier);
	}
}