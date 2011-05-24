struct point{
    int x, y, id;
    int dist(const point& p) { return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y); }
    int left(const point& p, const point& q) {
        int l = (p.x - x)*(q.y - y) - (q.x - x)*(p.y - y);
        return l < 0 ? -1 : l > 0 ? 1 : 0;
    }
} PIVO;
bool cmp(const point& i, const point& j){
    int l = PIVO.left(i, j);
    if (l > 0 || (l == 0 && PIVO.dist(i) < PIVO.dist(j))) return true;
    return false;
}
void graham(struct point* P, int N, struct point* CH, int* CH_N) {
    int pivo = 0;
    for (int i = 1; i < N; i++)
        if (P[i].y < P[pivo].y || (P[i].y == P[pivo].y && P[i].x < P[pivo].x)) pivo = i;
    swap(P[pivo], P[0]);
    PIVO = P[0];
    sort(P + 1, P + N, cmp);
    int rev = N-1;
    for (int r = N-2; r >= 1; r--)
        if (PIVO.left(P[r], P[rev]) == 0) rev = r;
        else break;
    if (rev != 1) reverse(P + rev, P + N);
    *CH_N = 0;
    for (int i = 0; i < N; i++) {
        if (i > 1) {
            /* Se mudar a comparacao para '<', incluira todos os 
               pontos colineares no convex-hull*/
            while (*CH_N >= 2 && CH[*CH_N-2].left(CH[*CH_N-1],P[i]) <= 0) (*CH_N)--;
        }
        CH[(*CH_N)++] = P[i];
    }
}
point P[MAX], CH[MAX];
int CH_N, lbl[MAX];
bool pointInLine(point a, point b, point c) {
    if(b.y != c.y) return (min(b.y,c.y) <= a.y && a.y <= max(b.y, c.y));
    if(b.x != c.x) return (min(b.x,c.x) <= a.x && a.x <= max(b.x, c.x));
    return false;
}
bool cruza(point a, point b, point c, point d) {
    if (a.left(c,d) == 0 && b.left(c,d) == 0) {
        if (pointInLine(a,c,d)) return true;
        if (pointInLine(b,c,d)) return true;
        if (pointInLine(c,a,b)) return true;
        if (pointInLine(d,a,b)) return true;
        return false;
    }
    return (a.left(c,d) != b.left(c,d)) && (c.left(a,b) != d.left(a,b));
}
int main() {
    int N;
    while(scanf("%d", &N) == 1 && N) {
        for(int i = 0; i < N; i++) scanf("%d %d", &P[i].x, &P[i].y);
        graham(P, N, CH, &CH_N);
    }
    return 0;
}
