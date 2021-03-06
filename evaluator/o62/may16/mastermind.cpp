#include <bits/stdc++.h>
#include "mastermind-lib.h"
#define pii pair<int, int>
#define x first
#define y second
#define all(x) x.begin(), x.end()
using namespace std;

const int N = 1e5+5;

// int gg[] = {0, 9, 7, 5, 4, 3, 2, 2};
int gg[] = {0, 9, 9, 6, 4, 3, 2, 2};


namespace gen_process {
  class solve_obj {
  private:
    int n, k, u;
    vector<string> ansVec, sVec;
    void rguess(string answer, string g, int& num_matched, int& num_misplaced) {
      num_matched = num_misplaced = 0;
      map<char,int> ma, mg;

      for (int i {0}; i < n; ++i)
      {
        if (answer[i] == g[i])
          ++num_matched;
        else
        {
          ++ma[answer[i]];
          ++mg[g[i]];
        }
      }
      for (char c {'1'}; c <= '0' + k; ++c)
        num_misplaced += min(ma[c], mg[c]);
    }
  public:
    solve_obj(int n, int k, int u) : n(n), k(k), u(u) {}

    bool mark[10];
    string str;

    void dfs_gen(int lv) {
      if(lv == n) {
        ansVec.emplace_back(str);
        return;
      }
      for(int i = 1; i <= k; ++i) {
        if(u && mark[i]) continue;
        mark[i] = true;
        str.push_back('0' + i);
        dfs_gen(lv+1);
        mark[i] = false;
        str.pop_back();
      }
    }

    void dfs_gen2(int lv) {
      if(lv == n) {
        sVec.emplace_back(str);
        return;
      }
      for(int i = 1; i <= k; ++i) {
        str.push_back('0' + i);
        dfs_gen2(lv+1);
        str.pop_back();
      }
    }

    void gen_ans() {
      dfs_gen(0);
      dfs_gen2(0);
    }

    map<pii, int> Query;

    void gen_query() {
      for(auto x : ansVec) for(auto y : sVec) {
        int a, b;
        rguess(x, y, a, b);
        Query[pii(a, b)] = -1;
      }
      int step = 0;
      for(auto &x : Query) x.y = step++;
    }

    vector<int> d_tree;
    bool get_dtree(vector<int> vec, int lv, int p = 1) {
      if(!lv) return false;
      if(vec.size() > ((1 << (lv+1)))) return false;
      d_tree.resize(max((int)d_tree.size(), p+1));
      vector<pii> vecN;
      for(int i = 0; i < sVec.size(); ++i) {
        map<pii, vector<int> > Mp;
        vecN.emplace_back(0, i);
        for(auto x : vec) {
          int a, b;
          rguess(ansVec[x], sVec[i], a, b);
          // cerr << ansVec[i] << ' ' << ansVec[x] << " => " << a << ' ' << b << endl;
          Mp[pii(a, b)].emplace_back(x);
        }
        int mx = 0;
        for(auto x : Mp) mx = max(mx, (int)x.y.size());
        vecN.emplace_back(mx, i);
      }
      sort(vecN.begin(), vecN.end());
      for(int z = 0; z < vecN.size(); ++z) {
        int i = vecN[z].y;
        d_tree[p] = i;
        map<pii, vector<int> > Mp;
        // cerr << "==> " << lv << endl;
        for(auto x : vec) {
          int a, b;
          rguess(ansVec[x], sVec[i], a, b);
          // cerr << ansVec[i] << ' ' << ansVec[x] << " => " << a << ' ' << b << endl;
          Mp[pii(a, b)].emplace_back(x);
        }
        vector<pair<pii, vector<int> > > vv;
        for(auto x : Mp) vv.emplace_back(x);
        sort(vv.begin(), vv.end(), [&](const pair<pii, vector<int> > &a, const pair<pii, vector<int> > &b) { return a.y.size() > b.y.size(); });
        bool is_status = true;
        for(auto x : vv) {
          // cerr << "----> "<< x.x.x << ' ' << x.x.y << ' ' << is_status << endl;
          // cerr << "this: " << a << endl;
          if(x.x.x == n) {
            // cerr << "get_into: " << x.y.size() << endl;
            continue;
          }
          if(x.y.size() == vec.size()) {
            is_status = false;
            break;
          }
          bool status = get_dtree(x.y, lv-1, p * (Query.size()) + Query[x.x]);
          // cerr << (status ? "Yes" : "No") << endl;
          if(status == false) {
            is_status = false;
            break;
          }
        }
          // cerr << (is_status ? "Yes!" : "No!") << endl;
        if(is_status) return true;
      }
      return false;
    }

