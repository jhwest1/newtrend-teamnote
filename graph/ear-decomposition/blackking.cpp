// no loop, 2-vertex-connected: only first ear is cycle, 2-edge-connected: every ear can be cycle.
const int MAXN = 101010;
int n, cnt = 0; vector<int> gph[MAXN], ls[MAXN]; bool chc[MAXN]; int ord[MAXN], rev[MAXN], par[MAXN]; vector<vector<int>> ear;
void init() {
	for(int i = 0; i <= n; ++i) gph[i].clear(), chc[i] = false, ord[i] = 0, rev[i] = 0, par[i] = 0, ls[i].clear();
	ear.clear();
	cnt = 0;
}
void dfs(int x, int p) {
	par[x] = p;
	ord[x] = ++cnt;
	for(auto y : gph[x]) if(!ord[y]) dfs(y, x);
}
void ear_decomposition(void) {
	dfs(0, 0); // 0-based
	for(int i = 0; i < n; ++i) rev[ord[i]] = i;
	for(int i = 1; i <= n; ++i) {
		int x = rev[i];
		for(auto y : gph[x]) if(ord[y] > ord[x] && par[y] != x) {
			vector<int> path{x};
			chc[x] = true;
			while(!chc[y]) {
				chc[y] = true;
				path.push_back(y);
				y = par[y];
			}
			path.push_back(y);
			ear.push_back(path);
		}
	}
}