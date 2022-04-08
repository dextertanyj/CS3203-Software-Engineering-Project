#ifndef SPA_SRC_QP_CLAUSEARGUMENT_H
#define SPA_SRC_QP_CLAUSEARGUMENT_H

#include <string>

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/TypeDefs.h"
#include "QP/QP.h"
#include "QP/Types.h"

class QP::ClauseArgument {
public:
	explicit ClauseArgument();
	explicit ClauseArgument(Types::Declaration synonym);
	explicit ClauseArgument(Types::Attribute attribute);
	explicit ClauseArgument(Types::Name name);
	explicit ClauseArgument(Types::Number number);
	explicit ClauseArgument(Common::EP::Expression expression, bool exact);

	[[nodiscard]] Types::ArgumentType getType() const;
	[[nodiscard]] Types::Declaration getSynonym() const;
	[[nodiscard]] std::string getSynonymSymbol() const;
	[[nodiscard]] Types::DesignEntity getSynonymType() const;
	[[nodiscard]] Types::Attribute getAttribute() const;
	[[nodiscard]] Types::AttributeType getAttributeType() const;
	[[nodiscard]] Types::Name getName() const;
	[[nodiscard]] Types::Number getNumber() const;
	[[nodiscard]] Common::EP::Expression getExpression() const;
	[[nodiscard]] bool operator==(const ClauseArgument& other) const;
	[[nodiscard]] bool operator!=(const ClauseArgument& other) const;

	friend std::hash<ClauseArgument>;

private:
	Types::ArgumentValue value;
};

#endif  // SPA_SRC_QP_CLAUSEARGUMENT_H