#ifndef SPA_SRC_COMMON_HASH_H
#define SPA_SRC_COMMON_HASH_H

#include <functional>
#include <memory>
#include <utility>
#include <vector>

#define LEFT_SHIFT 6
#define RIGHT_SHIFT 2
#define PAIR_SEED 0
// See https://stackoverflow.com/questions/4948780/magic-number-in-boosthash-combine
#define GOLDEN_RATIO 0x9e3779b9

template <class T>
static void combineHash(std::size_t& seed, const T& value) {
	seed ^= std::hash<T>{}(value) + GOLDEN_RATIO + (seed << LEFT_SHIFT) + (seed >> RIGHT_SHIFT);
}

namespace std {
template <typename T, typename S>
struct hash<pair<T, S>> {
	size_t operator()(const pair<T, S>& pair) const {
		std::size_t seed = PAIR_SEED;
		combineHash(seed, pair.first);
		combineHash(seed, pair.second);
		return seed;
	}
};

template <typename T>
struct hash<vector<T>> {
	size_t operator()(const vector<T>& vector) const {
		std::size_t seed = vector.size();
		for (const T& value : vector) {
			combineHash(seed, value);
		}
		return seed;
	}
};
}

#endif  // SPA_SRC_COMMON_HASH_H
