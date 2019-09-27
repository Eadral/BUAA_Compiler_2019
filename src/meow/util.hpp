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

	template <typename charT>
	inline bool starts_with(const std::basic_string<charT>& big, const std::basic_string<charT>& small) {
		if (&big == &small) return true;
		const typename basic_string<charT>::size_type big_size = big.size();
		const typename basic_string<charT>::size_type small_size = small.size();
		const bool valid_ = (big_size >= small_size);
		const bool starts_with_ = (big.compare(0, small_size, small) == 0);
		return valid_ && starts_with_;
	}

	template <typename charT>
	inline bool ends_with(const std::basic_string<charT>& big, const std::basic_string<charT>& small) {
		if (&big == &small) return true;
		const typename basic_string<charT>::size_type big_size = big.size();
		const typename basic_string<charT>::size_type small_size = small.size();
		const bool valid_ = (big_size >= small_size);
		const bool ends_with_ = (big.compare(big_size - small_size, small_size, small) == 0);
		return valid_ && ends_with_;
	}

}


#endif
