#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "../Common/TypeDefs.h"

using namespace std;
typedef short PROC;

class TNode;

class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

class PKB {
public:
	static VarTable* varTable; 
	static int setProcToAST(PROC p, TNode* r);
	static TNode* getRootAST (PROC p);

	PKB();

	unordered_set<StmtRef> getAllStmt();

	// Parent get methods
	StmtRef getParent(StmtRef stmtNo);
	unordered_set<StmtRef> getChildren(StmtRef stmtNo);
	bool checkParents(StmtRef stmtNo1, StmtRef stmtNo2);
};