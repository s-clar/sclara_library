#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef std::pair<long long, long long> P;
typedef std::priority_queue<P, std::vector<P>, std::greater<P>> PQ;
typedef std::complex<double> cd;

struct P3 {
    long long first, second, third;
};

struct P3P {
    P first, second, third;
};

struct compP3{
    bool operator()(const P3 &p1,const P3 &p2) const {
        if (p1.first != p2.first) return p1.first < p2.first;
        if (p1.second != p2.second) return p1.second < p2.second;
        else return p1.third < p2.third;
    }
};

struct gcompP3{
    bool operator()(const P3 &p1,const P3 &p2) const {
        if (p1.first != p2.first) return p1.first > p2.first;
        if (p1.second != p2.second) return p1.second > p2.second;
        else return p1.third > p2.third;
    }
};

const double PI = acos(-1.0);

bool ckran(int a, int n) {
    return (a >= 0 && a < n);
}

void yn (bool f) {
    if (f) std::cout << "Yes" << '\n';
    else std::cout << "No" << '\n';
}

P pplus(P a, P b) {
    return {a.first + b.first, a.second + b.second};
}

long long pminus(P a, P b) {
    return {a.first - b.first, a.second - b.second};
}

long long ptime(P a, P b) {
    return {a.first * b.first, a.second * b.second};
}

long long pdiv(P a, P b) {
    if (b.first == 0) {
        b.first = 1;
    }
    if (b.second == 0) {
        b.second = 1;
    }
    return {a.first / b.first, a.second / b.second};
}

template<typename T, typename U>
bool chmax(T& a, U b) {
    if (a < b) {
        a = b;
        return true;
    } else {
        return false;
    }
}

template<typename T, typename U>
bool chmin(T& a, U b) {
    if (a > b) {
        a = b;
        return true;
    } else {
        return false;
    }
}

template <class T>
void outspace_one(const T& x) {
    cout << x;
}

template <class T>
void outspace_one(const vector<T>& v) {
    for (int i = 0; i < (int)v.size(); ++i) {
        if (i) cout << ' ';
        outspace_one(v[i]);
    }
}

template <class T, class U>
void outspace_one(const pair<T, U>& p) {
    outspace_one(p.first);
    cout << ' ';
    outspace_one(p.second);
}

template <class T, class... Ts>
void outspace(const T& first, const Ts&... rest) {
    outspace_one(first);
    ((cout << ' ', outspace_one(rest)), ...);
    cout << '\n';
}

template<typename T>
void outendl(std::vector<T> a) {
    int n = a.size();
    for (int i = 0; i < n; ++i) {
        std::cout << a[i] << '\n';
    }
}

void outdouble(long double a) {
    std::cout << std::fixed << std::setprecision(10) << a << std::endl;
}

std::vector<long long> lltovec(long long n, long long base = 10, long long minsize = -1) {
    std::vector<long long> res;
    while (minsize-- > 0 || n > 0) {
        res.push_back(n % base);
        n /= base;
    }
    // std::reverse(res.begin(), res.end());
    return res;
}

long long vectoll(std::vector<long long> vec, long long base = 10) {
    long long res = 0;
    std::reverse(vec.begin(), vec.end());
    for (auto i : vec) {
        res *= base;
        res += i;
    }
    std::reverse(vec.begin(), vec.end());
    return res;
}

static const long long MOD = 998244353;
static const int MAXN = 1000000;  // 必要に応じて大きく設定

// 階乗・階乗逆元の配列
static long long fact[MAXN+1], invFact[MAXN+1];

// 繰り返し二乗法 (a^b mod M)
long long modpow(long long a, long long b, long long M) {
    long long ret = 1 % M;
    a %= M;
    while (b > 0) {
        if (b & 1) ret = (ret * a) % M;
        a = (a * a) % M;
        b >>= 1;
    }
    return ret;
}

// 前処理: 階乗と逆元のテーブルを作る
void initFactorials() {
    // 階乗テーブル
    fact[0] = 1;
    for (int i = 1; i <= MAXN; i++) {
        fact[i] = fact[i-1] * i % MOD;
    }
    // 階乗逆元テーブル
    invFact[MAXN] = modpow(fact[MAXN], MOD - 2, MOD);  // フェルマーの小定理で逆元を計算
    for (int i = MAXN; i >= 1; i--) {
        invFact[i-1] = invFact[i] * i % MOD;
    }
}

// 組み合わせ数 C(n, r)
long long comb(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invFact[r] % MOD * invFact[n - r] % MOD;
}

vector<pair<long long, long long>> rlell(vector<long long> a) {
    vector<pair<long long, long long>> res;
    if (a.empty()) return res;
    res.push_back({a[0], 1});
    long long n = a.size();
    for (int i = 1; i < n; ++i) {
        if (a[i] == a[i - 1]) {
            res.back().second++;
        } else {
            res.push_back({a[i], 1});
        }
    }
    return res;
}

vector<pair<char, long long>> rlest(string a) {
    vector<pair<char, long long>> res;
    if (a.empty()) return res;
    res.push_back({a[0], 1});
    long long n = a.size();
    for (int i = 1; i < n; ++i) {
        if (a[i] == a[i - 1]) {
            res.back().second++;
        } else {
            res.push_back({a[i], 1});
        }
    }
    return res;
}

vector<pair<long long, long long>> prime_factorize(long long n) {
    vector<pair<long long, long long>> res;

    for (long long p = 2; p <= n / p; ++p) {
        if (n % p != 0) continue;

        long long exponent = 0;

        while (n % p == 0) {
            n /= p;
            ++exponent;
        }

        res.emplace_back(p, exponent);
    }

    // 最後に素数が1つ残っている場合
    if (n > 1) {
        res.emplace_back(n, 1);
    }

    return res;
}

template <class T>
void vecin(T& x) {
    cin >> x;
}

template <class T>
void vecin(vector<T>& v) {
    for (auto& x : v) vecin(x);
}

    // std::vector<bool> prime(3001000, true);
    // prime[0] = false;
    // prime[1] = false;
    // std::vector<long long> p;
    // for (long long i = 2; i < 2000; ++i) {
    //     if (prime[i] == true) {
    //         p.push_back(i);
    //         for (long long j = i * 2; j < 3001000; j += i) {
    //             if (j % i == 0) prime[j] = false;
    //         }
    //     }
    // }
    // for (long long i = 2000; i < 3001000; i += 2) {
    //     if (prime[i] == true) {
    //         p.push_back(i);
    //     }
    // }

// vector<int> cl = {-1, 0, 1, 0, -1};

// vector<ll> cl = {-1, 0, 1, 1, 1, 0, -1, -1, -1, 0};

int main()
{
    ll n, m;
    cin >> n >> m;
    vector<ll> a(n), b(n - 1);
    vecin(a);
    vecin(b);
    ll ans = 1e8;
    ll cur = 0;
    auto c = a;
    for (int i = 0; i < n - 1; ++i) {
        if ((a[i] + a[i + 1]) % 2 != b[i]) {
            a[i + 1]++;
            cur++;
        }
    }
    chmin(ans, cur);
    cur = 1;
    a = c;
    a[0] ^= 1;
    for (int i = 0; i < n - 1; ++i) {
        if ((a[i] + a[i + 1]) % 2 != b[i]) {
            a[i + 1]++;
            cur++;
        }
    }
    chmin(ans, cur);
    cout << ans << endl;
}
