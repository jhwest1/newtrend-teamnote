// shuffle the list before running the algorithm
// O(n) randomized

pt getCenter(pt a, pt b){ return pt((a.x+b.x)/2, (a.y+b.y)/2); } //두 점의 중점
pt getCenter(pt a, pt b, pt c){ //세 점의 외접원의 중심
    pt aa = b - a, bb = c - a;
    auto c1 = aa*aa * 0.5, c2 = bb*bb * 0.5;
    auto d = aa / bb;
    auto x = a.x + (c1 * bb.y - c2 * aa.y) / d;
    auto y = a.y + (c2 * aa.x - c1 * bb.x) / d;
    return pt(x, y);
}

Circle solve(vector<pt> v){
    pt p = {0, 0};
    double r = 0; int n = v.size();
    for(int i=0; i<n; i++) if(dst(p, v[i]) > r){ //break point 1
        p = v[i]; r = 0;
        for(int j=0; j<i; j++) if(dst(p, v[j]) > r){ //break point 2
            p = getCenter(v[i], v[j]); r = dst(p, v[i]);
            for(int k=0; k<j; k++) if(dst(p, v[k]) > r){ //break point 3
                p = getCenter(v[i], v[j], v[k]);
                r = dst(v[k], p);
            }
        }
    }
    return {p, r};
}