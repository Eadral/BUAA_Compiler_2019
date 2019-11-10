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

		class GlobalDefine {

		public:
			enum {
				CONST_STR,
				VAR_INT,
			} type;

			ConstStr const_str;
			VarInt var_int;

			GlobalDefine(ConstStr const_str) : type(CONST_STR), const_str(const_str) {}

			GlobalDefine(VarInt var_int) : type(VAR_INT), var_int(var_int) {}

		};
	}
}

