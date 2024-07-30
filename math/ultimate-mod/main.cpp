typedef unsigned long long ull;
const ull MOD = 0xffff'ffff'0000'0001;
struct mint
{
	ull x;
	mint(void) : x(0) {}
	mint(ull x) : x(x) {}
	mint operator+ (ull ot) const { return x >= MOD - ot ? x - MOD + ot : x + ot; }
	mint operator- (ull ot) const { return x >= ot ? x - ot : x - ot + MOD; }
	mint operator- (void) const { return x ? MOD - x : 0; }
	mint operator* (ull ot) const
	{
		unsigned __int128 p = (unsigned __int128)x * ot;
		ull a = (ull)p;
		ull b = (ui)(p >> 64);
		ull c = p >> 96;
		return mint(a) - b - c + (b << 32);
	}
	mint operator+= (ull ot) { return *this = *this + ot; }
	mint operator-= (ull ot) { return *this = *this - ot; }
	mint operator*= (ull ot) { return *this = *this * ot; }
	operator unsigned long long(void) const { return x; }
};