int T;
int X,Y,M;
int cmp(double a, double b) {
    if(fabs(a-b)<=1e-8)
        return 0;
    if(a<b)
        return -1;
    return 1;
}
struct point {
    double x,y;
    point() {}
    point(double a, double b) : x(a), y(b) {}
    point operator+(const point& a) const {
        return point(x+a.x,y+a.y);
    }
    point operator-(const point& a) const {
        return point(x-a.x,y-a.y);
    }
    point operator/(double a) const {
        return point(x/a,y/a);
    }
};
double sqrdist(const point& a, const point &b) {
    return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}
struct line {
    double a,b,c;
    line() {}
    line(const point& a1,const point& a2) {
        a=a1.y-a2.y;
        b=a2.x-a1.x;
        c=a1.x*a2.y-a2.x*a1.y;
    }
    line(double a, double b, double c) : a(a), b(b), c(c) {}
    line(double ang) : a(-ang), b(1.0), c(0.0) { }
    line passa_por(const point& p) const {
        return line(a,b,-a*p.x-b*p.y);
    }
    line ortogonal() const {
        return line(-b,a,c);
    }
    bool paralela(const line& l) const {
        return cmp((-a*l.b),(-l.a*b))==0;
    }
};
point intersection(const line& l, const line& l2) {
    return point((l.b*l2.c-l.c*l2.b)/(l.a*l2.b-l2.a*l.b),(-l.a*l2.c+l.c*l2.a)/(l.a*l2.b-l2.a*l.b));;
}
point pontos[2000];
point polygon[2000];
int npolygon;
point polaux[2000];
int npolaux;
int main(void) {
    scanf("%d",&T);
    while(T--) {
        scanf("%d %d %d",&X,&Y,&M);
        for(int i=0;i<M;i++) scanf("%lf %lf",&pontos[i].x,&pontos[i].y);
        double ansd=-1;
        point ans;
        for(int i=0;i<M;i++) {
            npolygon=4;
            polygon[0]=point(0,0);
            polygon[1]=point(0,Y);
            polygon[2]=point(X,Y);
            polygon[3]=point(X,0);
            for(int j=0;j<M;j++) if(pontos[i].x!=pontos[j].x or pontos[i].y!=pontos[j].y) {
                line biseccao = line(pontos[i],pontos[j]).ortogonal().passa_por((pontos[i]+pontos[j])/2.0);
                int inter[8];
                int ninter=0;
                for(int k=0;k<npolygon;k++) {
                    int l=(k+1)%npolygon;
                    double l1=biseccao.a*polygon[k].x + biseccao.b*polygon[k].y + biseccao.c;
                    double l2=biseccao.a*polygon[l].x + biseccao.b*polygon[l].y + biseccao.c;
                    //considera que a ponta do comeco intersecta e a ponta do final nao intersecta
                    if( (cmp(l1,0.0)<=0 and cmp(l2,0.0)>0) or (cmp(l1,0.0)>=0 and cmp(l2,0.0)<0) ) {
                        inter[ninter++]=k;
                    }
                }
                assert(ninter<=2);
                if(ninter==2) {
                    double lp=biseccao.a*pontos[i].x + biseccao.b*pontos[i].y + biseccao.c;
                    double lpol=biseccao.a*polygon[(inter[0]+1)%npolygon].x + biseccao.b*polygon[(inter[0]+1)%npolygon].y + biseccao.c;
                    //verifica se a ordem do loop precisa ser trocada
                    if( (cmp(lp,0)<0 and cmp(lpol,0)>0) or (cmp(lp,0)>0 and cmp(lpol,0)<0) )
                        swap(inter[0],inter[1]);
                    npolaux=0;
                    polaux[npolaux++]=intersection(biseccao,line(polygon[inter[0]],polygon[(inter[0]+1)%npolygon]));
                    for(int k=(inter[0]+1)%npolygon;k!=inter[1];k=(k+1)%npolygon) {
                        polaux[npolaux++]=polygon[k];
                    }
                    polaux[npolaux++]=polygon[inter[1]];
                    polaux[npolaux++]=intersection(biseccao,line(polygon[inter[1]],polygon[(inter[1]+1)%npolygon]));
                    //se os 2 ultimos pontos coincidirem
                    if(cmp(polaux[npolaux-1].x,polaux[npolaux-2].x)==0 and cmp(polaux[npolaux-1].y,polaux[npolaux-2].y)==0)
                        npolaux--;
                    memcpy(polygon,polaux,sizeof(polygon[0])*npolaux);
                    npolygon=npolaux;
                }
            }
            for(int j=0;j<npolygon;j++)
                if(sqrdist(pontos[i],polygon[j]) > ansd) {
                    ansd=sqrdist(pontos[i],polygon[j]);
                    ans=polygon[j];
                }
        }
        printf("The safest point is (%.1lf, %.1lf).\n",ans.x,ans.y);
    }
}
