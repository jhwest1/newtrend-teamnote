// Get length of lexicographically smallest suffix for all prefix
// S is 0-based
// lyndon(S = "ababca") = [1, 2, 1, 2, 5, 1]
vector<int> lyndon(string &S)
{
    int N=S.size();
    vector<int> dp(N);
    for(int i=0; i<N; )
    {
        dp[i]=1;
        int j=i+1, k=i;
        for(; j<N; j++)
        {
            if(S[j]==S[k])
            {
                dp[j]=dp[k];
                k++;
            }
            else if(S[j]>S[k])
            {
                dp[j]=j-i+1;
                k=i;
            }
            else break;
        }
        for(; i<=k; i+=j-k);
    }
    return dp;
}