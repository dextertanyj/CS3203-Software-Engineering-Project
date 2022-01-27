#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "ParentStore.h"
#include "FollowStore.h"
#include "UseStore.h"
#include "ModifyStore.h"

using namespace std;

class PKB {
public:
    /*
	static VarTable* varTable; 
	static int setProcToAST(PROC p, TNode* r);
	static TNode* getRootAST (PROC p);
     */
    PKB();

    // Set methods called by Source processor
    bool setFollows(int stmtNo1, int stmtNo2);
    bool setParent(int stmtNo1, int stmtNo2);
    void setProc(string proc_name, vector<int> idxList);
    void setStmtType(int stmtNo, StmtType type);
    void setUses(int stmtNo, string var_name);
    void setModifies(int stmtNo, string var_name);

    // Get methods called by PQL
    // Parent get methods
    int getParent(int stmtNo);
    unordered_set<int> getChildren(int stmtNo);
    bool checkParents(int stmtNo1, int stmtNo2);

    // Follow get methods
    int getFollowee(int stmtNo);
    int getFollower(int stmtNo);
    bool checkFollows(int stmtNo1, int stmtNo2);

    // Others
    void clear();
    void setNumStatements(int size);
    void populateComplexRelations();

private:
    static ParentStore parentStore;
    static FollowStore followStore;
    static UseStore useStore;
    static ModifyStore modifyStore;
    bool checkInvalidStmts(int stmtNo1, int stmtNo2);
    static unordered_map<string, vector<int>> procMap;
    static unordered_map<int, stmtType> typeMap;
};
