/**
 * Author: Gian Alingog
 * Date: 2025-10-17
 * Source: unknown
 * Description: For some reason, this custom bitset performs almost twice that
 * of the standard library's bitset.
 * Time: O(N / w)
 * Status: Tested on CF
 */

// Do not access with negative indices
#pragma once

template <size_t N>
struct FastBitset64 {
    // ensure unsigned and 64 bit
    // do not use 'long long' as it may evaluate do a different word size
    using u64 = uint64_t;

    static constexpr size_t POW_2 = 6;
    static constexpr size_t WORD_SIZE = u64(1) << POW_2;
    static constexpr u64 FULL_MASK = WORD_SIZE - 1;
    static constexpr size_t WORDS = (N + WORD_SIZE - 1) / WORD_SIZE;

    array<u64, WORDS> a{};

    static constexpr u64 LAST_MASK = (N % WORD_SIZE == 0 ? ~u64(0) : ((u64(1) << (N % WORD_SIZE)) - 1));
    static inline size_t ctz(u64 x) { return __builtin_ctzll(x); }
    static inline size_t clz(u64 x) { return __builtin_clzll(x); }
    static inline size_t pop(u64 x) { return __builtin_popcountll(x); }

    inline void fix_last() {
        if constexpr (N % WORD_SIZE) a[WORDS - 1] &= LAST_MASK;
    }

    static constexpr size_t size() { return N; }
    inline void reset() { a.fill(0); }
    inline void set() {
        a.fill(~u64(0));
        fix_last();
    }

    inline void set(size_t i, bool v = true) {
        // if (i >= N) return; // for good practice it should panic
        assert(i < N);
        size_t w = i >> POW_2, b = i & FULL_MASK;
        u64 m = u64(1) << b;
        if (v) a[w] |= m;
        else a[w] &= ~m;
    }

    inline bool test(size_t i) const {
        assert(i < N);
        size_t w = i >> POW_2, b = i & FULL_MASK;
        return (a[w] >> b) & u64(1);
    }

    inline void flip() {
        for (size_t i = 0; i < WORDS; i++) a[i] = ~a[i];
        fix_last();
    }

    inline void flip(size_t i) {
        assert(i < N);
        a[i >> POW_2] ^= (u64(1) << (i & FULL_MASK));
    }

    // currently linear
    // i don't see the point in this function honestly
    // if you want constant time,
    // keep track of all bitcounts after each operation
    inline bool any() const {
        for (size_t i = 0; i < WORDS; i++) if (a[i]) return true;
        return false;
    }

    inline bool none() const { return !any(); }

    inline bool all() const {
        for (size_t i = 0; i + 1 < WORDS; i++) if (~a[i]) return false;
        if constexpr (N % WORD_SIZE) return (a[WORDS - 1] & LAST_MASK) == LAST_MASK;
        else return (a[WORDS - 1] == ~u64(0));
    }

    inline size_t count() const {
        size_t c = 0;
        for (size_t i = 0; i < WORDS; i++) c += pop(a[i]);
        return c;
    }

    // can only return the first word
    inline u64 to_llong() const { return (WORDS ? a[0] : u64(0)); }

    string to_string() const {
        string s;
        s.reserve(N);
        for (size_t i = N; i --> 0;) s.push_back(char('0' + test(i)));
        return s;
    }

    inline FastBitset64& operator&=(const FastBitset64 &o) {
        for (size_t i = 0; i < WORDS; i++) a[i] &= o.a[i];
        return *this;
    }

    inline FastBitset64& operator|=(const FastBitset64 &o) {
        for (size_t i = 0; i < WORDS; i++) a[i] |= o.a[i];
        return *this;
    }

    inline FastBitset64& operator^=(const FastBitset64 &o) {
        for (size_t i = 0; i < WORDS; i++) a[i] ^= o.a[i];
        return *this;
    }

    friend inline FastBitset64 operator&(FastBitset64 lhs, const FastBitset64 &rhs) { lhs &= rhs; return lhs; }
    friend inline FastBitset64 operator|(FastBitset64 lhs, const FastBitset64 &rhs) { lhs |= rhs; return lhs; }
    friend inline FastBitset64 operator^(FastBitset64 lhs, const FastBitset64 &rhs) { lhs ^= rhs; return lhs; }
    friend inline bool operator==(const FastBitset64 &x, const FastBitset64 &y) { return x.a == y.a; }
    friend inline bool operator!=(const FastBitset64 &x, const FastBitset64 &y) { return !(x == y); }

    inline FastBitset64& operator<<=(size_t i) {
        if (i >= N) { reset(); return *this; }
        size_t w = i >> POW_2, b = i & FULL_MASK;
        if (b == 0) {
            for (size_t i = WORDS; i --> 0;) {
                a[i] = (i >= w) ? a[i - w] : 0;
            }
        } else {
            for (size_t i = WORDS; i --> 0;) {
                u64 big = (i >= w) ? (a[i - w] << b) : 0;
                u64 sml = (i >= w + 1) ? (a[i - w - 1] >> (WORD_SIZE - b)) : 0;
                a[i] = big | sml;
            }
        }
        fix_last();
        return *this;
    }

    inline FastBitset64& operator>>=(size_t i) {
        if (i >= N) { reset(); return *this; }
        size_t w = i >> POW_2, b = i & FULL_MASK;
        if (b == 0) {
            for (size_t i = 0; i < WORDS; i++) {
                a[i] = (i + w < WORDS) ? a[i + w] : 0;
            }
        } else {
            for (size_t i = WORDS; i --> 0;) {
                u64 sml = (i + w < WORDS) ? (a[i + w] >> b) : 0;
                u64 big = (i + w + 1 < WORDS) ? (a[i + w + 1] << (WORD_SIZE - b)) : 0;
                a[i] = sml | big;
            }
        }
        fix_last();
        return *this;
    }

    friend inline FastBitset64 operator<<(FastBitset64 x, size_t i) { x <<= i; return x; }
    friend inline FastBitset64 operator>>(FastBitset64 x, size_t i) { x >>= i; return x; }

    inline size_t find_first() const {
        for (size_t i = 0; i < WORDS; i++) {
            if (a[i]) return (i << POW_2) + ctz(a[i]);
        }
        return N;
    }

    // upper_bound
    inline size_t find_next(size_t i) const {
        i++;
        if (i >= N) return N;
        size_t w = i >> POW_2, b = i & FULL_MASK;
        
        // need to check immediate word first
        u64 x = a[w] & (~u64(0) << b);
        if (x) return (w << POW_2) + ctz(x);
        for (++w; w < WORDS; w++) if (a[w]) return (w << POW_2) + ctz(a[w]);
        return N;
    }

    // standard: use N for invalid
    inline size_t find_last() const {
        for (size_t i = WORDS; i --> 0;) {
            u64 x = a[i];
            if constexpr (N % WORD_SIZE) if (i == WORDS - 1) x &= LAST_MASK;
            if (x) return (i << POW_2) + (FULL_MASK - clz(x));
        }
        return N;
    }

    inline size_t find_prev(size_t i) const {
        if (i == 0) return N;
        i--;
        size_t w = i >> POW_2, b = i & FULL_MASK;
        u64 m = (b == FULL_MASK) ? ~u64(0) : ((u64(1) << (b + 1)) - 1);
        u64 x = a[w] & m;
        if (x) return (w << POW_2) + (FULL_MASK - clz(x));
        while (w--) if (a[w]) return (w << POW_2) + (FULL_MASK - clz(a[w]));
        return N;
    }
};
