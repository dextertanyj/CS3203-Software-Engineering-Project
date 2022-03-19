#include "PKB/Storage.h"

#include <algorithm>
#include <iterator>

using namespace std;

// This method will store information about a statement into PKB's statement map.
// Source Processor is guaranteed to call this method before storing relationships and variables.
void PKB::Storage::setStmtType(StmtRef index, StmtType type) {
	statement_store.insert(index, type);
	shared_ptr<StmtInfo> info = statement_store.get(index);
	setNode(move(info));
}

void PKB::Storage::setConstant(ConstVal value) { constant_store.insert(value); }

void PKB::Storage::setConstant(const unordered_set<ConstVal> &values) { constant_store.insert(values); }

void PKB::Storage::setProc(ProcRef procedure, StmtRef start, StmtRef end) {
	vector<shared_ptr<StmtInfo>> statements;
	for (StmtRef index = start; index <= end; index++) {
		shared_ptr<StmtInfo> statement = statement_store.get(index);
		if (statement == nullptr) {
			throw invalid_argument("Statement not found.");
		}
		statements.push_back(statement);
	}
	procedure_store.insert(procedure, statements);
}

void PKB::Storage::setCall(StmtRef index, ProcRef name) {
	shared_ptr<StmtInfo> info = statement_store.get(index);
	call_statement_store.set(info, name);
}

void PKB::Storage::setParent(StmtRef parent, StmtRef child) {
	shared_ptr<StmtInfo> parent_info = statement_store.get(parent);
	shared_ptr<StmtInfo> child_info = statement_store.get(child);
	if (parent_info == nullptr || child_info == nullptr) {
		throw invalid_argument("Statement does not exist.");
	}
	parent_store.set(parent_info, child_info);
}

void PKB::Storage::setFollows(StmtRef front, StmtRef back) {
	shared_ptr<StmtInfo> following_info = statement_store.get(front);
	shared_ptr<StmtInfo> follower_info = statement_store.get(back);
	if (following_info == nullptr || follower_info == nullptr) {
		throw invalid_argument("Statement does not exist.");
	}
	follows_store.set(following_info, follower_info);
}

void PKB::Storage::setModifies(StmtRef index, VarRef name) {
	shared_ptr<StmtInfo> statement = statement_store.get(index);
	if (statement == nullptr) {
		throw invalid_argument("Statement does not exist.");
	}
	variable_store.insert(name);
	modifies_s_store.set(move(statement), move(name));
}

void PKB::Storage::setModifies(StmtRef index, VarRefSet names) {
	shared_ptr<StmtInfo> statement = statement_store.get(index);
	if (statement == nullptr) {
		throw invalid_argument("Statement does not exist.");
	}
	variable_store.insert(names);
	modifies_s_store.set(move(statement), move(names));
}

void PKB::Storage::setUses(StmtRef index, VarRef name) {
	shared_ptr<StmtInfo> statement = statement_store.get(index);
	if (statement == nullptr) {
		throw invalid_argument("Statement does not exist.");
	}
	variable_store.insert(name);
	uses_s_store.set(statement, move(name));
}

void PKB::Storage::setUses(StmtRef index, VarRefSet names) {
	shared_ptr<StmtInfo> statement = statement_store.get(index);
	if (statement == nullptr) {
		throw invalid_argument("Statement does not exist.");
	}
	variable_store.insert(names);
	uses_s_store.set(statement, move(names));
}

void PKB::Storage::setAssign(StmtRef index, VarRef variable, Common::ExpressionProcessor::Expression expression) {
	shared_ptr<StmtInfo> statement = statement_store.get(index);
	return assign_store.setAssign(statement, move(variable), move(expression));
}

void PKB::Storage::setIfControl(StmtRef index, VarRefSet names) {
	shared_ptr<StmtInfo> info = statement_store.get(index);
	if_control_store.set(info, names);
}

void PKB::Storage::setIfControl(StmtRef index, VarRef name) {
	shared_ptr<StmtInfo> info = statement_store.get(index);
	if_control_store.set(info, name);
}

void PKB::Storage::setWhileControl(StmtRef index, VarRefSet names) {
	shared_ptr<StmtInfo> info = statement_store.get(index);
	while_control_store.set(info, names);
}

void PKB::Storage::setWhileControl(StmtRef index, VarRef name) {
	shared_ptr<StmtInfo> info = statement_store.get(index);
	while_control_store.set(info, name);
}

void PKB::Storage::setNode(shared_ptr<StmtInfo> info) { this->control_flow_graph.createNode(move(info)); }

void PKB::Storage::setNext(StmtRef previous, StmtRef next) { this->control_flow_graph.setNext(previous, next); }

void PKB::Storage::setIfNext(StmtRef prev, StmtRef then_next, StmtRef else_next) {
	this->control_flow_graph.setIfNext(prev, then_next, else_next);
}

void PKB::Storage::setIfExit(StmtRef then_prev, StmtRef else_prev, StmtRef if_stmt_ref) {
	this->control_flow_graph.setIfExit(then_prev, else_prev, if_stmt_ref);
}

