#ifndef QUERY_TYPE_DEFS_H_
#define QUERY_TYPE_DEFS_H_

#include <string>

using namespace std;

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

#endif // QUERY_TYPE_DEFS_H_
