#!/bin/bash
# P8: what can be checked without the ROM (CI, port/ci/github-actions.yml):
# the executable starts, explains itself, asks for the dump when the game's
# data is missing, and refuses files that are not the dump.
#   port/tests/smoke_no_rom.sh path/to/MightyFinalFight[.exe]
# Runs in a scratch directory with HOME/APPDATA pointed there, so no data
# of this machine is found; nothing is written elsewhere.
set -e
exe=${1:?usage: smoke_no_rom.sh path/to/MightyFinalFight}
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT
mkdir -p "$work/bin" "$work/home" "$work/other"
cp "$exe" "$work/bin/"
for lib in "$(dirname "$exe")"/SDL2.dll; do [ -e "$lib" ] && cp "$lib" "$work/bin/"; done
bin="$work/bin/$(basename "$exe")"
export HOME="$work/home" APPDATA="$work/home" XDG_DATA_HOME="$work/home"
export SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy
cd "$work/other"
fail() { echo "FAILED: $*"; exit 1; }

"$bin" --help | grep -q "usage: MightyFinalFight" || fail "--help"
echo "ok: --help"

set +e
"$bin" --headless --smoke 1 > out.txt 2>&1; code=$?
set -e
[ $code -eq 1 ] && grep -q "data is missing" out.txt || { cat out.txt; fail "a start without data (exit $code)"; }
echo "ok: without the game's data it asks for the dump"

head -c 393232 /dev/urandom > junk.nes
if "$bin" --extract junk.nes --assets out > out.txt 2>&1; then fail "a random file was taken for the dump"; fi
[ ! -e out/prg.bin ] || fail "a random file left data behind"
grep -q "no iNES header\|CRC32" out.txt || { cat out.txt; fail "the refusal says why"; }
echo "ok: a file that is not a .nes dump is refused: $(cat out.txt)"

printf 'NES\032\010\020\020\000\000\000\000\000\000\000\000\000' > other.nes
head -c 262144 /dev/zero >> other.nes
if "$bin" --extract other.nes --assets out > out.txt 2>&1; then fail "another game was taken for the dump"; fi
grep -q "expected mapper 4\|CRC32" out.txt || { cat out.txt; fail "the refusal says why"; }
echo "ok: another iNES file is refused: $(cat out.txt)"

cp junk.nes "$work/bin/next_to_it.nes"
set +e
"$bin" --headless --smoke 1 > out.txt 2>&1; code=$?
set -e
[ $code -eq 1 ] && [ ! -e "$work/bin/assets/prg.bin" ] || { cat out.txt; fail "a wrong .nes next to the executable was used"; }
echo "ok: a wrong .nes next to the executable is not used"

echo "smoke test passed"
