#pragma once
#include "vector"
#include "string"
#include "windows.h"
#include "psapi.h"
#include "ranges"

#define VFUNC_OF(x,idx) ((*reinterpret_cast<void***>(x))[idx])


namespace utils {

	struct pattern_t {
		uint8_t byte = 0x0;
		bool match = false;
	};

	uintptr_t find_pattern(std::wstring, std::string);
	uintptr_t find_pattern(std::wstring mod, const std::vector<pattern_t>& pattern);
	uintptr_t find_pattern(std::wstring mod, const std::vector<uint8_t>& pattern);
	bool is_number(const std::string& s);
}