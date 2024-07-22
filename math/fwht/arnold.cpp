void DFT(vector<ll> &A, bool inv)
{
    int N=A.size();
    for(int i=1; i<N; i<<=1)
    {
        for(int j=0; j<N; j+=i+i)
        {
            for(int k=0; k<i; k++)
            {
                ll a=A[j+k], b=A[j+i+k];
                A[j+k]=a+b;
                A[j+i+k]=a-b;
                if(inv) A[j+k]/=2, A[j+i+k]/=2;
            }
        }
    }
}