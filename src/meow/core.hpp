//
// Created by rudyc on 2019/9/6.
//

#pragma once

#define FORMAT meow::fmt::Format::format
#define PRINT_TRACE meow::core::printStack

#pragma region MACRO

#define println(...)                \
    do {                            \
        std::cout << FORMAT(__VA_ARGS__) << std::endl;        \
    } while(0)

#define eprintf(...)            \
    do {        \
        std::cerr << FORMAT(__VA_ARGS__); \
    } while(0)

#define eprintln(...)   \
    do {    \
        eprintf(__VA_ARGS__);   \
        eprintf("\n");  \
    } while(0)  \

#define panic(...)                  \
    do {                            \
        eprintf("panic at {}:{} \n", __FILE__, __LINE__);   \
        eprintf("\t");              \
        eprintf(__VA_ARGS__);        \
        eprintf("\n");    \
        PRINT_TRACE();    \
        exit(-1);        \
    } while(0)

#ifdef _DEBUG
#define debugln(...)      \
    do {        \
        eprintf("DEBUG[ {}:{} ]\t", __FILE__, __LINE__);  \
        eprintf(__VA_ARGS__);   \
        eprintf("\n");       \
    } while(0)
#else
#define debugln(...)
#endif

#pragma endregion 

#ifndef WIN32
#include <execinfo.h>
#include <zconf.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <string>
#include <array>
#include <memory>
#include <iostream>
#include <regex>

#include "interface.hpp"
#include "fmt/display.hpp"
#include "fmt/format.hpp"
#include "result.hpp"
#include "util.hpp"
#include "enum.hpp"

namespace meow {
    class core {

    public:
        /// An ugly function for debugging
        static void printStack() {
#ifdef WIN32
			std::cout << "print_stack unimplemented in win32." << std::endl;
#else
            void *backtrace_address[1024];
            int backtrace_size;
            char **backtrace_address_buffer;

            backtrace_size = backtrace(backtrace_address, 1024);
            backtrace_address_buffer = backtrace_symbols(backtrace_address, backtrace_size);
            std::regex re("\\[(.+)\\]");
            char exec_path_buffer[PATH_MAX];
            ssize_t count = readlink("/proc/self/exe", exec_path_buffer, PATH_MAX);
            auto exec_path = std::string(exec_path_buffer, (count > 0) ? count : 0);
            std::string addrs;
            for (int i = 1; i < backtrace_size; i++) {
                std::string symbol = backtrace_address_buffer[i];
                std::smatch match_results;
                if (std::regex_search(symbol, match_results, re)) {
                    std::string m = match_results[1];
                    addrs += " " + m;
                }
            }
            std::array<char, 128> buffer{};
            std::string result;
            std::shared_ptr<FILE> pipe(popen(("addr2line -e " + exec_path + " -f -C " + addrs).c_str(), "r"), pclose);
            if (!pipe) {
                fprintf(stderr, "popen() failed!");
                exit(-1);
            }
            while (!feof(pipe.get())) {
                if (fgets(buffer.data(), 128, pipe.get()) != nullptr) {
                    result += buffer.data();
                }
            }
            std::cout << result << std::endl;
            free(backtrace_address_buffer);
#endif
        }
    };
}

using namespace meow;


