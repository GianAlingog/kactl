/**
 * Author: unknown
 * Date: 2025-10-18
 * Source: unknown
 * Description: Adds a time dimension to the segment tree.
 * This is mostly an example implementation of a persistent segment tree.
 * It would likely be better to instead modify the implementations of the other segment trees to be persistent.
 * Usage: roots[ind++] = build(0, n-1, a); to build, roots[ind] = update(roots[ind], pos, new_val, 0, n-1); to update,
 * ll sum = query(roots[ind], l, r, 0, n-1); to query, and roots[ind++] = new segtree(roots[old_ind]); to copy a version.
 * Time: O(n log n)
 * Status: lightly tested
 */

#pragma once

int ind = 0;
struct segtree {
    ll val;
    segtree *left, *right;
 
    segtree(ll x) : val(x), left(nullptr), right(nullptr) {;;}
 
    segtree(segtree *left, segtree *right) : left(left), right(right) {
        val = 0;
        if (left) val += left->val;
        if (right) val += right->val;
    }
 
    segtree(segtree *copy) : val(copy->val), left(copy->left), right(copy->right) {;;}
};
 
segtree *build(int l, int r, vector<ll> &a) {
    if (l == r) return new segtree(a[l]);
    int m = (l + r) / 2;
    return new segtree(build(l, m, a), build(m+1, r, a));
}
 
segtree *update(segtree *curr, int ind, ll upd, int l, int r) {
    if (l == r) return new segtree(upd);
    int m = (l + r) / 2;
    if (ind <= m) return new segtree(update(curr->left, ind, upd, l, m), curr->right);
    else return new segtree(curr->left, update(curr->right, ind, upd, m+1, r));
}
 
ll query(segtree *curr, int ql, int qr, int l, int r) {
    if (ql > r or qr < l) return 0;
    if (ql <= l and r <= qr) return curr->val;
    int m = (l + r) / 2;
    return query(curr->left, ql, qr, l, m) + query(curr->right, ql, qr, m+1, r);
}
 
const int MAXN = 2e5+5;
segtree *roots[MAXN];
