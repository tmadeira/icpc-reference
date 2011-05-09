/* Geometria 3D */

#include <stdio.h>
#include <math.h>
#include <algorithm>

using namespace std;

const double epsilon = 1e-11;

/* PONTOS E VETORES */

struct ponto {
	double x, y, z;
	ponto(double X = 0, double Y = 0, double Z = 0): x(X), y(Y), z(Z) { }
};

struct vetor {
	double x, y, z;
	vetor(double X = 0, double Y = 0, double Z = 0): x(X), y(Y), z(Z) { }
	vetor(ponto p) { x = p.x; y = p.y; z = p.z; }
	vetor(ponto p, ponto q) { x = q.x - p.x; y = q.y - p.y; z = q.z - p.z; }
};

ponto operator + (const ponto &p, const vetor &v) { return ponto(p.x + v.x, p.y + v.y, p.z + v.z); }
ponto operator + (const ponto &p, const ponto &q) { return ponto(p.x + q.x, p.y + q.y, p.z + q.z); }
ponto operator - (const ponto &p, const vetor &v) { return ponto(p.x - v.x, p.y - v.y, p.z - v.z); }
ponto operator - (const ponto &p, const ponto &q) { return ponto(p.x - q.x, p.y - q.y, p.z - q.z); }
vetor operator + (const vetor &u, const vetor &v) { return vetor(u.x + v.x, u.y + v.y, u.z + v.z); }
vetor operator - (const vetor &u, const vetor &v) { return vetor(u.x - v.x, u.y - v.y, u.z - v.z); }
vetor operator * (const double &a, const vetor &v) { return vetor(a * v.x, a * v.y, a * v.z); }
double dot(const vetor u, const vetor v) { return u.x * v.x + u.y * v.y + u.z * v.z; }
vetor cross(const vetor u, const vetor v) { return vetor(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x); }
double norma(const vetor v) { return sqrt(dot(v, v)); }

void debug(vetor v) { printf("(%.2f, %.2f, %.2f)\n", v.x, v.y, v.z); }

/* RETAS, SEMIRETAS, SEGMENTOS E TRIANGULOS */

struct reta {
	ponto a, b;
	reta(ponto A, ponto B): a(A), b(B) { }
	reta(ponto P, vetor V): a(P) { b = P + V; }
};

struct semireta {
	ponto a, b;
	semireta(ponto A, ponto B): a(A), b(B) { }
	semireta(ponto P, vetor V): a(P) { b = P + V; }
};

struct segmento {
	ponto a, b;
	segmento(ponto A, ponto B): a(A), b(B) { }
};

struct triangulo {
	ponto a, b, c;
	triangulo(ponto A, ponto B, ponto C): a(A), b(B), c(C) { }
};

/* DISTANCIA ENTRE OBJETOS GEOMETRICOS */

double distancia(const ponto a, const ponto b) {
	return norma(vetor(a, b));
}

double distancia(const ponto p, const reta r) {
	vetor v(r.a, r.b), w(r.a, p);
	return norma(cross(v, w)) / norma(v);
}

double distancia(const ponto p, const semireta s) {
	vetor v(s.a, s.b), w(s.a, p);
	if (dot(v, w) <= 0) return distancia(p, s.a);
	return distancia(p, reta(s.a, s.b));
}

double distancia(const ponto p, const segmento s) {
	vetor v(s.a, s.b), w(s.a, p);
	double c1 = dot(v, w), c2 = dot(v, v);
	if (c1 <= 0) return distancia(p, s.a);
	if (c2 <= c1) return distancia(p, s.b);
	return distancia(p, s.a + (c1/c2)*v);
}

double distancia(const reta r, const reta s) {
	vetor u(r.a, r.b), v(s.a, s.b), w(r.a, s.a);
	double a = dot(u, u), b = dot(u, v), c = dot(v, v), d = dot(u, w), e = dot(v, w);
	double D = a*c - b*b, sc, tc;
	if (D < epsilon) {
		sc = 0;
		tc = (b > c) ? d/b : e/c;
	} else {
		sc = (b*e - c*d) / D;
		tc = (a*e - b*d) / D;
	}
	vetor dP = w + (sc * u) - (tc * v);
	return norma(dP);
}

