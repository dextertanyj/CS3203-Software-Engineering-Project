#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "ParentStore.h"
#include "FollowStore.h"

using namespace std;
typedef short PROC;

class TNode;

class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

enum stmtType { assign, print, call, read, whileStmt, ifStmt, standard };

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
    void setProc(std::string proc_name, vector<int> idxList);
    void setStmtType(int stmtNo, stmtType type);
    // void setUses(int stmtNo, std::string var_name);
    // void setModifies(int stmtNo, std::string var_name);

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

private:
    static ParentStore parentStore;
    static FollowStore followStore;
    bool checkInvalidStmts(int stmtNo1, int stmtNo2);
    static std::unordered_map<std::string, vector<int>> procMap;
    static std::unordered_map<int, stmtType> typeMap;
};