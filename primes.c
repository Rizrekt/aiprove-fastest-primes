/* fastest-primes: read integer(s) N on stdin, print pi(N) = #primes <= N.
 * Odd-only bitset sieve of Eratosthenes over [3, maxN], then popcount.
 * Handles the full spec range 1 <= N <= 5,000,000 (and far beyond).
 * Build: gcc -O3 -march=native -o primes primes.c    Run: ./primes  (N on stdin)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned long long pi_upto(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    /* bit i represents odd number 2*i+3; count = 1 (prime 2) + unmarked bits */
    size_t nbits = (size_t)((n - 1) / 2);          /* odds 3,5,...,<=n */
    unsigned char *bits = calloc((nbits + 7) / 8, 1);
    if (!bits) exit(1);
    for (long long p = 3; p * p <= n; p += 2) {
        size_t pi_ = (size_t)((p - 3) / 2);
        if (bits[pi_ >> 3] & (1u << (pi_ & 7))) continue;
        for (long long j = p * p; j <= n; j += 2 * p) {
            size_t ji = (size_t)((j - 3) / 2);
            bits[ji >> 3] |= (unsigned char)(1u << (ji & 7));
        }
    }
    unsigned long long marked = 0;
    size_t full = nbits / 8;
    for (size_t i = 0; i < full; i++) marked += (unsigned)__builtin_popcount(bits[i]);
    if (nbits % 8) {
        unsigned char last = bits[full] & (unsigned char)((1u << (nbits % 8)) - 1);
        marked += (unsigned)__builtin_popcount(last);
    }
    free(bits);
    return 1 + (nbits - marked);                   /* +1 for prime 2 */
}

int main(void) {
    int ns[1024]; int cnt = 0, v;
    while (cnt < 1024 && scanf("%d", &v) == 1) ns[cnt++] = v;
    for (int i = 0; i < cnt; i++) printf("%llu\n", pi_upto(ns[i]));
    return 0;
}