double distancia(const segmento r, const segmento s) {
	vetor u(r.a, r.b), v(s.a, s.b), w(s.a, r.a);
	double a = dot(u, u), b = dot(u, v), c = dot(v, v), d = dot(u, w), e = dot(v, w);
	double D = a*c - b*b;
	double sc, sN, sD = D;
	double tc, tN, tD = D;

	if (D < epsilon) {
		sN = 0;
		sD = 1;
		tN = e;
		tD = c;
	} else {
		sN = (b*e - c*d);
		tN = (a*e - b*d);
		if (sN < 0) {
			sN = 0;
			tN = e;
			tD = c;
		} else if (sN > sD) {
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}
	
	if (tN < 0) {
		tN = 0;
		if (-d < 0) {
			sN = 0;
		} else if (-d > a) {
			sN = sD;
		} else {
			sN = -d;
			sD = a;
		}
	} else if (tN > tD) {
		tN = tD;
		if ((-d + b) < 0) {
			sN = 0;
		} else if (-d + b > a) {
			sN = sD;
		} else {
			sN = -d + b;
			sD = a;
		}
	}

	sc = fabs(sN) < epsilon ? 0 : sN / sD;
	tc = fabs(tN) < epsilon ? 0 : tN / tD;

	vetor dP = w + (sc * u) - (tc * v);
	return norma(dP);
}

/* Inicio das funcoes do ITA no G da subregional */
vetor projecao(vetor u, vetor v) {
	return (dot(v, u) / dot(u, u)) * u;
}

bool between(ponto a, ponto b, ponto p) {
	return dot(vetor(p - a), vetor(p - b)) < epsilon;
}

double linedist(ponto a, ponto b, ponto p) {
	ponto proj = a + projecao(vetor(a, b), vetor(a, p));
	if (between(a, b, proj)) {
		return norma(vetor(proj, p));
	} else {
		return min(norma(vetor(a, p)), norma(vetor(b, p)));
	}
}

double distancia(const ponto p, const triangulo T) {
	vetor X(T.a, T.b), Y(T.a, T.c), P(T.a, p);
	vetor PP = P - projecao(cross(X, Y), P);
	ponto PPP = T.a + PP;
	vetor R1 = cross(vetor(T.a, T.b), vetor(T.a, PPP));
	vetor R2 = cross(vetor(T.b, T.c), vetor(T.b, PPP));
	vetor R3 = cross(vetor(T.c, T.a), vetor(T.c, PPP));
	if (dot(R1, R2) > -epsilon && dot(R2, R3) > -epsilon && dot(R1, R3) > -epsilon) {
		return norma(vetor(PPP, p));
	} else {
		return min(linedist(T.a, T.b, p), min(linedist(T.b, T.c, p), linedist(T.c, T.a, p)));
	}
}
/* Fim das funcoes do ITA no G da subregional */

#define NMAX 4
ponto P[NMAX], Q[NMAX];

int main() {
	int tests;
	scanf("%d", &tests);
	for (int test = 0; test < tests; test++) {
		int x, y, z;
		for (int i = 0; i < 4; i++) {
			scanf("%d %d %d", &x, &y, &z);
			P[i] = ponto(x, y, z);
		}
		for (int i = 0; i < 4; i++) {
			scanf("%d %d %d", &x, &y, &z);
			Q[i] = ponto(x, y, z);
		}
		double out = 1000000000.0;
		for (int i = 0; i < 4; i++) {
			for (int j = i+1; j < 4; j++) {
				segmento r(P[i], P[j]);
				for (int k = 0; k < 4; k++) {
					for (int l = k+1; l < 4; l++) {
						segmento s(Q[k], Q[l]);
						out = min(out, distancia(r, s));
					}
				}
			}
		}
		for (int i = 0; i < 4; i++) {
			for (int j = i+1; j < 4; j++) {
				for (int k = j+1; k < 4; k++) {
					triangulo t(P[i], P[j], P[k]), u(Q[i], Q[j], Q[k]);
					for (int l = 0; l < 4; l++) {
						out = min(out, distancia(P[l], u));
						out = min(out, distancia(Q[l], t));
					}
				}
			}
		}
		printf("%.2f\n", out);
	}
	return 0;
}

