//
// Created by rudyc on 2019/9/5.
//

#ifndef MEOW_DISPLAY_HPP
#define MEOW_DISPLAY_HPP

#include "format.hpp"
#include "../interface.hpp"
#include <string>
#include <iostream>

namespace meow {
    namespace fmt {
        interface Display {
        public:
	        virtual ~Display() = default;
	        virtual std::string toString() = 0;
        };
    	
        std::ostream& operator<<(std::ostream& out, Display &display) {
            out << display.toString();
            return out;
        }
    	
    }

}

#endif