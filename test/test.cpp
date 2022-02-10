#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "libtorus.hpp"

Torus<int> IntTorus(unsigned int x, unsigned int y) {
    return Torus<int>(x, y);
}

TEST_CASE( "Torus need make sense", "[torus]") {
    REQUIRE( IntTorus(1,1).get_x() == 0);
}

