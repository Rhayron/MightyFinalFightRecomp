cmake_minimum_required(VERSION 3.20)
project(MightyFinalFight C)

# PLAN_DECOMP.md P5: the port on its own runtime. It builds from
#   port/src/game          the game's routines
#   port/src/glue          dispatch.c, the address -> routine table
#   port/src/runtime       the machine they run on (C11)
# and nothing from nesrecomp. The scaffold (port/scaffold) still builds the
# same routines on NESRecomp's runtime for the P4 checks.
#
#   cmake -S port -B build_port -A x64 && cmake --build build_port --config Release

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB MFF_GAME_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/game/*.c")
file(GLOB MFF_RUNTIME_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/runtime/*.c")

add_executable(MightyFinalFight
    ${MFF_GAME_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/glue/dispatch.c
    ${MFF_RUNTIME_SOURCES}
    # the in-process player of the test routes (MFF_BOT), shared with the scaffold
    ${CMAKE_CURRENT_SOURCE_DIR}/scaffold/bot.c
)
target_include_directories(MightyFinalFight PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
    ${CMAKE_CURRENT_SOURCE_DIR}/src/game
)

if(MSVC)
    target_compile_options(MightyFinalFight PRIVATE /W3 /wd4102 /bigobj /MP)
    target_compile_definitions(MightyFinalFight PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_link_options(MightyFinalFight PRIVATE /STACK:16777216)
else()
    target_compile_options(MightyFinalFight PRIVATE -Wall -Wno-unused-label -Wno-unused-variable
                           -Wno-unused-but-set-variable)
endif()
