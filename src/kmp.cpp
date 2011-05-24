int pi[MAX];
char p[MAX], p2[MAX], T;
void compute_prefix_function(int m){
    int k = pi[0] = -1;
    for (int i = 1; i < m; i++) {
        while (k >= 0 && p[i] != p[k+1])
            k = pi[k];
        if (p[i] == p[k+1]) k++;
        pi[i] = k;
    }
}
int KMP(int m, int n){
    int q = -1;
    compute_prefix_function(m);
    for (int i = 0; i < n; i++) {
        while (q >= 0 && p2[i] != p[q+1])
            q = pi[q];
        if (p2[i] == p[q+1]) q++;
        if (q >= m-1) {
            printf("%d\n",i-m+1);
            q = pi[q];
        }
    }
    return 0;
}
/* p := padrao a ser procurado em p2
scanf("%s", p); scanf("%s", p2); n = strlen(p2); m = strlen(p); KMP(m, n); */
