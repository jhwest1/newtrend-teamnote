// len[v] = longest substring corresponding to v
// link[v] = suffix link of v
// chd[v] = children node of v in suffix automaton ** NOT SUFFIX TREE **
struct Node
{
    int len, link;
    vector<int> chd;
    Node()
    {
        len=0; link=-1;
        chd=vector<int>(26, -1);
    }
};

vector<Node> NS;
// last, root needs to be initialized
int last=-1, root=-1;

// Maximum size of NS is 2|S|
// Maximum number of edge is 3|S|
int newNode()
{
    NS.push_back(Node());
    return NS.size()-1;
}

// Insert c into suffix automaton
// last = last node corresponding to entire string before c
void SA_push(char c)
{
    int cur=newNode();
    NS[cur].len=NS[last].len+1;
    int p=last;
    for(; p!=-1 && NS[p].chd[c-'a']==-1; p=NS[p].link) NS[p].chd[c-'a']=cur;

    if(p==-1) NS[cur].link=0;
    else
    {
        int q=NS[p].chd[c-'a'];
        if(NS[p].len+1==NS[q].len) NS[cur].link=q;
        else
        {
            int clone=newNode();
            NS[clone].len=NS[p].len+1;
            NS[clone].link=NS[q].link;
            NS[clone].chd=NS[q].chd;
            for(; p!=-1 && NS[p].chd[c-'a']==q; p=NS[p].link) NS[p].chd[c-'a']=clone;
            NS[q].link=clone; NS[cur].link=clone;
        }
    }

    last=cur;
}

// Get suffix automaton of S
// root = root node of suffix automaton
void getSA(string S) // S is 1-based (leading "?")
{
    int N=S.size()-1;
    root=last=newNode();
    for(int i=1; i<=N; i++) SA_push(S[i]); // last is current node
}

// Get suffix tree of S
// Return array of nodes corresponding to each suffix
// pos[i] = leaf node of S[i...] in suffix tree of S
// root = root node of suffix tree
vector<int> getST(string S) // S is 1-based (leading "?")
{
    int N=S.size()-1;
    vector<int> pos(N+1);

    root=last=newNode();
    for(int i=N; i>=1; i--)
    {
        SA_push(S[i]);
        pos[i]=last;
    }
    return pos;
}