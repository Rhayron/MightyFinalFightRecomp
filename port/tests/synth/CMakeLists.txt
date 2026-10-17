# synth_2a03 against the cycle-accurate oracle (docs/sound.md, "The
# synthesizer"). Builds:
#   synth_2a03      the library (port/src/audio), with strict warnings
#   synth_replay    plays an oracle capture through it, compares the WAVs
#   synth_selftest  checks without the oracle (DMC, 5-step, IRQs, ...)
# and, with MSVC and MFF_ORACLE_ROOT pointing at a checkout whose oracle builds
# are done (build_headless, build_cyc), the capture tools: copies of those
# programs, relinked from their own object files plus capture/oracle_capture.c.
#   mff_apu_capture     MightyFinalFightRecomp.exe (build_headless)
#   interp_apu_capture  cyc_interp.exe (build_cyc: any ROM, on the interpreter)
#
#   cmake -S port/tests/synth -B build_synth -A x64 -DMFF_ORACLE_ROOT=C:/path/to/mff
#   cmake --build build_synth --config Release
cmake_minimum_required(VERSION 3.20)
project(mff_synth_tests C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(SYNTH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../src/audio)
add_library(synth_2a03 STATIC ${SYNTH_DIR}/synth_2a03.c)
target_include_directories(synth_2a03 PUBLIC ${SYNTH_DIR})
if(MSVC)
  target_compile_options(synth_2a03 PRIVATE /W4 /WX /fp:precise)
else()
  # No contraction: a fused multiply-add would round differently from the oracle.
  target_compile_options(synth_2a03 PRIVATE -Wall -Wextra -Wpedantic -Werror -ffp-contract=off)
endif()

foreach(tool synth_replay synth_selftest)
  add_executable(${tool} ${tool}.c)
  target_link_libraries(${tool} PRIVATE synth_2a03)
  target_compile_definitions(${tool} PRIVATE _CRT_SECURE_NO_WARNINGS)   # plain fopen/sscanf
  if(MSVC)
    target_compile_options(${tool} PRIVATE /W4)
  else()
    target_compile_options(${tool} PRIVATE -Wall -Wextra -Wpedantic)
    target_link_libraries(${tool} PRIVATE m)
  endif()
endforeach()

set(MFF_ORACLE_ROOT "" CACHE PATH "checkout with the oracle builds (build_headless, build_cyc) and nesrecomp")
if(MSVC AND MFF_ORACLE_ROOT)
  set(capture_targets
      "mff_apu_capture|build_headless/MightyFinalFightRecomp.dir/Release"
      "interp_apu_capture|build_cyc/cyc_interp.dir/Release")
  foreach(entry IN LISTS capture_targets)
    string(REPLACE "|" ";" parts "${entry}")
    list(GET parts 0 name)
    list(GET parts 1 dir)
    file(GLOB objs "${MFF_ORACLE_ROOT}/${dir}/*.obj")
    if(objs)
      add_executable(${name} capture/oracle_capture.c ${objs})
      target_include_directories(${name} PRIVATE ${MFF_ORACLE_ROOT}/nesrecomp/runner/cyc)
      message(STATUS "${name}: ${MFF_ORACLE_ROOT}/${dir}")
    else()
      message(STATUS "${name}: no objects in ${MFF_ORACLE_ROOT}/${dir}, skipped")
    endif()
  endforeach()
endif()
