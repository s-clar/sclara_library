// 調整中！

/*
    Segment Tree
    1点更新・区間クエリを行えるデータ構造

    使い方：
        ・宣言

        基本は
        SegTree<型, 演算, 単位元> sgt;

        vector<型> a = ...;
        SegTree<型, 演算, 単位元> sgt(a);
        など、vectorを用いて初期化することも可能

        型はlong longなど
        演算は2つの情報を結合する関数
        単位元は、演算してももとの値を変えない元(分からなければ理解しなくても大体使える)

        多く使う演算に関しては関数が宣言済

        区間和SegTree：SegTree<long long, sum_op, sum_e> sgt;
        区間最小値SegTree：SegTree<long long, min_op, min_e> sgt;
        区間最大値SegTree：SegTree<long long, max_op, max_e> sgt;
*/


long long sum_op(long long a, long long b) {
    return a + b;
}

long long sum_e() {
    return 0LL;
}

long long min_op(long long a, long long b) {
    return min(a, b);
}

long long min_e() {
    return LLONG_MAX;
}

long long max_op(long long a, long long b) {
    return min(a, b);
}

long long max_e() {
    return LLONG_MAX;
}

template<class S, S (*op)(S, S), S (*e)()>
class SegTree {
private:
    int n, size;
    vector<S> d;

    void pull(int k) {
        d[k] = op(d[k * 2], d[k * 2 + 1]);
    }

public:
    // 要素数を指定。初期値はすべて e()
    explicit SegTree(int n_) : SegTree(vector<S>(n_, e())) {}

    // vector から構築
    explicit SegTree(const vector<S>& v)
        : n((int)v.size()), size(1) {

        while (size < n) size <<= 1;
        d.assign(size * 2, e());

        for (int i = 0; i < n; ++i) {
            d[size + i] = v[i];
        }
        for (int i = size - 1; i >= 1; --i) {
            pull(i);
        }
    }

    int length() const {
        return n;
    }

    // 1点代入: a[p] = x
    void update(int p, S x) {
        assert(0 <= p && p < n);

        p += size;
        d[p] = x;
        while (p >>= 1) pull(p);
    }

    // 1点取得
    S get(int p) const {
        assert(0 <= p && p < n);
        return d[size + p];
    }

    template<class F>
    void apply(int p, F f) {
        assert(0 <= p && p < n);
    
        p += size;
        d[p] = f(d[p]);
        while (p >>= 1) pull(p);
    }

    void add(int p, S x) {
        apply(p, [&](S a) {
            return a + x;
        });
    }

    // 両閉区間 [l, r] のクエリ
    S query(int l, int r) const {
        if (l > r) return e();
        assert(0 <= l && r < n);

        l += size;
        r += size + 1;

        S left = e(), right = e();

        while (l < r) {
            if (l & 1) {
                left = op(left, d[l++]);
            }
            if (r & 1) {
                right = op(d[--r], right);
            }
            l >>= 1;
            r >>= 1;
        }

        return op(left, right);
    }

    // 全区間のクエリ
    S all_query() const {
        return d[1];
    }

    // [l, r) の集約値が f を満たす最大の r
    // 戻り値は「条件が初めて崩れる位置」
    template<class F>
    int max_right(int l, F f) const {
        assert(0 <= l && l <= n);
        assert(f(e()));
        if (l == n) return n;

        l += size;
        S sm = e();

        do {
            while ((l & 1) == 0) l >>= 1;

            if (!f(op(sm, d[l]))) {
                while (l < size) {
                    l *= 2;
                    if (f(op(sm, d[l]))) {
                        sm = op(sm, d[l]);
                        ++l;
                    }
                }
                return min(n, l - size);
            }

            sm = op(sm, d[l]);
            ++l;
        } while ((l & -l) != l);

        return n;
    }

    // [l, r) の集約値が f を満たす最小の l
    // r は右端の次の位置
    template<class F>
    int min_left(int r, F f) const {
        assert(0 <= r && r <= n);
        assert(f(e()));
        if (r == 0) return 0;

        r += size;
        S sm = e();

        do {
            --r;
            while (r > 1 && (r & 1)) r >>= 1;

            if (!f(op(d[r], sm))) {
                while (r < size) {
                    r = r * 2 + 1;
                    if (f(op(d[r], sm))) {
                        sm = op(d[r], sm);
                        --r;
                    }
                }
                return max(0, r + 1 - size);
            }

            sm = op(d[r], sm);
        } while ((r & -r) != r);

        return 0;
    }
};
