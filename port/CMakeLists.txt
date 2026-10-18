cmake_minimum_required(VERSION 3.20)
project(MightyFinalFight C)

# PLAN_DECOMP.md P5: the port on its own runtime. It builds from
#   port/src/game          the game's routines
#   port/src/glue          dispatch.c, the address -> routine table
#   port/src/runtime       the machine they run on (C11)
#   port/src/audio         the 2A03 synthesizer (P6)
# and nothing from nesrecomp. The scaffold (port/scaffold) still builds the
# same routines on NESRecomp's runtime for the P4 checks.
#
# The game's data is extracted once from your dump into BUILD/assets, which
# the executable loads at start; it never reads the ROM:
#   cmake -S port -B build_port -DMFF_ROM="path/to/Mighty Final Fight (USA).nes" -DMFF_SDL=ON
#   cmake --build build_port --config Release
#   build_port/Release/MightyFinalFight.exe      (build_port/MightyFinalFight elsewhere)
#
# P8, three systems: Windows (MSVC; SDL2 from nesrecomp's bundled copy),
# Linux and macOS (gcc or clang; SDL2 from the system: its CMake package,
# else pkg-config). The same routes give the same frame logs on all of them.

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(MFF_ROM "" CACHE FILEPATH "Mighty Final Fight (USA) dump (CRC32 15EB0BEE) to extract the game's data from")
option(MFF_SDL "A window to play in (SDL2)" OFF)
if(WIN32)
    set(mff_sdl2_default "${CMAKE_CURRENT_SOURCE_DIR}/../nesrecomp/runner/external/SDL2")
else()
    set(mff_sdl2_default "")
endif()
set(MFF_SDL2_DIR "${mff_sdl2_default}" CACHE PATH
    "SDL2 development files: on Windows the copy bundled with nesrecomp by default; empty uses the system's")

file(GLOB MFF_GAME_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/game/*.c")
file(GLOB MFF_RUNTIME_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/runtime/*.c")

add_executable(MightyFinalFight
    ${MFF_GAME_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/glue/dispatch.c
    ${MFF_RUNTIME_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/audio/synth_2a03.c
    # the in-process player of the test routes (MFF_BOT), shared with the scaffold
    ${CMAKE_CURRENT_SOURCE_DIR}/scaffold/bot.c
)
target_include_directories(MightyFinalFight PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
    ${CMAKE_CURRENT_SOURCE_DIR}/src/game
    ${CMAKE_CURRENT_SOURCE_DIR}/src/audio
    ${CMAKE_CURRENT_BINARY_DIR}/generated
)

# The executable's own extractor (runtime/extract.c) needs the PRG code list
# that prg.bin leaves out: port/tools/prg_code.txt and prg_code_read.txt,
# offsets only, compiled in as mff_prg_code.h (made again when they change).
function(mff_ranges_to_c txt var)
    file(READ "${txt}" text)
    string(REPLACE ";" "," text "${text}")
    string(REPLACE "\r" "" text "${text}")
    string(REPLACE "\n" ";" lines "${text}")
    set(out "")
    foreach(line IN LISTS lines)
        string(REGEX REPLACE "#.*" "" line "${line}")
        string(STRIP "${line}" line)
        if(line MATCHES "^([0-9A-Fa-f]+)[ \t]+([0-9A-Fa-f]+)[ \t]+([0-9A-Fa-f]+)")
            string(APPEND out "    { 0x${CMAKE_MATCH_1}, 0x${CMAKE_MATCH_2}, 0x${CMAKE_MATCH_3} },\n")
        endif()
    endforeach()
    set(${var} "${out}" PARENT_SCOPE)
endfunction()
set(mff_code_txt "${CMAKE_CURRENT_SOURCE_DIR}/tools/prg_code.txt")
set(mff_code_read_txt "${CMAKE_CURRENT_SOURCE_DIR}/tools/prg_code_read.txt")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${mff_code_txt}" "${mff_code_read_txt}")
mff_ranges_to_c("${mff_code_txt}" mff_code)
mff_ranges_to_c("${mff_code_read_txt}" mff_code_read)
file(CONFIGURE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/generated/mff_prg_code.h" CONTENT
"/* Made by port/CMakeLists.txt from port/tools/prg_code.txt and
 * prg_code_read.txt: bank (8 KiB), offset in the bank, length. */
#define MFF_RANGES_END 0xFFFFFFFFu
static const uint32_t k_prg_code[][3] = {
@mff_code@    { MFF_RANGES_END, 0, 0 }
};
static const uint32_t k_prg_code_read[][3] = {
@mff_code_read@    { MFF_RANGES_END, 0, 0 }
};
" @ONLY)
# the synthesizer matches the oracle sample for sample only with strict
# floating point: no reassociation, no fused multiply-add (synth_2a03.h)
if(MSVC)
    set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/audio/synth_2a03.c PROPERTIES
                                COMPILE_OPTIONS "/fp:precise")
