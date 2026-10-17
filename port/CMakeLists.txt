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
#   cmake -S port -B build_port -A x64 -DMFF_ROM="path/to/Mighty Final Fight (USA).nes"
#   cmake --build build_port --config Release
#   build_port/Release/MightyFinalFight.exe --window      (with -DMFF_SDL=ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(MFF_ROM "" CACHE FILEPATH "Mighty Final Fight (USA) dump (CRC32 15EB0BEE) to extract the game's data from")
option(MFF_SDL "A window to play in (SDL2)" OFF)
set(MFF_SDL2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../nesrecomp/runner/external/SDL2" CACHE PATH
    "SDL2 development files (the one bundled with nesrecomp by default)")

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
)
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
    target_compile_options(MightyFinalFight PRIVATE -Wall -Wno-unused-label -Wno-unused-variable
                           -Wno-unused-but-set-variable)
endif()

if(MFF_SDL)
    list(APPEND CMAKE_PREFIX_PATH "${MFF_SDL2_DIR}/cmake")
    find_package(SDL2 REQUIRED)
    target_compile_definitions(MightyFinalFight PRIVATE MFF_HAVE_SDL)
    # our own main(): SDL2 without SDL2main (video_sdl.c sets SDL_MAIN_HANDLED)
    target_link_libraries(MightyFinalFight PRIVATE SDL2::SDL2)
    if(WIN32 AND EXISTS "${MFF_SDL2_DIR}/lib/x64/SDL2.dll")
        add_custom_command(TARGET MightyFinalFight POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MFF_SDL2_DIR}/lib/x64/SDL2.dll"
                    "$<TARGET_FILE_DIR:MightyFinalFight>")
    endif()
endif()

# ---- the game's data (port/tools/extract_assets.py, part "runtime") ----
# Extracted when CMake configures with MFF_ROM set; the build itself never
# reads the dump, so it can go away afterwards.
set(MFF_ASSETS_DIR "${CMAKE_BINARY_DIR}/assets")
if(MFF_ROM AND EXISTS "${MFF_ROM}")
    find_package(Python3 3.8 REQUIRED COMPONENTS Interpreter)
    execute_process(
        COMMAND ${Python3_EXECUTABLE} -I "${CMAKE_CURRENT_SOURCE_DIR}/tools/extract_assets.py"
                "${MFF_ROM}" --only runtime --out "${MFF_ASSETS_DIR}"
        RESULT_VARIABLE mff_extract_result
        OUTPUT_VARIABLE mff_extract_out
        ERROR_VARIABLE mff_extract_out)
    if(NOT mff_extract_result EQUAL 0)
        message(FATAL_ERROR "Extracting the game's data from ${MFF_ROM} failed:\n${mff_extract_out}")
    endif()
    message(STATUS "Game data extracted from ${MFF_ROM} into ${MFF_ASSETS_DIR}")
elseif(EXISTS "${MFF_ASSETS_DIR}/prg.bin")
    message(STATUS "Game data: ${MFF_ASSETS_DIR}")
elseif(MFF_ROM)
    message(FATAL_ERROR "MFF_ROM: ${MFF_ROM} does not exist")
else()
    message(STATUS "No game data in ${MFF_ASSETS_DIR} yet: configure with -DMFF_ROM=<your dump> "
                   "to extract it (the executable says the same when it starts without it)")
endif()
