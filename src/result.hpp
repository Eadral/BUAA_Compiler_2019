//
// Created by rudyc on 2019/9/6.
//

#ifndef BUAAC_RESULT_HPP
#define BUAAC_RESULT_HPP

#include "core/interface.hpp"
#include "fmt/display.hpp"
#include "core/core.hpp"

namespace buaac {

    template <typename Result, typename Error>
    interface Result: implement Display {
    public:
        Result unwrap() {
            if (result_type_ == ResultType::OK) {
                return result_;
            } else {
                panic("unwrap on error");
            }
        }

    private:
        enum ResultType {
            OK,
            ERROR,
        } result_type_;
        union {
            Result result_;
            Error error_;
        };
    };

}



#endif //BUAAC_RESULT_HPP
