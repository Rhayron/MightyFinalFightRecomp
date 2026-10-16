#!/bin/bash
# P4: rebuild the scaffold after game.base.toml or port/src/game changes.
#   port/tests/regen.sh          # candidate: port/scaffold/build (with port/src/game)
#   port/tests/regen.sh --pure   # baseline: runs/p4/pure/build, every [[replace_func]]
#                                # after the "# P4:" line of game.base.toml left out
# Regenerates game.toml and the generated C from your ROM, then builds Release.
set -e
cd "$(dirname "$0")/../.."
ROOT="$(pwd)"
ROM="$ROOT/rom/Mighty Final Fight (USA).nes"
CMAKE=${CMAKE:-cmake}
command -v "$CMAKE" > /dev/null || CMAKE="/c/Program Files/CMake/bin/cmake"
NESRECOMP="$ROOT/build_headless/nesrecomp-host/Release/NESRecomp.exe"
mkdir -p runs/p4

if [ "$1" = --pure ]; then
  WORK="$ROOT/runs/p4/pure"
  mkdir -p "$WORK"
  sed '/^# P4:/,$d' port/scaffold/game.base.toml > "$WORK/game.base.toml"
  BASE="$WORK/game.base.toml"
  BUILD="$WORK/build"
  EXTRA=(-DMFF_PURE=ON "-DMFF_GENERATED_DIR=$WORK/generated")
else
  WORK="$ROOT/port/scaffold"
  BASE="$ROOT/port/scaffold/game.base.toml"
  BUILD="$ROOT/port/scaffold/build"
  EXTRA=(-DMFF_PURE=OFF "-DMFF_GENERATED_DIR=$WORK/generated")
fi
LOG="$ROOT/runs/p4/build$([ "$1" = --pure ] && echo _pure || true)"

py -3 -I port/tools/state_h.py > /dev/null
py -3 -I port/tools/legacy_config.py "$ROM" --map runs/cdl/prgmap.bin --tables runs/cdl/tables.json \
    --extra runs/cdl/extra.json --base "$BASE" --out "$WORK/game.toml"
mkdir -p "$WORK/generated"
rm -f "$WORK"/generated/*.c "$WORK"/generated/*.h "$WORK"/generated/*.txt
(cd "$WORK" && "$NESRECOMP" "$ROM" --legacy --game game.toml) > "$LOG.gen.log" 2>&1 \
    || { tail -20 "$LOG.gen.log"; exit 1; }
"$CMAKE" -S port/scaffold -B "$BUILD" -A x64 "${EXTRA[@]}" > "$LOG.cmake.log" 2>&1 \
    || { tail -20 "$LOG.cmake.log"; exit 1; }
"$CMAKE" --build "$BUILD" --config Release > "$LOG.build.log" 2>&1 \
    || { grep -E " error " "$LOG.build.log"; exit 1; }
grep -E "warning C[0-9]+" "$LOG.build.log" | grep -E "src.game|bot\.c" || true
echo "scaffold rebuilt: $BUILD/Release/MffScaffold.exe"
