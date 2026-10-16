#!/bin/bash
# P4 regression runner: play every route in port/tests/routes on the scaffold
# build and either record the baseline or check against it.
#   port/tests/run_routes.sh baseline     # record runs/p4/base_<route>.flog
#   port/tests/run_routes.sh check        # run again and selfcheck each route
set -e
cd "$(dirname "$0")/../.."
MODE=${1:-check}
EXE=port/scaffold/build/Release/MffScaffold.exe
ROM="$(pwd -W)/rom/Mighty Final Fight (USA).nes"
mkdir -p runs/p4
status=0
for keyed in port/tests/routes/*.keyed; do
  name=$(basename "$keyed" .keyed)
  frames=$(wc -l < "$keyed")
  out=runs/p4/$([ "$MODE" = baseline ] && echo base || echo cand)_$name.flog
  SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy MFF_INPUT="$(pwd -W)/$keyed" MFF_FRAMELOG="$(pwd -W)/$out" \
    "$EXE" "$ROM" --smoke $((frames + 300)) --smoke-interval 100000 --smoke-output runs/p4 > runs/p4/$name.log 2>&1
  if [ "$MODE" = check ]; then
    echo "== $name"
    py -3.14 -I port/tests/selfcheck.py runs/p4/base_$name.flog "$out" --max 5 || status=1
  else
    echo "$name: baseline $(( $(stat -c %s "$out") / 2340 )) frames"
  fi
done
exit $status
