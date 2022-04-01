#ifndef SPA_SRC_PKB_TRANSITIVERELATION_H
#define SPA_SRC_PKB_TRANSITIVERELATION_H

#include <memory>
#include <unordered_set>

#include "PKB/PKB.h"

template <typename T>
class PKB::TransitiveRelation {
public:
	explicit TransitiveRelation(shared_ptr<T> self);
	[[nodiscard]] shared_ptr<T> getSelf() const;
	[[nodiscard]] unordered_set<shared_ptr<T>> getForwardTransitive() const;
	[[nodiscard]] unordered_set<shared_ptr<T>> getReverseTransitive() const;

	virtual void appendForwardTransitive(const unordered_set<shared_ptr<T>>& new_forward);
	virtual void appendReverseTransitive(const unordered_set<shared_ptr<T>>& new_reverse);

private:
	shared_ptr<T> self;
	unordered_set<shared_ptr<T>> forward_transitive;
	unordered_set<shared_ptr<T>> reverse_transitive;
};

#endif  // SPA_SRC_PKB_TRANSITIVERELATION_H
