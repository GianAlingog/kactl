/**
 * Author: Gian Alingog
 * Date: 2025-10-17
 * Source: unknown
 * Description: XOR Basis, support various XOR queries.
 * This specific version supports range queries by using a time dimension.
 * Time: O(log_2 maxA)
 * Status: Tested on CF
 */
#pragma once

const int BIT = 21;
using tl = bitset<BIT>;
 
struct xor_basis {
    array<pair<tl, int>, BIT> b{};
 
    xor_basis() {;;}
    
    bool add(tl x, int id) {
        for (int i = BIT-1; i >= 0; i--) {
            if (!x.test(i)) continue;
            if (!b[i].first.any()) {
                b[i] = {x, id};
                return true;
            }
 
            if (b[i].second < id) {
                swap(x, b[i].first);
                swap(id, b[i].second);
            }
 
            x ^= b[i].first;
        }
        return false;
    }
 
    bool check(tl x, int id) {
        for (int i = BIT-1; i >= 0; i--) {
            if (!x.test(i)) continue;
            if (!b[i].first.any()) return false;
            if (b[i].second < id) return false;
            x ^= b[i].first;
        }
        return true;
    }
 
    tl find_max(int id) {
        tl res;
        for (int i = BIT-1; i >= 0; i--) {
            if (!b[i].first.any()) continue;
            if (b[i].second < id) continue;
            if (!res.test(i)) res ^= b[i].first;
        }
        return res;
    }
};
