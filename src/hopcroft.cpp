/*
 * ALGORITMO DE HOPCROFT-KARP
 * Encontra emparelhamento maximo num grafo bipartido em O(E sqrt(V))
 */
#include <stdio.h>
#include <string.h>
#include <queue>
#include <vector>
using namespace std;
#define MMAX 1024
#define NMAX 1024
int UMATE[MMAX], M[MMAX], D[MMAX];
int VMATE[NMAX], RET[NMAX];
int m, n, len;
vector <int> E[MMAX];
void bfs() {
    queue <int> Q;
    len = m;
    memset(M, 0, sizeof(M));
    for (int u = 0; u < m; u++) {
        if (UMATE[u] == -1) {
            Q.push(u);
            M[u] = 1;
            D[u] = 0;
        }
    }
    while (!Q.empty()) {
        int u = Q.front(); Q.pop();
        for (int i = 0; i < E[u].size(); i++) {
            int v = E[u][i];
            int w = VMATE[v];
            if (w == -1) {
                len = D[u];
                return;
            } else if (!M[w]) {
                Q.push(w);
                M[w] = 1;
                D[w] = D[u] + 1;
            }
        }
    }
}
void augment(int v) {
    do {
        int u = RET[v];
        int w = UMATE[u];
        UMATE[u] = v;
        VMATE[v] = u;
        v = w;
    } while (v != -1);
}
int dfs(int u) {
    M[u] = 1;
    for (int i = 0; i < E[u].size(); i++) {
        int v = E[u][i];
        int w = VMATE[v];
        if (w == -1) {
            if (D[u] == len) {
                RET[v] = u;
                augment(v);
                return 1;
            }
        } else if (D[u] < D[w] && !M[w]) {
            RET[v] = u;
            if (dfs(w)) {
                return 1;
            }
        }
    }
    return 0;
}
int match() {
    memset(UMATE, -1, sizeof(UMATE));
    memset(VMATE, -1, sizeof(VMATE));
    for (int u = 0; u < m; u++) {
        for (int i = 0; i < E[u].size(); i++) {
            int v = E[u][i];
            if (VMATE[v] == -1) {
                UMATE[u] = v;
                VMATE[v] = u;
                break;
            }
        }
    }
    while (bfs(), len != m) {
        memset(M, 0, sizeof(M));
        for (int u = 0; u < m; u++) {
            if (UMATE[u] == -1) {
                dfs(u);
            }
        }
    }
    int matching = 0;
    for (int u = 0; u < m; u++) {
        if (UMATE[u] != -1) {
            matching++;
        }
    }
    return matching;
}
