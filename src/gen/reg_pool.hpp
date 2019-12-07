#ifndef REG_POOL_HPP
#define REG_POOL_HPP

#include "../meow/core.hpp"

namespace buaac {
	class RegPool {

		vector<string> globalReg = {
			"$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9",
			"$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
			"$v1"
		};

	};
}


#endif
