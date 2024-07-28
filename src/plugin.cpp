#include "cstdint"
#include "cstring"
#include "main.hpp"
class Plugin {

public:

	virtual bool Load(uintptr_t interfaceFactory, uintptr_t gameServerFactory) {
		big::create_hooks();
		return true;
	};

	virtual void			Unload(void) {
		big::destroy_hooks();
	};
	virtual void			Pause(void) {}
	virtual void			UnPause(void) {}

	virtual const char* GetPluginDescription(void) {
		return "Toybox URL Patch";
	}

	virtual void			LevelInit(char const* pMapName) {}
	virtual void			ServerActivate(uintptr_t pEdictList, int edictCount, int clientMax) {}
	virtual void			GameFrame(bool simulating) {}
	virtual void			LevelShutdown(void) {}
	virtual void			ClientActive(uintptr_t pEntity) {}
	virtual void			ClientDisconnect(uintptr_t pEntity) {}
	virtual void			ClientPutInServer(uintptr_t pEntity, char const* playername) {}
	virtual void			SetCommandClient(int index) {}
	virtual void			ClientSettingsChanged(uintptr_t pEdict) {}
	virtual int	ClientConnect(bool* bAllowConnect, uintptr_t pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen) {
		return 0;
	}

	// grrr
	virtual int ClientCommand(void* pEntity, const void* args) {
		return 0;
	}

	virtual int	NetworkIDValidated(const char* pszUserName, const char* pszNetworkID) {
		return 0;
	}
	virtual void			OnQueryCvarValueFinished(int iCookie, uintptr_t pPlayerEntity, int eStatus, const char* pCvarName, const char* pCvarValue) {}
	virtual void			OnEdictAllocated(uintptr_t edict) {}
	virtual void			OnEdictFreed(const uintptr_t edict) {}
} g_plugin;

extern "C" __declspec(dllexport) void* CreateInterface(const char* name, int* _) {
	if (strstr(name, "ISERVERPLUGINCALLBACKS")) {
		return &g_plugin;
	}
	else {
		return 0;
	}
}