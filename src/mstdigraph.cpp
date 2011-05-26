using namespace std;
#define NMAX 1024
vector< pair<int,int> > adj[NMAX], grafo[NMAX];
int n,m;
int ja[NMAX];
int peso[NMAX][NMAX];
int fora[NMAX];
void dfs(int v) {
	if(ja[v]) return;
	ja[v] = 1;
	for(int i = 0; i < (int) grafo[v].size(); i++) dfs(grafo[v][i].first);
}
int consegue() {
	memset(ja, 0, sizeof(ja));
	dfs(0);
	for(int i = 0; i < n; i++) if(!ja[i]) return 0;
	return 1;
}
pair<int,int> arco[NMAX];
int pai[NMAX];
int reconstroi_ciclo(int v, int fim, vector<int> & c) {
	c.clear();
	int x = v;
	while(x != fim) {
		c.push_back(x);
		x = pai[x];
	}
	c.push_back(fim);
	return 1;
}
int ciclo(int v, vector<int> & c) {
	ja[v] = 1;
	for(int i = 0; i < (int) adj[v].size(); i++) {
		int viz = adj[v][i].first;
		if(viz == v) continue;
		if(ja[viz] == 1) {
			return reconstroi_ciclo(v,viz,c);
		} else if(!ja[viz]) {
			pai[viz] = v;
			if(ciclo(viz, c)) return 1;
		}
	}
	ja[v] = 2;
	return 0;
}
int tem_ciclo(vector<int> & c) {
	memset(ja, 0, sizeof(ja));
	for(int i = 0; i < n; i++) {
		if(!ja[i] && ciclo(i,c)) {
			return 1;
		}
	}
	return 0;
}
int no_ciclo[NMAX];
void contrai(vector<int> c) {
	reverse(c.begin(), c.end());
	memset(no_ciclo, 0, sizeof(no_ciclo));
	int ciclo_custo = 0;
	int id_contraido = 0;
	for(int i = 0; i < (int)c.size(); i++) {
		ciclo_custo += arco[ c[i] ].first;
		no_ciclo[c[i]] = 1;
		if(c[i]) {
			id_contraido = c[i];
		}
	}
	/* arruma as arestas que saem do vertice contraido */
	for(int i = 0; i < (int) c.size(); i++) {
		int v = c[i];
		if(v != id_contraido) {
			for(int j = 0; j < (int) grafo[v].size(); j++) {
				if(!no_ciclo[grafo[v][j].first]) {
					grafo[id_contraido].push_back( grafo[v][j] );
					if( arco[ grafo[v][j].first ].second == v ) {
						arco[ grafo[v][j].first ].second = id_contraido;
					}
				}
			}
		} else {
			for(int j = (int) grafo[v].size() - 1; j >= 0; j--) {
				if(no_ciclo[ grafo[v][j].first ]) {
					swap( grafo[v][j], grafo[v][ (int) grafo[v].size() - 1]);
					grafo[v].pop_back();
				}
			}
		}
	}
	pair<int,int> novo_arco = make_pair( (1<<29) , -1);
	/* arruma as arestas que entram no vertice contraido */
	for(int i = 0; i < n; i++) {
		if(!no_ciclo[i]) {
			for(int j = (int) grafo[i].size() - 1; j >= 0; j--) {
				if(no_ciclo[ grafo[i][j].first ]) {
					int novo_custo = ciclo_custo - arco[ grafo[i][j].first ].first + grafo[i][j].second;
					grafo[i][j].second = novo_custo;
					grafo[i][j].first = id_contraido;
					novo_arco = min(novo_arco, make_pair(novo_custo, i));
				}
			}
		}
	}
	arco[id_contraido] = novo_arco;
	for(int i = 0; i < (int)c.size(); i++) {
		if(c[i] != id_contraido) {
			fora[c[i]] = 1;
			grafo[c[i]].clear();
		}
	}
}

int calcula() {
	memset(fora, 0, sizeof(fora));
	for(int i = 0; i < n; i++) {
		arco[i] = make_pair( (1<<29) , -1);
	}
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < (int) grafo[i].size(); j++) {
			int viz = grafo[i][j].first;
			int cst = grafo[i][j].second;
			if(viz) {
				arco[viz] = min(arco[viz], make_pair(cst,i));
			}
		}
	}
	while(1) {
		for(int i = 0; i < n; i++) {
			adj[i].clear();
		}
		for(int i = 1; i < n; i++) {
			if(fora[i] || fora[arco[i].second])continue;
			adj[ arco[i].second ].push_back( make_pair( i, arco[i].first ) );
		}
		vector<int> c;
		int foi = 0;
		if(tem_ciclo(c)) {
			contrai(c);
		} else break; 
	}

	int resp = 0;
	for(int i = 1; i < n; i++) {
		if(!fora[i]) {
			resp += arco[i].first;
		}
	}
	return resp;
}

int main() {
	int casos;
	scanf("%d", &casos);
	for(int teste = 1; teste <= casos; teste++) {
		printf("Case #%d: ", teste);
		scanf("%d %d", &n, &m);
		for(int i = 0; i < n; i++) grafo[i].clear();
		memset(peso, -1, sizeof(peso));
		for(int i = 0; i < m; i++) {
			int a,b,c;
			scanf("%d %d %d", &a, &b, &c);
			if(a != b) {
				if(peso[a][b] == -1) peso[a][b] = c;
				else peso[a][b] = min(peso[a][b], c);				
			}
		}
		int root = 0;
		for(int i = 0; i < n; i++)
			for(int j = 0; j < n; j++)
				if(peso[i][j] != -1 && j != root) {
					grafo[i].push_back(make_pair(j,peso[i][j]));
				}
		if(!consegue()) {
			printf("Possums!\n");
		} else {
			printf("%d\n", calcula());
		}
	}
}
