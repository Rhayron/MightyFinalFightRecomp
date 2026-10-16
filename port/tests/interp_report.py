"""How much ROM code the runtime's interpreter ran in one route.

    py -3 -I port/tests/interp_report.py LOG.jsonl

LOG is what the runtime writes with NESRECOMP_FALLBACK_LOG (run_routes.sh
sets it per route). A routine the build has no function for still runs,
interpreted from the ROM; the port must have them all, so anything but 0
fails, listing where the interpreter was entered
(port/tools/find_entries.py looks for the rest statically).
"""
import collections
import json
import sys


def main():
    path = sys.argv[1]
    try:
        samples = [json.loads(line) for line in open(path) if line.strip()]
    except OSError:
        print(f"interpreter: no log {path}")
        return 1
    if not samples:
        print(f"interpreter: empty log {path}")
        return 1
    total = samples[-1].get("total_instrs", 0)
    if not total:
        return 0
    entries = collections.Counter()
    for s in samples:
        for e in s.get("entries", []):
            entries[(e["bank"], e["entry"])] += e["instrs"]
    print(f"interpreter ran {total} ROM instructions:")
    for (bank, entry), n in entries.most_common(10):
        print(f"    bank {bank} {entry}: {n}")
    return 1


if __name__ == "__main__":
    sys.exit(main())
