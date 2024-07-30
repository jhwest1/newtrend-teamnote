struct fastio {
	char buf[1 << 25]; // size > input size
	int idx;
	fastio() : idx(0) {
		fread(buf, sizeof(buf), 1, stdin);
	}
	inline int read() {
	  int x = 0, s = 1;
	  int c = buf[idx++];
	  while (c <= 32) c = buf[idx++];
	  if (c == '-') s = -1, c = buf[idx++];
	  while (c > 32) x = (x << 3) + (x << 1) + (c - '0'), c = buf[idx++];
	  if (s < 0) x = -x;
	  return x;
	}
}fi;