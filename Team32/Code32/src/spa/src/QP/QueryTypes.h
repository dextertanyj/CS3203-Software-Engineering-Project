#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYTYPEDEFS_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYTYPEDEFS_H_

#include <memory>
#include <string>
#include <vector>

#include "QP/Relationship/Relationship.h"

using namespace std;

namespace QP::Types {
enum class DesignEntity {
	Stmt,
	Read,
	Print,
	Call,
	While,
	If,
	Assign,
	Variable,
	Constant,
	Procedure,
};

enum class StmtRefType { Synonym, Underscore, StmtNumber };
enum class EntRefType { Synonym, Underscore, VarName };
enum class ExpressionType { Expression, Underscore, ExpressionUnderscore };

typedef struct QueryStmtRef {
	StmtRefType type;
	string stmt_ref;
} QueryStmtRef;

typedef struct QueryEntRef {
	EntRefType type;
	string ent_ref;
} QueryEntRef;

typedef struct Declaration {
	DesignEntity type;
	string symbol;
} Declaration;

typedef struct PatternClause {
	shared_ptr<Relationship::Relation> relation;
} PatternClause;

typedef struct SuchThatClause {
	shared_ptr<Relationship::Relation> relation;
} SuchThatClause;

typedef struct Node {
	string declaration_symbol;
	vector<string> adjacent_symbols;
} Node;

typedef vector<Declaration> DeclarationList;
typedef vector<SuchThatClause> SuchThatClauseList;
typedef vector<PatternClause> PatternClauseList;
}  // namespace QP::Types

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYTYPEDEFS_H_
