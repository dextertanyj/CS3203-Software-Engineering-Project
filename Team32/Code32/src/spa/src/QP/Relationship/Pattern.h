#ifndef SPA_SRC_QP_RELATIONSHIP_PATTERN_H
#define SPA_SRC_QP_RELATIONSHIP_PATTERN_H

#include <optional>
#include <string>
#include <vector>

#include "Common/ExpressionProcessor/Expression.h"
#include "QP/QueryExpressionLexer.h"
#include "QP/QueryTypes.h"
#include "QP/Relationship/Relation.h"

class QP::Relationship::Pattern : public Relation {
public:
	Pattern(Declaration syn_assign, QueryEntRef ent_ref, ExpressionType expression_type,
	        optional<Common::ExpressionProcessor::Expression> expression);

	Declaration getSynAssign();
	QueryEntRef getEntRef();
	ExpressionType getExpressionType();
	Common::ExpressionProcessor::Expression getExpression();

	vector<string> getDeclarationSymbols() override;

private:
	QueryResult executeTrivial(PKB::Storage& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB::Storage& pkb, unordered_map<string, DesignEntity>& map) override;

	Declaration syn_assign;
	QueryEntRef ent_ref;
	ExpressionType expression_type;
	optional<Common::ExpressionProcessor::Expression> expression;
};

#endif  // SPA_SRC_QP_RELATIONSHIP_PATTERN_H
