//
// Created by rudyc on 2019/9/5.
//

#ifndef BUAAC_DISPLAY_HPP
#define BUAAC_DISPLAY_HPP

#include "format.hpp"
#include "../core/interface.hpp"
#include <string>
#include <iostream>

namespace buaac {
    namespace fmt {
        interface Display {
        public:
            virtual std::string toString() {
                panic("Display unimplemented.");
            }
        };
        std::ostream& operator<<(std::ostream& out, Display &display) {
            out << display.toString();
            return out;
        }
    }

}




#endif //BUAAC_DISPLAY_HPP
