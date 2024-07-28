cmake -A Win32 -S . -B "out" -DSAFETYHOOK_FETCH_ZYDIS=ON
cmake --build out --config Release