#!/bin/bash
# P4 regression runner: play every route in port/tests/routes and either
# record the baseline (on the pure scaffold, regen.sh --pure) or check a
# candidate against it, frame by frame.
#   port/tests/run_routes.sh baseline [ROUTE...]
#   port/tests/run_routes.sh check [ROUTE...]
# Routes: NAME.keyed (MFF_INPUT, nmi_input.py) or NAME.bot (one line:
# "MFF_BOT-value frames", see port/scaffold/bot.c). Routes run in parallel.
# Both modes also fail when the runtime's interpreter ran any ROM code (a
# routine with no function; port/tests/interp_report.py).
# Environment (for a worktree or a second build; defaults are this checkout):
#   MFF_EXE   the candidate (default port/scaffold/build/Release/MffScaffold.exe)
#   MFF_ROM   the dump (default rom/Mighty Final Fight (USA).nes)
#   MFF_BASE  directory with the baselines base_ROUTE.flog/.pic (default runs/p4)
#   MFF_OUT   where this run writes its logs (default runs/p4)
#   MFF_NMI_CYCLES  cycles per NMI the candidate charges and the baseline
#             does not (selfcheck.py --nmi-cycles; default 0)
# The port on its own runtime (MightyFinalFight.exe) plays the routes with
# MFF_TIMING=scaffold (docs/timing.md): the scaffold runtime's clock, which
# the baselines were recorded with, cycle for cycle (no lag frames, the NMI
# only at the scheduler's $FEBD, no 7-cycle reset or NMI entry); the
# scaffold ignores the variable.
#   PY        the Python to run the checks with (default: py -3 if there is
#             one, as on Windows, else python3)
set -e
cd "$(dirname "$0")/../.."
# a path the executable can open: Windows form under Git Bash, as is elsewhere
hostpwd() { pwd -W 2>/dev/null || pwd; }
if [ -z "$PY" ]; then
  if command -v py >/dev/null 2>&1; then PY="py -3"; else PY=python3; fi
fi
MODE=${1:-check}
shift || true
BASE=${MFF_BASE:-runs/p4}
OUT=${MFF_OUT:-runs/p4}
if [ "$MODE" = baseline ]; then
  EXE=runs/p4/pure/build/Release/MffScaffold.exe
  OUT=$BASE
else
  EXE=${MFF_EXE:-port/scaffold/build/Release/MffScaffold.exe}
fi
NMI_CYCLES=${MFF_NMI_CYCLES:-0}
ROM=${MFF_ROM:-"$(hostpwd)/rom/Mighty Final Fight (USA).nes"}
mkdir -p "$OUT"
OUTW=$(cd "$OUT" && hostpwd)
if [ $# -gt 0 ]; then
  routes=()
  for n in "$@"; do routes+=($(ls port/tests/routes/$n.keyed port/tests/routes/$n.bot 2>/dev/null || true)); done
else
  routes=(port/tests/routes/*.keyed port/tests/routes/*.bot)
fi

run_one() {
  local route=$1 name ext out frames env
  name=$(basename "$route"); ext=${name##*.}; name=${name%.*}
  out=$OUTW/$([ "$MODE" = baseline ] && echo base || echo cand)_$name.flog
  if [ "$ext" = keyed ]; then
    frames=$(( $(wc -l < "$route") + 300 ))
    env=(MFF_INPUT="$(hostpwd)/$route")
  else
    read -r bot frames < "$route"
    env=(MFF_BOT="$bot")
  fi
  rm -f "${out%.flog}.interp.jsonl"
  env SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy MFF_TIMING=scaffold "${env[@]}" MFF_FRAMELOG="$out" \
    MFF_PICLOG="${out%.flog}.pic" NESRECOMP_FALLBACK_LOG="${out%.flog}.interp.jsonl" \
    timeout --kill-after=10 600 "$EXE" "$ROM" --smoke "$frames" --smoke-interval 1000000 --smoke-output "$OUT" \
    > "$OUT/$name.log" 2>&1 || echo "$name: exit $?" >> "$OUT/$name.log"
}

for r in "${routes[@]}"; do run_one "$r" & done
wait

status=0
for r in "${routes[@]}"; do
  name=$(basename "$r"); name=${name%.*}
  if [ "$MODE" = check ]; then
    echo "== $name"
    $PY -I port/tests/selfcheck.py "$BASE/base_$name.flog" "$OUT/cand_$name.flog" --max 5 \
      --pics "$BASE/base_$name.pic" "$OUT/cand_$name.pic" --nmi-cycles "$NMI_CYCLES" || status=1
    $PY -I port/tests/interp_report.py "$OUT/cand_$name.interp.jsonl" || status=1
  else
    echo "$name: baseline $(( $(stat -c %s "$BASE/base_$name.flog") / 2340 )) frames"
    $PY -I port/tests/interp_report.py "$BASE/base_$name.interp.jsonl" || status=1
  fi
  grep -h "MISMATCH\|TRAP\|exit [0-9]\|reached data" "$OUT/$name.log" | head -3 || true
done
exit $status
