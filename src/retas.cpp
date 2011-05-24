typedef long long ll;
struct RETA {
	ll A,B,C;
	bool operator==( const RETA & x ) {
		if(x.A != A) return 0;
		if(x.B != B) return 0;
		if(x.C != C) return 0;
		return 1;
	}
	bool operator < (const RETA & x) const {
		if(A != x.A) return A < x.A;
		if(B != x.B) return B < x.B;
		if(C != x.C) return C < x.C;
		return 0;
	}
};
int Inter(RETA X, RETA Y) {
	ll A1 = X.A; ll A2 = Y.A;
	ll B1 = X.B; ll B2 = Y.B;
	ll C1 = X.C; ll C2 = Y.C;
	pii res;
	double det = A1*B2 - A2*B1;
	if(det == 0){
		//Lines are parallel
		return 0;
	}else{
		double x = (B2*C1 - B1*C2)/det;
		double y = (A1*C2 - A2*C1)/det;
		res = pii(x,y);
		return 1;
	}
}
vector< pii > v;
map< RETA, int > conta;
ll mdc(ll a, ll b) {
	if(b > a) return mdc(b, a);
	if(b == 0) return a;
	return mdc(b, a%b);
}
ll left(pii a, pii b, pii c) {
	return (a.first - c.first)*(b.second - c.second) - (a.second - c.second)*(b.first - c.first);
}
int leftM(pii a, pii b, pii c) {
	ll x = left(a,b,c);
	if(x > 0) return 1;
	if(x < 0) return -1;
	return 0;
}
RETA Reta( pii a, pii b ) {
	RETA nova;
	ll A = b.second - a.second;
	ll B = a.first - b.first;
	ll m = mdc(max(A,-A), max(B,-B));
	A /= m;
	B /= m;
	if(A < 0) {
		A *= -1;
		B *= -1;
	}
	else if(A==0) {
		if(B < 0) B *= -1;
	}
	ll C = -A * a.first - B * a.second;
	nova.A = A; nova.B = B; nova.C = C;
	return nova;
}
RETA Perpendicular(RETA r, pii a) {
	RETA nova;
	ll A = -r.B;
	ll B = r.A;
	ll m = mdc(max(A,-A), max(B,-B));
	A /= m;
	B /= m;
	if(A < 0) {
		A *= -1;
		B *= -1;
	}
	else if(A==0) {
		if(B < 0) B *= -1;
	}
	ll C = -A * a.first - B * a.second;
	nova.A = A; nova.B = B; nova.C = C;
	return nova;
}
void ok(int a, int b) {
	ll x1 = v[a].first;
	ll y1 = v[a].second;
	ll x2 = v[b].first;
	ll y2 = v[b].second;
	ll xmeio = (x1+x2)/2;
	ll ymeio = (y1+y2)/2;
	RETA S = Reta(v[a], v[b]);
	S = Perpendicular(S, pii( xmeio, ymeio) );
	conta[S] += 2;
}
int colinear() {
	for(int i=2; i< sz(v); i++) {
		if(leftM( v[0], v[1], v[i]) != 0) return 0;
	}
	return 1;
}
int naReta(RETA r, pii p) {
	ll x = r.A * p.first + r.B * p.second + r.C;
	if(x == 0) return 1;
	return 0;
}
class Symmetry {
	int countLines(vector <string> points) {
		v.clear();
		string tot = "";
		FOR(i,sz(points)) {
			tot += points[i] + " ";
		}
		stringstream ss(tot);
		ll a,b;
		set < pii > opa;
		opa.clear();
		while(ss >> a >> b) {
			a *= 2;
			b *= 2;
			if(opa.count( pii(a,b) )) continue;
			opa.insert( pii(a,b) );
			v.pb( pii(a,b));
		}
		conta.clear();
		FOR(i,sz(v)) for(int j=i+1; j< sz(v); j++) {
			ok(i,j);
		}
		int r2 = 0;
		if(sz(v) == 2) { r2++; }
		if(sz(v) >= 3) if(colinear()) { r2++; }
		for(map<RETA, int>::iterator it = conta.begin(); it != conta.end(); it++) {
			int tenho = (*it).second;
			FOR(i, sz(v)) {
				if(naReta( (*it).first , v[i] )) tenho++;
			}
			if(tenho == sz(v)) r2++;
		}
		printf("%d\n", r2);
		return r2;
	}
};
