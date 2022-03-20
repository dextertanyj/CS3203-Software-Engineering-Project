#ifndef SPA_SRC_QP_REFERENCEARGUMENT_H
#define SPA_SRC_QP_REFERENCEARGUMENT_H

#include <string>
#include <variant>

#include "Common/ExpressionProcessor/Expression.h"
#include "Common/TypeDefs.h"
#include "QP/QP.h"
#include "QP/QueryTypes.h"

using namespace std;

class QP::Types::ReferenceArgument {
public:
	/**
	 * Constructs a ReferenceArgument object with wildcard type.
	 */
	explicit ReferenceArgument();

	/**
	 * Constructs a ReferenceArgument object with synonym type.
	 * @param synonym
	 */

	explicit ReferenceArgument(Declaration synonym);
	/**
	 * Constructs a ReferenceArgument object with synonym type.
	 * @param name
	 */
	explicit ReferenceArgument(string name);

	/**
	 * Constructs a ReferenceArgument object storing a statement index.
	 * @param statement_index
	 */
	explicit ReferenceArgument(StmtRef statement_index);

	explicit ReferenceArgument(Common::ExpressionProcessor::Expression expression, bool exact);

	/**
	 * Returns the type of reference stored.
	 * @return the type of reference stored.
	 */
	[[nodiscard]] ReferenceType getType() const;

	/**
	 * Returns the synonym stored.
	 * @throws ReferenceArgumentException if a synonym is not stored.
	 * @return the synonym stored.
	 */
	[[nodiscard]] Declaration getSynonym() const;

	/**
	 * Returns the name stored.
	 * @throws ReferenceArgumentException if a name is not stored.
	 * @return the name stored.
	 */
	[[nodiscard]] string getName() const;

	/**
	 * Returns the statement index stored.
	 * @throws ReferenceArgumentException if a statement index is not stored.
	 * @return the statement index stored.
	 */
	[[nodiscard]] StmtRef getStatementIndex() const;

	/**
	 * Returns the expression stored.
	 * @throws ReferenceArgumentException if an expression is not stored.
	 * @return the expression stored.
	 */
	[[nodiscard]] Common::ExpressionProcessor::Expression getExpression() const;

private:
	variant<monostate, Declaration, string, StmtRef, pair<Common::ExpressionProcessor::Expression, bool>> value;
};

#endif  // SPA_SRC_QP_REFERENCEARGUMENT_H