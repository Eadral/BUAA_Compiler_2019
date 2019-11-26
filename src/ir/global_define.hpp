#pragma once

#include "../meow/core.hpp"

namespace buaac {
	namespace global {
		struct ConstStr {
			std::string label;
			std::string value;
		};

		struct VarInt {
			std::string label;
		};
		
		struct VarIntArr {
			std::string label;
			int len;
		};

		class GlobalDefine {

		public:
			enum {
				CONST_STR,
				VAR_INT,
				VAR_INT_ARR,
			} type;

			ConstStr const_str;
			VarInt var_int;
			VarIntArr var_int_arr;

			GlobalDefine(ConstStr const_str) : type(CONST_STR), const_str(const_str) {}

			GlobalDefine(VarInt var_int) : type(VAR_INT), var_int(var_int) {}
			
			GlobalDefine(VarIntArr var_int_arr) : type(VAR_INT_ARR), var_int_arr(var_int_arr) {}

		};
	}
}

