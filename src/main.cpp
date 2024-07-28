#include "windows.h"
#include "string"
#include "safetyhook.hpp"
#include "vector"
#include "utils.hpp"
#include "main.hpp"
#include "sdk_externs.hpp"
#include "vpk.hpp"

namespace big {
	const std::string new_server = "ingame.reboxed.fun";
	const std::vector<std::string> to_replace = {
		"api.garrysmod.com",
		"stats.garrysmod.com",
		"toybox.garrysmod.com",
		"toyboxapi.garrysmod.com",
	};

	sdk_color pink(255, 128, 255, 255); // can't live without making everything pink

	Msg_t Msg;
	Msg_t Warning;
	ConColorMsg_t ConColorMsg;

	safetyhook::InlineHook g_gethostbyname{}, g_getaddrinfo{};

	bool strvec_has_elem(const std::vector<std::string>& vec, std::string str) {
		for (auto& s : vec) {
			if (s == str) return true;
		}
		return false;
	}

	void* __stdcall gethostbyname_hook(const char* name) {
#ifdef _DEBUG
		ConColorMsg(pink, "resolv %s\n", name);
#endif
		if (strvec_has_elem(to_replace, name)) return g_gethostbyname.stdcall<void*>(new_server.c_str());
		return g_gethostbyname.stdcall<void*>(name);
	}

	int __stdcall getaddrinfo(const char* pNodeName, const char* pServiceName, const void* pHints, void* ppResult) {
#ifdef _DEBUG
		ConColorMsg(pink, "resolv %s %s\n", pNodeName, pServiceName);
#endif

		if (strvec_has_elem(to_replace, pNodeName)) return g_getaddrinfo.stdcall<int>(new_server.c_str(), pServiceName, pHints, ppResult);
		return g_getaddrinfo.stdcall<int>(pNodeName, pServiceName, pHints, ppResult);
	};

	void create_hooks() {

		HMODULE tier0 = GetModuleHandle(L"tier0.dll");
		bool secure = true;
		if (!tier0) return;
		Msg = reinterpret_cast<Msg_t>(GetProcAddress(tier0, "Msg"));
		Warning = reinterpret_cast<Msg_t>(GetProcAddress(tier0, "Warning"));
		ConColorMsg = reinterpret_cast<ConColorMsg_t>(GetProcAddress(tier0, "?ConColorMsg@@YAXABVColor@@PBDZZ"));

		if (std::string(GetCommandLineA()).find("-insecure") != std::string::npos) secure = false;

#ifdef _DEBUG
		ConColorMsg(pink, "Toybox plugin loaded!\n");
#endif
		init_vpk();

		HMODULE wsock = GetModuleHandleA("ws2_32.dll");
		if (!wsock) {
			Warning("[patch plugin] WinSock2 isn't loaded!");
			return;
		}

		void* p_gethostbyname = GetProcAddress(wsock, "gethostbyname");
		void* p_getaddrinfo = GetProcAddress(wsock, "getaddrinfo");
		g_gethostbyname = safetyhook::create_inline(p_gethostbyname, &gethostbyname_hook);
		g_getaddrinfo = safetyhook::create_inline(p_getaddrinfo, &getaddrinfo);

		if (secure) {
			ConColorMsg(pink, "Resetting secure mode!\n");
#pragma pack(push, 1)
			struct mov_t {
				short ins;
				uint8_t* byteaddr;
				char newbyte;
			};
#pragma pack(pop)

			mov_t* mov_ins = reinterpret_cast<mov_t*>(utils::find_pattern(L"engine.dll", "C6 05 ? ? ? ? ? FF D7 83 C4"));
			if (!mov_ins) return;
			*mov_ins->byteaddr = 0x1;
		}

	}

	void destroy_hooks() {
		g_gethostbyname = {};
		g_getaddrinfo = {};
		//g_lua_shared = nullptr;
	}
}
