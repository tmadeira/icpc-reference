#define LMAX 20
#define NMAX 131072
int pool[LMAX][4][NMAX];
int tmp[NMAX];
inline bool leq(int a1, int a2, int b1, int b2) { return a1 < b1 || (a1 == b1 && a2 <= b2); }
inline bool leq(int a1, int a2, int a3, int b1, int b2, int b3) { return a1 < b1 ||
    (a1 == b1 && leq(a2, a3, b2, b3)); }
static void radix(int *a, int *b, int *r, int n, int K) {
    int *c = tmp;
    for (int i = 0; i <= K; i++) { c[i] = 0; }
    for (int i = 0; i < n; i++) { c[r[a[i]]]++; }
    for (int i = 0, sum = 0; i <= K; i++) { int t = c[i]; c[i] = sum; sum+= t; }
    for (int i = 0; i < n; i++) { b[c[r[a[i]]]++] = a[i]; }
}
#define GetI() (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2)
void suffix_array(int *s, int *SA, int n, int K, int level = 0) {
    int n0 = (n+2)/3, n1 = (n+1)/3, n2 = n/3, n02 = n0+n2; 
    int *s12 = pool[level][0], *SA12 = pool[level][1];
    int *s0 = pool[level][2], *SA0 = pool[level][3];
    s12[n02] = s12[n02+1] = s12[n02+2] = 0; SA12[n02] = SA12[n02+1] = SA12[n02+2] = 0;
    for (int i = 0, j = 0; i < n+(n0-n1); i++) { if (i % 3 != 0) { s12[j++] = i; } }
    radix(s12, SA12, s+2, n02, K);
    radix(SA12, s12, s+1, n02, K);
    radix(s12, SA12, s+0, n02, K);
    int name = 0, c0 = -1, c1 = -1, c2 = -1;
    for (int i = 0; i < n02; i++) {
        if (s[SA12[i]] != c0 || s[SA12[i]+1] != c1 || s[SA12[i]+2] != c2)
        { name++; c0 = s[SA12[i]]; c1 = s[SA12[i]+1]; c2 = s[SA12[i]+2]; }
        if (SA12[i] % 3 == 1) { s12[SA12[i]/3] = name; }
        else { s12[SA12[i]/3 + n0] = name; }
    }
    if (name < n02) {
        suffix_array(s12, SA12, n02, name, level+1);
        for (int i = 0; i < n02; i++) { s12[SA12[i]] = i + 1; }
    } else {
        for (int i = 0; i < n02; i++) { SA12[s12[i]-1] = i; }
    }
    for (int i = 0, j = 0; i < n02; i++) { if (SA12[i] < n0) { s0[j++] = 3*SA12[i]; } }
    radix(s0, SA0, s, n0, K);
    for (int p = 0, t = n0-n1, k = 0; k < n; k++) {
        int i = GetI(); int j = SA0[p];
        if (SA12[t] < n0 ? leq(s[i], s12[SA12[t] + n0], s[j], s12[j/3])
                : leq(s[i], s[i+1], s12[SA12[t]-n0+1], s[j], s[j+1], s12[j/3+n0])) {
            SA[k] = i; t++;
            if (t == n02) { for (k++; p < n0; p++, k++) { SA[k] = SA0[p]; } }
        } else { 
            SA[k] = j; p++; 
            if (p == n0) { for (k++; t < n02; t++, k++) { SA[k] = GetI(); } }
        }
    } 
}
void compute_lcp(int *str, int *sa, int *lcp, int n) {
    int *rank = tmp, i, j, h = 0;
    for (i = 0; i != n; ++i) { rank[sa[i]] = i; }
    for (i = 0; i != n; ++i) if (rank[i] > 0) {
        j = sa[rank[i]-1];
        while (str[i+h] == str[j+h]) { h++; }
        lcp[rank[i]] = h;
        if (h > 0) { h--; }
    }
}
int n, m, k;
int A[NMAX], sa[NMAX], lcp[NMAX];
int main() {
    scanf("%d %d %d", &n, &m, &k);
    for (int i = 0; i < n; i++) { scanf("%d", &A[i]); A[n+i] = A[i]; }
    n*= 2; A[n] = A[n+1] = A[n+2] = 0;
    suffix_array(A, sa, n, m-1); compute_lcp(A, sa, lcp, n);
    return 0;
}
