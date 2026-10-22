//  from: https://github.com/s-clar/sclara_library/blob/main/segtree/segtree.cpp
//  version: 2026-10-22 13:39(GMT+9)

/*
    Segment Tree
    1点更新・区間クエリを行えるデータ構造
    query(l, r) は両閉区間 [l, r]
    詳しい使い方はコード末尾に記載
*/

template<class T>
struct Sum {
    using S = T;

    static S op(S a, S b) {
        return a + b;
    }

    static S e() {
        return S(0);
    }
};

template<class T>
struct Min {
    using S = T;

    static S op(S a, S b) {
        return std::min(a, b);
    }

    static S e() {
        return std::numeric_limits<S>::max();
    }
};

template<class T>
struct Max {
    using S = T;

    static S op(S a, S b) {
        return std::max(a, b);
    }

    static S e() {
        return std::numeric_limits<S>::lowest();
    }
};

template<class M>
class SegTree {
    using S = typename M::S;

private:
    int n, size;
    std::vector<S> d;

    void pull(int k) {
        d[k] = M::op(d[k * 2], d[k * 2 + 1]);
    }

public:
    // 要素数を指定。初期値はすべて M::e()
    explicit SegTree(int n_) : SegTree(std::vector<S>(n_, M::e())) {}

    // vector から構築
    explicit SegTree(const std::vector<S>& v)
        : n((int)v.size()), size(1) {

        while (size < n) size <<= 1;
        d.assign(size * 2, M::e());

        for (int i = 0; i < n; ++i) {
            d[size + i] = v[i];
        }
        for (int i = size - 1; i >= 1; --i) {
            pull(i);
        }
    }

    // 要素数
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

    // 1点取得（読み取り専用）
    S operator[](int p) const {
        return get(p);
    }

    // 任意の1点更新: a[p] = f(a[p])
    template<class F>
    void apply(int p, F f) {
        assert(0 <= p && p < n);

        p += size;
        d[p] = f(d[p]);
        while (p >>= 1) pull(p);
    }

    // 1点加算: a[p] += x
    // S に operator+ が定義されている場合に使用可能
    void add(int p, S x) {
        apply(p, [&](S a) {
            return a + x;
        });
    }

    // 両閉区間 [l, r] のクエリ
    // 空区間 [l, l-1] は M::e() を返す
    S query(int l, int r) const {
        assert(0 <= l && l <= n);
        assert(-1 <= r && r < n);
        assert(l <= r + 1);

        if (l > r) return M::e();

        l += size;
        r += size + 1;

        S left = M::e(), right = M::e();

        while (l < r) {
            if (l & 1) {
                left = M::op(left, d[l++]);
            }
            if (r & 1) {
                right = M::op(d[--r], right);
            }
            l >>= 1;
            r >>= 1;
        }

        return M::op(left, right);
    }

    // 全区間のクエリ
    S all_query() const {
        return d[1];
    }

    // [l, r) の集約値に対して f が成立する最大の r を返す
    // f(M::e()) == true が必要
    // f は区間を右に広げると true -> false となる単調な条件
    // 戻り値は条件が初めて崩れる要素の位置（崩れなければ n）
    template<class F>
    int max_right(int l, F f) const {
        assert(0 <= l && l <= n);
        assert(f(M::e()));
        if (l == n) return n;

        l += size;
        S sm = M::e();

        do {
            while ((l & 1) == 0) l >>= 1;

            if (!f(M::op(sm, d[l]))) {
                while (l < size) {
                    l *= 2;
                    if (f(M::op(sm, d[l]))) {
                        sm = M::op(sm, d[l]);
                        ++l;
                    }
                }
                return std::min(n, l - size);
            }

            sm = M::op(sm, d[l]);
            ++l;
        } while ((l & -l) != l);

        return n;
    }

    // [l, r) の集約値に対して f が成立する最小の l を返す
    // r は右端の次の位置（半開区間）
    // f(M::e()) == true が必要
    // 左に区間を広げると true -> false となる単調な条件
    template<class F>
    int min_left(int r, F f) const {
        assert(0 <= r && r <= n);
        assert(f(M::e()));
        if (r == 0) return 0;

        r += size;
        S sm = M::e();

        do {
            --r;
            while (r > 1 && (r & 1)) r >>= 1;

            if (!f(M::op(d[r], sm))) {
                while (r < size) {
                    r = r * 2 + 1;
                    if (f(M::op(d[r], sm))) {
                        sm = M::op(d[r], sm);
                        --r;
                    }
                }
                return std::max(0, r + 1 - size);
            }

            sm = M::op(d[r], sm);
        } while ((r & -r) != r);

        return 0;
    }
};

