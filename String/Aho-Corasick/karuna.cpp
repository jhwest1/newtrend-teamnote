#include <bits/stdc++.h>
using namespace std;

namespace aho_corasick {
	const int SZ = 26;

	struct node {
		int fail, link, par, ch;
		vector<int> chd;
		vector<int> nxt;

		node() {
			fail = link = par = ch = -1;
			chd = vector<int>(SZ, -1);
			nxt = vector<int>(SZ, -1);
		}
	};
	vector<node> nd;
	int new_node() { 
		nd.push_back(node());
		return (int)nd.size() - 1;
	}
	void init() { new_node(); }
	void insert(string S) {
		int v = 0;
		for (int i = 0; i < S.size(); i++) {
			int c = S[i] - 'a';
			if (nd[v].chd[c] == -1) {
				int x = new_node();
				nd[v].chd[c] = x;
				nd[x].par = v;
				nd[x].ch = c;
			}
			v = nd[v].chd[c];
		}
		nd[v].link = v;
	}
	
	int get_fail(int v);
	int get_next(int v, int c);

	int get_fail(int v) {
		if (nd[v].fail != -1) return nd[v].fail;
		return nd[v].fail = (v == 0 || nd[v].par == 0) ? 0 : get_next(get_fail(nd[v].par), nd[v].ch);
	}
	int get_next(int v, int c) {
		if (nd[v].nxt[c] != -1) return nd[v].nxt[c];
		if (nd[v].chd[c] != -1) return nd[v].nxt[c] = nd[v].chd[c];
		return nd[v].nxt[c] = (v == 0) ? 0 : get_next(get_fail(v), c);
	}
	void calc() {
		queue<int> Q;
		Q.push(0);
		while (!Q.empty()) {
			int v = Q.front(); Q.pop();
			for (int c = 0; c < SZ; c++) {
				int x = nd[v].chd[c];
				if (x != -1) {
					Q.push(x);
					if (nd[x].link == -1) nd[x].link = nd[get_fail(x)].link;
				}
			}
		}
	}
};

// BOJ 9250
int main() {
	int n;
	cin >> n;
	aho_corasick::init();
	for (int i = 0; i < n; i++) {
		string S; cin >> S;
		aho_corasick::insert(S);
	}
	aho_corasick::calc();
	int q;
	cin >> q;
	for (int i = 0; i < q; i++) {
		string S; cin >> S;
		int v = 0;
		bool flag = false;
		for (int j = 0; j < S.size(); j++) {
			int c = S[j] - 'a';
			v = aho_corasick::get_next(v, c);
			if (aho_corasick::nd[v].link != -1) flag = true;
		}
		cout << (flag ? "YES\n" : "NO\n");
	}
}