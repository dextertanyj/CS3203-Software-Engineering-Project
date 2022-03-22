#include "QP/ReferenceArgument.h"

#include "catch_tools.h"

TEST_CASE("QP::ReferenceArgument Test") {
	SECTION("Wildcard") {
		QP::Types::ReferenceArgument arg;
		REQUIRE_EQUALS(arg.getType(), QP::Types::ReferenceType::Wildcard);
	}

	SECTION("Statement Index") {
		QP::Types::ReferenceArgument arg = QP::Types::ReferenceArgument(1);
		REQUIRE_EQUALS(arg.getType(), QP::Types::ReferenceType::StatementIndex);
		REQUIRE_EQUALS(arg.getStatementIndex(), 1);
	}

	SECTION("Name") {
		QP::Types::ReferenceArgument arg = QP::Types::ReferenceArgument("Name");
		REQUIRE_EQUALS(arg.getType(), QP::Types::ReferenceType::Name);
		REQUIRE_EQUALS(arg.getName(), "Name");
	}

	SECTION("Synonym") {
		QP::Types::ReferenceArgument arg = QP::Types::ReferenceArgument(QP::Types::Declaration{QP::Types::DesignEntity::If, "ifs"});
		REQUIRE_EQUALS(arg.getType(), QP::Types::ReferenceType::Synonym);
		REQUIRE_EQUALS(arg.getSynonym().type, QP::Types::DesignEntity::If);
		REQUIRE_EQUALS(arg.getSynonym().symbol, "ifs");
	}

	SECTION("Synonym") {
		QP::Types::ReferenceArgument arg = QP::Types::ReferenceArgument({{QP::Types::DesignEntity::If, "ifs"}, QP::Types::AttributeType::Index});
		REQUIRE_EQUALS(arg.getType(), QP::Types::ReferenceType::Attribute);
		REQUIRE_EQUALS(arg.getAttribute().synonym.type, QP::Types::DesignEntity::If);
		REQUIRE_EQUALS(arg.getAttribute().synonym.symbol, "ifs");
		REQUIRE_EQUALS(arg.getAttribute().attribute, QP::Types::AttributeType::Index);
	}
}