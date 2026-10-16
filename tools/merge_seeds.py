"""Merge per-run --miss-log files into cyc_seeds.txt (counts summed).

    py -3 -I tools/merge_seeds.py cyc_seeds.txt runs/*.miss

Runs write their own miss log so parallel runs never race on the seed file;
this folds them in. Prints how many seed lines were new.
"""
import sys


def read(path, into, order):
    try:
        lines = open(path).read().splitlines()
    except FileNotFoundError:
        return
    for line in lines:
        if not line.strip() or line.startswith("#"):
            continue
        key, _, count = line.rpartition(" ")
        if not key:
            key, count = count, "1"
        if key not in into:
            order.append(key)
            into[key] = 0
        into[key] += int(count)


def main(argv):
    out, runs = argv[0], argv[1:]
    seeds, order = {}, []
    read(out, seeds, order)
    before = len(seeds)
    for run in runs:
        read(run, seeds, order)
    header = [l for l in open(out).read().splitlines() if l.startswith("#")][:2]
    with open(out, "w", newline="\n") as f:
        f.write("\n".join(header + [f"{k} {seeds[k]}" for k in sorted(order, key=sort_key)]) + "\n")
    print(f"{len(seeds) - before} new seed lines ({len(seeds)} total)")


def sort_key(key):
    # 4k:BB:AAAA -> (bank, addr); a bare AAAA sorts first
    parts = key.split(":")
    return (int(parts[-2], 16) if len(parts) >= 2 else -1, int(parts[-1], 16))


if __name__ == "__main__":
    main(sys.argv[1:])
