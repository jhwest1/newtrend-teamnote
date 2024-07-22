#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
namespace suffix_automaton {
	const int CH = 26;
	struct node {
		int link, len, pos;
		vector<int> chd;
		bool flag;

		node() {
			link = len = pos = 0;
			flag = false;
			chd = vector<int>(CH);
		}
	};
	vector<node> nd;

	int new_node() {
		nd.push_back(node());
		return (int)nd.size() - 1;
	}
	void build(string S) {
		new_node(); 
		nd[0].link = -1;
		int pos = 0;
		for (int i = 0; i < S.size(); i++) {
			int c = S[i] - 'a';
			int x = new_node();
			nd[x].len = nd[pos].len + 1;
			nd[x].pos = i;
			nd[x].flag = true;

			for (int v = pos; v != -1; v = nd[v].link) {
				if (!nd[v].chd[c]) nd[v].chd[c] = x;
				else {
					int y = nd[v].chd[c];
					if (nd[y].len == nd[v].len + 1) nd[x].link = y;
					else {
						int z = new_node();
						nd[z].len = nd[v].len + 1;
						nd[z].pos = nd[y].pos;
						nd[z].link = nd[y].link;
						nd[z].chd = nd[y].chd;
						nd[y].link = nd[x].link = z;

						for (; v != -1 && nd[v].chd[c] == y; v = nd[v].link) {
							nd[v].chd[c] = z;
						}
					}
					break;
				}
			}
			pos = x;
		}
	}
	vector<vector<pii>> g;
	void suffix_tree(string S) {
		reverse(S.begin(), S.end());
		build(S);
		int n = nd.size();
		g.resize(n);
		for (int v = 1; v < n; v++) {
			int x = nd[v].link;
			int c = S[nd[v].pos - nd[x].len] - 'a';
			g[x].push_back({c, v});
		}
		for (int v = 0; v < n; v++) sort(g[v].begin(), g[v].end());
	}
	void dfs(int v, vector<int> &sa) {
		if (nd[v].flag) sa.push_back(nd[v].pos);
		for (auto [c, x] : g[v]) {
			dfs(x, sa);
		}
	}
	void suffix_array(string S, vector<int> &sa, vector<int> &lcp) {
		suffix_tree(S);
		dfs(0, sa);
		int n = S.size();
		for (int i = 0; i < n; i++) sa[i] = n - 1 - sa[i];
		lcp.resize(n);
		vector<int> r(n);
		for (int i = 0; i < n; i++) r[sa[i]] = i;
		for (int i = 0, p = 0; i < n; i++) if (r[i]) {
			int j = sa[r[i] - 1];
			while ((i + p < n ? S[i + p] : -1) == (j + p < n ? S[j + p] : -2)) ++p;
			lcp[r[i]] = p;
			p = max(p - 1, 0);
		}
	}
};

// BOJ 9248
int main() {
	string S;
	cin >> S;
	vector<int> sa, lcp;
	suffix_automaton::suffix_array(S, sa, lcp);
	int n = S.size();
	for (int i = 0; i < n; i++) cout << sa[i] + 1 << ' ';
	cout << '\n';
	cout << "x ";
	for (int i = 1; i < n; i++) cout << lcp[i] << ' ';
	cout << '\n';
}