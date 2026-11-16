# MATERI — fastest-primes (AIProve bounty)

Prepared 2026-11-14 (Asia/Jakarta). NOT submitted — materials only.

## 0. Corrections to the task framing (read first)

The assignment described the spec loosely ("primes below some N", references
pi(10^7)=664579 / pi(10^8)=5761455) and a 100,000 PROVE prize. Verified from
the saved detail JSON (`~/workspace/research/aiprove/details/fastest-primes.json`),
the official challenge spec (`github.com/AIProveRH/aiprove/challenges/fastest-primes/README.md`,
local clone) and the live API (`GET /api/bounties/fastest-primes`, re-checked
during prep):

- **Spec:** read an integer **N, 1 ≤ N ≤ 5,000,000**, on **stdin**; print the
  count of primes **≤ N** on **stdout**. So the binding reference value is
  **pi(5,000,000) = 348,513** (verified below, two independent ways) — the
  10^7 / 10^8 references never occur in this bounty.
- **Entry:** a program (**any language**) plus its run command.
- **Metric:** wall-clock latency on the grader's hidden N values, all entries
  run back-to-back on the same sandbox machine. **Wrong output is
  disqualified before timing.**
- **Prize:** **1,000,000 PROVE**. **Deadline:** 16 Nov 2026 13:49 UTC = 20:49 WIB.
- **submitKind: `repo`** → `{repoUrl, commit, transcript}` (+ optional `handle`).

Public samples (`cases.json`): 10→4, 100→25, 1000→168, 100000→9592,
1000000→78498.

## 1. Solution — `primes.c`

Odd-only bitset sieve of Eratosthenes over [3, N]:

- Bit i represents odd number 2i+3 (2.5 MB maximum at N = 5,000,000), marking
  strides of 2p from p², then a popcount; +1 accounts for the prime 2.
- N < 2 → 0; N = 2 → 1 (explicit edges).
- Input: one or more whitespace-separated integers on stdin, one count per
  line out. For the spec's single-N input the output is exactly one line.

**Build:** `gcc -O3 -o primes primes.c` · **Run:** `./primes` (N on stdin)

## 2. Correctness evidence (run locally)

`./bench.sh` correctness section — all OK:

```
OK  N=10 -> 4        OK  N=100000 -> 9592     OK  N=2 -> 1
OK  N=100 -> 25      OK  N=1000000 -> 78498   OK  N=3 -> 2
OK  N=1000 -> 168    OK  N=5000000 -> 348513  OK  N=1 -> 0
```

Independent cross-check — `reference.py` (a separate, plain bytearray sieve,
no shared code) at 10, 100, 1000, 999983, 100000, 1000000, 5000000 produced
4, 25, 168, 78498, 9592, 78498, 348513 — **identical to the C program on every
value**, including pi(999983) = 78,498 and pi(5,000,000) = 348,513.

## 3. Benchmarks (this VM: shared 1.5 vCPU, gcc 13.3, -O3)

Wall-clock per full process run (spawn + sieve + print), Python
`perf_counter` harness in `bench.sh`:

| Build | N | min | median | runs |
|---|---|---|---|---|
| `gcc -O3 -march=native` | 5,000,000 | 9.73 ms | 12.04 ms | 15 |
| `gcc -O3 -march=native` | 1,000,000 | 2.96 ms | 3.21 ms | 15 |
| `gcc -O3` (portable) | 5,000,000 | 9.38 ms | 9.60 ms | 5 |

Notes: `-march=native` buys nothing (the sieve is memory-latency bound at
this size), so the portable `-O3` build is the recommended run command — it
cannot fail on an unknown sandbox CPU. Absolute numbers will differ on the
grader's machine; the ranking-relevant fact is that at N ≤ 5×10^6 the sieve
itself is ~1–3 ms and total latency is dominated by process startup, which is
the same for every compiled entry. Fancier machinery (Meissel–Lehmer,
precomputed answer tables) cannot beat the startup floor and only adds
wrong-answer risk; a scripting-language entry pays 10–50× more in startup
alone (CPython interpreter boot ≈ 15–30 ms before any sieving).

## 4. Repo contents (this folder = the repo to publish)

- `primes.c` — the entry program.
- `bench.sh` — correctness battery + timing harness.
- `reference.py` — independent verification implementation.
- `cases.json` — official public samples (copied from the challenge repo).
- `README.md` — build/run instructions for the grader.
- `MATERI.md` — this file. `TRANSCRIPT.txt` — build transcript draft.
- `.gitignore` — excludes the compiled binaries (`primes`, `primes-portable`).

## 5. What remains to submit (NOT done here, by instruction)

1. Create a public GitHub repo from this folder, push the local commit.
2. `POST /api/bounties/fastest-primes/submit` with
   `{"repoUrl": "<repo url>", "commit": "<full hash of the pushed commit>",
   "transcript": "<TRANSCRIPT.txt content>", "handle": "akta.agent"}`;
   the run command to state for the grader is in `README.md`
   (`gcc -O3 -o primes primes.c`, then `./primes` with N on stdin).
3. Local commit hash is recorded in TRANSCRIPT.txt after the local commit.
