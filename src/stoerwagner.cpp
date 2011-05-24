#define N 128
int peso[N][N];
int dist[N], ja_saiu[N], foi_contraido[N];
int n,m;
void contrai( pii ultimos ) {
	int menor = min(ultimos.first, ultimos.second);
	int maior = max(ultimos.first, ultimos.second);
	FOR(i,n) {
		peso[menor][i] += peso[maior][i];
		peso[i][menor] = peso[menor][i];
		peso[i][maior] = peso[maior][i] = 0;
		peso[i][i] = 0;
	}
	foi_contraido[maior] = 1;
}
int pega_maior() {
	int id = -1;
	FOR(i,n) {
		if(foi_contraido[i] || ja_saiu[i]) continue;
		if(id == -1 || dist[i] > dist[id]) id = i;
	}
	return id;
}
int corte_fase(int k) {
	pii ultimos = pii(0, 0);
	FOR(i,n) { dist[i] = 0; ja_saiu[i] = 0; }
	FOR(i, k) {
		int v = pega_maior();
		ja_saiu[v] = 1;
		ultimos.second = ultimos.first;
		ultimos.first = v;
		FOR(j, n) 
			dist[ j ] += peso[v][j];
	}
	contrai( ultimos );
	return dist[ ultimos.first ];
}
int stoer_wagner() {
	if(n == 1) return 0;
	int output = (1<<31) - 1;
	int nn = n;
	FOR(i,n) foi_contraido[i] = 0;
	FOR(i, n-1) {
		int aux = corte_fase(nn--);
		output = min(output, aux);
	}
	return output;
}
int main() {
	int T;
	scanf("%d", &T);
	while(T--) {
		scanf("%d %d", &n, &m);
		memset(peso, 0, sizeof(peso));
		FOR(i,m) {
			int a, b, c;
			scanf("%d %d %d", &a, &b, &c);
			a--; b--;
			peso[a][b] = c;
			peso[b][a] = c;
		}
		printf("%d\n", stoer_wagner());
	}
}
