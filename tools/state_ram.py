"""Read or patch CPU RAM inside a cycle save state (runner/cyc/cyc_state.c).

    py -3 -I tools/state_ram.py STATE get 0061 003E
    py -3 -I tools/state_ram.py STATE set 0061=02 [003E=09 ...] -o OUT
    py -3 -I tools/state_ram.py STATE verify MEMDUMP     # --mem-out of the same frame

A coverage aid only: it starts a real run of the game's own code from a state
that would take a long input route to reach (a later round, a boss). States
carry no checksum over their contents; CPU RAM is HwMachine.ram, the last
field of the 'HW  ' chunk (hw_internal.h).
"""
import argparse
import struct
import sys

HEADER = 32  # magic[8] + version, prg_hash, cart_hash, layout, program_crc, frame
RAM = 0x800
# Measured against --mem-out at nesrecomp 2b4fcdb: 23 bytes of HwMachine
# precede ram[], one byte of padding follows it.
HW_SIZE, HW_RAM_OFFSET = 2072, 23


def ram_span(data):
    if data[:8] != b"CYCSTATE":
        sys.exit("not a cycle save state")
    at = HEADER
    while at + 8 <= len(data):
        tag, n = struct.unpack_from("<4sI", data, at)
        if tag == b"HW  ":
            if n != HW_SIZE:
                sys.exit(f"HW chunk is {n} bytes, expected {HW_SIZE}: HwMachine changed; "
                         "re-derive HW_RAM_OFFSET with `verify`")
            return at + 8 + HW_RAM_OFFSET, at + 8 + n
        if tag == b"END ":
            break
        at += 8 + n
    sys.exit("no HW chunk")


def main():
    p = argparse.ArgumentParser()
    p.add_argument("state")
    p.add_argument("cmd", choices=["get", "set", "verify"])
    p.add_argument("args", nargs="+")
    p.add_argument("-o", "--out")
    a = p.parse_args()
    data = bytearray(open(a.state, "rb").read())
    base, _ = ram_span(data)
    if a.cmd == "get":
        for addr in a.args:
            print(f"{addr.upper()}={data[base + int(addr, 16)]:02X}")
    elif a.cmd == "set":
        for kv in a.args:
            addr, val = kv.split("=")
            data[base + int(addr, 16)] = int(val, 16)
        open(a.out or a.state, "wb").write(data)
    else:
        ram = bytearray()
        for line in open(a.args[0]):
            if line.startswith("ram "):
                ram += bytes.fromhex(line.split(":", 1)[1])
        same = data[base:base + RAM] == ram[:RAM]
        print("RAM matches" if same else "RAM MISMATCH")
        sys.exit(0 if same else 1)


if __name__ == "__main__":
    main()
