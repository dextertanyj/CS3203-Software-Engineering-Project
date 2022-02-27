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

QP::QueryResult QP::Relationship::UsesP::executeTrivial(PKB::Storage& /*pkb*/, unordered_map<string, DesignEntity>& /*map*/) { return {}; }

QP::QueryResult QP::Relationship::UsesP::executeNonTrivial(PKB::Storage& /*pkb*/, unordered_map<string, DesignEntity>& /*map*/) {
	return {};
}