    void get_ans() {
      vector<int> vec(ansVec.size());
      iota(all(vec), 0);
      bool ans = false;
      for(int i = 1; i < 50; ++i) {
        d_tree.clear();
        cerr << "level = " << i << endl;
        if(get_dtree(vec, i)) {
          ans = true;
          break;
        }
      }
      if(ans) cerr << "Have Answer! " << endl;
    }

    vector<int> vans;

    void dfsenc(vector<int> vec, int p = 1) {
      // if(!vec.size()) return;
      // cerr << "=> " << p << endl;
      // for(auto x : vec) cerr << x << endl;
      map<pii, vector<int> > Mp;
      for(auto x : vec) {
        int a, b; 
        rguess(ansVec[x], sVec[d_tree[p]], a, b);
        Mp[pii(a, b)].emplace_back(x);
      }
      vans.emplace_back(d_tree[p]);
      for(auto x : Mp) {
        int a, b;
        rguess(ansVec[x.y[0]], sVec[d_tree[p]], a, b);
        if(x.y.size() == 1 && a == n) continue;

        dfsenc(x.y, p * Query.size() + Query[x.x]);
      }
    }

    void encrypt_ans() {
      vector<int> vec(ansVec.size());
      iota(all(vec), 0);
      dfsenc(vec);
    }

    bool solve() {
      memset(mark, 0, sizeof mark);
      gen_ans();
      if(ansVec.size() == 0) {
        return false;
      }
      gen_query();
      cerr << "Size : " << ansVec.size() << endl;
      cerr << "Query size : " << Query.size() << endl;
      get_ans();
      encrypt_ans();
      cerr << "vAns : " << vans.size() << endl;
      printf("output_from_gen[%d][%d][%d] = {", n, k, u);
      for(auto x : vans) printf("%d, ", x);
      printf("0};\n");
      cerr << d_tree.size() << endl;
      // for(auto x : d_tree) cerr << x << ' ' ;
      cerr << endl;
      return true;
    }
  };

  void main() {
    char str[10];
    freopen("out.txt", "w", stdout);
    for(int i = 2; i <= 7; ++i) {
      for(int j = 2; j <= gg[i]; ++j) {
        for(int k = 0; k < 2; ++k) {
          cerr << "====================" << endl;
          cerr << i << ' ' << j << ' ' << k << endl;
          solve_obj* s = new solve_obj(i, j, k);
          s->solve();
          cerr << "finished" << endl;
        }
      }
    }
  }
}
// =======================================================================================================================
vector<int> output_from_gen[10][10][2];


namespace run_process {
  class solve_obj {
  private:
    int n, k, u, ptr;
    vector<int> dtree_ent;
    vector<string> ansVec, sVec;
    void rguess(string answer, string g, int& num_matched, int& num_misplaced) {
      num_matched = num_misplaced = 0;
      map<char,int> ma, mg;

      for (int i {0}; i < n; ++i)
      {
        if (answer[i] == g[i])
          ++num_matched;
        else
        {
          ++ma[answer[i]];
          ++mg[g[i]];
        }
      }
      for (char c {'1'}; c <= '0' + k; ++c)
        num_misplaced += min(ma[c], mg[c]);
    }
  public:
    solve_obj(int n, int k, int u, vector<int> v) : n(n), k(k), u(u), dtree_ent(v), ptr(0) {}

    bool mark[10];
    string str;

    void dfs_gen(int lv) {
      if(lv == n) {
        ansVec.emplace_back(str);
        return;
      }
      for(int i = 1; i <= k; ++i) {
        if(u && mark[i]) continue;
        mark[i] = true;
        str.push_back('0' + i);
        dfs_gen(lv+1);
        mark[i] = false;
        str.pop_back();
      }
    }

    void dfs_gen2(int lv) {
      if(lv == n) {
        sVec.emplace_back(str);
        return;
      }
      for(int i = 1; i <= k; ++i) {
        str.push_back('0' + i);
        dfs_gen2(lv+1);
        str.pop_back();
      }
    }

    void gen_ans() {
      dfs_gen(0);
      dfs_gen2(0);
    }

    map<pii, int> Query;

    void gen_query() {
      for(auto x : ansVec) for(auto y : sVec) {
        int a, b;
        rguess(x, y, a, b);
        Query[pii(a, b)] = -1;
      }
      int step = 0;
      for(auto &x : Query) x.y = step++;
    }

    vector<int> d_tree;

