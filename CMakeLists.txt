# Copyright (C) 2022-2023 Jacob R. Green
# All Rights Reserved.


add_library(core
        src/core.cpp
        src/timers.cpp
)

target_include_directories(core
    PUBLIC
        inc
)

target_precompile_headers(core
        PUBLIC
        <typeinfo>

        <span>

        <string>
        <string_view>

        <array>
        <vector>

        <set>
        <unordered_set>

        <map>
        <unordered_map>

        <algorithm>

        <chrono>
        <filesystem>
        <iostream>
        <fstream>

        <future>
        <atomic>
        <thread>
        <mutex>
        <latch>
        <barrier>
        <semaphore>
)



if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_precompile_headers(core
        PUBLIC
            <Windows.h>
    )
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    message(FATAL_ERROR "MacOS is not currently supported")
else()
    message(FATAL_ERROR "Unsupported system ${CMAKE_SYSTEM_NAME}")
endif()




