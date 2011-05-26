#include <iostream>
#include <string.h>
#include<stdio.h>
using namespace std;
#define maxN 300
int n,match[maxN],Head, Tail,Queue[maxN],Start, Finish,NewBase,Father[maxN],Base[maxN],Count;
bool graph[maxN][maxN],InQueue[maxN], InPath[maxN], InBlossom[maxN];

void CreateGraph() {
    int u,v;
    memset(graph,0,sizeof(graph));
    scanf("%d",&n);
    while(scanf("%d%d",&u,&v)!=EOF) {
        if(u==-1) break;
        graph[u][v]=graph[v][u]=1;
    }
}
void Push(int u) {
    Queue[Tail++]= u; 
    InQueue[u]= true;
}
int Pop() {
    return Queue[Head++]; 
}
int FindCommonAncestor(int u, int v) {
    memset(InPath, 0, sizeof(InPath));
    while(true) {
        u=Base[u];
        InPath[u]= true;
        if (u == Start)break;
        u= Father[match[u]];
    }
    while(true) {
        v= Base[v];
        if (InPath[v]) break;
        v = Father[match[v]];
    }
    return v;
}
void ResetTrace(int u) {
    int v;
    while (Base[u] != NewBase) {
        v= match[u];
        InBlossom[Base[u]]= 1;
        InBlossom[Base[v]]= 1;
        u= Father[v];
        if (Base[u] != NewBase)Father[u]=v;
    }
}
void BlossomContract(int u,int v) {
    NewBase= FindCommonAncestor(u, v);
    memset(InBlossom,0 ,sizeof(InBlossom));
    ResetTrace(u);
    ResetTrace(v);
    if (Base[u] != NewBase)Father[u]= v;
    if (Base[v] != NewBase)Father[v]= u;
    for(u=1;u<=n;u++) if (InBlossom[Base[u]]) {
        Base[u]= NewBase;
        if (!InQueue[u]) Push(u);
    }
}
void FindAugmentingPath() {
    int u,v;
    memset(InQueue,false, sizeof(InQueue));
    memset(Father,0,sizeof(Father));
    for(u=1;u<=n;u++) Base[u]=u; 
    Head= 1;
    Tail= 1;
    Push(Start);
    Finish = 0;
    while (Head < Tail) {
        u= Pop();
        for (v=1;v<=n;v++)
            if ((graph[u][v])&&(Base[u]!=Base[v])&&(match[u]!= v))
                if ((v == Start)||((match[v] > 0)&&(Father[match[v]] > 0)))
                    BlossomContract(u, v);
                else if (Father[v] == 0) {
                    Father[v]=u;
                    if (match[v] > 0)
                        Push(match[v]);
                    else {
                        Finish=v;
                        return;
                    }
                }
    }
}
void AugmentPath() {
    int u,v,w;
    u=Finish;
    while(u > 0) {
        v=Father[u]; w=match[v];
        match[v]= u; match[u]= v; u = w;
    }
}
void Edmonds() {
    int u;
    memset(match,0,sizeof(match));
    for(u=1;u<=n;u++)
        if (match[u]==0)
        {
            Start=u;
            FindAugmentingPath();
            if (Finish > 0) AugmentPath();
        }
}
void PrintMatch() {
    int u;
    for(u=1;u<=n;u++)
        if (match[u] > 0) Count++; 
    printf("%d\n",Count);
    for(u=1;u<=n;u++)
        if (u < match[u])printf("%d %d\n",u,match[u]);
}
int main() {
    CreateGraph();
    Edmonds();
    PrintMatch();
}
