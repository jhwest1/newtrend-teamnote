#include <bits/stdc++.h>
using namespace std;

namespace aho_corasick {
	const int SZ = 26;

	struct node {
		int fail, link;
		vector<int> chd;

		node() {
			fail = link = 0;
			chd = vector<int>(SZ);
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
			if (!nd[v].chd[c]) {
				nd[v].chd[c] = new_node();
			}
			v = nd[v].chd[c];
		}
		nd[v].link = v;
	}
	void calc() {
		queue<int> Q;
		for (int c = 0; c < SZ; c++) {
			if (nd[0].chd[c]) Q.push(nd[0].chd[c]);
		}
		while (!Q.empty()) {
			int v = Q.front(); Q.pop();
			for (int c = 0; c < SZ; c++) {
				int x = nd[v].chd[c];
				if (x) {
					int f = nd[v].fail;
					while (f > 0 && !nd[f].chd[c]) f = nd[f].fail;
					if (nd[f].chd[c]) {
						f = nd[f].chd[c];
						nd[x].fail = f;
						if (!nd[x].link) nd[x].link = nd[f].link;
					}
					Q.push(x);
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
			while (v > 0 && !aho_corasick::nd[v].chd[c]) {
				v = aho_corasick::nd[v].fail;
			}
			v = aho_corasick::nd[v].chd[c];
			if (aho_corasick::nd[v].link) flag = true;
		}
		cout << (flag ? "YES\n" : "NO\n");
	}
}