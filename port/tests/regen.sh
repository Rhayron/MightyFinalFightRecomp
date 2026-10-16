#!/bin/bash
# P4: rebuild the scaffold after game.base.toml or port/src/game changes.
# Regenerates game.toml and generated/ from your ROM, then builds Release.
set -e
cd "$(dirname "$0")/../.."
ROM="rom/Mighty Final Fight (USA).nes"
CMAKE=${CMAKE:-cmake}
command -v "$CMAKE" > /dev/null || CMAKE="/c/Program Files/CMake/bin/cmake"
py -3 -I port/tools/legacy_config.py "$ROM" --map runs/cdl/prgmap.bin --tables runs/cdl/tables.json \
    --extra runs/cdl/extra.json --base port/scaffold/game.base.toml --out port/scaffold/game.toml
GEN="$(pwd)/port/scaffold/generated"
rm -f "$GEN"/*.c "$GEN"/*.h "$GEN"/*.txt
(cd port/scaffold && ../../build_headless/nesrecomp-host/Release/NESRecomp.exe "../../$ROM" --legacy --game game.toml) \
    > runs/p4/gen.log 2>&1 || { tail -20 runs/p4/gen.log; exit 1; }
"$CMAKE" -S port/scaffold -B port/scaffold/build -A x64 > runs/p4/cmake.log 2>&1
"$CMAKE" --build port/scaffold/build --config Release > runs/p4/build.log 2>&1 || { grep -E " error " runs/p4/build.log; exit 1; }
grep -E "warning C[0-9]+" runs/p4/build.log | grep "src.game" || true
echo "scaffold rebuilt"
