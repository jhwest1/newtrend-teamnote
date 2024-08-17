#include <bits/stdc++.h>
#define pii pair<int, int>
#define pll pair<long long, long long>
#define piii pair<int, pii>
#define plll pair<long long, pll>
#define tiii array<int, 3>
#define tiiii array<int, 4>
#define ff first
#define ss second
#define ee ss.ff
#define rr ss.ss
typedef long long ll;
typedef long double ld;
using namespace std;


// no loop, 2-vertex-connected: only first ear is cycle, 2-edge-connected: every ear can be cycle.
// call init(|V|, |E|) first, add_edge(x, y), then ear_decomposition()
// vear: vertex list of ear / ear : edge list of ear
// path: vear = {1, 2, 3}, ear = {{1, 2}, {2, 3}}
// cycle: vear = {1, 2, 3, 1}, ear = {{1, 2}, {2, 3}, {3, 1}}
const int SV = 101010, SE = 1010101;
int n, cnt = 0, ord[SV], rev[SV]; bool chc[SE]; pii par[SV]; vector<pii> gph[SV]; vector<vector<int>> vear, ear;
void init(int _n, int _m) {
	n = _n;
	for(int i = 0; i <= _n; ++i) gph[i].clear(), ord[i] = 0, rev[i] = 0, par[i] = {0, 0};
	for(int i = 0; i <= _m; ++i) chc[i] = false;
	vear.clear(); ear.clear();
	cnt = 0;
}
void add_edge(int x, int y, int e) {
	gph[x].push_back({y, e});
	gph[y].push_back({x, e});
}
void dfs(int x, int p, int pe) {
	par[x] = {p, pe};
	ord[x] = ++cnt;
	for(auto [y, e] : gph[x]) if(!ord[y]) dfs(y, x, e);
}
void ear_decomposition() {
	dfs(1, 0, 0); // 1-based
	for(int i = 1; i <= n; ++i) rev[ord[i]] = i;
	for(int i = 1; i <= n; ++i) {
		int x = rev[i];
		for(auto [y, e] : gph[x]) if(ord[y] > ord[x] && par[y].ff != x) {
			vector<int> V{x, y};
			vector<int> E{e};
			chc[e] = true;
			while(!chc[par[y].ss] && y != x) {
				chc[par[y].ss] = true;
				V.push_back(par[y].ff);
				E.push_back(par[y].ss);
				y = par[y].ff;
			}
			vear.push_back(V);
			ear.push_back(E);
		}
	}
}


// checker

mt19937 rnd(1557);
int rng(int l, int r) { return uniform_int_distribution<int>(l, r)(rnd); }
pii M[SE];
void gen_gph(int m)
{
	for(int i = 1; i <= m; ++i)
	{
		int x = rng(1, n);
		int y = rng(1, n);
		if(x == y) {--i; continue;}
		add_edge(x, y, i);
		M[i] = {x, y};
	}
}

// int ncnt = 0;
// int nord[SV];
// bool flag = true;
// int ndfs(int x, int p)
// {
// 	int ret = nord[x] = ++ncnt;
// 	int ch = 0;
// 	for(auto [y, e] : gph[x])
// 	{
// 		if(!nord[y])
// 		{
// 			int t = ndfs(y, x);
// 			ret = min(ret, t);
// 			if(p != -1 && t == nord[x]) flag = false;
// 			++ch;
// 		}
// 		else ret = min(ret, nord[y]);
// 	}
// 	if(p == -1 && ch > 1) flag = false;
// 	return ret;
// }

// bool _2_con_check()
// {
// 	flag = true;
// 	ncnt = 0;
// 	for(int i = 1; i <= n; ++i) nord[i] = false;
// 	ndfs(1, -1);

// 	if(!flag) return false;
// 	for(int i = 1; i <= n; ++i) if(!nord[i]) return false;
// 	return true;
// }

// bool used[SV];
// bool check()
// {
// 	for(int i = 1; i <= n; ++i) used[i] = false;
// 	bool flag = false;
// 	for(int i = 0; i < (int)vear.size(); ++i)
// 	{
// 		auto &v = vear[i], &e = ear[i];
// 		if(flag && (!used[v.front()] || !used[v.back()])) return false;
// 		if(flag && v.front() == v.back()) return false;
// 		flag = true;
// 		for(int i = 0; i < (int)v.size(); ++i)
// 		{
// 			if(i != 0 && i != (int)v.size() - 1 && used[v[i]]) return false;
// 			used[v[i]] = true;
// 			if(i != 0)
// 			{
// 				if(M[e[i - 1]] != pii{v[i - 1], v[i]} && M[e[i - 1]] != pii{v[i], v[i - 1]}) return false;
// 			}
// 		}
// 	}
// 	return true;
// }

int ncnt = 0;
int nord[SV];
bool flag = true;
int ndfs(int x, int p)
{
	int ret = nord[x] = ++ncnt;
	for(auto [y, e] : gph[x])
	{
		if(!nord[y])
		{
			int t = ndfs(y, x);
			ret = min(ret, t);
		}
		else ret = min(ret, nord[y]);
	}
	if(p != -1 && ret == nord[x]) flag = false;
	return ret;
}

bool _2_con_check()
{
	flag = true;
	ncnt = 0;
	for(int i = 1; i <= n; ++i) nord[i] = false;
	ndfs(1, -1);

	if(!flag) return false;
	for(int i = 1; i <= n; ++i) if(!nord[i]) return false;
	return true;
}

bool used[SV];
bool check()
{
	for(int i = 1; i <= n; ++i) used[i] = false;
	bool flag = false;
	for(int i = 0; i < (int)vear.size(); ++i)
	{
		auto &v = vear[i], &e = ear[i];
		if(flag && (!used[v.front()] || !used[v.back()])) return false;
		flag = true;
		for(int i = 0; i < (int)v.size(); ++i)
		{
			if(i != 0 && i != (int)v.size() - 1 && used[v[i]]) return false;
			used[v[i]] = true;
			if(i != 0)
			{
				if(M[e[i - 1]] != pii{v[i - 1], v[i]} && M[e[i - 1]] != pii{v[i], v[i - 1]}) return false;
			}
		}
	}
	return true;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	n = 10000;
	int c = 0;
	while(c <= 1'000'000)
	{
		init(n, c += 1000);
		gen_gph(c);
		if(_2_con_check())
		{
			ear_decomposition();
			cout << "c = " << c << ": ";
			if(check())
			{
				cout << "correct" << endl;
			}
			else
			{
				cout << "error!" << endl;
				cout << "graph:" << endl;
				for(int i = 1; i <= c; ++i) cout << M[i].ff << ' ' << M[i].ss << endl;
				cout << "output:" << endl;
				for(int i = 0; i < (int)vear.size(); ++i)
				{
					cout << "("; for(auto j : vear[i]) cout << j << ' '; cout << ") ";
					cout << "("; for(auto j : ear[i]) cout << j << ' '; cout << ")" << endl;
					cout << endl;
				}
				return 0;
			}
		}
	}
}