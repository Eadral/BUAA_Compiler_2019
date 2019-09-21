//
// Created by rudyc on 2019/9/6.
//

#define CATCH_CONFIG_MAIN
#include "../3rdparty/catch.hpp"

#include "../src/core/fmt/format.hpp"

TEST_CASE("simple format", "[format]") {
    REQUIRE( format("this is {} and {}.", 2, "x") == "this is 2 and x." );
}
