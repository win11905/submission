#include <bits/stdc++.h>
#define complex complex<double>
#define all(x) (x).begin(), (x).end()
#define long long long
#define LL long
#define pii pair<int, int>
#define x first
#define y second
using namespace std;
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};

const int N = 1<<16;
const long G = 3;
//const long M = 1e9+7;
const long M = 998244353;

class POLYMUL {
private:
    int n, rev[N];
    long power(long a, long r) {
        long ret = 1;
        for(; r; r >>= 1, a = a * a % M) if(r & 1) ret = ret * a % M;
        return ret;
    }
    void NTT(long A[], bool inv = false) {
        for(int i = 0; i < N; ++i) if(i < rev[i]) swap(A[i], A[rev[i]]);
        for(int i = 1; i < N; i <<= 1) {
            long omega = power(G, (M-1) / i >> 1);
            if(inv) omega = power(omega, M-2);
            for(int j = 0; j < n; j += i << 1) {
                long w = 1, u, v;
                int l = j, r = i+j;
                for(int k = 0; k < i; ++k, ++l, ++r, w = w * omega % M) {
                    u = A[l], v = A[r] * w % M;
                    A[l] = (u + v) % M;
                    A[r] = (u - v + M) % M;
                }
            }
        }
        if(inv) {
            long z = power(n, M-2);
            for(int i = 0; i < n; ++i, ++A) *A = *A * z % M;
        }
    }
    /*
    struct complex {
        double x, y;
        complex() : x(0), y(0) {}
        complex(double x, double y) : x(x), y(y) {}
        friend complex operator+(const complex &a, const complex &b) {
            return complex(a.x+b.x, a.y+b.y);
        }
        friend complex operator-(const complex &a, const complex &b) {
            return complex(a.x-b.x, a.y-b.y);
        }
        friend complex operator*(const complex &a, const complex &b) {
            return complex(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
        }
    };
     */
    void FFT(complex A[], bool inv = false) {
        double pi = acos(-1.0);
        for(int i = 0; i < N; ++i) if(i < rev[i]) swap(A[i], A[rev[i]]);
        for(int i = 1; i < N; i <<= 1) {
            complex omega(cos(2*pi / (i<<1)), (inv ? -1 : 1) * sin(2*pi / (i<<1)));
            for(int j = 0; j < N; j += i << 1) {
                complex w(1, 0), u, v;
                int l = j, r = i+j;
                for(int k = 0; k < i; ++k, ++l, ++r, w = w * omega) {
                    u = A[l], v = A[r] * w;
                    A[l] = u + v;
                    A[r] = u - v;
                }
            }
        }
        if(inv) for(int i = 0; i < N; ++i) A[i] /= N;
    }
    complex A[N], B[N];
public:
    void solve(istream& cin, ostream& cout) {
        for(int i = 0; i < N; ++i) rev[i] = rev[i>>1]>>1 | ((i&1) * (N>>1));
        cin >> n;
        for(int i = n, val; ~i; --i) cin >> val, A[i] = complex(val, 0.0);
        for(int i = n, val; ~i; --i) cin >> val, B[i] = complex(val, 0.0);
        FFT(A), FFT(B);
        for(int i = 0; i < N; ++i) A[i] = A[i] * B[i];
        FFT(A, true), FFT(B, true);
        for(int i = n<<1; ~i; --i) cout << (long)(A[i].real()+0.5) << ' ';
        cout << endl;
    }
};