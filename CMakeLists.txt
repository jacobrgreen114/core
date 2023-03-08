
add_library(core
        src/core.cpp
        src/Timers.cpp
)

target_include_directories(core
    PUBLIC
        inc
)

target_precompile_headers(core
    PUBLIC
        <string_view>
        <string>
        <span>
        <array>
        <vector>
        <algorithm>
        <iostream>
        <fstream>
        <unordered_map>
        <map>
        <Windows.h>

)