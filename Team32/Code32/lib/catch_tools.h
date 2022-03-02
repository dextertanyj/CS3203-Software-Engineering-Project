#ifndef CATCH_TOOLS_H
#define CATCH_TOOLS_H

#include "catch.hpp"

template <typename T, typename R>
void REQUIRE_EQUALS(T lhs, R rhs) {
	REQUIRE(lhs == (T)rhs);
};

#endif  // CATCH_TOOLS_H
