#ifndef TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYTYPEDEFS_H_
#define TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYTYPEDEFS_H_

#include <string>

using std::string;

// while and if are reserved words
enum class DesignEntity {
	stmt,
	read,
	print,
	while_,
	if_,
	assign,
	variable,
	constant,
	procedure,
};

enum class StmtRefType { synonym, underscore, stmtNumber };
enum class EntRefType { synonym, underscore, varName, constant };

typedef struct QueryStmtRef {
	StmtRefType type;
	string stmtRef;
} QueryStmtRef;

typedef struct QueryEntRef {
	EntRefType type;
	string entRef;
} QueryEntRef;

typedef struct Declaration {
	DesignEntity type;
	string symbol;
} Declaration;

#endif  // TEAM32_CODE32_SRC_SPA_SRC_QP_QUERYTYPEDEFS_H_
