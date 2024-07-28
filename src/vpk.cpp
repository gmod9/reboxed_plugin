#include "sdk_externs.hpp"
#include "windows.h"
#include "utils.hpp"
#include "vpk.hpp"
#include "filesystem"

namespace big {
    typedef void(__cdecl* pfn_mountvpk)(const char* filename);
    pfn_mountvpk g_mountvpk;


    void init_vpk() {

        auto is_part = [](std::string& path){
            if (path.length() <= 7) return false;
            return utils::is_number(path.substr(path.length() - 7, 3));  
        };

        g_mountvpk = reinterpret_cast<pfn_mountvpk>(utils::find_pattern(L"steam.dll", "55 8B EC 64 A1 ? ? ? ? 6A ? 68 ? ? ? ? 50 64 89 25 ? ? ? ? 81 EC ? ? ? ? 56 33 F6"));
        if (!g_mountvpk) {
            Warning("Couldn't find mountvpk, not mounting vpk files!\n");
            return;
        }

        if (!std::filesystem::exists("vpk_files") || !std::filesystem::is_directory("vpk_files")) return;

        for (const auto& elem : std::filesystem::recursive_directory_iterator("vpk_files")){
            std::string cur_path = elem.path().string();

            if (!cur_path.ends_with(".vpk") || is_part(cur_path)) continue;
            cur_path = cur_path.ends_with("_dir.vpk") ? cur_path.substr(0, cur_path.length() - 8) : cur_path; // makes parted archives work somehow lol

            ConColorMsg(pink, "Mounting %s\n", cur_path.c_str());

            g_mountvpk(cur_path.c_str());
        }
    }
}