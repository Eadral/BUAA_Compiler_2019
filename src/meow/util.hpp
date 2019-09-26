#ifndef MEOW_UTIL_HPP
#define MEOW_UTIL_HPP


#include <string>
#include <fstream>

namespace meow {

	std::string readFileToString(const std::string& filename) {
		std::ifstream file(filename);

		std::ostringstream buffer;
		char c;
		while (file.get(c))
			buffer.put(c);

		return buffer.str();
	}


}


#endif