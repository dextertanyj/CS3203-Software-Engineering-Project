#include "QP/Relationship/UsesP.h"

#include <utility>

QP::Relationship::UsesP::UsesP(ReferenceArgument left_ent, ReferenceArgument right_ent) {
	if (left_ent.getType() == ReferenceType::Wildcard) {
		throw QueryException("Ambiguous wildcard _.");
	}
	this->left_ent = std::move(std::move(left_ent));
	this->right_ent = std::move(std::move(right_ent));
}

ReferenceArgument QP::Relationship::UsesP::getLeftEnt() { return left_ent; }

ReferenceArgument QP::Relationship::UsesP::getRightEnt() { return right_ent; }

vector<string> QP::Relationship::UsesP::getDeclarationSymbols() {
	vector<string> declaration_symbols;
	if (this->left_ent.getType() == ReferenceType::Synonym) {
		declaration_symbols.push_back(this->left_ent.getSynonym().symbol);
	}
	if (this->right_ent.getType() == ReferenceType::Synonym) {
		declaration_symbols.push_back(this->right_ent.getSynonym().symbol);
	}
	return declaration_symbols;
}

QP::QueryResult QP::Relationship::UsesP::executeTrivial(PKB::StorageAccessInterface &pkb) {
	if (left_ent.getType() == ReferenceType::Name && right_ent.getType() == ReferenceType::Name) {
		return executeTrivialNameName(pkb, left_ent, right_ent);
	}
	if ((left_ent.getType() == ReferenceType::Name && right_ent.getType() == ReferenceType::Wildcard) ||
	    (left_ent.getType() == ReferenceType::Name && right_ent.getType() == ReferenceType::Synonym)) {
		return executeTrivialName(pkb, left_ent);
	}
	if (left_ent.getType() == ReferenceType::Synonym && right_ent.getType() == ReferenceType::Name) {
		return executeTrivialSynonymName(pkb, right_ent);
	}
	if ((left_ent.getType() == ReferenceType::Synonym && right_ent.getType() == ReferenceType::Wildcard) ||
	    (left_ent.getType() == ReferenceType::Synonym && right_ent.getType() == ReferenceType::Synonym)) {
		return executeTrivialSynonym(pkb);
	}
	return {};
}

QP::QueryResult QP::Relationship::UsesP::executeNonTrivial(PKB::StorageAccessInterface &pkb) {
	if (left_ent.getType() == ReferenceType::Name && right_ent.getType() == ReferenceType::Synonym) {
		return executeVarNameSynonym(pkb, left_ent, right_ent);
	}
	if (left_ent.getType() == ReferenceType::Synonym && right_ent.getType() == ReferenceType::Name) {
		return executeSynonymName(pkb, left_ent, right_ent);
	}
	if (left_ent.getType() == ReferenceType::Synonym && right_ent.getType() == ReferenceType::Wildcard) {
		return executeSynonymWildcard(pkb, left_ent);
	}
	if (left_ent.getType() == ReferenceType::Synonym && right_ent.getType() == ReferenceType::Synonym) {
		return executeSynonymSynonym(pkb, left_ent, right_ent);
	}
	return {};
}

QP::QueryResult QP::Relationship::UsesP::executeTrivialNameName(PKB::StorageAccessInterface &pkb, const ReferenceArgument &left_ent,
                                                                const ReferenceArgument &right_ent) {
	return QueryResult(pkb.checkUses(left_ent.getName(), right_ent.getName()));
}

QP::QueryResult QP::Relationship::UsesP::executeTrivialName(PKB::StorageAccessInterface &pkb, const ReferenceArgument &left_ent) {
	VarRefSet var_set = pkb.getUsesByProc(left_ent.getName());
	return QueryResult(!var_set.empty());
}

QP::QueryResult QP::Relationship::UsesP::executeTrivialSynonymName(PKB::StorageAccessInterface &pkb, const ReferenceArgument &right_ent) {
	ProcRefSet proc_set = pkb.getProcUsesByVar(right_ent.getName());
	return QueryResult(!proc_set.empty());
}

QP::QueryResult QP::Relationship::UsesP::executeTrivialSynonym(PKB::StorageAccessInterface &pkb) {
	unordered_set<ProcRef> proc_set = pkb.getProcedures();
	for (auto const &proc : proc_set) {
		VarRefSet var_set = pkb.getUsesByProc(proc);
		if (!var_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::UsesP::executeVarNameSynonym(PKB::StorageAccessInterface &pkb, const ReferenceArgument &left_ent,
                                                               const ReferenceArgument &right_ent) {
	VarRefSet var_set = pkb.getUsesByProc(left_ent.getName());
	vector<string> column;

	for (auto const &var : var_set) {
		column.push_back(var);
	}

	QueryResult result = QueryResult();
	result.addColumn(right_ent.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::UsesP::executeSynonymName(PKB::StorageAccessInterface &pkb, const ReferenceArgument &left_ent,
                                                               const ReferenceArgument &right_ent) {
	ProcRefSet proc_set = pkb.getProcUsesByVar(right_ent.getName());
	vector<string> column;
	for (auto const &proc : proc_set) {
		column.push_back(proc);
	}
	QueryResult result = QueryResult();
	result.addColumn(left_ent.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::UsesP::executeSynonymWildcard(PKB::StorageAccessInterface &pkb, const ReferenceArgument &left_ent) {
	unordered_set<ProcRef> proc_set = pkb.getProcedures();
	vector<string> column;
	for (auto const &proc : proc_set) {
		VarRefSet var_set = pkb.getUsesByProc(proc);
		if (!var_set.empty()) {
			column.push_back(proc);
		}
	}
	QueryResult result = QueryResult();
	result.addColumn(left_ent.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::UsesP::executeSynonymSynonym(PKB::StorageAccessInterface &pkb, const ReferenceArgument &left_ent,
                                                               const ReferenceArgument &right_ent) {
	unordered_set<ProcRef> proc_set = pkb.getProcedures();
	vector<string> proc_column;
	vector<string> var_column;
	for (auto const &proc : proc_set) {
		VarRefSet var_set = pkb.getUsesByProc(proc);
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
