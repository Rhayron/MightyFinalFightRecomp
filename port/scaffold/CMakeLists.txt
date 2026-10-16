cmake_minimum_required(VERSION 3.20)
project(MffScaffold C CXX)

# PLAN_DECOMP.md P3: Mighty Final Fight on NESRecomp's legacy (function-level)
# backend, the scaffold that gets ejected into port/src/game/ in P4. The code
# in generated/ comes from your dump (NESRecomp --legacy --game game.toml) and
# is never committed. Build: cmake -S port/scaffold -B port/scaffold/build -A x64

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(NESRECOMP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../nesrecomp" CACHE PATH "NESRecomp engine checkout")
include(${NESRECOMP_ROOT}/runner/runner.cmake)

list(APPEND CMAKE_PREFIX_PATH "${NESRECOMP_ROOT}/runner/external/SDL2/cmake")
find_package(SDL2 REQUIRED)

# generated/ by default; the P4 baseline build (port/tests/regen.sh --pure)
# points this at its own copy, generated without the hand-written routines
set(MFF_GENERATED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/generated" CACHE PATH "NESRecomp --legacy output for this build")
option(MFF_PURE "Build without port/src/game (the unrefactored scaffold)" OFF)
# the port alone: port/src/game plus port/src/glue (port/tools/dispatch_gen.py),
# no generated code at all
option(MFF_EJECTED "Build without generated/ (every routine in port/src/game)" OFF)
if(MFF_EJECTED)
    set(MFF_GENERATED "${CMAKE_CURRENT_SOURCE_DIR}/../src/glue/dispatch.c")
else()
    file(GLOB MFF_GENERATED CONFIGURE_DEPENDS
        "${MFF_GENERATED_DIR}/mff_full*.c"
        "${MFF_GENERATED_DIR}/mff_dispatch.c")
endif()
# P4: the hand-written routines that replace generated ones ([[replace_func]])
set(MFF_GAME "")
if(NOT MFF_PURE)
    file(GLOB MFF_GAME CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../src/game/*.c")
endif()
if(NOT MFF_GENERATED)
    message(FATAL_ERROR "Run NESRecomp --legacy --game game.toml in port/scaffold first (generated/ is empty)")
endif()

add_executable(MffScaffold
    ${NESRECOMP_RUNNER_SOURCES}
    extras.c
    bot.c
    nes_snapshot.c
    verify_mode.c
    ${MFF_GAME}
    ${MFF_GENERATED}
)

target_include_directories(MffScaffold PRIVATE
    ${NESRECOMP_RUNNER_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${MFF_GENERATED_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/game
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(MffScaffold PRIVATE ${SDL2_LIBRARIES})
# parts compile as their own TUs (in parallel) instead of being #included by mff_full.c
target_compile_definitions(MffScaffold PRIVATE RECOMP_STACK_TRACKING NES_TRACE_STACK_WRITES NESRECOMP_SPLIT_PARTS_EXTERNAL)

if(WIN32)
    target_link_libraries(MffScaffold PRIVATE ws2_32)
    add_custom_command(TARGET MffScaffold POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${NESRECOMP_ROOT}/runner/external/SDL2/lib/x64/SDL2.dll"
            "$<TARGET_FILE_DIR:MffScaffold>")
endif()

if(MSVC)
    target_compile_options(MffScaffold PRIVATE /W3 /WX- /wd4102 /bigobj /MP)
endif()
