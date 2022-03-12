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

	// Trivial Executors

	static QueryResult executeTrivialNameWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& name);
	static QueryResult executeTrivialSynonymOrWildcardWildcard(PKB::StorageAccessInterface& pkb);
	static QueryResult executeTrivialNameExpression(PKB::StorageAccessInterface& pkb, const ReferenceArgument& name,
	                                                const ReferenceArgument& expression);
	static QueryResult executeTrivialSynonymOrWildcardExpression(PKB::StorageAccessInterface& pkb, const ReferenceArgument& expression);

	// Executors

	static QueryResult executeNameWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& assign,
	                                       const ReferenceArgument& name);
	static QueryResult executeWildcardWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& assign);
	static QueryResult executeSynonymWildcard(PKB::StorageAccessInterface& pkb, const ReferenceArgument& assign,
	                                          const ReferenceArgument& synonym);
	static QueryResult executeNameExpression(PKB::StorageAccessInterface& pkb, const ReferenceArgument& assign,
	                                         const ReferenceArgument& name, const ReferenceArgument& expression);
	static QueryResult executeWildcardExpression(PKB::StorageAccessInterface& pkb, const ReferenceArgument& assign,
	                                             const ReferenceArgument& expression);
	static QueryResult executeSynonymExpression(PKB::StorageAccessInterface& pkb, const ReferenceArgument& assign,
	                                            const ReferenceArgument& synonym, const ReferenceArgument& expression);

private:
	QueryResult executeTrivial(PKB::StorageAccessInterface& pkb) override;
	QueryResult executeNonTrivial(PKB::StorageAccessInterface& pkb) override;

	ReferenceArgument syn_assign;
	ReferenceArgument ent_ref;
	ReferenceArgument expression;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_PATTERN_H
