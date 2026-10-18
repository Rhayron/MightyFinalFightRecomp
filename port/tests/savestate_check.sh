#!/bin/bash
# P8: save states. For each route, play it once uninterrupted, saving states
# on the way (MFF_SAVE_AT: at 10, 30, 50, 70 and 90% of it, each at the
# first frame from there where a state can be taken, savestate.c); then
# start a new process from each state (--load) with the same route and
# compare the rest of its frame log and picture log with the uninterrupted
# run's, byte for byte (savestate_cmp.py).
#   MFF_EXE=build/MightyFinalFight port/tests/savestate_check.sh [ROUTE...]
# Environment: MFF_EXE (required), MFF_OUT (default runs/savestate),
# MFF_SAVE_AT (frames, comma-separated, instead of the five above), PY.
# The logs and states are from the dump: never commit MFF_OUT.
set -e
cd "$(dirname "$0")/../.."
hostpwd() { pwd -W 2>/dev/null || pwd; }
if [ -z "$PY" ]; then
  if command -v py >/dev/null 2>&1; then PY="py -3"; else PY=python3; fi
fi
EXE=${MFF_EXE:?set MFF_EXE to the MightyFinalFight executable}
OUT=${MFF_OUT:-runs/savestate}
mkdir -p "$OUT"
OUTW=$(cd "$OUT" && hostpwd)
if [ $# -gt 0 ]; then
  routes=()
  for n in "$@"; do routes+=($(ls port/tests/routes/$n.keyed port/tests/routes/$n.bot 2>/dev/null || true)); done
else
  routes=(port/tests/routes/*.keyed port/tests/routes/*.bot)
fi

check_route() {
  local route=$1 name ext frames at env st
  name=$(basename "$route"); ext=${name##*.}; name=${name%.*}
  if [ "$ext" = keyed ]; then
    frames=$(( $(wc -l < "$route") + 300 ))
    env=(MFF_INPUT="$(hostpwd)/$route")
  else
    read -r bot frames < "$route"
    frames=${frames%$'\r'}; bot=${bot%$'\r'}
    env=(MFF_BOT="$bot")
  fi
  at=${MFF_SAVE_AT:-$((frames / 10)),$((frames * 3 / 10)),$((frames / 2)),$((frames * 7 / 10)),$((frames * 9 / 10))}
  rm -rf "$OUT/states_$name"; mkdir -p "$OUT/states_$name"
  env SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy "${env[@]}" MFF_SAVE_AT="$at" \
    MFF_SAVE_DIR="$OUTW/states_$name" MFF_FRAMELOG="$OUTW/full_$name.flog" MFF_PICLOG="$OUTW/full_$name.pic" \
    "$EXE" --headless --smoke "$frames" > "$OUT/full_$name.log" 2>&1
  for st in "$OUT/states_$name"/*.mffstate; do
    local f=${st##*state_}; f=${f%.mffstate}
    env SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy "${env[@]}" \
      MFF_FRAMELOG="$OUTW/load_${name}_$f.flog" MFF_PICLOG="$OUTW/load_${name}_$f.pic" \
      "$EXE" --headless --load "$OUTW/states_$name/state_$f.mffstate" --smoke "$frames" \
      > "$OUT/load_${name}_$f.log" 2>&1 &
  done
  wait
}

for r in "${routes[@]}"; do check_route "$r" & done
wait

status=0
for r in "${routes[@]}"; do
  name=$(basename "$r"); name=${name%.*}
  echo "== $name: states at $(ls "$OUT/states_$name" | sed 's/state_//; s/.mffstate//' | sort -n | tr '\n' ' ')"
  for st in "$OUT/states_$name"/*.mffstate; do
    f=${st##*state_}; f=${f%.mffstate}
    echo "-- loaded at frame $f"
    if $PY -I port/tests/savestate_cmp.py "$OUT/full_$name.flog" "$OUT/full_$name.pic" \
         "$OUT/load_${name}_$f.flog" "$OUT/load_${name}_$f.pic"; then
      rm -f "$OUT/load_${name}_$f.flog" "$OUT/load_${name}_$f.pic"
    else
      status=1
      tail -3 "$OUT/load_${name}_$f.log"
    fi
  done
done
exit $status
