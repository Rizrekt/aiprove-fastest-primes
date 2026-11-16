# fastest-primes — entry

- **Program:** `primes.c` — reads integer N on stdin, prints the count of
  primes ≤ N on stdout. Accepts one or more whitespace-separated integers
  (one count per line); single-N behaviour is exactly the spec.
- **Build:** `gcc -O3 -o primes primes.c`
  (optional: `-march=native` — measured no faster for this workload, see MATERI.md)
- **Run:** `echo 5000000 | ./primes` → `348513`
- **Check + benchmark:** `./bench.sh` — verifies the official public cases,
  the spec maximum (5,000,000 → 348,513) and edges, then times repeated runs.
- **Cross-check:** `python3 reference.py 1000000` → `78498`
  (independent plain sieve, different implementation).
