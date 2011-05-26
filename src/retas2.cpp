typedef long long ll;
struct P { double x, y; P() {}; P( double q, double w ) : x( q ), y( w ) {} };
struct RETA {
    int a, b, c;
    void init( int A, int B, int C ) {
        if( A < 0 || A == 0 && B < 0 ) { A = -A; B = -B; C = -C; }
        int d = A ?
            gcd( gcd( abs( A ), abs( B ) ), C ) :
            ( B || C ? gcd( abs( B ), C ) : 1 );
        a = A / d; b = B / d; c = C / d;
    }
    RETA() {}
    RETA( int A, int B, int C ) { init( A, B, C ); }
    RETA( P p, P q ) { init( q.y - p.y, p.x - q.x, p.x * q.y - p.y * q.x ); }
    bool operator<( const RETA &l ) const {
        return a < l.a || a == l.a && ( b < l.b || b == l.b && c < l.c );
    }
};
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
ll left(P a, P b, P c) {
    return (a.first - c.first)*(b.second - c.second) - (a.second - c.second)*(b.first - c.first);
}
RETA Perpendicular(RETA r, pii a) {
    RETA nova; nova.init(-r.B, r.A, -A * a.first - B * a.second); return nova;
}
int naReta(RETA r, pii p) {
    return r.A * p.first + r.B * p.second + r.C == 0;
}
double polarAngle( P p ) {
    if( fabs( p.x ) <= EPS && fabs( p.y ) <= EPS ) return -1.0;
    if( fabs( p.x ) <= EPS ) return ( p.y > EPS ? 1.0 : 3.0 ) * acos( 0 );
    double theta = atan( 1.0 * p.y / p.x );
    if( p.x > EPS ) return( p.y >= -EPS ? theta : ( 4 * acos( 0 ) + theta ) );
    return( 2 * acos( 0 ) + theta );
}
bool pointInPoly( P p, vector< P > &poly ) {
    int n = poly.size();
    double ang = 0.0;
    for( int i = n - 1, j = 0; j < n; i = j++ ) {
        P v( poly[i].x - p.x, poly[i].y - p.y );
        P w( poly[j].x - p.x, poly[j].y - p.y );
        double va = polarAngle( v );
        double wa = polarAngle( w );
        double xx = wa - va;
        if( va < -0.5 || wa < -0.5 || fabs( fabs( xx ) - 2 * acos( 0 ) ) < EPS ) {
            // POINT IS ON THE EDGE
            assert( false );
            ang += 2 * acos( 0 );
            continue;
        }
        if( xx < -2 * acos( 0 ) ) ang += xx + 4 * acos( 0 );
        else if( xx > 2 * acos( 0 ) ) ang += xx - 4 * acos( 0 );
        else ang += xx;
    }
    return( ang * ang > 1.0 );
}
double distToLine(double ax, double ay,double bx, double by,
double px, double py, double *cpx, double *cpy ) {
    //Formula: cp = a + (p-a).(b-a) / |b-a| * (b-a)
    double proj = ( ( px - ax ) * ( bx - ax ) + ( py - ay ) * ( by - ay ) ) /
                  ( ( bx - ax ) * ( bx - ax ) + ( by - ay ) * ( by - ay ) );
    *cpx = ax + proj * ( bx - ax ); *cpy = ay + proj * ( by - ay );
    return dist( px, py, *cpx, *cpy );
}
double distToLineSegment(double ax, double ay,double bx, double by,
double px, double py,double *cpx, double *cpy ) {
    if( ( bx - ax ) * ( px - ax ) + ( by - ay ) * ( py - ay ) < EPS ) {
        *cpx = ax; *cpy = ay;
        return dist( ax, ay, px, py );
    }
    if( ( ax - bx ) * ( px - bx ) + ( ay - by ) * ( py - by ) < EPS ) {
        *cpx = bx;*cpy = by;
        return dist( bx, by, px, py );
    }
    return distToLine( ax, ay, bx, by, px, py, cpx, cpy );
}
bool lineIntersect( P a, P b, P c, P d, P &r ) {
    P n; n.x = d.y - c.y; n.y = c.x - d.x;
    double denom = n.x * ( b.x - a.x ) + n.y * ( b.y - a.y );
    if( fabs( denom ) < EPS ) return false;
    double num = n.x * ( a.x - c.x ) + n.y * ( a.y - c.y );
    double t = -num / denom;
    r.x = a.x + t * ( b.x - a.x );
    r.y = a.y + t * ( b.y - a.y );
    return true;
}
template< class T > bool lineSegIntersect( vector< T > &x, vector< T > &y ) {
    double ucrossv1 = ( x[1] - x[0] ) * ( y[2] - y[0] ) - ( y[1] - y[0] ) * ( x[2] - x[0] );
    double ucrossv2 = ( x[1] - x[0] ) * ( y[3] - y[0] ) - ( y[1] - y[0] ) * ( x[3] - x[0] );
    if( ucrossv1 * ucrossv2 > 0 ) return false;
    double vcrossu1 = ( x[3] - x[2] ) * ( y[0] - y[2] ) - ( y[3] - y[2] ) * ( x[0] - x[2] );
    double vcrossu2 = ( x[3] - x[2] ) * ( y[1] - y[2] ) - ( y[3] - y[2] ) * ( x[1] - x[2] );
    return( vcrossu1 * vcrossu2 <= 0 );
}

