#ifndef SPA_SRC_QP_RELATIONSHIP_PATTERN_H
#define SPA_SRC_QP_RELATIONSHIP_PATTERN_H

#include <optional>
#include <string>
#include <vector>

#include "Common/ExpressionProcessor/Expression.h"
#include "QP/QueryExpressionLexer.h"
#include "QP/QueryTypes.h"
#include "QP/ReferenceArgument.h"
#include "QP/Relationship/Relation.h"

class QP::Relationship::Pattern : public Relation {
public:
	Pattern(ReferenceArgument syn_assign, ReferenceArgument ent_ref, ReferenceArgument expression);

	ReferenceArgument getSynAssign();
	ReferenceArgument getEntRef();
	ReferenceType getExpressionType();
	Common::ExpressionProcessor::Expression getExpression();

	vector<string> getDeclarationSymbols() override;

private:
	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb, unordered_map<string, DesignEntity>& map) override;

	ReferenceArgument syn_assign;
	ReferenceArgument ent_ref;
	ReferenceArgument expression;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_PATTERN_H
