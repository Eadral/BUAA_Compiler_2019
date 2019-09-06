//
// Created by rudyc on 2019/9/6.
//

#ifndef BUAAC_TESTCLASSA_HPP
#define BUAAC_TESTCLASSA_HPP

#include "core/core.hpp"
#include "fmt/display.hpp"
#include "fmt/format.hpp"

namespace buaac {
    class TestClassA: implement fmt::Display {
    public:
        explicit TestClassA(int val): val(val) {}

        std::string toString() override {
            return fmt::Format::format("2 {}.", val);
        }

    private:
        int val;

    };
}



#endif //BUAAC_TESTCLASSA_HPP
