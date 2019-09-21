//
// Created by rudyc on 2019/9/5.
//

#ifndef BUAAC_DISPLAY_HPP
#define BUAAC_DISPLAY_HPP

#include "format.hpp"
#include "../interface.hpp"
#include <string>
#include <iostream>

namespace meow {
    namespace fmt {
        interface Display {
        public:
			virtual std::string toString() = 0;
        };
    	
        std::ostream& operator<<(std::ostream& out, Display &display) {
            out << display.toString();
            return out;
        }
    	
    }

}




#endif //BUAAC_DISPLAY_HPP
