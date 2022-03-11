#include "QP/Relationship/UsesP.h"

#include <utility>

QP::QueryResult QP::Relationship::UsesP::executeTrivialNameName(PKB::StorageAccessInterface &pkb, const Types::ReferenceArgument& left_ent,
                                                                const Types::ReferenceArgument& right_ent) {
	return QueryResult(pkb.checkUses(left_ent.getName(), right_ent.getName()));
}

QP::QueryResult QP::Relationship::UsesP::executeTrivialNameWildcardOrSynonym(PKB::StorageAccessInterface &pkb,
                                                                             const Types::ReferenceArgument& left_ent) {
	VarRefSet var_set = pkb.getUsesByProc(left_ent.getName());
	return QueryResult(!var_set.empty());
}

QP::QueryResult QP::Relationship::UsesP::executeTrivialSynonymName(PKB::StorageAccessInterface &pkb, const Types::ReferenceArgument& right_ent) {
	ProcRefSet proc_set = pkb.getProcUsesByVar(right_ent.getName());
	return QueryResult(!proc_set.empty());
}

QP::QueryResult QP::Relationship::UsesP::executeTrivialSynonymWildcardOrSynonym(PKB::StorageAccessInterface &pkb) {
	unordered_set<ProcRef> proc_set = pkb.getProcedures();
	for (auto const &proc : proc_set) {
		VarRefSet var_set = pkb.getUsesByProc(proc);
		if (!var_set.empty()) {
			return QueryResult(true);
		}
	}
	return {};
}

QP::QueryResult QP::Relationship::UsesP::executeNameSynonym(PKB::StorageAccessInterface &pkb, const Types::ReferenceArgument& left_ent,
                                                            const Types::ReferenceArgument& right_ent) {
	VarRefSet var_set = pkb.getUsesByProc(left_ent.getName());
	vector<string> column;

	for (auto const &var : var_set) {
		column.push_back(var);
	}

	QueryResult result = QueryResult();
	result.addColumn(right_ent.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::UsesP::executeSynonymName(PKB::StorageAccessInterface &pkb, const Types::ReferenceArgument& left_ent,
                                                            const Types::ReferenceArgument& right_ent) {
	ProcRefSet proc_set = pkb.getProcUsesByVar(right_ent.getName());
	vector<string> column;
	for (auto const &proc : proc_set) {
		column.push_back(proc);
	}
	QueryResult result = QueryResult();
	result.addColumn(left_ent.getSynonym().symbol, column);
	return result;
}

QP::QueryResult QP::Relationship::UsesP::executeSynonymWildcard(PKB::StorageAccessInterface &pkb, const Types::ReferenceArgument& left_ent) {
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

QP::QueryResult QP::Relationship::UsesP::executeSynonymSynonym(PKB::StorageAccessInterface &pkb, const Types::ReferenceArgument& left_ent,
                                                               const Types::ReferenceArgument& right_ent) {
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
