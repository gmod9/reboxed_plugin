#include "utils.hpp"
namespace utils {

	uintptr_t _find_pattern_internal(std::wstring mod, std::vector<pattern_t> pattern) {

		MODULEINFO mi{};

		HMODULE module = GetModuleHandle(mod.c_str());
		if (!module) return 0;

		GetModuleInformation(GetCurrentProcess(), module, &mi, sizeof(MODULEINFO));

		uint8_t* cur = reinterpret_cast<uint8_t*>(mi.lpBaseOfDll);

		for (size_t i = 0; i < mi.SizeOfImage - pattern.size(); i++) {
			size_t j = 0;
			for (j = 0; j < pattern.size(); j++) {
				if ((cur[i + j] != pattern[j].byte) && pattern[j].match) break;
			}

			if (j == pattern.size()) return reinterpret_cast<uintptr_t>(cur + i);
		}
		return 0;
	}

	std::vector<pattern_t> vectorize_pattern(std::string pattern) {
		auto split_strings = std::string_view(pattern) | std::ranges::views::split(' ');
		std::vector<pattern_t> ret;
		for (const auto& token : split_strings) {
			std::string_view tokstr({ token.begin(), token.end() });
			if (tokstr == "?") {
				ret.push_back({ 0x0, false });
			}
			else {
				std::string _t(tokstr);
				ret.push_back({ static_cast<uint8_t>(std::stoul(_t, nullptr, 16)), true
					});
			}
		}

		return ret;
	}

	uintptr_t find_pattern(std::wstring mod, std::string pattern) {
		return _find_pattern_internal(mod, vectorize_pattern(pattern));
	}

	uintptr_t find_pattern(std::wstring mod, const std::vector<pattern_t>& pattern) {
		return _find_pattern_internal(mod, pattern);
	}

	uintptr_t find_pattern(std::wstring mod, const std::vector<uint8_t>& pattern) {
		std::vector<pattern_t> _pattern;

		for (auto& b : pattern) {
			_pattern.push_back({ b, true });
		}

		return _find_pattern_internal(mod, _pattern);
	}


	bool is_number(const std::string& s)
	{
    	return !s.empty() && std::find_if(s.begin(), 
    	    s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
	}	
}