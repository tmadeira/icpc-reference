const int NMAX = 100000;
struct par {
	int a, b;
	bool operator < (const par &p) const {
		return b > p.b || (b == p.b && a < p.a);
	}
} A[NMAX];
int v[NMAX], k, pai[NMAX], fim, id[NMAX];
int main() {
	int n; scanf("%d", &n);
	set <par> S;
	for (int i = 0; i < n; i++) {
		scanf("%d %d", &A[i].a, &A[i].b);
		if (S.count(A[i])) {
			i--; n--;
		} else {
			S.insert(A[i]);
		}
	}
	sort(A, A+n);
	k = 0; fim = -1;
	memset(pai, -1, sizeof(pai));
	for(int i = 0; i < n; i++) {
		int pos = upper_bound(v, v+k, A[i].a) - v;
		if(pos) { pai[i] = id[ pos-1 ]; }
		id[pos] = i;
		v[pos] = A[i].a;
		if(pos == k) {
			fim = i;
			k++;
		} 
	}
	vector <par> out;
	while(fim != -1) {
		out.push_back(A[fim]);
		fim = pai[fim];
	}
	printf("%d\n", (int) out.size());
	for (int i = out.size()-1; i >= 0; i--) {
		printf("%d %d\n", out[i].a, out[i].b);
	}
}
