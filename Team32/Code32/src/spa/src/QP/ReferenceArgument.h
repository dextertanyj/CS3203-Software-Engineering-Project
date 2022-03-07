#ifndef SPA_REFERENCEARGUMENT_H
#define SPA_REFERENCEARGUMENT_H

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
	ReferenceType getType();

	/**
	 * Returns the synonym stored.
	 * @throws ReferenceArgumentException if a synonym is not stored.
	 * @return the synonym stored.
	 */
	Declaration getSynonym();

	/**
	 * Returns the name stored.
	 * @throws ReferenceArgumentException if a name is not stored.
	 * @return the name stored.
	 */
	string getName();

	/**
	 * Returns the statement index stored.
	 * @throws ReferenceArgumentException if a statement index is not stored.
	 * @return the statement index stored.
	 */
	StmtRef getStatementIndex();

	/**
	 * Returns the expression stored.
	 * @throws ReferenceArgumentException if an expression is not stored.
	 * @return the expression stored.
	 */
	Common::ExpressionProcessor::Expression getExpression();

private:
	variant<monostate, Declaration, string, StmtRef, pair<Common::ExpressionProcessor::Expression, bool>> value;
};

#endif  // SPA_REFERENCEARGUMENT_H