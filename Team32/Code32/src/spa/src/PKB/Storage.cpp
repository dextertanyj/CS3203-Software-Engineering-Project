#include "PKB/Storage.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <utility>
#include <vector>

using namespace std;

PKB::Storage::Storage() : next_manager(control_flow_graph), affects_manager(control_flow_graph, modifies_s_store, uses_s_store) {}

void PKB::Storage::setStmtType(StmtRef index, StmtType type) {
	statement_store.insert(index, type);
	StmtInfoPtr info = statement_store.get(index);
	setNode(info);
}

void PKB::Storage::setConstant(ConstVal value) { constant_store.insert(value); }

void PKB::Storage::setConstant(const ConstValSet& values) { constant_store.insert(values); }

void PKB::Storage::setProc(ProcRef procedure, StmtRef start, StmtRef end) {
	vector<StmtInfoPtr> statements;
	for (StmtRef index = start; index <= end; index++) {
		StmtInfoPtr statement = statement_store.get(index);
		assert(statement != nullptr);
		statements.push_back(statement);
	}
	procedure_store.insert(procedure, statements);
}

void PKB::Storage::setCall(StmtRef index, ProcRef name) {
	StmtInfoPtr info = statement_store.get(index);
	calls_statement_store.set(info, name);
}

void PKB::Storage::setParent(StmtRef parent, StmtRef child) {
	StmtInfoPtr parent_info = statement_store.get(parent);
	StmtInfoPtr child_info = statement_store.get(child);
	assert(parent_info != nullptr && child_info != nullptr);
	parent_store.set(parent_info, child_info);
}

void PKB::Storage::setFollows(StmtRef front, StmtRef back) {
	StmtInfoPtr following_info = statement_store.get(front);
	StmtInfoPtr follower_info = statement_store.get(back);
	assert(following_info != nullptr && follower_info != nullptr);
	follows_store.set(following_info, follower_info);
}

void PKB::Storage::setModifies(StmtRef index, VarRef name) {
	StmtInfoPtr statement = statement_store.get(index);
	assert(statement != nullptr);
	variable_store.insert(name);
	modifies_s_store.set(move(statement), move(name));
}

void PKB::Storage::setModifies(StmtRef index, VarRefSet names) {
	StmtInfoPtr statement = statement_store.get(index);
	assert(statement != nullptr);
	variable_store.insert(names);
	modifies_s_store.set(move(statement), move(names));
}

void PKB::Storage::setUses(StmtRef index, VarRef name) {
	StmtInfoPtr statement = statement_store.get(index);
	assert(statement != nullptr);
	variable_store.insert(name);
	uses_s_store.set(statement, move(name));
}

void PKB::Storage::setUses(StmtRef index, VarRefSet names) {
	StmtInfoPtr statement = statement_store.get(index);
	assert(statement != nullptr);
	variable_store.insert(names);
	uses_s_store.set(statement, move(names));
}

void PKB::Storage::setAssign(StmtRef index, VarRef variable, Common::EP::Expression expression) {
	StmtInfoPtr statement = statement_store.get(index);
	return assign_store.setAssign(statement, variable, expression);
}

void PKB::Storage::setIfControl(StmtRef index, VarRefSet names) {
	StmtInfoPtr info = statement_store.get(index);
	if_control_store.set(info, names);
}

void PKB::Storage::setIfControl(StmtRef index, VarRef name) {
	StmtInfoPtr info = statement_store.get(index);
	if_control_store.set(info, name);
}

void PKB::Storage::setWhileControl(StmtRef index, VarRefSet names) {
	StmtInfoPtr info = statement_store.get(index);
	while_control_store.set(info, names);
}

void PKB::Storage::setWhileControl(StmtRef index, VarRef name) {
	StmtInfoPtr info = statement_store.get(index);
	while_control_store.set(info, name);
}

void PKB::Storage::setNode(const StmtInfoPtr& info) { control_flow_graph.createNode(info); }

void PKB::Storage::setNext(StmtRef previous, StmtRef next) { control_flow_graph.setNext(previous, next); }

void PKB::Storage::setIfNext(StmtRef prev, StmtRef then_next, StmtRef else_next) {
	control_flow_graph.setIfNext(prev, then_next, else_next);
}

void PKB::Storage::setIfExit(StmtRef then_prev, StmtRef else_prev, StmtRef if_stmt_ref) {
	control_flow_graph.setIfExit(then_prev, else_prev, if_stmt_ref);
}

StmtInfoPtrSet PKB::Storage::getStatements() {
	unordered_set<shared_ptr<StatementInfo>> set = statement_store.getAll();
	return statementInfoPtrSetToInterfacePtrSet(set);
}

VarRefSet PKB::Storage::getVariables() { return variable_store.getAll(); }

