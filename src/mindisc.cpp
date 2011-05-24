struct point { double x, y; };
struct circle { point c; double r; };
point P[NMAX]; circle D[NMAX];
const double eps = 1e-9;
double dist2(point p,point q) { return (p.x-q.x)*(p.x-q.x) + (p.y-q.y)*(p.y-q.y); }
double dist(point p,point q) { return sqrt(dist2(p,q)); }
bool in_circle(point p,circle c){ return (dist2(p,c.c) <= c.r*c.r); }
point middle(point p,point q) { return (point) { (p.x + q.x)/2.0, (p.y + q.y)/2.0 }; }
point make_point(double x,double y) { return (point) { x, y }; }
circle make_circle(point p) { return (circle) { p, 0.0 }; }
circle make_circle (point p,point q) { return (circle) { middle(p, q), dist(p, q)/2.0 }; }
bool perpendicular(point a,point b,point c){
    double yDelta_a = b.y - a.y, xDelta_a = b.x - a.x, yDelta_b = c.y - b.y, xDelta_b = c.x - b.x;
    if (fabs(xDelta_a) <= eps && fabs(yDelta_b) <= eps) return false;
    if (fabs(yDelta_a) <= eps) return true; if (fabs(yDelta_b) <= eps) return true;
    if (fabs(xDelta_a) <= eps) return true; if (fabs(xDelta_b) <= eps) return true;
    return false;
}
circle calc_circle(point a,point b,point c) {
    double yDelta_a = b.y - a.y, xDelta_a = b.x - a.x, yDelta_b = c.y - b.y, xDelta_b = c.x - b.x;
    circle resp;
    if (fabs(xDelta_a) <= eps && fabs(yDelta_b) <= eps) {
        resp.c.x = 0.5*(b.x + c.x);
        resp.c.y = 0.5*(a.y + b.y);
        resp.r = dist(resp.c , a);
        return resp;
    }
    double aSlope = yDelta_a/xDelta_a;
    double bSlope = yDelta_b/xDelta_b;
    if (fabs(aSlope-bSlope) <= eps) {
        printf("ferrou!\n");
        return make_circle(a,b);
    }
    resp.c.x = (aSlope*bSlope*(a.y - c.y) + bSlope*(a.x + b.x) - aSlope*(b.x + c.x))/
        (2.0 * (bSlope-aSlope));
    resp.c.y = -1.0*(resp.c.x - (a.x + b.x)/2.0)/aSlope + (a.y + b.y)/2.0;
    resp.r = dist(resp.c , a);
    return resp;
}
circle make_circle(point a, point b, point c){
    if (!perpendicular(a,b,c)) return calc_circle(a,b,c);
    if (!perpendicular(a,c,b)) return calc_circle(a,c,b);
    if (!perpendicular(b,a,c)) return calc_circle(b,a,c);
    if (!perpendicular(b,c,a)) return calc_circle(b,c,a);
    if (!perpendicular(c,b,a)) return calc_circle(c,b,a);
    if (!perpendicular(c,a,b)) return calc_circle(c,a,b);
    printf("ferrou!\n");
    return make_circle(a,b);
}
circle MiniDiscWith2Points(int N,point p,point q){
    circle at = make_circle(p, q);
    for (int i = 0; i < N; i++) {
        if (i == 0 && in_circle(P[i],at)) D[i] = at;
        else if (i != 0 && in_circle(P[i],D[i-1])) D[i] = D[i-1];
        else D[i] = make_circle(p,q,P[i]);
    }
    return D[N-1];
}
circle MiniDiscWithPoint(int N, point q){
    D[0] = make_circle(P[0],q);
    for(int i = 1; i < N; i++){
        if (in_circle(P[i],D[i-1])) D[i] = D[i-1];
        else D[i] = MiniDiscWith2Points(i,P[i],q);
    }
    return D[N-1];
}
circle MiniDisc(int N){
    if(N <= 1) return make_circle(P[0]);
    random_shuffle(P, P+N);
    D[1] = make_circle(P[0],P[1]);
    for (int i = 2; i < N; i++){
        if (in_circle(P[i],D[i-1])) D[i] = D[i-1];
        else D[i] = MiniDiscWithPoint(i,P[i]);
    }
    return D[N-1];
}
int main(){
    int N; double x, y;
    while (scanf("%d",&N) == 1 && N){
        for (int i = 0;i < N;i++) { scanf("%lf %lf",&x,&y); P[i] = make_point(x,y); }
        if (test == -1){
            printf("%d\n",N);
            for (int i = 0;i < N;i++) printf("%.2lf %.2lf\n",P[i].x,P[i].y);
        }
        circle resp;
        if (N == 1) resp = make_circle(P[0]);
        else resp = MiniDisc(N);
        printf("Instancia %d\n",test++);
        printf("%.2lf %.2lf %.2lf\n\n",resp.c.x,resp.c.y,resp.r);
    }
    return 0;
}
