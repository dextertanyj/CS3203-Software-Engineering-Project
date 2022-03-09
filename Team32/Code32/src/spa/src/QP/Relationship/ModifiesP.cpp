#include "QP/Relationship/ModifiesP.h"

#include <utility>

QP::Relationship::ModifiesP::ModifiesP(ReferenceArgument left_ent, ReferenceArgument right_ent) {
	if (left_ent.getType() == ReferenceType::Wildcard) {
		throw QueryException("Ambiguous wildcard _.");
	}
	this->left_ent = std::move(std::move(left_ent));
	this->right_ent = std::move(std::move(right_ent));
}

ReferenceArgument QP::Relationship::ModifiesP::getLeftEnt() { return left_ent; }

ReferenceArgument QP::Relationship::ModifiesP::getRightEnt() { return right_ent; }

vector<string> QP::Relationship::ModifiesP::getDeclarationSymbols() {
	vector<string> declaration_symbols;
	if (this->left_ent.getType() == ReferenceType::Synonym) {
		declaration_symbols.push_back(this->left_ent.getSynonym().symbol);
	}
	if (this->right_ent.getType() == ReferenceType::Synonym) {
		declaration_symbols.push_back(this->right_ent.getSynonym().symbol);
	}
	return declaration_symbols;
}

QP::QueryResult QP::Relationship::ModifiesP::executeTrivial(PKB::StorageAccessInterface &pkb) {
	if (left_ent.getType() == ReferenceType::Name && right_ent.getType() == ReferenceType::Name) {
		return QueryResult(pkb.checkModifies(left_ent.getName(), right_ent.getName()));
	}
	if ((left_ent.getType() == ReferenceType::Name && right_ent.getType() == ReferenceType::Wildcard) ||
	    (left_ent.getType() == ReferenceType::Name && right_ent.getType() == ReferenceType::Synonym)) {
		VarRefSet var_set = pkb.getModifiesByProc(left_ent.getName());
		return QueryResult(!var_set.empty());
	}
	if (left_ent.getType() == ReferenceType::Synonym && right_ent.getType() == ReferenceType::Name) {
		ProcRefSet proc_set = pkb.getProcModifiesByVar(right_ent.getName());
		return QueryResult(!proc_set.empty());
	}
	if ((left_ent.getType() == ReferenceType::Synonym && right_ent.getType() == ReferenceType::Wildcard) ||
	    (left_ent.getType() == ReferenceType::Synonym && right_ent.getType() == ReferenceType::Synonym)) {
		return executeTrivialBothUnknown(pkb);
	}
	return {};
}

QP::QueryResult QP::Relationship::ModifiesP::executeNonTrivial(PKB::StorageAccessInterface &pkb) {
	if (left_ent.getType() == ReferenceType::Name && right_ent.getType() == ReferenceType::Synonym) {
		return executeNonTrivialLeftEntVarName(pkb);
	}
	if (left_ent.getType() == ReferenceType::Synonym && right_ent.getType() == ReferenceType::Name) {
		return executeNonTrivialRightEntVarName(pkb);
	}
	if (left_ent.getType() == ReferenceType::Synonym && right_ent.getType() == ReferenceType::Wildcard) {
		return executeNonTrivialRightEntUnderscore(pkb);
	}
	if (left_ent.getType() == ReferenceType::Synonym && right_ent.getType() == ReferenceType::Synonym) {
		return executeNonTrivialRightEntSynonym(pkb);
	}
	return {};
}

QP::QueryResult QP::Relationship::ModifiesP::executeTrivialBothUnknown(PKB::StorageAccessInterface &pkb) {
	unordered_set<ProcRef> proc_set = pkb.getProcedures();
	for (auto const &proc : proc_set) {
		VarRefSet var_set = pkb.getModifiesByProc(proc);
		if (!var_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::ModifiesP::executeNonTrivialLeftEntVarName(PKB::StorageAccessInterface &pkb) {
	VarRefSet var_set = pkb.getModifiesByProc(left_ent.getName());
	vector<string> column;
	for (auto const &var : var_set) {
		column.push_back(var);
	}
	QueryResult result = QueryResult();
	result.addColumn(right_ent.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::ModifiesP::executeNonTrivialRightEntVarName(PKB::StorageAccessInterface &pkb) {
	ProcRefSet proc_set = pkb.getProcModifiesByVar(right_ent.getName());
	vector<string> column;
	for (auto const &proc : proc_set) {
		column.push_back(proc);
	}
	QueryResult result = QueryResult();
	result.addColumn(left_ent.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::ModifiesP::executeNonTrivialRightEntUnderscore(PKB::StorageAccessInterface &pkb) {
	unordered_set<ProcRef> proc_set = pkb.getProcedures();
	vector<string> column;
	for (auto const &proc : proc_set) {
		VarRefSet var_set = pkb.getModifiesByProc(proc);
		if (!var_set.empty()) {
			column.push_back(proc);
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(left_ent.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::ModifiesP::executeNonTrivialRightEntSynonym(PKB::StorageAccessInterface &pkb) {
	unordered_set<ProcRef> proc_set = pkb.getProcedures();
	vector<string> proc_column;
	vector<string> var_column;
	for (auto const &proc : proc_set) {
		VarRefSet var_set = pkb.getModifiesByProc(proc);
		for (auto const &var : var_set) {
			proc_column.push_back(proc);
			var_column.push_back(var);
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(left_ent.getSynonym().symbol, proc_column);
	result.addColumn(right_ent.getSynonym().symbol, var_column);
	return result;
}
