//
// Created by rudyc on 2019/9/6.
//

#ifndef BUAAC_FORMAT_HPP
#define BUAAC_FORMAT_HPP

#include "../core.hpp"
#include <string>
#include <vector>
#include <sstream>

namespace meow {
    namespace fmt {
        class Format {
        public:
            static std::string format(const char *fmt) {
                return toString(fmt);
            }

            template<typename ...Args>
            static std::string format(const char *fmt, Args... args) {
                auto string_buffer = std::vector<std::string>();
                auto strings = expandToString(string_buffer, args...);
				std::stringstream out;
                int i = 0;
                while (*fmt != '\0') {
                    if (*fmt == '{') {
                        fmt++;
						std::stringstream cmd_buffer;
                        while (*fmt != '\0' && *fmt != '}') {
                            cmd_buffer << *fmt;
                            fmt++;
                        }
                        if (*fmt == '\0') {
                            panic("wrong format");
                        } else {
                            if (i >= strings.size()) {
                                panic("unexpected argument.");
                            }
                            // TODO(zyc): add format control support.
                            out << strings[i++];
                        }
                        fmt++;
                        continue;
                    }
                    out << *fmt;
                    fmt++;
                }
                if (i != strings.size()) {
                    panic("redundant argument.");
                }
                return out.str();
            }
        
        template<typename T>
        static std::string toString(T t) {
			std::stringstream out;
            out << t;
            return out.str();
        }

		private:
            template<typename T>
            static std::vector<std::string> &expandToString(std::vector<std::string> &out, T t) {
                out.emplace_back(toString(t));
                return out;
            }

            template<typename T, typename ...Types>
            static std::vector<std::string> &expandToString(std::vector<std::string> &out, T t, Types... types) {
                out.emplace_back(toString(t));
                expandToString(out, types...);
                return out;
            }
        };


    }

}


#endif //BUAAC_FORMAT_HPP
