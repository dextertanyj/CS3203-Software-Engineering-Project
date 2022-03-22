#ifndef SPA_SRC_COMMON_HASH_H
#define SPA_SRC_COMMON_HASH_H

#include <memory>
#include <utility>

// See https://stackoverflow.com/questions/4948780/magic-number-in-boosthash-combine
#define GOLDEN_RATIO 0x9e3779b9

namespace Common::Hash {
// Adapted from BOOST Libraries
// https://stackoverflow.com/questions/35985960/c-why-is-boosthash-combine-the-best-way-to-combine-hash-values
template <class T>
void combineHash(std::size_t& seed, const T& value) {
	seed ^= std::hash<T>{}(value) + GOLDEN_RATIO + (seed << 6) + (seed >> 2);
}

struct PairHash {
	template <class S, class T>
	std::size_t operator()(const std::pair<S, T>& pair) const {
		std::size_t seed = 0;
		combineHash(seed, pair.first);
		combineHash(seed, pair.second);
		return seed;
	}
};

struct VectorHash {
	template <class T>
	std::size_t operator()(std::vector<T> const& vector) const {
		std::size_t seed = vector.size();
		for (const T& value : vector) {
			combineHash(seed, value);
		}
		return seed;
	}
};
}

#endif  // SPA_SRC_COMMON_HASH_H