/*
    【使用方法】

    1. 宣言・初期化

    SegTree<Sum<ll>> seg(n);  // 要素数n、区間和
    SegTree<Min<ll>> seg(n);  // 要素数n、区間最小値
    SegTree<Max<ll>> seg(n);  // 要素数n、区間最大値

    vector<ll> a = {1, 2, 3, 4, 5};
    SegTree<Sum<ll>> seg(a);  // vectorから初期化

    要素数だけを指定した場合、各要素はM::e()で初期化される。
    Sumなら0、Minなら型の最大値、Maxなら型の最小値。


    2. 基本操作

    seg.update(p, x);       // a[p] = x（1点代入）
    seg.add(p, x);          // a[p] += x（1点加算）
    seg.get(p);             // a[p]を取得
    seg[p];                 // a[p]を取得（読み取り専用）
    seg.query(l, r);        // [l, r]の集約値
    seg.all_query();        // 全区間の集約値
    seg.length();           // 要素数を取得

    query(l, r)は両閉区間[l, r]を扱う。
    l == rなら1要素の取得、r == l-1なら空区間となる。
    空区間の場合はM::e()を返す。

    updateは代入、addは算術的な加算。
    MinやMaxでもaddは通常の加算を行う。
    addを使うには型Sのoperator+が必要。


    3. 任意の1点更新

    seg.apply(p, f);

    a[p]をf(a[p])に更新する。
    fはS型の値を受け取り、S型に変換可能な値を返す関数。

    // 例：位置pの値を2倍にする
    seg.apply(p, [](ll x) {
        return x * 2;
    });

    // 例：位置pの値を10以上にする
    seg.apply(p, [](ll x) {
        return max(x, 10LL);
    });

    // 例：位置pの値に7をXORする
    seg.apply(p, [](ll x) {
        return x ^ 7;
    });


    4. 区間の二分探索

    max_right(l, f):
        [l, r)の集約値がfを満たす最大のrを返す。
        条件が初めて崩れる要素の位置を返し、
        最後まで成立する場合はnを返す。

    min_left(r, f):
        [l, r)の集約値がfを満たす最小のlを返す。
        rは右端の次の位置であり、0 <= r <= n。

    これらはqueryと異なり、半開区間を基準にする。

    使用条件：
        f(M::e()) == true
        区間を広げるとtrueからfalseに変わる単調な条件
        fは同じ値に対して同じ結果を返すこと

    // 例：非負配列の区間和が初めてx以上になる位置
    // x > 0 とする
    int p = seg.max_right(l, [&](ll sum) {
        return sum < x;
    });
    // p == nなら該当する位置は存在しない

    // 例：右端r（含まない）から左方向に探索
    // 区間和がx以下である最も左の位置を求める
    // 非負配列かつx >= 0とする
    int p = seg.min_left(r, [&](ll sum) {
        return sum <= x;
    });

    ※負数がある区間和では、上記の単調性が
      成立しない場合があるので注意。


    5. 独自の演算を定義する

    以下の形式で構造体Mを定義する。

    struct Custom {
        using S = ll;

        static S op(S a, S b) {
            return ...;  // 区間情報の結合
        }

        static S e() {
            return ...;  // 単位元
        }
    };

    SegTree<Custom> seg(n);

    opには結合律が必要。
        op(op(a,b),c) == op(a,op(b,c))

    eは左右両方の単位元である必要がある。
        op(a,e()) == op(e(),a) == a

    交換律は不要。
    つまり、op(a,b)とop(b,a)が異なってもよい。
    行列積や関数合成などにも対応可能。


    6. 計算量（opがO(1)の場合）

    初期化             O(N)
    update             O(log N)
    add                O(log N)
    apply              O(log N)
    get                O(1)
    operator[]         O(1)
    query              O(log N)
    all_query          O(1)
    max_right          O(log N)
    min_left           O(log N)
    length             O(1)

    使用メモリ：O(N)


    7. 注意事項

    ・添字は0-indexed。
    ・queryは両閉区間[l, r]。
    ・max_rightとmin_leftは半開区間[l, r)を基準とする。
    ・seg[p] = x のような直接代入は不可。
      update(p, x)を使用する。
    ・区間更新には対応していない。
      区間更新が必要ならLazy Segment Treeを使う。
    ・Sumでは加算のオーバーフローに注意。
    ・Min/Maxの初期値は0ではなく単位元。
*/
