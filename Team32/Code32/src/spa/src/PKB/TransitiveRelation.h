#ifndef SPA_SRC_PKB_TRANSITIVERELATION_H
#define SPA_SRC_PKB_TRANSITIVERELATION_H

#include <memory>
#include <unordered_set>

#include "PKB/PKB.h"

template <typename T>
class PKB::TransitiveRelation {
public:
	explicit TransitiveRelation(std::shared_ptr<T> self);
	[[nodiscard]] std::shared_ptr<T> getSelf() const;
	[[nodiscard]] std::unordered_set<std::shared_ptr<T>> getForwardTransitive() const;
	[[nodiscard]] std::unordered_set<std::shared_ptr<T>> getReverseTransitive() const;

	virtual void appendForwardTransitive(const std::unordered_set<std::shared_ptr<T>>& new_forward);
	virtual void appendReverseTransitive(const std::unordered_set<std::shared_ptr<T>>& new_reverse);

private:
	std::shared_ptr<T> self;
	std::unordered_set<std::shared_ptr<T>> forward_transitive;
	std::unordered_set<std::shared_ptr<T>> reverse_transitive;
};

#endif  // SPA_SRC_PKB_TRANSITIVERELATION_H
