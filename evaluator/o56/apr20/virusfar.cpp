#include <bits/stdc++.h>
#define all(x) (x).begin(), (x).end()
#define vi vector<int>
#define iii tuple<int, int, int>
#define long long long
#define pii pair<int, int>
#define x first
#define y second
using namespace std;
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};

const int N = 1e6+5;
const int M = 5e2+5;

class virusfar {
private:
    int n, m;
    char A[N], B[M];
    int dp[M];
    vector<int> vec[26];
public:
    void solve(istream& cin, ostream& cout) {
        cin >> n >> m;
        cin >> A;
        for(int i = 0; i < n; ++i) vec[A[i] - 'a'].emplace_back(i);
        while(m--) {
            cin >> B;
            int len = strlen(B);
            fill_n(dp, M, INF);
            for(int i = 0; i < len; ++i) {
                int now = B[i] - 'a';
                for(int j = len; j > 1; --j) if(dp[j-1] != INF) {
                    auto it = upper_bound(all(vec[now]), dp[j-1]);
                    if(it != vec[now].end()) dp[j] = min(dp[j], *it);
                }
                if(!vec[now].empty()) dp[1] = min(dp[1], vec[now][0]);
            }
            int ans;
            for(int j = 1; j <= len; ++j) if(dp[j] != INF) ans = j;
            cout << ans << endl;
        }
    }
};