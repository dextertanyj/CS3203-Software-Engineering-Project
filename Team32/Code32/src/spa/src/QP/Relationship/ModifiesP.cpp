#include "QP/Relationship/ModifiesP.h"

#include <utility>

QP::Relationship::ModifiesP::ModifiesP(QueryEntRef left_ent, QueryEntRef right_ent)
	: left_ent(std::move(std::move(left_ent))), right_ent(std::move(std::move(right_ent))) {}

QueryEntRef QP::Relationship::ModifiesP::getLeftEnt() { return left_ent; }

QueryEntRef QP::Relationship::ModifiesP::getRightEnt() { return right_ent; }

QP::QueryResult QP::Relationship::ModifiesP::execute(PKB::StorageAccessInterface& pkb, bool is_trivial,
                                                     unordered_map<string, DesignEntity>& map) {
	return is_trivial ? executeTrivial(pkb, map) : executeNonTrivial(pkb, map);
}

vector<string> QP::Relationship::ModifiesP::getDeclarationSymbols() {
	vector<string> declaration_symbols;
	if (this->left_ent.type == EntRefType::Synonym) {
		declaration_symbols.push_back(this->left_ent.ent_ref);
	}
	if (this->right_ent.type == EntRefType::Synonym) {
		declaration_symbols.push_back(this->right_ent.ent_ref);
	}
	return declaration_symbols;
}

QP::QueryResult QP::Relationship::ModifiesP::executeTrivial(PKB::StorageAccessInterface& /*pkb*/,
                                                            unordered_map<string, DesignEntity>& /*map*/) {
	return {};
}

QP::QueryResult QP::Relationship::ModifiesP::executeNonTrivial(PKB::StorageAccessInterface& /*pkb*/,
                                                               unordered_map<string, DesignEntity>& /*map*/) {
	return {};
}
