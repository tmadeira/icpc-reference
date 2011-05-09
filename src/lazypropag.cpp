#include<cstdio>

#define N 100000

struct Arv {
	int esq,dir;
	int ini,fim;
	int trocas;
	int qtd[3];
};

Arv tree[2*N];
int cabeca, qnos, nos[N], TROCAS[N];

int monta(int ini, int fim) {
	int at = cabeca++;
	tree[at].ini = ini;
	tree[at].fim = fim;
	tree[at].trocas = 0;
	if(ini == fim) {
		tree[at].esq = tree[at].dir = -1;
		tree[at].qtd[1] = tree[at].qtd[2] = 0;
		tree[at].qtd[0] = 1;
		return at;
	}
	int e = tree[at].esq = monta(ini, (ini+fim)/2);
	int d = tree[at].dir = monta((ini+fim)/2 + 1, fim);
	for(int i = 0; i < 3; i++) {
		tree[at].qtd[i] = tree[e].qtd[i] + tree[d].qtd[i];
	}
	return at;
}

int AUX[3];

void soma(int at, int ini, int fim) {
	if(ini <= tree[at].ini && tree[at].fim <= fim) {
		tree[at].trocas++;
		tree[at].trocas %= 3;
		for(int i = 0; i < 3; i++) {
			AUX[i] = tree[at].qtd[i];
		}
		for(int i = 0; i < 3; i++) {
			tree[at].qtd[i] = AUX[(i-1+3)%3];
		}
		return;
	}
	int e = tree[at].esq,d = tree[at].dir;
	if(tree[e].ini <= fim && tree[e].fim >= ini) {
		soma(e,ini,fim);
	}
	if(tree[d].ini <= fim && tree[d].fim >= ini) {
		soma(d,ini,fim);
	}

	int trocas = tree[at].trocas;
	for(int i = 0; i < 3; i++) {
		tree[at].qtd[i] = tree[e].qtd[(i-trocas+3)%3] + tree[d].qtd[(i-trocas+3)%3];
	}
}

void get(int at, int ini, int fim, int trocas) {
	if(ini <= tree[at].ini && tree[at].fim <= fim) {
		TROCAS[qnos] = trocas%3;
		nos[qnos++] = at;
		return;
	}
	trocas += tree[at].trocas;
	int e = tree[at].esq, d = tree[at].dir;
	if(tree[e].ini <= fim && tree[e].fim >= ini) {
		get(e,ini,fim,trocas);
	}
	if(tree[d].ini <= fim && tree[d].fim >= ini) {
		get(d,ini,fim,trocas);
	}
}

int vals[N];

int query(int a, int b) {
	qnos = 0;
	get(0,a,b,0);
	int r = 0;
	for(int i = 0; i < qnos; i++) {
		r += tree[nos[i]].qtd[ (-TROCAS[i]+3) % 3];
	}
	return r;
}



int main() {
	int n,q,op,a,b;
	scanf("%d %d", &n, &q);
	cabeca = 0;
	monta(0,n-1);
	for(int i = 0; i < q; i++) {
		scanf("%d %d %d", &op, &a, &b);
		if(op == 0) {
			soma(0,a,b);
		} else {
			printf("%d\n", query(a,b));
		}
	}
	return 0;
}