    void get_dtree(vector<int> vec, int p = 0, int pp = 1) {
      cerr << " -> " << p << ' ' << pp << endl;
      d_tree.resize(max((int)d_tree.size(), pp+1));
      d_tree[pp] = dtree_ent[p];
      map<pii, vector<int> > Mp;
      for(auto x : vec) {
        int a, b;
        rguess(ansVec[x], sVec[dtree_ent[p]], a, b);
        Mp[pii(a, b)].emplace_back(x);
      }
      vector<int> zidx;
      int cnt = ptr;
      for(auto x : Mp) {
        int a, b;
        rguess(ansVec[x.y[0]], sVec[dtree_ent[p]], a, b);
        if(x.y.size() == 1 && a == n) 
          continue;
        ptr++;
      }
      // cerr << cnt << endl;
      for(auto x : Mp) {
        int a, b;
        rguess(ansVec[x.y[0]], sVec[dtree_ent[p]], a, b);
        if(x.y.size() == 1 && a == n) {
          continue;
        } else get_dtree(x.y, ++cnt, pp * Query.size() + Query[x.x]);
      }
    }

    void get_dtree() {
      vector<int> vec(ansVec.size());
      iota(all(vec), 0);
      get_dtree(vec);
    }

    void pre_process() {
      memset(mark, 0, sizeof mark);
      gen_ans();
      gen_query();
      if(ansVec.size() == 0) return;
      get_dtree();
      cerr << "YES" << endl;
      for(auto x : d_tree) cerr << x << ' ';
      cerr << endl;
    }

    void traversal(int p = 1) {
      int a, b;
      #ifndef GEN
      guess(sVec[d_tree[p]], a, b);
      #endif
      if(a == n) return;
      traversal(p * Query.size() + Query[pii(a, b)]);
    }

