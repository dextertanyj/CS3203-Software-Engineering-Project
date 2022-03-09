#ifndef SPA_SRC_QP_QUERYTYPES_H
#define SPA_SRC_QP_QUERYTYPES_H

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

enum class ReferenceType { StatementIndex, Synonym, Wildcard, Name, ExactExpression, SubExpression };

class ReferenceArgument;

typedef struct Declaration {
	DesignEntity type;
	string symbol;
} Declaration;

typedef struct Clause {
	shared_ptr<Relationship::Relation> relation;
} Clause;

typedef struct Node {
	string declaration_symbol;
	vector<string> adjacent_symbols;
} Node;

typedef vector<Declaration> DeclarationList;
typedef vector<Clause> ClauseList;
}  // namespace QP::Types

#endif  // SPA_SRC_QP_QUERYTYPES_H
