#!/bin/bash
# P8: the executable's own extractor (MightyFinalFight --extract, runtime/
# extract.c) against the reference, port/tools/extract_assets.py --only
# runtime: chr.bin, prg.bin and manifest.json must be byte for byte the same,
# into an empty directory and into one a full Python extraction filled (the
# manifest keeps that run's files list).
#   MFF_EXE=build/MightyFinalFight port/tests/extract_check.sh [DUMP] [WORKDIR]
# Everything written is from the dump: WORKDIR (default runs/extract_check)
# is never committed.
set -e
cd "$(dirname "$0")/../.."
DUMP=${1:-rom/Mighty Final Fight (USA).nes}
WORK=${2:-runs/extract_check}
EXE=${MFF_EXE:?set MFF_EXE to the MightyFinalFight executable}
if [ -z "$PY" ]; then
  if command -v py >/dev/null 2>&1; then PY="py -3"; else PY=python3; fi
fi
rm -rf "$WORK"
mkdir -p "$WORK"
status=0
same() {    # same DIR_A DIR_B what
  for f in chr.bin prg.bin manifest.json; do
    if cmp -s "$1/$f" "$2/$f"; then echo "$3: $f identical"; else echo "$3: $f DIFFERS"; status=1; fi
  done
}
# 1. an empty directory
$PY -I port/tools/extract_assets.py "$DUMP" --only runtime --out "$WORK/py" > /dev/null
"$EXE" --extract "$DUMP" --assets "$WORK/c" > /dev/null
same "$WORK/py" "$WORK/c" "empty directory"
# 2. on top of a full extraction (the manifest lists the other parts too)
$PY -I port/tools/extract_assets.py "$DUMP" --only chr --only palettes --out "$WORK/py_full" > /dev/null
cp -r "$WORK/py_full" "$WORK/c_full"
$PY -I port/tools/extract_assets.py "$DUMP" --only runtime --out "$WORK/py_full" > /dev/null
"$EXE" --extract "$DUMP" --assets "$WORK/c_full" > /dev/null
same "$WORK/py_full" "$WORK/c_full" "after other parts"
# 3. a file that is not the dump is refused
head -c 40976 "$DUMP" > "$WORK/short.nes"
if "$EXE" --extract "$WORK/short.nes" --assets "$WORK/bad" 2> "$WORK/bad.err"; then
  echo "a cut dump was accepted"; status=1
else
  echo "a cut dump is refused: $(cat "$WORK/bad.err")"
fi
exit $status
