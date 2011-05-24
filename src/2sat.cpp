#define N 4096
int n,m;
vector<pii> pares, portas;
vector<int> adj[N], inv[N];
int scc[N], pos[N], outro[N], cnt;
int nao(int x) { return outro[x]; }
int sim(int x) { return x; }
void insereAmbas(int a, int b) {
	adj[a].pb(b); inv[b].pb(a);
}
void insere(int a, int b) {
	insereAmbas( nao(a), sim(b) );
	insereAmbas( nao(b), sim(a) );
}
void dfsInv(int v) {
	if(scc[v]) return; scc[v] = 1;
	FOR(i, sz(inv[v])) dfsInv( inv[v][i] );
	pos[cnt++] = v;
}
void pinta(int v, int c) {
	if(scc[v] != -1) return;
	scc[v] = c;
	FOR(i ,sz(adj[v])) pinta(adj[v][i], c );
}
int raiz_componente[N], qtd_componentes, valor[N];
void valora(int v, int val) {
	if(valor[v] != -1) return;
	valor[v] = val;
	valora( outro[v], 1 - val );
	FOR(i, sz(adj[v])) {
		if(scc[adj[v][i]] == scc[v]) 
			valora( adj[v][i], val );
	}
}
int ok(int qtd) {
	FOR(i, 2*n) adj[i].clear(); inv[i].clear();	
	FOR(i, qtd) insere( portas[i].first, portas[i].second );
	cnt = 0;
	memset(scc, 0, sizeof(scc));
	FOR(i, 2*n) dfsInv(i);
	memset(scc, -1, sizeof(scc));
	int cor = 0;
	qtd_componentes = 0;
	for(int i = cnt-1; i>= 0; i--) {
		if(scc[ pos[i] ] == -1) {
			pinta( pos[i] ,cor++);
			raiz_componente[ qtd_componentes++ ] = pos[i];
		}
	}
	FOR(i, n) {
		if( scc[ pares[i].first ] == scc[ pares[i].second ] ) return 0;
	}
	//valoracao
	memset(valor, -1, sizeof(valor));
	for(int i = 0; i < qtd_componentes; i++) {
		int raiz = raiz_componente[i];
		if(valor[ raiz ] == -1) {
			valora( raiz, 1 );
		}
	}
	return 1;
}

int main() {
	while(scanf("%d %d", &n, &m) == 2 && (n || m)) {
		pares.clear();
		FOR(i, n) {
			pii aux;
			scanf("%d %d", &aux.first, &aux.second);
			pares.pb(aux);
			outro[aux.first] = aux.second;
			outro[aux.second] = aux.first;
		}
		portas.clear();
		FOR(i, m) {
			pii aux;
			scanf("%d %d", &aux.first, &aux.second);
			portas.pb(aux);
		}
		int ini = 0, fim = m, meio;
		while(ini + 3 < fim) {
			meio = (ini+fim) / 2;
			if(ok(meio)) ini = meio;
			else fim = meio;
		}
		for(int i = fim; i >= ini; i--) {
			if(ok(i)) {
				printf("%d\n", i);
				break;
			}
		}
	}
    return 0;
}
