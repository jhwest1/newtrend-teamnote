namespace SA_LCP
{
    // uncomment code to use cmp

    // MAXN must be defined
    const int MAXN = 5e5;

    // SA[i] : starting position of i-th suffix when sorted
    // R[i] : lexicographic order of i-th suffix, R[SA[i]]=i
    // LCP[i] : LCP(S[SA[i-1]...], S[SA[i]...])
    int SA[MAXN+10], R[MAXN+10], LCP[MAXN+10];

    // R2[i][j] : R[i] when d = 1<<j
    // int R2[MAXN+10][21];

    // Get Suffix array, LCP array of S
    // Fill SA[i], R[i], LCP[i]
    // S is 1-based (leading "?")
    // getSA("?banana") =>
    //  SA = [-, 6, 4, 2, 1, 5, 3]
    //   R = [-, 4, 3, 6, 2, 5, 1]
    // LCP = [-, -, 1, 3, 0, 0, 2]
    void SA_LCP(string S)
    {
        int N=S.size()-1, t=1;
        vector<int> F(N+1), T(N+1);

        for(int i=1; i<=N; i++) SA[i]=i;
        sort(SA+1, SA+N+1, [&](const int &p, const int &q) { return S[p]<S[q]; });
        for(int i=1; i<=N; i++) R[SA[i]]=R[SA[i-1]]+(i==1 || S[SA[i]]!=S[SA[i-1]]);
        // for(int i=1; i<=N; i++) R2[i][0]=R[i];

        for(int d=1; d<=N; d<<=1, t++)
        {
            if(R[SA[N]]==N) break;

            F[0]=d;
            for(int i=d+1; i<=N; i++) F[R[i]]++; 
            for(int i=1; i<=N; i++) F[i]+=F[i-1];
            for(int i=1; i<=N; i++) SA[F[(i+d>N ? 0 : R[i+d])]--]=i;
            for(int i=0; i<=N; i++) F[i]=0;

            for(int i=1; i<=N; i++) F[R[i]]++; 
            for(int i=1; i<=N; i++) F[i]+=F[i-1];
            for(int i=1; i<=N; i++) T[i]=SA[i];
            for(int i=N; i>=1; i--) SA[F[R[T[i]]]--]=T[i];
            for(int i=0; i<=N; i++) F[i]=0;

            for(int i=1; i<=N; i++) T[i]=R[i];
            for(int i=1; i<=N; i++)
            {
                R[SA[i]]=R[SA[i-1]];
                if(i==1 || T[SA[i]]!=T[SA[i-1]]) R[SA[i]]++;
                else
                {
                    int p=(SA[i-1]+d>N ? 0 : T[SA[i-1]+d]);
                    int q=(SA[i]+d>N ? 0 : T[SA[i]+d]);
                    if(p!=q) R[SA[i]]++;
                }
            }
            // for(int i=1; i<=N; i++) R2[i][t]=R[i];
        }
        // for(; t<=20; t++) for(int i=1; i<=N; i++) R2[i][t]=R[i];

        for(int i=1, k=0; i<=N; i++)
        {
            if(R[i]>1)
            {
                for(; i+k<=N && SA[R[i]-1]+k<=N && S[i+k]==S[SA[R[i]-1]+k]; k++);
                LCP[R[i]]=k;
            }
            if(k) k--;
        }
    }

    // Compare two substrings, return true if S[l1 ... r1] < S[l2 ... r2]
    bool cmp(int l1, int r1, int l2, int r2)
    {
        if(l2>r2) return false;
        if(l1>r1) return true;
        int len=min(r1-l1+1, r2-l2+1), t=__lg(len);
        if(R2[l1][t]!=R2[l2][t]) return R2[l1][t] < R2[l2][t];
        if(R2[l1+len-(1<<t)][t]!=R2[l2+len-(1<<t)][t]) return R2[l1+len-(1<<t)][t] < R2[l2+len-(1<<t)][t];
        return (r1-l1) < (r2-l2);
    }
}