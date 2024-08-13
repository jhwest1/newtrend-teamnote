// no loop, 2-vertex-connected: only first ear is cycle, 2-edge-connected: every ear can be cycle.
const int SV = 101010;
int n, cnt = 0; vector<int> gph[SV], ls[SV]; bool chc[SV]; int ord[SV], rev[SV], par[SV]; vector<vector<int>> ear;
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
void ear_decomposition() {
	dfs(1, 1); // 1-based
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