    void solve() {
      traversal();
    }
  };
  solve_obj* d_tree[10][10][2];
  void init() {
    
    // gen file here !!!!!!
    output_from_gen[1][2][0] = {1, 0, 0};
output_from_gen[1][2][1] = {0, 1, 0};
output_from_gen[1][3][0] = {2, 0, 1, 0};
output_from_gen[1][3][1] = {1, 0, 2, 0};
output_from_gen[1][4][0] = {3, 1, 0, 2, 0};
output_from_gen[1][4][1] = {1, 2, 0, 3, 0};
output_from_gen[1][5][0] = {0, 2, 4, 1, 3, 0};
output_from_gen[1][5][1] = {3, 0, 1, 2, 4, 0};
output_from_gen[1][6][0] = {3, 0, 4, 2, 5, 1, 0};
output_from_gen[1][6][1] = {5, 1, 2, 3, 0, 4, 0};
output_from_gen[1][7][0] = {2, 3, 4, 6, 5, 1, 0, 0};
output_from_gen[1][7][1] = {4, 1, 0, 5, 2, 3, 6, 0};
output_from_gen[1][8][0] = {7, 0, 1, 3, 6, 4, 2, 5, 0};
output_from_gen[1][8][1] = {4, 6, 5, 7, 1, 3, 0, 2, 0};
output_from_gen[1][9][0] = {6, 8, 5, 2, 4, 1, 7, 3, 0, 0};
output_from_gen[1][9][1] = {3, 8, 7, 4, 6, 5, 0, 1, 2, 0};
output_from_gen[2][2][0] = {3, 0, 1, 2, 0};
output_from_gen[2][2][1] = {2, 1, 0};
output_from_gen[2][3][0] = {1, 8, 7, 5, 6, 3, 0, 7, 4, 2, 0};
output_from_gen[2][3][1] = {2, 5, 7, 3, 6, 1, 5, 0};
output_from_gen[2][4][0] = {7, 11, 0, 2, 3, 10, 8, 2, 14, 9, 12, 12, 1, 14, 13, 9, 1, 15, 3, 4, 6, 15, 5, 11, 0};
output_from_gen[2][4][1] = {15, 14, 6, 1, 4, 0, 9, 8, 4, 2, 6, 11, 9, 12, 13, 14, 0, 7, 3, 0};
output_from_gen[2][5][0] = {10, 15, 14, 6, 21, 22, 9, 24, 1, 23, 8, 21, 18, 19, 16, 22, 17, 1, 3, 4, 16, 7, 17, 2, 4, 21, 18, 12, 13, 11, 23, 5, 15, 20, 19, 0, 14, 0};
output_from_gen[2][5][1] = {7, 22, 5, 3, 15, 8, 4, 19, 18, 20, 23, 16, 4, 10, 14, 13, 5, 21, 1, 11, 13, 15, 9, 5, 2, 22, 17, 8, 0};
output_from_gen[2][6][0] = {13, 24, 30, 21, 23, 29, 33, 35, 17, 10, 3, 22, 34, 5, 4, 18, 17, 28, 29, 27, 30, 0, 29, 31, 4, 20, 2, 3, 6, 9, 11, 32, 10, 31, 26, 11, 8, 24, 14, 20, 33, 7, 31, 19, 18, 17, 15, 9, 16, 1, 1, 12, 25, 0};
output_from_gen[2][6][1] = {16, 32, 24, 8, 19, 9, 10, 3, 1, 11, 18, 6, 3, 11, 23, 5, 25, 19, 30, 33, 31, 27, 34, 25, 2, 8, 32, 20, 8, 12, 29, 24, 25, 26, 2, 33, 10, 22, 34, 19, 17, 15, 13, 12, 4, 0};
output_from_gen[2][7][0] = {13, 7, 17, 33, 39, 28, 40, 32, 5, 10, 30, 25, 37, 26, 23, 10, 37, 18, 19, 16, 39, 24, 31, 38, 34, 5, 3, 2, 4, 29, 0, 19, 21, 35, 14, 28, 26, 16, 7, 46, 29, 1, 42, 15, 44, 17, 36, 45, 17, 47, 22, 43, 14, 12, 39, 3, 48, 27, 34, 41, 45, 29, 11, 8, 10, 10, 6, 9, 19, 7, 20, 0};
output_from_gen[2][7][1] = {6, 43, 26, 46, 30, 18, 46, 10, 37, 17, 31, 39, 38, 18, 4, 23, 33, 31, 19, 25, 4, 3, 40, 9, 12, 10, 11, 37, 25, 29, 22, 15, 36, 47, 45, 46, 20, 7, 14, 28, 21, 35, 12, 23, 46, 45, 44, 43, 42, 18, 45, 35, 1, 5, 1, 41, 13, 27, 3, 0, 34, 2, 13, 4, 20, 0};
output_from_gen[2][8][0] = {11, 38, 55, 26, 21, 0, 40, 40, 5, 45, 24, 21, 16, 22, 5, 42, 2, 18, 48, 26, 61, 58, 56, 34, 42, 60, 7, 63, 47, 23, 28, 43, 18, 63, 48, 55, 50, 53, 32, 21, 60, 44, 20, 4, 52, 21, 58, 12, 16, 54, 6, 32, 36, 39, 62, 43, 34, 46, 55, 37, 22, 18, 4, 29, 39, 49, 57, 41, 31, 30, 60, 24, 33, 49, 28, 1, 15, 26, 17, 25, 34, 31, 62, 29, 32, 9, 8, 13, 14, 42, 54, 3, 51, 43, 59, 58, 27, 15, 36, 19, 12, 17, 35, 10, 0};
output_from_gen[2][8][1] = {55, 43, 25, 12, 20, 2, 16, 16, 34, 32, 24, 20, 4, 8, 39, 10, 12, 40, 33, 17, 1, 30, 18, 8, 37, 5, 13, 21, 60, 8, 26, 25, 24, 28, 29, 59, 19, 1, 44, 40, 41, 42, 11, 34, 3, 19, 35, 14, 8, 61, 20, 59, 58, 60, 56, 57, 24, 10, 12, 46, 38, 22, 6, 30, 62, 50, 49, 51, 37, 7, 47, 39, 31, 15, 23, 17, 0, 37, 51, 52, 53, 48, 49, 0};
output_from_gen[2][9][0] = {43, 33, 18, 10, 47, 80, 77, 53, 50, 68, 75, 17, 73, 46, 14, 0, 13, 75, 47, 74, 11, 80, 11, 5, 1, 8, 2, 59, 25, 8, 9, 72, 0, 17, 20, 19, 26, 45, 23, 73, 9, 30, 25, 59, 56, 56, 48, 21, 3, 54, 76, 12, 62, 21, 55, 75, 57, 26, 48, 56, 3, 15, 6, 60, 4, 28, 27, 32, 75, 51, 30, 12, 78, 29, 48, 24, 35, 41, 42, 55, 28, 2, 71, 63, 65, 66, 64, 69, 2, 64, 62, 31, 76, 58, 13, 22, 4, 49, 68, 67, 33, 23, 42, 2, 71, 16, 79, 70, 7, 79, 4, 37, 36, 40, 44, 46, 38, 52, 7, 41, 25, 4, 61, 39, 68, 42, 34, 0};
output_from_gen[2][9][1] = {58, 23, 11, 66, 9, 8, 72, 71, 27, 7, 79, 35, 34, 30, 7, 71, 63, 79, 3, 75, 69, 44, 5, 28, 1, 73, 64, 53, 36, 35, 16, 12, 9, 17, 62, 5, 79, 10, 29, 11, 65, 79, 66, 46, 48, 52, 45, 2, 74, 53, 47, 10, 80, 65, 34, 5, 32, 4, 21, 18, 25, 68, 74, 26, 77, 19, 14, 17, 21, 60, 7, 41, 36, 43, 63, 51, 6, 69, 28, 38, 33, 76, 24, 39, 4, 78, 37, 10, 44, 15, 42, 75, 45, 38, 9, 61, 55, 7, 13, 67, 22, 56, 1, 59, 4, 46, 54, 49, 15, 31, 62, 29, 76, 57, 0};
output_from_gen[3][2][0] = {2, 5, 7, 3, 4, 1, 3, 0, 6, 0};
output_from_gen[3][3][0] = {4, 26, 25, 18, 20, 17, 24, 11, 15, 9, 22, 0, 14, 6, 19, 8, 2, 11, 25, 23, 6, 14, 19, 16, 21, 14, 10, 12, 20, 13, 3, 7, 14, 1, 22, 5, 0};
output_from_gen[3][3][1] = {20, 22, 15, 7, 5, 21, 7, 11, 19, 0};
output_from_gen[3][4][0] = {43, 39, 0, 6, 56, 17, 16, 1, 25, 4, 22, 5, 20, 21, 60, 22, 39, 2, 18, 6, 54, 29, 53, 6, 49, 13, 21, 52, 12, 48, 56, 61, 28, 52, 32, 17, 62, 54, 30, 14, 50, 9, 63, 11, 44, 59, 23, 55, 31, 51, 45, 19, 32, 36, 44, 3, 15, 57, 7, 37, 5, 24, 33, 49, 8, 25, 51, 16, 38, 26, 24, 45, 10, 44, 31, 34, 57, 56, 15, 58, 46, 31, 1, 42, 40, 41, 50, 11, 35, 51, 39, 59, 19, 47, 27, 0};
output_from_gen[3][4][1] = {32, 46, 7, 53, 27, 57, 13, 54, 30, 51, 55, 9, 6, 14, 11, 54, 2, 28, 19, 45, 49, 39, 52, 30, 56, 51, 24, 50, 18, 26, 8, 35, 44, 9, 36, 33, 0};
output_from_gen[3][5][0] = {60, 80, 124, 47, 13, 119, 118, 123, 121, 19, 46, 49, 121, 43, 44, 41, 48, 116, 76, 30, 109, 34, 34, 93, 98, 21, 94, 99, 106, 31, 101, 33, 91, 96, 108, 31, 103, 84, 83, 81, 33, 51, 45, 122, 24, 99, 4, 104, 29, 92, 24, 117, 97, 21, 64, 1, 19, 101, 79, 41, 76, 16, 122, 42, 99, 3, 78, 18, 105, 29, 9, 106, 23, 103, 36, 107, 47, 114, 5, 26, 6, 28, 82, 42, 101, 32, 8, 28, 81, 47, 2, 102, 22, 51, 17, 27, 55, 77, 7, 21, 40, 88, 68, 122, 102, 90, 75, 70, 38, 58, 68, 89, 69, 113, 114, 56, 90, 100, 115, 43, 25, 40, 39, 80, 30, 59, 105, 65, 36, 86, 120, 56, 0, 74, 73, 82, 15, 66, 61, 120, 95, 5, 111, 45, 54, 20, 71, 8, 104, 72, 112, 6, 69, 87, 67, 117, 54, 37, 51, 53, 14, 57, 10, 53, 11, 13, 54, 12, 52, 80, 105, 62, 64, 59, 63, 61, 48, 26, 10, 50, 110, 70, 23, 35, 65, 88, 55, 85, 0};
output_from_gen[3][5][1] = {54, 23, 21, 29, 82, 42, 86, 79, 114, 40, 116, 80, 38, 55, 96, 16, 108, 105, 48, 8, 67, 76, 73, 105, 45, 21, 33, 101, 95, 11, 111, 103, 35, 23, 58, 85, 30, 113, 13, 53, 58, 47, 107, 116, 7, 115, 97, 98, 17, 117, 23, 110, 22, 114, 37, 76, 28, 7, 66, 58, 35, 84, 44, 56, 110, 19, 103, 40, 89, 77, 38, 27, 39, 76, 73, 84, 65, 9, 101, 32, 71, 55, 4, 70, 102, 14, 46, 53, 41, 79, 69, 59, 51, 29, 0};
output_from_gen[3][6][0] = {43, 176, 167, 0, 129, 26, 108, 3, 111, 48, 129, 21, 111, 18, 126, 23, 64, 36, 96, 123, 87, 84, 72, 135, 136, 106, 142, 180, 120, 132, 171, 112, 75, 183, 65, 105, 12, 93, 33, 130, 4, 94, 24, 28, 135, 185, 90, 15, 201, 27, 84, 113, 198, 77, 22, 133, 203, 131, 5, 200, 69, 169, 106, 16, 196, 76, 88, 96, 100, 29, 159, 77, 137, 120, 89, 17, 92, 124, 99, 125, 131, 146, 208, 204, 184, 209, 89, 207, 94, 192, 197, 128, 195, 95, 202, 198, 101, 196, 184, 107, 56, 122, 128, 159, 92, 86, 119, 14, 37, 160, 165, 171, 20, 141, 143, 9, 168, 212, 162, 138, 156, 35, 30, 33, 163, 189, 0, 74, 2, 215, 31, 110, 213, 172, 166, 130, 210, 144, 147, 148, 21, 34, 80, 194, 98, 173, 189, 160, 107, 214, 67, 192, 134, 156, 207, 149, 182, 202, 102, 142, 200, 34, 159, 105, 26, 167, 202, 161, 171, 106, 206, 191, 23, 170, 158, 146, 164, 94, 16, 174, 32, 83, 177, 140, 50, 178, 104, 159, 212, 179, 126, 84, 211, 177, 64, 154, 134, 155, 65, 169, 114, 70, 190, 61, 191, 71, 71, 205, 175, 64, 188, 152, 68, 62, 10, 184, 56, 103, 53, 193, 154, 97, 157, 83, 52, 82, 71, 80, 50, 85, 199, 119, 40, 38, 40, 8, 10, 189, 41, 152, 184, 51, 63, 11, 10, 81, 153, 69, 184, 13, 25, 193, 1, 73, 145, 31, 189, 181, 50, 9, 39, 167, 92, 108, 6, 36, 48, 78, 13, 49, 186, 116, 100, 66, 60, 121, 96, 139, 118, 133, 15, 150, 88, 56, 91, 145, 119, 58, 199, 48, 163, 59, 3, 143, 57, 117, 74, 19, 109, 69, 202, 114, 127, 54, 122, 13, 148, 47, 46, 42, 201, 176, 61, 151, 67, 187, 37, 7, 112, 79, 119, 55, 45, 33, 160, 44, 49, 115, 0};
output_from_gen[3][6][1] = {5, 82, 6, 192, 147, 134, 164, 159, 142, 157, 56, 51, 135, 163, 63, 121, 133, 42, 157, 62, 179, 158, 91, 116, 4, 99, 43, 124, 58, 153, 182, 104, 52, 97, 152, 192, 118, 168, 81, 94, 147, 197, 64, 103, 68, 188, 193, 67, 176, 78, 196, 200, 206, 190, 84, 140, 106, 48, 206, 199, 205, 61, 139, 70, 35, 32, 65, 114, 202, 54, 60, 46, 158, 90, 120, 96, 142, 132, 35, 12, 189, 195, 21, 63, 176, 175, 10, 105, 69, 173, 57, 156, 150, 207, 162, 177, 97, 111, 198, 138, 102, 186, 192, 178, 66, 174, 75, 204, 102, 173, 199, 68, 111, 15, 75, 110, 8, 38, 11, 39, 9, 103, 20, 13, 73, 109, 19, 141, 187, 147, 16, 76, 40, 10, 22, 112, 6, 76, 125, 95, 92, 191, 147, 53, 2, 59, 146, 145, 51, 26, 85, 119, 83, 27, 25, 15, 66, 155, 65, 33, 101, 20, 137, 167, 161, 170, 17, 123, 181, 183, 128, 31, 33, 30, 184, 34, 179, 182, 32, 163, 38, 17, 77, 148, 11, 29, 193, 113, 41, 15, 149, 23, 0};
output_from_gen[4][2][0] = {9, 6, 11, 4, 2, 1, 14, 7, 1, 15, 0, 3, 12, 10, 5, 11, 12, 1, 8, 13, 0};
output_from_gen[4][3][0] = {51, 56, 13, 67, 73, 4, 10, 1, 2, 45, 68, 34, 14, 58, 5, 10, 56, 64, 38, 55, 11, 74, 59, 65, 62, 40, 0, 48, 34, 9, 54, 32, 37, 28, 3, 46, 12, 41, 31, 40, 71, 20, 8, 74, 62, 70, 29, 37, 57, 19, 77, 68, 7, 63, 71, 34, 66, 76, 33, 22, 38, 70, 67, 32, 16, 41, 57, 73, 61, 22, 17, 23, 53, 42, 18, 9, 6, 27, 30, 22, 36, 39, 19, 43, 49, 80, 62, 53, 21, 46, 61, 50, 15, 72, 42, 34, 44, 79, 26, 60, 45, 63, 75, 35, 25, 69, 75, 47, 47, 34, 11, 78, 24, 42, 33, 45, 52, 48, 27, 53, 45, 0};
output_from_gen[4][4][0] = {107, 0, 42, 179, 204, 192, 252, 240, 128, 45, 179, 87, 4, 20, 16, 48, 126, 60, 21, 10, 17, 18, 5, 1, 12, 60, 2, 252, 149, 23, 130, 129, 90, 133, 145, 94, 6, 18, 22, 176, 213, 148, 132, 144, 48, 197, 197, 29, 209, 20, 194, 193, 14, 13, 53, 242, 49, 50, 74, 176, 221, 245, 123, 210, 28, 212, 208, 10, 176, 52, 144, 196, 140, 204, 173, 242, 118, 241, 62, 61, 174, 205, 206, 188, 253, 143, 244, 4, 254, 220, 10, 217, 189, 181, 157, 141, 180, 148, 177, 156, 205, 214, 246, 222, 247, 141, 150, 35, 190, 210, 223, 178, 146, 117, 30, 198, 54, 221, 134, 142, 51, 158, 182, 32, 207, 85, 255, 83, 207, 69, 99, 35, 84, 63, 110, 243, 81, 141, 149, 195, 68, 184, 65, 15, 169, 51, 80, 59, 64, 3, 8, 20, 191, 229, 138, 200, 179, 143, 162, 190, 131, 125, 46, 113, 152, 77, 66, 135, 57, 236, 248, 223, 247, 225, 254, 136, 10, 124, 77, 117, 93, 152, 199, 200, 5, 160, 34, 211, 224, 223, 153, 70, 33, 9, 112, 82, 76, 177, 50, 215, 86, 140, 31, 44, 93, 56, 55, 173, 195, 116, 15, 19, 7, 255, 25, 92, 37, 99, 24, 121, 36, 23, 50, 219, 165, 153, 65, 159, 231, 152, 164, 151, 237, 182, 217, 229, 137, 235, 135, 172, 220, 201, 228, 216, 132, 154, 166, 123, 14, 161, 174, 238, 94, 137, 183, 249, 31, 26, 184, 202, 225, 78, 128, 45, 147, 87, 132, 250, 186, 236, 26, 38, 126, 118, 142, 57, 226, 46, 68, 58, 114, 11, 122, 173, 230, 246, 185, 218, 48, 28, 170, 101, 89, 239, 73, 87, 95, 142, 71, 203, 79, 108, 251, 40, 119, 42, 232, 127, 168, 23, 97, 88, 100, 212, 47, 77, 227, 72, 83, 245, 11, 67, 200, 115, 39, 96, 59, 40, 35, 19, 79, 135, 234, 169, 102, 87, 232, 74, 182, 109, 121, 108, 120, 202, 175, 187, 90, 99, 209, 139, 41, 231, 98, 216, 163, 219, 39, 27, 7, 102, 233, 122, 110, 155, 167, 116, 210, 171, 235, 43, 106, 189, 105, 111, 9, 99, 91, 75, 243, 104, 123, 103, 0};
output_from_gen[4][4][1] = {97, 10, 238, 180, 156, 159, 216, 135, 100, 171, 30, 27, 187, 198, 54, 254, 62, 111, 147, 75, 39, 234, 141, 47, 120, 57, 60, 201, 228, 20, 78, 210, 146, 212, 45, 108, 94, 177, 114, 102, 225, 99, 0};
output_from_gen[5][2][0] = {21, 10, 29, 2, 8, 20, 11, 4, 26, 14, 0, 0, 13, 1, 30, 27, 15, 12, 3, 18, 22, 9, 24, 6, 10, 31, 5, 16, 3, 4, 1, 17, 28, 7, 16, 13, 22, 29, 19, 25, 28, 30, 24, 5, 17, 23, 20, 29, 0};
output_from_gen[5][3][0] = {123, 109, 239, 236, 103, 74, 16, 182, 20, 3, 218, 45, 164, 56, 2, 1, 130, 8, 217, 221, 54, 163, 55, 40, 167, 59, 5, 66, 181, 44, 235, 185, 19, 168, 73, 23, 4, 77, 238, 217, 22, 214, 76, 184, 42, 166, 58, 220, 178, 0, 242, 240, 3, 8, 36, 162, 100, 54, 18, 96, 62, 11, 170, 26, 133, 216, 66, 180, 72, 211, 51, 224, 80, 188, 180, 241, 234, 191, 82, 79, 21, 77, 25, 75, 75, 10, 158, 230, 7, 187, 57, 237, 9, 61, 183, 157, 82, 230, 103, 119, 223, 11, 166, 155, 165, 112, 65, 101, 149, 169, 219, 108, 212, 28, 93, 227, 83, 58, 173, 47, 137, 173, 29, 209, 198, 157, 81, 113, 104, 140, 85, 86, 47, 13, 31, 1, 68, 154, 50, 144, 32, 14, 100, 136, 22, 229, 211, 99, 194, 176, 109, 64, 46, 116, 226, 208, 207, 172, 190, 53, 225, 139, 175, 114, 67, 103, 193, 49, 219, 100, 215, 161, 9, 24, 92, 168, 27, 6, 233, 81, 157, 186, 78, 240, 95, 60, 168, 222, 102, 143, 232, 35, 71, 197, 35, 111, 179, 74, 214, 53, 143, 129, 17, 104, 63, 225, 189, 160, 41, 135, 218, 171, 132, 89, 207, 45, 30, 107, 58, 153, 12, 141, 99, 84, 225, 19, 112, 130, 131, 232, 152, 34, 88, 142, 52, 106, 199, 94, 40, 109, 188, 146, 48, 110, 142, 56, 102, 16, 231, 128, 196, 178, 148, 202, 130, 60, 91, 37, 148, 203, 149, 150, 95, 192, 38, 210, 92, 178, 138, 231, 156, 70, 68, 174, 200, 146, 66, 228, 207, 95, 233, 113, 208, 41, 145, 9, 127, 199, 5, 25, 231, 28, 159, 213, 121, 55, 203, 134, 206, 152, 215, 90, 113, 36, 108, 220, 69, 141, 174, 105, 177, 189, 51, 195, 43, 87, 60, 15, 33, 117, 137, 205, 133, 44, 151, 98, 116, 122, 109, 143, 39, 93, 111, 18, 118, 205, 144, 198, 126, 97, 9, 129, 201, 41, 147, 119, 79, 115, 43, 224, 186, 120, 117, 124, 58, 96, 42, 132, 84, 125, 114, 210, 150, 204, 0};
output_from_gen[6][2][0] = {46, 17, 47, 16, 1, 37, 8, 19, 25, 48, 21, 49, 0, 28, 26, 33, 3, 3, 48, 38, 9, 51, 5, 18, 22, 24, 20, 52, 27, 21, 61, 51, 57, 13, 23, 29, 53, 7, 24, 32, 8, 21, 2, 4, 32, 31, 24, 55, 12, 59, 61, 52, 11, 58, 12, 7, 13, 26, 24, 22, 28, 56, 12, 35, 37, 57, 37, 50, 52, 41, 7, 40, 63, 37, 10, 38, 12, 34, 36, 6, 11, 38, 60, 54, 30, 55, 45, 39, 58, 40, 15, 43, 28, 47, 43, 38, 42, 62, 7, 44, 14, 0};
output_from_gen[7][2][0] = {121, 6, 18, 4, 2, 39, 99, 76, 22, 14, 70, 91, 38, 7, 0, 2, 100, 126, 5, 50, 12, 20, 33, 68, 36, 25, 69, 34, 66, 60, 3, 77, 18, 10, 25, 102, 30, 57, 71, 39, 95, 117, 46, 54, 116, 78, 86, 88, 10, 23, 15, 30, 47, 74, 16, 64, 65, 75, 32, 8, 1, 107, 126, 21, 25, 37, 84, 52, 28, 122, 93, 37, 97, 13, 69, 28, 98, 19, 100, 53, 76, 44, 34, 66, 26, 82, 50, 10, 79, 76, 35, 98, 67, 27, 41, 74, 42, 11, 69, 62, 106, 55, 31, 24, 16, 47, 118, 94, 110, 43, 87, 33, 79, 103, 115, 28, 80, 114, 9, 40, 72, 24, 105, 112, 17, 59, 80, 48, 79, 37, 96, 33, 65, 65, 41, 126, 63, 59, 60, 58, 53, 19, 111, 95, 119, 104, 35, 29, 10, 53, 27, 51, 59, 52, 92, 116, 74, 45, 114, 90, 43, 52, 106, 108, 15, 12, 103, 83, 99, 22, 101, 85, 109, 75, 77, 26, 97, 127, 89, 48, 63, 104, 41, 66, 49, 112, 115, 73, 81, 65, 56, 15, 88, 25, 58, 37, 93, 17, 109, 117, 9, 20, 115, 124, 13, 40, 91, 107, 61, 126, 59, 122, 0, 76, 123, 125, 104, 71, 57, 36, 89, 113, 82, 105, 120, 0};

    for(int i = 2; i <= 2; ++i) {
      for(int j = 3; j <= gg[i] && j <= 3; ++j) {
        for(int k = 0; k < 1; ++k) {
          d_tree[i][j][k] = new solve_obj(i, j, k, output_from_gen[i][j][k]);
          d_tree[i][j][k]->pre_process();
        }
      }
    }
  }
}

#ifdef GEN

int main() {
  gen_process::main();
  // run_process::main();
  return 0;
}

#endif

bool start_init;

void find_answer(int n, int k, int u) {
  if(!start_init) run_process::init();
  #ifndef GEN
  // guess("1", n, k);
  #endif
  run_process::d_tree[n][k][u]->solve();
}

