/**
 * Author: Gian Alingog
 * Date: 2025-10-17
 * Source: unknown
 * Description: Trie, support min and max XOR queries
 * Perform a sweep for prefix and suffix min / max XOR
 * Time: O(log_2 maxA)
 * Status: Tested on CF
 */

#pragma once

struct jiayou {
    int count, children[2];
    jiayou(int left = -1, int right = -1, int count = 0) : count(count) {
        children[0] = left;
        children[1] = right;
    }
};

int query(vector<jiayou> &trie, int x) {
    int res = 0;
    int at = 0;
    for (int i = 30; i >= 0; i--) {
        int bit = (x & (1 << i)) != 0;
        int want = bit ^ 1;

        if (trie[at].children[want] != -1 and trie[trie[at].children[want]].count > 0) {
            res |= (1 << i);
            at = trie[at].children[want];
        } else {
            at = trie[at].children[bit];
        }
    }
    return res;
}

void insert(vector<jiayou> &trie, int x) {
    int at = 0;
    trie[at].count++;
    for (int i = 30; i >= 0; i--) {
        int bit = (x & (1 << i)) != 0;
        if (trie[at].children[bit] == -1) {
            trie[at].children[bit] = trie.size();
            trie.push_back(jiayou());
        }
        at = trie[at].children[bit];
        trie[at].count++;
    }
}

void erase(vector<jiayou> &trie, int x) {
    int at = 0;
    trie[at].count--;
    for (int i = 30; i >= 0; i--) {
        int bit = (x & (1 << i)) != 0;
        at = trie[at].children[bit];
        trie[at].count--;
    }
}

// vector<jiayou> trie = {jiayou()};
// insert(trie, 0);
