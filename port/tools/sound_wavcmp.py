"""Compare two mono WAVs of the same passage (e.g. apu_render.py vs the oracle's --wav-out).

    py -3 -I port/tools/sound_wavcmp.py A.wav B.wav [--block 2048] [--max-lag 2400]

Oscillator phases differ between two renderings of the same register writes,
so sample-by-sample differences say little. Reported instead, after finding
the lag that best aligns the loudness envelopes:
  - envelope correlation: Pearson r of the RMS per 800 samples (one frame at 48 kHz)
  - spectral similarity: mean cosine similarity of the magnitude spectra of
    Hann-windowed blocks (1.0 = same spectrum), and the mean of the absolute
    difference of the dB spectra over bins within 30 dB of each block's peak
  - level: RMS of each file
"""
import argparse
import cmath
import math
import struct
import wave


def load(path):
    w = wave.open(path)
    if w.getnchannels() != 1 or w.getsampwidth() != 2:
        raise SystemExit(f"{path}: want 16-bit mono")
    n = w.getnframes()
    return list(struct.unpack(f"<{n}h", w.readframes(n))), w.getframerate()


def fft(x):
    n = len(x)
    a = list(x)
    j = 0
    for i in range(1, n):
        bit = n >> 1
        while j & bit:
            j ^= bit
            bit >>= 1
        j |= bit
        if i < j:
            a[i], a[j] = a[j], a[i]
    size = 2
    while size <= n:
        w = cmath.exp(-2j * math.pi / size)
        half = size // 2
        for start in range(0, n, size):
            k = 1
            for m in range(half):
                u = a[start + m]
                v = a[start + m + half] * k
                a[start + m] = u + v
                a[start + m + half] = u - v
                k *= w
        size *= 2
    return a


def rms_env(x, hop=800):
    return [math.sqrt(sum(v * v for v in x[i:i + hop]) / hop) for i in range(0, len(x) - hop + 1, hop)]


def pearson(a, b):
    n = min(len(a), len(b))
    a, b = a[:n], b[:n]
    ma, mb = sum(a) / n, sum(b) / n
    va = sum((x - ma) ** 2 for x in a)
    vb = sum((y - mb) ** 2 for y in b)
    if va == 0 or vb == 0:
        return float("nan")
    return sum((x - ma) * (y - mb) for x, y in zip(a, b)) / math.sqrt(va * vb)


def best_lag(a, b, max_lag, hop=100):
    """Lag (samples, applied to b) maximizing the correlation of fine RMS envelopes."""
    ea, eb = rms_env(a, hop), rms_env(b, hop)
    best = (-2.0, 0)
    for lag in range(-max_lag // hop, max_lag // hop + 1):
        if lag >= 0:
            r = pearson(ea[lag:], eb)
        else:
            r = pearson(ea, eb[-lag:])
        if r == r and r > best[0]:
            best = (r, lag * hop)
    return best[1]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("a")
    ap.add_argument("b")
    ap.add_argument("--block", type=int, default=2048)
    ap.add_argument("--max-lag", type=int, default=2400)
    o = ap.parse_args()
    a, ra = load(o.a)
    b, rb = load(o.b)
    if ra != rb:
        raise SystemExit("sample rates differ")
    lag = best_lag(a, b, o.max_lag)
    if lag > 0:
        a = a[lag:]
    elif lag < 0:
        b = b[-lag:]
    n = min(len(a), len(b))
    a, b = a[:n], b[:n]
    env_r = pearson(rms_env(a), rms_env(b))
    hann = [0.5 - 0.5 * math.cos(2 * math.pi * i / (o.block - 1)) for i in range(o.block)]
    cos_sum, db_sum, blocks, silent = 0.0, 0.0, 0, 0
    for s in range(0, n - o.block + 1, o.block):
        fa = fft([a[s + i] * hann[i] for i in range(o.block)])[:o.block // 2]
        fb = fft([b[s + i] * hann[i] for i in range(o.block)])[:o.block // 2]
        ma, mb = [abs(v) for v in fa], [abs(v) for v in fb]
        na, nb = math.sqrt(sum(v * v for v in ma)), math.sqrt(sum(v * v for v in mb))
        if na < 1e3 and nb < 1e3:
            silent += 1
            continue
        blocks += 1
        cos_sum += sum(x * y for x, y in zip(ma, mb)) / (na * nb) if na and nb else 0.0
        peak = max(max(ma), max(mb))
        floor = peak * 10 ** (-30 / 20)
        diffs = [abs(20 * math.log10(max(x, floor) / max(y, floor))) for x, y in zip(ma, mb) if x > floor or y > floor]
        db_sum += sum(diffs) / len(diffs) if diffs else 0.0
    rms = lambda x: math.sqrt(sum(v * v for v in x) / len(x)) if x else 0.0
    print(f"aligned with lag {lag} samples ({lag / ra * 1000:.1f} ms), {n / ra:.2f} s compared")
    print(f"  RMS: {rms(a):.1f} vs {rms(b):.1f}")
    print(f"  loudness envelope correlation (per 800 samples): {env_r:.4f}")
    if blocks:
        print(f"  spectral similarity over {blocks} blocks of {o.block} ({silent} silent skipped): "
              f"cosine {cos_sum / blocks:.4f}, mean |dB difference| {db_sum / blocks:.2f} dB")


if __name__ == "__main__":
    main()
