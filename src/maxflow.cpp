//OBSERVACAO: Sempre colocar as arestas de ida e volta.
using namespace std;
vector<int> grafo[MAX];
int cap[MAX][MAX];
int parnt[MAX];
int flow(int s, int t) {
    int f = 0;
    while(true) {
        memset(parnt,-1,sizeof(parnt));
        parnt[s] = -2;
        queue<int> q;
        q.push(s);
        while(!q.empty() && parnt[t] == -1) {
            int v = q.front(); q.pop();
            for(int i = 0, w; i < grafo[v].size(); i++) {
                if(parnt[w = grafo[v][i]] == -1 && cap[v][w] > 0){
                    parnt[w] = v;
                    q.push(w);
                }
            }
        }
        if(parnt[t] == -1) break;
        int bot = inf;
        for(int v = t, w = parnt[t]; w >= 0; v = w, w = parnt[w]) {
            bot = min(bot,cap[w][v]);
        }
        for(int v = t, w = parnt[t]; w >= 0; v = w, w = parnt[w]) {
            cap[w][v] -= bot;
            cap[v][w] += bot;
        }
        f += bot;
    }
    return f;
}
