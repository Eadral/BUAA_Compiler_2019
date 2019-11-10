#pragma once

#include <iostream>
#include <string>
#include <cstdio>
#include <fstream>

namespace buaac {

	class Output {
		static Output* instance;

		Output() {};
		~Output() {};
		
	public:

		static Output* getInstance() {
			if (instance == nullptr) {
				instance = new Output();
			}
			return instance;
		}
		
		std::ofstream mpis;

		void init() {
			mpis.open("mips.txt");
		}

		void cout(std::string str) {
			std::cerr << str << std::endl;
		}


	};

	Output* Output::instance = nullptr;

}