ConstValSet PKB::Storage::getConstants() { return constant_store.getAll(); }

ProcRefSet PKB::Storage::getProcedures() {
	unordered_set<shared_ptr<ProcedureInfo>> procedures = procedure_store.getAll();
	return procedureInfoToProcRef(procedures);
}

bool PKB::Storage::checkParent(StmtRef parent, StmtRef child) { return parent_store.isRelated(parent, child); }

bool PKB::Storage::checkParentStar(StmtRef parent, StmtRef child) { return parent_store.isTransitivelyRelated(parent, child); }

StmtInfoPtr PKB::Storage::getParent(StmtRef index) {
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

StmtInfoPtr PKB::Storage::getPreceding(StmtRef index) {
	auto result = follows_store.getForward(index);
	if (result.empty()) {
		return nullptr;
	}
	return *result.begin();
}

StmtInfoPtr PKB::Storage::getFollower(StmtRef index) {
	auto result = follows_store.getReverse(index);
	if (result.empty()) {
		return nullptr;
	}
	return *result.begin();
}

StmtInfoPtrSet PKB::Storage::getPrecedingStar(StmtRef index) { return follows_store.getForwardTransitive(index); }

StmtInfoPtrSet PKB::Storage::getFollowerStar(StmtRef index) { return follows_store.getReverseTransitive(index); }

bool PKB::Storage::checkCalls(const ProcRef& caller, const ProcRef& callee) { return calls_store.isRelated(caller, callee); }

bool PKB::Storage::checkCallsStar(const ProcRef& caller, const ProcRef& callee) {
	return calls_store.isTransitivelyRelated(caller, callee);
}

ProcRefSet PKB::Storage::getCallee(const ProcRef& caller) {
	unordered_set<shared_ptr<ProcedureInfo>> callees = calls_store.getReverse(caller);
	return procedureInfoToProcRef(callees);
}

ProcRefSet PKB::Storage::getCaller(const ProcRef& callee) {
	unordered_set<shared_ptr<ProcedureInfo>> callers = calls_store.getForward(callee);
	return procedureInfoToProcRef(callers);
}

ProcRefSet PKB::Storage::getCalleeStar(const ProcRef& caller) {
	unordered_set<shared_ptr<ProcedureInfo>> callees = calls_store.getReverseTransitive(caller);
	return procedureInfoToProcRef(callees);
}

ProcRefSet PKB::Storage::getCallerStar(const ProcRef& callee) {
	unordered_set<shared_ptr<ProcedureInfo>> callers = calls_store.getForwardTransitive(callee);
	return procedureInfoToProcRef(callers);
}

bool PKB::Storage::checkModifies(StmtRef index, const VarRef& name) { return modifies_s_store.check(index, name); }

bool PKB::Storage::checkModifies(const ProcRef& procedure, const VarRef& variable) { return modifies_p_store.check(procedure, variable); }

StmtInfoPtrSet PKB::Storage::getStmtModifiesByVar(const VarRef& name) { return modifies_s_store.getByVar(name); }

ProcRefSet PKB::Storage::getProcModifiesByVar(const VarRef& name) { return modifies_p_store.getByVar(name); }

VarRefSet PKB::Storage::getModifiesByStmt(StmtRef index) { return modifies_s_store.getByStmt(index); }

VarRefSet PKB::Storage::getModifiesByProc(const ProcRef& name) { return modifies_p_store.getByProc(name); }

bool PKB::Storage::checkUses(StmtRef index, const VarRef& name) { return uses_s_store.check(index, name); }

bool PKB::Storage::checkUses(const ProcRef& procedure, const VarRef& variable) { return uses_p_store.check(procedure, variable); }

StmtInfoPtrSet PKB::Storage::getStmtUsesByVar(const VarRef& name) { return uses_s_store.getByVar(name); }

ProcRefSet PKB::Storage::getProcUsesByVar(const VarRef& name) { return uses_p_store.getByVar(name); }

VarRefSet PKB::Storage::getUsesByStmt(StmtRef index) { return uses_s_store.getByStmt(index); }

VarRefSet PKB::Storage::getUsesByProc(const ProcRef& name) { return uses_p_store.getByProc(name); }

bool PKB::Storage::patternExists(const VarRef& name, const Common::EP::Expression& expression, bool is_exact_match) {
	return assign_store.patternExists(name, expression, is_exact_match);
}

StmtInfoPtrSet PKB::Storage::getStmtsWithPattern(const VarRef& name, const Common::EP::Expression& expression, bool is_exact_match) {
	return assign_store.getStmtsWithPattern(name, expression, is_exact_match);
}

StmtInfoPtrSet PKB::Storage::getStmtsWithPatternLHS(const VarRef& name) { return assign_store.getStmtsWithPatternLHS(name); }

StmtInfoPtrVarRefSet PKB::Storage::getStmtsWithPatternRHS(const Common::EP::Expression& expression, bool is_exact_match) {
	return assign_store.getStmtsWithPatternRHS(expression, is_exact_match);
}

bool PKB::Storage::checkNext(StmtRef first, StmtRef second) { return next_manager.checkNext(first, second); }

bool PKB::Storage::checkNextStar(StmtRef first, StmtRef second) { return next_manager.checkNextStar(first, second); }

StmtInfoPtrSet PKB::Storage::getNext(StmtRef index) { return next_manager.getNext(index); }

StmtInfoPtrSet PKB::Storage::getNextStar(StmtRef index) { return next_manager.getNextStar(index); }

StmtInfoPtrSet PKB::Storage::getPrevious(StmtRef index) { return next_manager.getPrevious(index); }

StmtInfoPtrSet PKB::Storage::getPreviousStar(StmtRef index) { return next_manager.getPreviousStar(index); }

bool PKB::Storage::checkAffects(StmtRef first, StmtRef second) { return affects_manager.checkAffects(first, second); }

bool PKB::Storage::checkAffectsStar(StmtRef first, StmtRef second) { return affects_manager.checkAffectsStar(first, second); }

StmtInfoPtrSet PKB::Storage::getAffected(StmtRef index) { return affects_manager.getAffected(index); }

StmtInfoPtrSet PKB::Storage::getAffects(StmtRef index) { return affects_manager.getAffects(index); }

StmtInfoPtrSet PKB::Storage::getAffectedStar(StmtRef index) { return affects_manager.getAffectedStar(index); }

StmtInfoPtrSet PKB::Storage::getAffectsStar(StmtRef index) { return affects_manager.getAffectsStar(index); }

void PKB::Storage::resetCFGCache() {
	next_manager.resetCache();
	affects_manager.resetCache();
}

bool PKB::Storage::checkWhileControl(StmtRef index, VarRef name) { return while_control_store.check(index, name); }

bool PKB::Storage::checkIfControl(StmtRef index, VarRef name) { return if_control_store.check(index, name); }

StmtInfoPtrSet PKB::Storage::getIfControlStmt(VarRef name) { return if_control_store.getByVar(name); }

StmtInfoPtrSet PKB::Storage::getWhileControlStmt(VarRef name) { return while_control_store.getByVar(name); }

VarRefSet PKB::Storage::getIfControlVar(StmtRef index) { return if_control_store.getByStmt(index); }

VarRefSet PKB::Storage::getWhileControlVar(StmtRef index) { return while_control_store.getByStmt(index); }

ProcRef PKB::Storage::getCalledProcedure(StmtRef index) { return calls_statement_store.getProcedure(index); }

void PKB::Storage::populateComplexRelations() {
	calls_statement_store.populateCallStore(procedure_store, calls_store);
	calls_store.optimize();
	try {
		calls_graph.sort(procedure_store, calls_store);
	} catch (const TopologicalSortException& e) {
		throw CallGraphException(e.what());
	}
	parent_store.optimize();
	follows_store.optimize();
	ModifiesSRelation::optimize(parent_store, calls_statement_store, procedure_store, calls_graph, modifies_s_store);
	UsesSRelation::optimize(parent_store, calls_statement_store, procedure_store, calls_graph, uses_s_store);
	ModifiesPRelation::optimize(procedure_store, modifies_p_store, modifies_s_store);
	UsesPRelation::optimize(procedure_store, uses_p_store, uses_s_store);
	control_flow_graph.optimize();
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

ProcRefSet PKB::Storage::procedureInfoToProcRef(const unordered_set<shared_ptr<ProcedureInfo>>& set) {
	ProcRefSet result;
	transform(set.begin(), set.end(), inserter(result, result.begin()),
	          [](const shared_ptr<ProcedureInfo>& info) { return info->getIdentifier(); });
	return result;
}

StmtInfoPtrSet PKB::Storage::statementInfoPtrSetToInterfacePtrSet(const unordered_set<shared_ptr<StatementInfo>>& set) {
	StmtInfoPtrSet result;
	transform(set.begin(), set.end(), inserter(result, result.begin()),
	          [](const shared_ptr<StatementInfo>& info) { return static_pointer_cast<StmtInfo>(info); });
	return result;
}

unordered_map<StmtRef, StmtInfoPtr> PKB::Storage::getStmtInfoMap() {
	unordered_set<shared_ptr<StatementInfo>> set = statement_store.getAll();
	unordered_map<StmtRef, StmtInfoPtr> map;
	for (const auto& item : set) {
		map.emplace(item->getIdentifier(), item);
	}
	return map;
}
