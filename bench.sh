#!/bin/bash
# Correctness (public cases + spec max + edges) then timing, run from this dir.
set -e
for pair in "10 4" "100 25" "1000 168" "100000 9592" "1000000 78498" "5000000 348513" "1 0" "2 1" "3 2"; do
  set -- $pair
  got=$(echo "$1" | ./primes)
  [ "$got" = "$2" ] && echo "OK  N=$1 -> $got" || { echo "FAIL N=$1 -> $got (want $2)"; exit 1; }
done
python3 - << 'PYEOF'
import subprocess, time
for exe, n, runs in [("./primes", 5000000, 15), ("./primes", 1000000, 15), ("./primes-portable", 5000000, 5)]:
    ts = []
    for _ in range(runs):
        t0 = time.perf_counter()
        out = subprocess.run([exe], input=f"{n}\n", capture_output=True, text=True)
        ts.append((time.perf_counter() - t0) * 1000)
    ts.sort()
    print(f"{exe} N={n}: out={out.stdout.strip()} min={ts[0]:.2f}ms median={ts[len(ts)//2]:.2f}ms max={ts[-1]:.2f}ms ({runs} runs, incl. process start)")
PYEOF
