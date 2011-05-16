#include<cmath>
#include<stdio.h>
#include<algorithm>
#include<set>
#include<vector>
using namespace std;

#define NMAX 1024

struct point{
    double x;
    double y;
};

int n,m;
point P[NMAX];

double dist2(point p,point q){
    return (p.x-q.x)*(p.x-q.x) + (p.y-q.y)*(p.y-q.y);
}

double dist(point p,point q){
    return sqrt(dist2(p,q));
}

point middle(point p,point q){
    point resp;
    resp.x = (p.x + q.x)/2;
    resp.y = (p.y + q.y)/2;
    return resp;
}

point make_point(double x,double y){
    point resp;
    resp.x = x;
    resp.y = y;
    return resp;
}

struct ATOL {
	int id;
	bool entra;
	double ang;
	point c;
};

vector<ATOL> eventos;

bool cmp(ATOL a, ATOL b) {
	return a.ang < b.ang;
}

ATOL make_ATOL(int id, bool entra, double ang, point c) {
	ATOL novo;
	novo.id = id;
	novo.entra = entra;
	novo.ang = ang;
	novo.c = c;
	return novo;
}

double anguloAtan2(point centro, point novo) {
	double y = novo.y - centro.y;
	double x = novo.x - centro.x;
	return atan2(y,x);
}

//encontra onde deve estar o centro da sweep quando o ponto b for entrar e sair da sweep.
pair< point, point > calcula_pontos_entrada_e_saida_da_sweep( point a, point b, double r) {
	double d = dist(a, b);
	double anda = sqrt( r*r - ((d*d)/4) );
	double dx = b.x - a.x;
	double dy = b.y - a.y;
	double normaliza = sqrt(dx*dx + dy*dy);
	dx /= normaliza; dy /= normaliza;
	double salvo = dx;
	dx = -dy;
	dy = salvo;
	point medio = middle(a,b);
	point c1 = medio, c2 = medio;
	c1.x += anda*dx; c1.y += anda*dy;
	c2.x -= anda*dx; c2.y -= anda*dy;
	return make_pair(c1,c2);
}

void calcula_eventos(int id, double raio){
 eventos.clear();
 point a = P[id];
 for(int i = 0; i < n; i++) {
   if( i == id ) continue;
   if(dist( a, P[i] ) > 2*raio ) continue;
   pair< point , point > intersecoes = calcula_pontos_entrada_e_saida_da_sweep( a, P[i], raio);
   eventos.push_back( make_ATOL( i, false , anguloAtan2(a,intersecoes.first), intersecoes.first));
   eventos.push_back( make_ATOL( i, true , anguloAtan2(a,intersecoes.second), intersecoes.second));
 }
}

int dentro[NMAX];

int sweepGiratoria(int id,  double r) {
	calcula_eventos(id, r);
	if(eventos.empty()) return 1; //cobre soh ele mesmo
	sort(eventos.begin(), eventos.end(), cmp);
	for(int i = 0; i < n; i++) dentro[i] = 0;
	int cnt = 0;
	point inicio = eventos[0].c;
	for(int i = 0; i < n; i++) {
		if( dist( P[i], inicio ) <= r + 1e-9 )  {
			dentro[i] = 1;
			cnt++;
		}
	}
	int output = cnt;
	for(int i = 0; i < (int) eventos.size(); i++) {
		if( eventos[i].entra ) {
			if(!dentro[ eventos[i].id ]) {
				dentro[ eventos[i].id ] = 1;
				cnt++;
			}
		}
		else {
			if(dentro[ eventos[i].id ]) {
				dentro[ eventos[i].id ] = 0;
				cnt--;
			}
		}
		output = max( output , cnt );
	}

	return output;
}

int main(){
	while(scanf("%d %d",&n, &m) == 2){
		if(n == 0 && m == 0) break;
		for(int i = 0;i < n;i++){
			double x,y;
			scanf("%lf %lf",&x,&y);
			P[i] = make_point(x,y);
		}
		double lower = 0.0, upper = 100000.0, meio;
		for(int it = 0; it <= 60; ++it) {
			if(upper - lower <= 1e-5) break;
			meio = (lower+upper)/2;
			int cobreMax = 0;
			for(int i = 0; i < n && cobreMax < m; ++i) {
				cobreMax = max(cobreMax, sweepGiratoria(i ,meio));
			}
			if(cobreMax >= m) {
				upper = meio;
			}
			else {
				lower = meio;
			}
		}
		printf("%.3lf\n", (lower + upper)/2);
	}
	return 0;
}
