#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <cstring>
using namespace std;

#define NMAX 1000000
int bit[NMAX], bit_inic;

//O( NMAX )
void inicializa_bit() {
    memset(bit, 0, sizeof(bit));
    for(bit_inic = 1; bit_inic < NMAX; bit_inic <<= 1);
    bit_inic >>= 1;
}


//O( log NMAX )
void insere(int x, int val) {
    while(x < NMAX) {
        bit[x] += val;
        x += (x&-x);
    }
}

//O( log NMAX )
int get(int x) {
    int r = 0;
    while(x > 0) {
        r += bit[x];
        x -= (x&-x);
    }
    return r;
}

int sobra;

//Dado um valor freq, encontra o menor X tal que get(X) >= freq. 
// -1 se a maior soma acumulada < freq
//O( log NMAX )
int findS(int freq){
    int base = 0;
    int ans=-1;
    sobra = freq;

    for(int bitmask=bit_inic;bitmask!=0;bitmask>>=1) {
        int idx=base+bitmask;
        if(idx>NMAX)
            continue;
        if(freq > bit[idx]) {
            base=idx;
            freq-=bit[idx];
        }
        else if(freq <= bit[idx]) {
            ans = idx;
        }
    }
    sobra = sobra - freq;
    return ans;
}

//Dado um valor freq, encontra o maior X tal que get(X) <= freq. 
// -1 se a menor soma acumulada > freq
//O( log NMAX )
int findG(int freq){
    int ans = findS( freq + 1 );
    if(ans != -1) 
        return ans-1;

    //devolve o maior cara que tah na bit.
    //quando cai aqui, significa que freq >= get(NMAX-1).
    //poderia devolver NMAX-1.
    return findS( sobra );
}

void teste() {
    srand( time ( NULL ) );
    int sorted[NMAX];

    int n = 100000;
    for(int i = 0; i < n; i++) {
        int x = (rand()% (NMAX-1) ) + 1;
        insere(x, 1);
        sorted[i] = x;
    }
    sort(sorted, sorted + n);
    pos = 0;
    //OLHA UM HEAP - O(n logn)
    while( get( NMAX - 1 ) ) {
        int menor = findG(0) + 1;
        if(menor != sorted[pos++]) printf("NAO TAH ORDENADO\n");
        insere(menor, -1);
    }
}

int main() {
    inicializa_bit();
    teste();
    return 0;
}