StmtInfoPtrSet PKB::Storage::getStatements() {
	unordered_set<shared_ptr<StatementInfo>> set = statement_store.getAll();
	return statementInfoPtrSetToInterfacePtrSet(set);
}

VarRefSet PKB::Storage::getVariables() { return variable_store.getAll(); }

unordered_set<ConstVal> PKB::Storage::getConstants() { return constant_store.getAll(); }

unordered_set<ProcRef> PKB::Storage::getProcedures() {
	unordered_set<shared_ptr<ProcedureInfo>> procedures = procedure_store.getAll();
	return procedureInfoToProcRef(procedures);
}

bool PKB::Storage::checkParent(StmtRef parent, StmtRef child) { return parent_store.isRelated(parent, child); }

bool PKB::Storage::checkParentStar(StmtRef parent, StmtRef child) { return parent_store.isTransitivelyRelated(parent, child); }

shared_ptr<StmtInfo> PKB::Storage::getParent(StmtRef index) {
	auto result = parent_store.getForward(index);
	if (result.empty()) {
		return nullptr;
	}
	return *result.begin();
}

StmtInfoPtrSet PKB::Storage::getChildren(StmtRef index) { return parent_store.getReverse(index); }

StmtInfoPtrSet PKB::Storage::getParentStar(StmtRef index) { return parent_store.getForwardTransitive(index); }

StmtInfoPtrSet PKB::Storage::getChildStar(StmtRef index) { return parent_store.getReverseTransitive(index); }

bool PKB::Storage::checkFollows(StmtRef front, StmtRef back) { return follows_store.isRelated(front, back); }

bool PKB::Storage::checkFollowsStar(StmtRef front, StmtRef back) { return follows_store.isTransitivelyRelated(front, back); }

shared_ptr<StmtInfo> PKB::Storage::getPreceding(StmtRef index) {
	auto result = follows_store.getForward(index);
	if (result.empty()) {
		return nullptr;
	}
	return *result.begin();
}

shared_ptr<StmtInfo> PKB::Storage::getFollower(StmtRef index) {
	auto result = follows_store.getReverse(index);
	if (result.empty()) {
		return nullptr;
	}
	return *result.begin();
}

StmtInfoPtrSet PKB::Storage::getPrecedingStar(StmtRef index) { return follows_store.getForwardTransitive(index); }

StmtInfoPtrSet PKB::Storage::getFollowerStar(StmtRef index) { return follows_store.getReverseTransitive(index); }

bool PKB::Storage::checkCalls(const ProcRef &caller, const ProcRef &callee) { return call_store.isRelated(caller, callee); }

bool PKB::Storage::checkCallsStar(const ProcRef &caller, const ProcRef &callee) { return call_store.isTransitivelyRelated(caller, callee); }

ProcRefSet PKB::Storage::getCallee(const ProcRef &caller) {
	unordered_set<shared_ptr<ProcedureInfo>> callees = call_store.getReverse(caller);
	return procedureInfoToProcRef(callees);
}

ProcRefSet PKB::Storage::getCaller(const ProcRef &callee) {
	unordered_set<shared_ptr<ProcedureInfo>> callers = call_store.getForward(callee);
	return procedureInfoToProcRef(callers);
}

ProcRefSet PKB::Storage::getCalleeStar(const ProcRef &caller) {
	unordered_set<shared_ptr<ProcedureInfo>> callees = call_store.getReverseTransitive(caller);
	return procedureInfoToProcRef(callees);
}

ProcRefSet PKB::Storage::getCallerStar(const ProcRef &callee) {
	unordered_set<shared_ptr<ProcedureInfo>> callers = call_store.getForwardTransitive(callee);
	return procedureInfoToProcRef(callers);
}

bool PKB::Storage::checkModifies(StmtRef index, const VarRef &name) { return modifies_s_store.check(index, name); }

bool PKB::Storage::checkModifies(const ProcRef &procedure_name, const VarRef &name) { return modifies_p_store.check(procedure_name, name); }

StmtInfoPtrSet PKB::Storage::getStmtModifiesByVar(const VarRef &name) { return modifies_s_store.getByVar(name); }

ProcRefSet PKB::Storage::getProcModifiesByVar(const VarRef &name) { return modifies_p_store.getByVar(name); }

VarRefSet PKB::Storage::getModifiesByStmt(StmtRef index) { return modifies_s_store.getByStmt(index); }

VarRefSet PKB::Storage::getModifiesByProc(const ProcRef &name) { return modifies_p_store.getByProc(name); }

bool PKB::Storage::checkUses(StmtRef index, const VarRef &name) { return uses_s_store.check(index, name); }

bool PKB::Storage::checkUses(const ProcRef &procedure_name, const VarRef &name) { return uses_p_store.check(procedure_name, name); }

StmtInfoPtrSet PKB::Storage::getStmtUsesByVar(const VarRef &name) { return uses_s_store.getByVar(name); }

