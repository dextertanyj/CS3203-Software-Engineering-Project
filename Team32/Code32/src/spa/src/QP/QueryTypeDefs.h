#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYTYPEDEFS_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYTYPEDEFS_H_

#include <string>

using std::string;

// while and if are reserved words
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

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYTYPEDEFS_H_
