#ifndef SPA_SRC_PKB_TRANSITIVERELATION_TPP
#define SPA_SRC_PKB_TRANSITIVERELATION_TPP

#include "PKB/TransitiveRelation.h"

template <typename T>
PKB::TransitiveRelation<T>::TransitiveRelation(std::shared_ptr<T> self) : self(move(self)) {}

template <typename T>
std::shared_ptr<T> PKB::TransitiveRelation<T>::getSelf() const {
	return self;
}

template <typename T>
std::unordered_set<std::shared_ptr<T>> PKB::TransitiveRelation<T>::getForwardTransitive() const {
	return forward_transitive;
}

template <typename T>
std::unordered_set<std::shared_ptr<T>> PKB::TransitiveRelation<T>::getReverseTransitive() const {
	return reverse_transitive;
}

template <typename T>
void PKB::TransitiveRelation<T>::appendForwardTransitive(const std::unordered_set<std::shared_ptr<T>>& new_forward) {
	this->forward_transitive.insert(new_forward.begin(), new_forward.end());
}

template <typename T>
void PKB::TransitiveRelation<T>::appendReverseTransitive(const std::unordered_set<std::shared_ptr<T>>& new_reverse) {
	this->reverse_transitive.insert(new_reverse.begin(), new_reverse.end());
}

#endif  // SPA_SRC_PKB_TRANSITIVERELATION_TPP