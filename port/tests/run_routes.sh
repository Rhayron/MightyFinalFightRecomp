#!/bin/bash
# P4 regression runner: play every route in port/tests/routes and either
# record the baseline (on the pure scaffold, regen.sh --pure) or check the
# candidate (port/scaffold/build) against it, frame by frame.
#   port/tests/run_routes.sh baseline [ROUTE...]
#   port/tests/run_routes.sh check [ROUTE...]
# Routes: NAME.keyed (MFF_INPUT, nmi_input.py) or NAME.bot (one line:
# "MFF_BOT-value frames", see port/scaffold/bot.c). Routes run in parallel.
set -e
cd "$(dirname "$0")/../.."
MODE=${1:-check}
shift || true
if [ "$MODE" = baseline ]; then
  EXE=runs/p4/pure/build/Release/MffScaffold.exe
else
  EXE=port/scaffold/build/Release/MffScaffold.exe
fi
ROM="$(pwd -W)/rom/Mighty Final Fight (USA).nes"
mkdir -p runs/p4
if [ $# -gt 0 ]; then
  routes=()
  for n in "$@"; do routes+=($(ls port/tests/routes/$n.keyed port/tests/routes/$n.bot 2>/dev/null || true)); done
else
  routes=(port/tests/routes/*.keyed port/tests/routes/*.bot)
fi

run_one() {
  local route=$1 name ext out frames env
  name=$(basename "$route"); ext=${name##*.}; name=${name%.*}
  out=runs/p4/$([ "$MODE" = baseline ] && echo base || echo cand)_$name.flog
  if [ "$ext" = keyed ]; then
    frames=$(( $(wc -l < "$route") + 300 ))
    env=(MFF_INPUT="$(pwd -W)/$route")
  else
    read -r bot frames < "$route"
    env=(MFF_BOT="$bot")
  fi
  env SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy "${env[@]}" MFF_FRAMELOG="$(pwd -W)/$out" \
    MFF_PICLOG="$(pwd -W)/${out%.flog}.pic" \
    timeout --kill-after=10 600 "$EXE" "$ROM" --smoke "$frames" --smoke-interval 1000000 --smoke-output runs/p4 \
    > runs/p4/$name.log 2>&1 || echo "$name: exit $?" >> runs/p4/$name.log
}

for r in "${routes[@]}"; do run_one "$r" & done
wait

status=0
for r in "${routes[@]}"; do
  name=$(basename "$r"); name=${name%.*}
  if [ "$MODE" = check ]; then
    echo "== $name"
    py -3 -I port/tests/selfcheck.py runs/p4/base_$name.flog runs/p4/cand_$name.flog --max 5 \
      --pics runs/p4/base_$name.pic runs/p4/cand_$name.pic || status=1
  else
    echo "$name: baseline $(( $(stat -c %s runs/p4/base_$name.flog) / 2340 )) frames"
  fi
  grep -h "MISMATCH\|TRAP\|exit [0-9]" runs/p4/$name.log | head -3 || true
done
exit $status
