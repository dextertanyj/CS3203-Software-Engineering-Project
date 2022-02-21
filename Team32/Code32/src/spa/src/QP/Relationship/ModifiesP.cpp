#include "QP/Relationship/ModifiesP.h"

#include <utility>

ModifiesP::ModifiesP(QueryEntRef left_ent, QueryEntRef right_ent)
	: left_ent(std::move(std::move(left_ent))), right_ent(std::move(std::move(right_ent))) {}

QueryEntRef ModifiesP::getLeftEnt() { return left_ent; }

QueryEntRef ModifiesP::getRightEnt() { return right_ent; }

QueryResult ModifiesP::execute(PKB::Storage& pkb, bool is_trivial, unordered_map<string, DesignEntity>& map) {
	return is_trivial ? executeTrivial(pkb, map) : executeNonTrivial(pkb, map);
}

vector<string> ModifiesP::getDeclarationSymbols() {
	vector<string> declaration_symbols;
	if (this->left_ent.type == EntRefType::Synonym) {
		declaration_symbols.push_back(this->left_ent.ent_ref);
	}
	if (this->right_ent.type == EntRefType::Synonym) {
		declaration_symbols.push_back(this->right_ent.ent_ref);
	}
	return declaration_symbols;
}

QueryResult ModifiesP::executeTrivial(PKB::Storage& /*pkb*/, unordered_map<string, DesignEntity>& /*map*/) { return {}; }

QueryResult ModifiesP::executeNonTrivial(PKB::Storage& /*pkb*/, unordered_map<string, DesignEntity>& /*map*/) { return {}; }
