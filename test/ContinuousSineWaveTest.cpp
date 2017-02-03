#include "catch.hpp"

#include "../include/ContinuousSineWave.h"
#include "../include/global.h"

TEST_CASE("Cycle size") {
    ContinuousSineWave csw(1, 1);

    SECTION("First cycle size is correct with respect to sample rate") {
        // REQUIRE(csw.cycleSize() == );
    }
}
