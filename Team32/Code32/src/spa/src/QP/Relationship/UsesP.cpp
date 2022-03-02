#include "QP/Relationship/UsesP.h"

#include <utility>

QP::Relationship::UsesP::UsesP(QueryEntRef left_ent, QueryEntRef right_ent)
        : left_ent(std::move(std::move(left_ent))), right_ent(std::move(std::move(right_ent))) {}

QueryEntRef QP::Relationship::UsesP::getLeftEnt() { return left_ent; }

QueryEntRef QP::Relationship::UsesP::getRightEnt() { return right_ent; }

vector<string> QP::Relationship::UsesP::getDeclarationSymbols() {
    vector<string> declaration_symbols;
    if (this->left_ent.type == EntRefType::Synonym) {
        declaration_symbols.push_back(this->left_ent.ent_ref);
    }
    if (this->right_ent.type == EntRefType::Synonym) {
        declaration_symbols.push_back(this->right_ent.ent_ref);
    }
    return declaration_symbols;
}

QP::QueryResult QP::Relationship::UsesP::executeTrivial(PKB::StorageAccessInterface &pkb,
                                                        unordered_map<string, DesignEntity> & /*map*/) {
    if (left_ent.type == EntRefType::VarName && right_ent.type == EntRefType::VarName) {
        return QueryResult(pkb.checkUses(left_ent.ent_ref, right_ent.ent_ref));
    }
    if ((left_ent.type == EntRefType::VarName && right_ent.type == EntRefType::Underscore) ||
        (left_ent.type == EntRefType::VarName && right_ent.type == EntRefType::Synonym)) {
        VarRefSet var_set = pkb.getUsesByProc(left_ent.ent_ref);
        return QueryResult(!var_set.empty());
    }
    if (left_ent.type == EntRefType::Underscore) {
        return QueryResult(false);
    }
    if (left_ent.type == EntRefType::Synonym && right_ent.type == EntRefType::VarName) {
        ProcRefSet proc_set = pkb.getProcUsesByVar(right_ent.ent_ref);
        return QueryResult(!proc_set.empty());
    }
    if ((left_ent.type == EntRefType::Synonym && right_ent.type == EntRefType::Underscore) ||
        (left_ent.type == EntRefType::Synonym && right_ent.type == EntRefType::Synonym)) {
        unordered_set<ProcRef> proc_set = pkb.getProcedures();
        for (auto const &proc: proc_set) {
            VarRefSet var_set = pkb.getUsesByProc(proc);
            if (!var_set.empty()) {
                return QueryResult(true);
            }
        }
    }
    return {};
}

QP::QueryResult QP::Relationship::UsesP::executeNonTrivial(PKB::StorageAccessInterface &pkb,
                                                           unordered_map<string, DesignEntity> & /*map*/) {
    if (left_ent.type == EntRefType::VarName && right_ent.type == EntRefType::Synonym) {
        VarRefSet var_set = pkb.getUsesByProc(left_ent.ent_ref);
        vector<string> column;

        for (auto const &var: var_set) {
            column.push_back(var);
        }

        QueryResult result = QueryResult();
        result.addColumn(right_ent.ent_ref, column);
        return result;
    }
    if (left_ent.type == EntRefType::Synonym && right_ent.type == EntRefType::VarName) {
        ProcRefSet proc_set = pkb.getProcUsesByVar(right_ent.ent_ref);
        vector<string> column;
        for (auto const &proc: proc_set) {
            column.push_back(proc);
        }
        QueryResult result = QueryResult();
        result.addColumn(left_ent.ent_ref, column);
        return result;
    }
    if (left_ent.type == EntRefType::Synonym && right_ent.type == EntRefType::Underscore) {
        unordered_set<ProcRef> proc_set = pkb.getProcedures();
        vector<string> column;
        for (auto const &proc: proc_set) {
            VarRefSet var_set = pkb.getUsesByProc(proc);
            if (!var_set.empty()) {
                column.push_back(proc);
            }
        }
        QueryResult result = QueryResult();
        result.addColumn(left_ent.ent_ref, column);
        return result;
    }
    if (left_ent.type == EntRefType::Synonym && right_ent.type == EntRefType::Synonym) {
        unordered_set<ProcRef> proc_set = pkb.getProcedures();
        vector<string> proc_column;
        vector<string> var_column;
        for (auto const &proc: proc_set) {
            VarRefSet var_set = pkb.getUsesByProc(proc);
            for (auto const &var: var_set) {
                proc_column.push_back(proc);
                var_column.push_back(var);
            }
        }
        QueryResult result = QueryResult();
        result.addColumn(left_ent.ent_ref, proc_column);
        result.addColumn(right_ent.ent_ref, var_column);
        return result;
    }
    if (left_ent.type == EntRefType::Underscore) {
        QueryResult result = QueryResult();
        return result;
    }
    return {};
}
