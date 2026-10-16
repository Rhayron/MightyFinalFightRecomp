cmake_minimum_required(VERSION 3.20)
project(MightyFinalFightRecomp C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(NESRECOMP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/nesrecomp" CACHE PATH "NESRecomp engine checkout")

# nesrecomp/ and recomp-ui/ are git submodules (see .gitmodules); the pinned
# commit is the gitlink recorded in this repo. After cloning run setup.sh/.bat
# (git submodule update --init) to fetch them at the recorded commits.

# Cycle backend only: there is no legacy (function-level) port of this game.
set(NESRECOMP_BACKEND "cycle" CACHE STRING "CPU backend (only cycle is supported here)")
if(NOT NESRECOMP_BACKEND STREQUAL "cycle")
    message(FATAL_ERROR "MightyFinalFightRecomp only has the cycle backend")
endif()

option(NESRECOMP_HEADLESS "Build without the SDL window, audio and launcher" OFF)
set(NESRECOMP_ROM "" CACHE FILEPATH "ROM image to recompile (not distributed)")
set(NESRECOMP_CYCLE_SEEDS "${CMAKE_CURRENT_SOURCE_DIR}/cyc_seeds.txt" CACHE FILEPATH "Cycle coverage seeds")
set(NESRECOMP_HOST_COMPILER "" CACHE FILEPATH "Optional prebuilt host NESRecomp compiler")
if(NOT EXISTS "${NESRECOMP_ROM}")
    message(FATAL_ERROR "Set NESRECOMP_ROM to the original ROM image for this title")
endif()

set(_mff_headless)
if(NESRECOMP_HEADLESS)
    set(_mff_headless HEADLESS)
endif()

include("${NESRECOMP_ROOT}/runner/cyc/project.cmake")
nesrecomp_add_cycle_game(MightyFinalFightRecomp
    ROM "${NESRECOMP_ROM}"
    GAME_CONFIG "${CMAKE_CURRENT_SOURCE_DIR}/game.toml"
    RECOMPILER "${NESRECOMP_HOST_COMPILER}"
    SEED_FILE "${NESRECOMP_CYCLE_SEEDS}"
    PLAYERS 2
    ${_mff_headless})
