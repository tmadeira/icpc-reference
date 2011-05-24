struct Trie {
    short folha; /*Se w <= 1<<16*/ int goFail; map<char,int> filho;
};
#define WMAX 1024
#define TMAX 1024
#define TEXTSIZE 1024*1024
Trie trie[WMAX*TMAX];
char texto[TEXTSIZE], word[WMAX][TMAX];
int tam[WMAX], cabeca = WMAX*TMAX-1, w, id[WMAX];
vector<int> ocorrencias[WMAX];
int insere(char c, int at) {
    if (trie[at].filho.count(c)) return trie[at].filho[c];
    else return trie[at].filho[c] = ++cabeca;
}
void montaTrie() {
    for (int i = 0; i <= cabeca; i++) {
        trie[i].folha = 0; trie[i].goFail = -1; trie[i].filho.clear();
    }
    cabeca = 0;
    for (int i = 0; i < w; i++) {
        int at = 0;
        for (int j = 0; j < tam[i]; j++) at = insere(word[i][j], at);
        if (trie[at].folha == 0) /* evita repeticao */ trie[at].folha = i+1;
        else id[i] = trie[at].folha-1;
    }
}
void arrumaGoFails() {
    queue<int> fila; map<char,int>::iterator it;
    for (it = trie[0].filho.begin(); it != trie[0].filho.end(); it++) {
        trie[it->second].goFail = 0;
        fila.push( it->second );
    }
    while (!fila.empty()) {
        int no = fila.front();
        fila.pop();
        for (it = trie[no].filho.begin(); it != trie[no].filho.end(); it++) {
            int atual = it->second, failPai = trie[no].goFail, caminho = it->first;
            fila.push(atual);
            while (failPai >= 0 && !trie[failPai].filho.count(caminho))
                failPai = trie[failPai].goFail;
            if(failPai == -1) {
                if(trie[0].filho.count(caminho)) trie[atual].goFail = trie[0].filho[caminho];
                else trie[atual].goFail = 0;
            } else {
                trie[atual].goFail = trie[failPai].filho[caminho];
                if (trie[atual].folha == 0 && trie[ trie[atual].goFail ].folha != 0)
                    trie[atual].folha = trie[ trie[atual].goFail ].folha;
            }
        }		
    }
}
void fazConsulta(char * s) {
    int at = 0;
    for (int pos = 0; s[pos]; pos++) {
        while (at >= 0 && !trie[at].filho.count(s[pos])) at = trie[at].goFail;
        if (at == -1) at = 0;
        else at = trie[at].filho[s[pos]];
        int tmp = at;
        while (tmp >= 0 && trie[tmp].folha) {
            ocorrencias[ trie[tmp].folha-1 ].push_back( pos - tam[ trie[tmp].folha - 1 ] + 1 );
            while (trie[tmp].goFail != -1 && trie[ trie[tmp].goFail ].folha == trie[tmp].folha)
                tmp = trie[tmp].goFail;
            tmp = trie[tmp].goFail;
        }
    }
}
int main() {
    scanf("%s", texto); scanf("%d", &w);
    for(int i = 0; i < w; i++) {
        scanf("%s", word[i]);
        id[i] = i; tam[i] = strlen(word[i]);
    }
    montaTrie(); arrumaGoFails();
    for(int i = 0; i < w; i++) ocorrencias[i].clear();
    fazConsulta(texto);
    for(int i = 0; i < w; i++) printf("%s\n", ocorrencias[ id[i] ].empty() ? "n" : "y" ); 
}
