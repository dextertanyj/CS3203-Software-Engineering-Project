#ifndef SPA_SRC_QP_CLAUSEARGUMENT_H
#define SPA_SRC_QP_CLAUSEARGUMENT_H

#include <string>
#include <variant>

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/TypeDefs.h"
#include "QP/QP.h"
#include "QP/Types.h"

class QP::ClauseArgument {
public:
	/**
	 * Constructs a ClauseArgument object with wildcard type.
	 */
	explicit ClauseArgument();

	/**
	 * Constructs a ClauseArgument object with synonym type.
	 * @param synonym
	 */
	explicit ClauseArgument(Types::Declaration synonym);

	/**
	 * Constructs a ClauseArgument object with synonym and attribute type.
	 * @param attribute
	 */
	explicit ClauseArgument(Types::Attribute attribute);

	/**
	 * Constructs a ClauseArgument object with synonym type.
	 * @param name
	 */
	explicit ClauseArgument(std::string name);

	/**
	 * Constructs a ClauseArgument object storing a statement index.
	 * @param statement_index
	 */
	explicit ClauseArgument(StmtRef statement_index);

	explicit ClauseArgument(Common::ExpressionProcessor::Expression expression, bool exact);

	/**
	 * Returns the type of reference stored.
	 * @return the type of reference stored.
	 */
	[[nodiscard]] Types::ReferenceType getType() const;

	/**
	 * Returns the synonym stored.
	 * @throws ClauseArgumentException if a synonym is not stored.
	 * @return the synonym stored.
	 */
	[[nodiscard]] Types::Declaration getSynonym() const;

	[[nodiscard]] std::string getSynonymSymbol() const;
	[[nodiscard]] Types::DesignEntity getSynonymType() const;

	/**
	 * Returns the synonym and attribute stored.
	 * @throws ClauseArgumentException if a synonym and attribute are not stored.
	 * @return the synonym and attribute stored.
	 */
	[[nodiscard]] Types::Attribute getAttribute() const;
	[[nodiscard]] Types::AttributeType getAttributeType() const;

	/**
	 * Returns the name stored.
	 * @throws ClauseArgumentException if a name is not stored.
	 * @return the name stored.
	 */
	[[nodiscard]] std::string getName() const;

	/**
	 * Returns the statement index stored.
	 * @throws ClauseArgumentException if a statement index is not stored.
	 * @return the statement index stored.
	 */
	[[nodiscard]] StmtRef getStatementIndex() const;

	/**
	 * Returns the expression stored.
	 * @throws ClauseArgumentException if an expression is not stored.
	 * @return the expression stored.
	 */
	[[nodiscard]] Common::ExpressionProcessor::Expression getExpression() const;

	[[nodiscard]] bool operator==(const ClauseArgument& other) const;

	friend std::hash<ClauseArgument>;

private:
	Types::ArgumentValue value;
};

#endif  // SPA_SRC_QP_CLAUSEARGUMENT_H