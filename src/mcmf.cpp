using namespace std;
#define MAX 256
typedef long long ll;
const ll inf = (1LL << 60);

int grau[MAX];
int pai[MAX];
int adj[MAX][MAX];
ll dist[MAX];
int N,M;
int queue[2][MAX];
int lbl[2][MAX];
int qf[2];

struct aresta{
    ll v;
    ll cst;
    ll cap;
    ll flow;
};

aresta arestas[MAX*MAX*2];

struct flow{
    ll cost;
    ll flow;
};

bool belman(int S,int T) {
    for(int i = 0; i < N; i++) {
        dist[i] = inf;
        lbl[i] = 0;
        pai[i] = -1;
    }

    qf[0] = qf[1] = 0;
    queue[0][qf[0]++] = S;
    pai[S] = -2;
    dist[S] = 0;

    for(int k = 0; k < N; k++) {
        fila_at = k&1;
        fila_prox = 1 - fila_at;

        qf[fila_prox] = 0;

        for(int i = 0; i < qf[fila_at]; i++) {
            int v = queue[fila_at][i];
            lbl[fila_at][v] = 0;
            for(int j = 0; j < grau[v]; j++) {
                aresta e = arestas[adj[v][j]];
                aresta ei = arestas[adj[v][j] ^ 1];
                int w = e.v;
                ll cap = e.cap - e.flow;
                ll cst = e.cst;
                if(ei.flow) cst = -ei.cst;

                if(cap > 0 && dist[w] > dist[v] + cst) {
                    if(!lbl[fila_prox][w]){
                        queue[fila_prox][qf[fila_prox]++] = w;
                        lbl[fila_prox][w] = 1;
                    }
                    dist[w] = dist[v] + cst;
                    pai[w] = adj[v][j];
                }
            }
        }
    }
    return pai[T] != -1;
}

flow mcmf(int S, int T) {
    flow f = (flow) { 0 , 0 };
    while(belman(S, T)) {
        ll bot = inf;
        ll cst = 0;

        for(int v = T; pai[v] >= 0; v = arestas[pai[v] ^ 1].v) {
            bot = min (bot , arestas[pai[v]].cap - arestas[pai[v]].flow);	
        }

        for(int v = T; pai[v] >= 0; v = arestas[pai[v] ^ 1].v) {
            aresta &e = arestas[pai[v]];
            aresta &ei = arestas[pai[v] ^ 1];
            if(ei.flow) {
                cst += bot*(-ei.cst);
            } else {
                cst += bot*e.cst;
            }

            e.flow += bot;
            ei.flow -= bot;

        }
        f.flow += bot;
        f.cost += cst;
    }
    return f;
}
