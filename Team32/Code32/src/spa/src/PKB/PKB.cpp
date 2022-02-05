#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"
#include "Common/TNode.h"

int PKB::setProcToAST(PROC p, TNode* r) {
	return 0;
}

TNode* PKB::getRootAST (PROC p){
	return nullptr;
}

PKB::PKB() {}

unordered_set<StmtRef> PKB::getAllStmt() {
	return unordered_set<StmtRef>();
}

StmtRef PKB::getParent(StmtRef stmtNo) {
	return StmtRef();
}

unordered_set<StmtRef> PKB::getChildren(StmtRef stmtNo) {
	return unordered_set<StmtRef>();
}

bool PKB::checkParents(StmtRef stmtNo1, StmtRef stmtNo2) {
	return false;
}


