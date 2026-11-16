# Independent reference: plain bytearray sieve (different code path from primes.c).
import sys
def pi(n):
    if n < 2: return 0
    s = bytearray([1]) * (n + 1)
    s[0:2] = b"\x00\x00"
    for p in range(2, int(n ** 0.5) + 1):
        if s[p]:
            s[p*p::p] = b"\x00" * len(s[p*p::p])
    return sum(s)
for arg in sys.argv[1:]:
    print(pi(int(arg)))
