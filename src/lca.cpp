#define N 40011
#define K 16
struct X{
	int v, c;
};
X no(int v, int c) {
	X novo; novo.v = v; novo.c = c; return novo;
}
vector<X> adj[N];
int pai[N][K], distancia[N][K], prof[N], n,m;
void monta(int v, int p, int pro, int d) {
	prof[v] = pro;
	pai[v][0] = p;
	distancia[v][0] = d;
	for(int i = 1; i < K; i++) {
		pai[v][i] = pai[ pai[v][i-1] ] [i-1];
		distancia[v][i] = distancia[v][i-1] + distancia[ pai[v][i-1] ][i-1];
	}
	FOR(i, sz(adj[v])) {
		if(adj[v][i].v == p) continue;
		monta(adj[v][i].v, v, pro+1, adj[v][i].c);
	}
}
//monta pd com pais em distancia 1, 2, 4, 8 ...
void calc() {
	prof[0] = 0;
	FOR(i,K) {
		pai[0][i] = 0;
		distancia[0][i] = 0;
	}
	FOR(i, sz(adj[0]))
		monta(adj[0][i].v, 0, 1, adj[0][i].c);
}
int dist(int a, int b) {
	int pa = a, pb = b, d = 0;
	if(prof[pb] > prof[pa]) swap(pa, pb);
	//iguala niveis
	while(prof[pa] > prof[pb]) {
		int j = 0;
		FOR(i,K) {
			if(prof[ pai[pa][i] ] < prof[pb]) break;
			j = i;
		}
		d += distancia[pa][j];
		pa = pai[pa][j];
	}
	//vai subindo
	while(pa != pb) {
		int j = 0;
		FOR(i, K) {
			if(pai[pa][i] == pai[pb][i]) break;
			j = i;
		}
		d += distancia[pa][j];
		d += distancia[pb][j];
		pa = pai[pa][j];
		pb = pai[pb][j];
	}
	return d;
}
int main() {
	scanf("%d %d", &n, &m);
	FOR(i,n) adj[i].clear();
	int a, b, c; char d;
	FOR(i,m) {
		scanf("%d %d %d %c\n", &a, &b , &c, &d);
		a--; b--;
		adj[a].pb(no(b,c));
		adj[b].pb(no(a,c));
	}
	calc();
	int k; scanf("%d", &k);
	while(k--) {
		scanf("%d %d", &a, &b); a--; b--;
		printf("%d\n", dist(a,b));
	}
}
