#include "catch.hpp"

#include "../include/ContinuousSineWave.h"
#include "../include/global.h"

SCENARIO ("Generating continuous wave with certain frequency and sample rate") {
    ContinuousSineWave csw(1, 100);

    SECTION ("Cycle size returned by method") {

        SECTION ("Before updating frequency") {
            REQUIRE(csw.cycleSize() == 100);
        }

        SECTION ("After updating frequency") {
            csw.updateFrequency(2);
            REQUIRE(csw.cycleSize() == 50);
        }
    }
}
