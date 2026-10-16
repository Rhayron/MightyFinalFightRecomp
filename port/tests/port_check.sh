#!/bin/bash
# Build the port alone (no generated code) and check it on every route.
#   port/tests/port_check.sh BUILD_DIR [ROUTE...]
# Regenerates port/src/glue/dispatch.c and port/src/game/state.h, configures
# and builds BUILD_DIR with -DMFF_EJECTED=ON, then runs run_routes.sh check
# with that executable. In a git worktree, point at the main checkout:
#   MFF_NESRECOMP=C:/.../mff/nesrecomp   the engine (default ./nesrecomp)
#   MFF_ROM, MFF_BASE, MFF_OUT           as in run_routes.sh
set -e
cd "$(dirname "$0")/../.."
BUILD=${1:?usage: port_check.sh BUILD_DIR [ROUTE...]}
shift
CMAKE=${CMAKE:-cmake}
command -v "$CMAKE" > /dev/null || CMAKE="/c/Program Files/CMake/bin/cmake"
NR=${MFF_NESRECOMP:-$(pwd -W)/nesrecomp}
mkdir -p "$BUILD"
py -3 -I port/tools/state_h.py > /dev/null
py -3 -I port/tools/dispatch_gen.py > /dev/null
"$CMAKE" -S port/scaffold -B "$BUILD" -A x64 -DMFF_EJECTED=ON "-DNESRECOMP_ROOT=$NR" > "$BUILD/cmake.log" 2>&1 \
  || { tail -20 "$BUILD/cmake.log"; exit 1; }
"$CMAKE" --build "$BUILD" --config Release > "$BUILD/build.log" 2>&1 \
  || { grep -E " error " "$BUILD/build.log" | head -20; exit 1; }
grep -E "warning C[0-9]+" "$BUILD/build.log" | grep "src.game" | head -10 || true
MFF_EXE="$BUILD/Release/MffScaffold.exe" bash port/tests/run_routes.sh check "$@"
