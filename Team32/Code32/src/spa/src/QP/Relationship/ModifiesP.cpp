#include "QP/Relationship/ModifiesP.h"

#include <unordered_set>
#include <utility>
#include <vector>

QP::QueryResult QP::Relationship::ModifiesP::executeTrivialNameName(PKB::StorageAccessInterface &pkb,
                                                                    const Types::ReferenceArgument &left_ent,
                                                                    const Types::ReferenceArgument &right_ent) {
	return QueryResult(pkb.checkModifies(left_ent.getName(), right_ent.getName()));
}
QP::QueryResult QP::Relationship::ModifiesP::executeTrivialNameWildcardOrSynonym(PKB::StorageAccessInterface &pkb,
                                                                                 const Types::ReferenceArgument &left_ent) {
	VarRefSet var_set = pkb.getModifiesByProc(left_ent.getName());
	return QueryResult(!var_set.empty());
}
QP::QueryResult QP::Relationship::ModifiesP::executeTrivialSynonymName(PKB::StorageAccessInterface &pkb,
                                                                       const Types::ReferenceArgument &right_ent) {
	ProcRefSet proc_set = pkb.getProcModifiesByVar(right_ent.getName());
	return QueryResult(!proc_set.empty());
}

QP::QueryResult QP::Relationship::ModifiesP::executeTrivialSynonymWildcardOrSynonym(PKB::StorageAccessInterface &pkb) {
	unordered_set<ProcRef> proc_set = pkb.getProcedures();
	for (auto const &proc : proc_set) {
		VarRefSet var_set = pkb.getModifiesByProc(proc);
		if (!var_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::ModifiesP::executeNameSynonym(PKB::StorageAccessInterface &pkb, const Types::ReferenceArgument &left_ent,
                                                                const Types::ReferenceArgument &right_ent) {
	VarRefSet var_set = pkb.getModifiesByProc(left_ent.getName());
	vector<string> column;
	for (auto const &var : var_set) {
		column.push_back(var);
	}
	QueryResult result = QueryResult();
	result.addColumn(right_ent.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::ModifiesP::executeSynonymName(PKB::StorageAccessInterface &pkb, const Types::ReferenceArgument &left_ent,
                                                                const Types::ReferenceArgument &right_ent) {
	ProcRefSet proc_set = pkb.getProcModifiesByVar(right_ent.getName());
	vector<string> column;
	for (auto const &proc : proc_set) {
		column.push_back(proc);
	}
	QueryResult result = QueryResult();
	result.addColumn(left_ent.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::ModifiesP::executeSynonymWildcard(PKB::StorageAccessInterface &pkb,
                                                                    const Types::ReferenceArgument &left_ent) {
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

QP::QueryResult QP::Relationship::ModifiesP::executeSynonymSynonym(PKB::StorageAccessInterface &pkb,
                                                                   const Types::ReferenceArgument &left_ent,
                                                                   const Types::ReferenceArgument &right_ent) {
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
