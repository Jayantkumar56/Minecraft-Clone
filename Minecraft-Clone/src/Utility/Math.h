

#pragma once

#include <Utility>

namespace Math {

	inline int32_t FloorDiv(int32_t a, int32_t b) {
		int32_t q = a / b;
		int32_t r = a % b;
		return q - ((r != 0) & (a < 0));
	}

}
