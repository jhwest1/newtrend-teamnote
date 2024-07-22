namespace Z
{
    // Get Z function of S
    // Z[i] = LCP(S[i...], S)
    // S is 1-based (leading "?")
    // getZ(S = "?abacaba") = [-, 0, 0, 1, 0, 3, 0, 1]
    vector<int> getZ(string S)
    {
        int N=S.size()-1;
        vector<int> Z(N+1);

        int pos=1;
        for(int i=2; i<=N; i++)
        {
            if(i<=pos+Z[pos]-1) Z[i]=min(pos+Z[pos]-i, Z[i-pos+1]);
            while(i+Z[i]<=N && S[i+Z[i]]==S[Z[i]+1]) Z[i]++;
            if(Z[pos]+pos<Z[i]+i) pos=i;
        }
        return Z;
    }

    // Get LCP of all suffixes in S with T
    // F[i] = LCP(S[i...], T)
    // S, T is 1-based (leading "?")
    // matchZ(S = "?abacabacaba", T = "?abacaba") = [-, 7, 0, 1, 0, 7, 0, 1, 0, 3, 0, 1]
    vector<int> matchZ(string S, string T)
    {
        int N=S.size()-1, M=T.size()-1;
        vector<int> Z = getZ(T);
        vector<int> F(N+1);

        int pos=0;
        for(int i=1; i<=N; i++)
        {
            if(i<=pos+F[pos]-1) F[i]=min(pos+F[pos]-i, Z[i-pos+1]);
            while(i+F[i]<=N && F[i]+1<=M && S[i+F[i]]==T[F[i]+1]) F[i]++;
            if(pos+F[pos]<i+F[i]) pos=i;
        }
        return F;
    }
}