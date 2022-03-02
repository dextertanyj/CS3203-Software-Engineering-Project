#ifndef SPA_MOCKUTILITIES_H
#define SPA_MOCKUTILITIES_H

#include <tuple>
#include <vector>

#include "Common/TypeDefs.h"
#include "PKB/StorageUpdateInterface.h"

class MockStorageUpdate : public PKB::StorageUpdateInterface {
public:
	MockStorageUpdate() = default;
	void setProc(ProcRef procedure, StmtRef start, StmtRef end) {
		set_proc_call_count++;
		set_proc_arguments.push_back({procedure, start, end});
	};
	void setCall(StmtRef index, ProcRef name) {
		set_call_call_count++;
		set_call_arguments.push_back({index, name});
	};
	void setFollows(StmtRef front, StmtRef back) {
		set_follows_call_count++;
		set_follows_arguments.push_back({front, back});
	};
	void setParent(StmtRef parent, StmtRef child) {
		set_parent_call_count++;
		set_parent_arguments.push_back({parent, child});
	};
	void setStmtType(StmtRef index, StmtType type) {
		set_stmt_type_call_count++;
		set_stmt_type_arguments.push_back({index, type});
	};
	void setConstant(ConstVal value) {
		set_constant_call_count++;
		set_constant_arguments.push_back(value);
	};
	void setConstant(const unordered_set<ConstVal>& values) {
		set_constant_set_call_count++;
		set_constant_set_arguments.push_back(values);
	};
	void setUses(StmtRef index, VarRef name) {
		set_uses_call_count++;
		set_uses_arguments.push_back({index, name});
	};
	void setModifies(StmtRef index, VarRef name) {
		set_modifies_call_count++;
		set_modifies_arguments.push_back({index, name});
	};
	void setUses(StmtRef index, VarRefSet names) {
		set_uses_set_call_count++;
		set_uses_set_arguments.push_back({index, names});
	};
	void setModifies(StmtRef index, VarRefSet names) {
		set_modifies_set_call_count++;
		set_modifies_set_arguments.push_back({index, names});
	};
	void setAssign(StmtRef index, VarRef variable, Common::ExpressionProcessor::Expression expression) {
		set_assign_call_count++;
	};
	void populateComplexRelations() {
		populate_complex_relations_call_count++;
	};

	vector<tuple<ProcRef, StmtRef, StmtRef>> set_proc_arguments;
	int set_proc_call_count = 0;
	vector<tuple<StmtRef, ProcRef>> set_call_arguments;
	int set_call_call_count = 0;
	vector<tuple<StmtRef, StmtRef>> set_follows_arguments;
	int set_follows_call_count = 0;
	vector<tuple<StmtRef, StmtRef>> set_parent_arguments;
	int set_parent_call_count = 0;
	vector<tuple<StmtRef, StmtType>> set_stmt_type_arguments;
	int set_stmt_type_call_count = 0;
	vector<ConstVal> set_constant_arguments;
	int set_constant_call_count = 0;
	vector<unordered_set<ConstVal>> set_constant_set_arguments;
	int set_constant_set_call_count = 0;
	vector<tuple<StmtRef, VarRef>> set_uses_arguments;
	int set_uses_call_count = 0;
	vector<tuple<StmtRef, VarRef>> set_modifies_arguments;
	int set_modifies_call_count = 0;
	vector<tuple<StmtRef, unordered_set<VarRef>>> set_uses_set_arguments;
	int set_uses_set_call_count = 0;
	vector<tuple<StmtRef, unordered_set<VarRef>>> set_modifies_set_arguments;
	int set_modifies_set_call_count = 0;
	vector<tuple<StmtRef, VarRef, Common::ExpressionProcessor::Expression>> set_assign_arguments;
	int set_assign_call_count = 0;
	int populate_complex_relations_call_count = 0;
};

#endif  // SPA_MOCKUTILITIES_H