else()
    set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/audio/synth_2a03.c PROPERTIES
                                COMPILE_OPTIONS "-ffp-contract=off")
    target_link_libraries(MightyFinalFight PRIVATE m)
endif()

if(MSVC)
    target_compile_options(MightyFinalFight PRIVATE /W3 /wd4102 /bigobj /MP)
    target_compile_definitions(MightyFinalFight PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_link_options(MightyFinalFight PRIVATE /STACK:16777216)
else()
    # -Wno-type-limits: the lifted code keeps each 6502 instruction's flag
    # formula, and CMP #$00 is "C = A >= 0", always true for a uint8_t.
    target_compile_options(MightyFinalFight PRIVATE -Wall -Wextra -Wno-type-limits)
endif()

if(MFF_SDL)
    if(MFF_SDL2_DIR)
        list(APPEND CMAKE_PREFIX_PATH "${MFF_SDL2_DIR}/cmake" "${MFF_SDL2_DIR}")
    endif()
    # SDL2's own CMake package (the bundled copy, distributions' libsdl2-dev,
    # Homebrew's sdl2); pkg-config where a system has only that
    find_package(SDL2 CONFIG QUIET)
    if(TARGET SDL2::SDL2)
        set(MFF_SDL2_TARGET SDL2::SDL2)
    else()
        find_package(PkgConfig QUIET)
        if(PkgConfig_FOUND)
            pkg_check_modules(MFF_SDL2 IMPORTED_TARGET sdl2)
        endif()
        if(NOT TARGET PkgConfig::MFF_SDL2)
            message(FATAL_ERROR "MFF_SDL: SDL2 not found. Install its development files (Debian/Ubuntu: "
                                "libsdl2-dev, Fedora: SDL2-devel, macOS: brew install sdl2) or point "
                                "MFF_SDL2_DIR at them.")
        endif()
        set(MFF_SDL2_TARGET PkgConfig::MFF_SDL2)
    endif()
    target_compile_definitions(MightyFinalFight PRIVATE MFF_HAVE_SDL)
    # our own main(): SDL2 without SDL2main (video_sdl.c sets SDL_MAIN_HANDLED)
    target_link_libraries(MightyFinalFight PRIVATE ${MFF_SDL2_TARGET})
    if(WIN32 AND EXISTS "${MFF_SDL2_DIR}/lib/x64/SDL2.dll")
        add_custom_command(TARGET MightyFinalFight POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MFF_SDL2_DIR}/lib/x64/SDL2.dll"
                    "$<TARGET_FILE_DIR:MightyFinalFight>")
    endif()
endif()

# ---- the game's data (part "runtime" of port/tools/extract_assets.py) ----
# With MFF_ROM set, the build runs the executable's own extractor on the
# dump (MightyFinalFight --extract, the same files as extract_assets.py
# --only runtime) into BUILD/assets, where the executable finds them; the
# game never reads the dump, so it can go away afterwards.
set(MFF_ASSETS_DIR "${CMAKE_BINARY_DIR}/assets")
if(MFF_ROM)
    if(NOT EXISTS "${MFF_ROM}")
        message(FATAL_ERROR "MFF_ROM: ${MFF_ROM} does not exist")
    endif()
    add_custom_command(
        OUTPUT "${MFF_ASSETS_DIR}/prg.bin" "${MFF_ASSETS_DIR}/chr.bin" "${MFF_ASSETS_DIR}/manifest.json"
        COMMAND MightyFinalFight --extract "${MFF_ROM}" --assets "${MFF_ASSETS_DIR}"
        DEPENDS MightyFinalFight "${MFF_ROM}"
        COMMENT "Extracting the game's data from ${MFF_ROM}"
        VERBATIM)
    add_custom_target(mff_assets ALL DEPENDS "${MFF_ASSETS_DIR}/prg.bin")
    message(STATUS "Game data: extracted from ${MFF_ROM} into ${MFF_ASSETS_DIR} when building")
elseif(EXISTS "${MFF_ASSETS_DIR}/prg.bin")
    message(STATUS "Game data: ${MFF_ASSETS_DIR}")
else()
    message(STATUS "No game data in ${MFF_ASSETS_DIR}: the executable asks for your dump when it starts "
                   "(or configure with -DMFF_ROM=<your dump>)")
endif()
