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
const int INF = (int)1e9 + 7;

using namespace std;

const int MAXN = 101010;
int n, cnt = 0; vector<int> gph[MAXN], ls[MAXN]; bool chc[MAXN]; int ord[MAXN], rev[MAXN], par[MAXN]; vector<vector<int>> ear;
void dfs(int x, int p)
{
	par[x] = p;
	for(auto y : gph[x]) if(y != p) if(!ord[y]) dfs(y, x);
}
void ear_decomposition(void)
{
	dfs(0, 0); // 0-based
	for(int i = 0; i < n; ++i) rev[ord[i]] = i;
	for(int i = 0; i < n; ++i)
	{
		int x = rev[i];
		for(auto y : gph[x]) if(ord[y] > ord[x] && par[y] != x)
		{
			vector<int> path{x};
			while(!chc[y])
			{
				chc[y] = true;
				path.push_back(y);
				y = par[y];
			}
			path.push_back(y);
			ear.push_back(path);
		}
	}
}

mt19937 rnd(1557);
int rng(int l, int r) { return uniform_int_distribution<int>(l, r)(rnd); }
void gen_gph(int m)
{
	for(int i = 0; i < m; ++i)
	{
		int x = rng(0, n - 1);
		int y = rng(0, n - 1);
		gph[x].push_back(y);
		gph[y].push_back(x);
	}
}

int ncnt = 0;
int nord[MAXN];
bool flag = true;
int ndfs(int x, int p)
{
	int ret = nord[x] = ++ncnt;
	for(auto y : gph[x]) if(y != p)
	{
		if(!nord[y]) ret = min(ret, ndfs(y, x));
		else ret = min(ret, nord[y]);
	}
	if(p != -1 && ret == nord[x]) flag = false;
	return ret;
}

bool _2_con_check()
{
	ndfs(0, -1);
	if(!flag) return false;
	for(int i = 0; i < n; ++i) if(!nord[i]) return false;
	return true;
}

bool check()
{

}

void init()
{
	for(int i = 0; i < n; ++i)
	{
		gph[i].clear();
		chc[i] = false;
		ord[i] = 0;
		rev[i] = 0;
		par[i] = 0;
		ls[i].clear();
		nord[i] = 0;
	}
	ear.clear();
	cnt = 0;
	ncnt = 0;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	n = 100'000;
	int c = 0;
	while(c <= 1'000'000)
	{
		init();
		gen_gph(c += 1000);
		if(_2_con_check())
		{
			ear_decomposition();
			cout << "c = " << c << ": ";
			if(check())
			{
				cout << "correct" <<< endl;
			}
			else
			{
				cout << "error!" << endl;
				cout << "graph:" << endl;
				for(int i = 0; i < n; ++i)
				{
					cout << i << ": ";
					for(auto j : gph[i]) cout << j << ' ';
					cout << endl;
				}
				cout << "output:" << endl;
				for(auto i : ear)
				{
					for(auto j : i) cout << j << ' ';
					cout << endl;
				}
				return 0;
			}
		}
	}
}