ProcRefSet PKB::Storage::getProcUsesByVar(const VarRef &name) { return uses_p_store.getByVar(name); }

VarRefSet PKB::Storage::getUsesByStmt(StmtRef index) { return uses_s_store.getByStmt(index); }

VarRefSet PKB::Storage::getUsesByProc(const ProcRef &name) { return uses_p_store.getByProc(name); }

bool PKB::Storage::patternExists(const VarRef &name, const Common::ExpressionProcessor::Expression &exp, bool is_exact_match) {
	return assign_store.patternExists(name, exp, is_exact_match);
}

StmtInfoPtrSet PKB::Storage::getStmtsWithPattern(const VarRef &name, const Common::ExpressionProcessor::Expression &expression,
                                                 bool is_exact_match) {
	return assign_store.getStmtsWithPattern(name, expression, is_exact_match);
}

StmtInfoPtrSet PKB::Storage::getStmtsWithPatternLHS(const VarRef &name) { return assign_store.getStmtsWithPatternLHS(name); }

vector<pair<shared_ptr<StmtInfo>, VarRef>> PKB::Storage::getStmtsWithPatternRHS(const Common::ExpressionProcessor::Expression &expression,
                                                                                bool is_exact_match) {
	return assign_store.getStmtsWithPatternRHS(expression, is_exact_match);
}

bool PKB::Storage::checkNext(StmtRef first, StmtRef second) { return control_flow_graph.checkNext(first, second); }

bool PKB::Storage::checkNextStar(StmtRef first, StmtRef second) { return control_flow_graph.checkNextStar(first, second); }

StmtInfoPtrSet PKB::Storage::getNext(StmtRef first) { return control_flow_graph.getNextNodes(first); }

StmtInfoPtrSet PKB::Storage::getNextStar(StmtRef node_ref) { return control_flow_graph.getNextStarNodes(node_ref); }

StmtInfoPtrSet PKB::Storage::getPrevious(StmtRef second) { return control_flow_graph.getPreviousNodes(second); }

StmtInfoPtrSet PKB::Storage::getPreviousStar(StmtRef node_ref) { return control_flow_graph.getPreviousStarNodes(node_ref); }

bool PKB::Storage::checkWhileControl(StmtRef index, VarRef name) { return while_control_store.check(index, name); }

bool PKB::Storage::checkIfControl(StmtRef index, VarRef name) { return if_control_store.check(index, name); }

StmtInfoPtrSet PKB::Storage::getIfControlStmt(VarRef name) { return if_control_store.getByVar(name); }

StmtInfoPtrSet PKB::Storage::getWhileControlStmt(VarRef name) { return while_control_store.getByVar(name); }

VarRefSet PKB::Storage::getIfControlVar(StmtRef index) { return if_control_store.getByStmt(index); }

VarRefSet PKB::Storage::getWhileControlVar(StmtRef index) { return while_control_store.getByStmt(index); }

void PKB::Storage::populateComplexRelations() {
	call_statement_store.populate(procedure_store, call_store);
	call_store.optimize();
	call_graph.sort(procedure_store, call_store);
	parent_store.optimize();
	follows_store.optimize();
	ModifiesSRelation::optimize(parent_store, call_statement_store, procedure_store, call_graph, modifies_s_store);
	UsesSRelation::optimize(parent_store, call_statement_store, procedure_store, call_graph, uses_s_store);
	ModifiesPRelation::optimize(procedure_store, modifies_p_store, modifies_s_store);
	UsesPRelation::optimize(procedure_store, uses_p_store, uses_s_store);
}

void PKB::Storage::clear() {
	parent_store.clear();
	follows_store.clear();
	assign_store.clear();
	uses_s_store.clear();
	uses_p_store.clear();
	modifies_s_store.clear();
	modifies_p_store.clear();
	statement_store.clear();
	control_flow_graph.clear();
}

ProcRefSet PKB::Storage::procedureInfoToProcRef(const unordered_set<shared_ptr<ProcedureInfo>> &set) {
	ProcRefSet result;
	transform(set.begin(), set.end(), inserter(result, result.begin()),
	          [](const shared_ptr<ProcedureInfo> &info) { return info->getIdentifier(); });
	return result;
}

StmtInfoPtrSet PKB::Storage::statementInfoPtrSetToInterfacePtrSet(const unordered_set<shared_ptr<StatementInfo>> &set) {
	StmtInfoPtrSet result;
	transform(set.begin(), set.end(), inserter(result, result.begin()),
	          [](const shared_ptr<StatementInfo> &info) { return static_pointer_cast<StmtInfo>(info); });
	return result;
}

unordered_map<StmtRef, shared_ptr<StmtInfo>> PKB::Storage::getStmtInfoMap() {
	unordered_set<shared_ptr<StatementInfo>> set = statement_store.getAll();
	unordered_map<StmtRef, shared_ptr<StmtInfo>> map;
	for (const auto &item : set) {
		map.insert({item->getIdentifier(), item});
	}
	return map;
}
