#ifndef MEOW_UTIL_HPP
#define MEOW_UTIL_HPP


#include <string>
#include <fstream>
#include <cstdio>

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
		const typename std::basic_string<charT>::size_type big_size = big.size();
		const typename std::basic_string<charT>::size_type small_size = small.size();
		const bool valid_ = (big_size >= small_size);
		const bool starts_with_ = (big.compare(0, small_size, small) == 0);
		return valid_ && starts_with_;
	}

	template <typename charT>
	inline bool ends_with(const std::basic_string<charT>& big, const std::basic_string<charT>& small) {
		if (&big == &small) return true;
		const typename std::basic_string<charT>::size_type big_size = big.size();
		const typename std::basic_string<charT>::size_type small_size = small.size();
		const bool valid_ = (big_size >= small_size);
		const bool ends_with_ = (big.compare(big_size - small_size, small_size, small) == 0);
		return valid_ && ends_with_;
	}

}


std::string i2a(int i) {
	return std::to_string(i);;
}

int a2i(std::string str) {
	return atoi(str.c_str());
}

int a2i(char str[]) {
	return atoi(str);
}

bool isNumber(std::string source) {
	return isdigit(source[0]) || source[0] == '-';
}

template<typename C, typename I>
bool notFound(C &container, I item) {
	return container.find(item) == container.end();
}

template<typename C, typename I>
bool found(C &container, I item) {
	return !notFound(container, item);
}

template<typename T>
set<T> setOr(const set<T> &a, const set<T> &b) {
	set<T> ans;
	for (auto i = a.begin(); i != a.end(); i++) {
		ans.insert(*i);
	}
	for (auto i = b.begin(); i != b.end(); i++) {
		ans.insert(*i);
	}
	return ans;
}

template<typename T>
set<T> setAnd(const set<T> &a, const set<T> &b) {
	set<T> ans;
	for (auto i = a.begin(); i != a.end(); i++) {
		if (found(b, *i)) {
			ans.insert(*i);
		}
	}
	for (auto i = b.begin(); i != b.end(); i++) {
		if (found(a, *i)) {
			ans.insert(*i);
		}
	}
	return ans;
}

template<typename T>
set<T> setSub(const set<T> &a, const set<T> &b) {
	set<T> ans;
	for (auto i = a.begin(); i != a.end(); i++) {
		if (notFound(b, *i)) {
			ans.insert(*i);
		}
	}
	return ans;
}

#endif
