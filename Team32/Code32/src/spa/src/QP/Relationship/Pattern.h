#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPATTERN_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPATTERN_H_

#include <optional>
#include <string>
#include <vector>

#include "Common/ExpressionProcessor/Expression.h"
#include "QP/QueryExpressionLexer.h"
#include "QP/QueryTypeDefs.h"
#include "QP/Relationship/Relation.h"

class Pattern : public Relation {
public:
	Pattern(Declaration syn_assign, QueryEntRef ent_ref, ExpressionType expression_type,
	        optional<Common::ExpressionProcessor::Expression> expression);

	Declaration getSynAssign();
	QueryEntRef getEntRef();
	ExpressionType getExpressionType();
	Common::ExpressionProcessor::Expression getExpression();

	QueryResult execute(PKB& pkb, bool is_trivial, unordered_map<string, DesignEntity>& map) override;
	vector<string> getDeclarationSymbols() override;

private:
	QueryResult executeTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) override;
	QueryResult executeNonTrivial(PKB& pkb, unordered_map<string, DesignEntity>& map) override;

	Declaration syn_assign;
	QueryEntRef ent_ref;
	ExpressionType expression_type;
	optional<Common::ExpressionProcessor::Expression> expression;
};

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYPATTERN_H_
