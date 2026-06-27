#include <stdio.h>
#include <stdlib.h>

typedef long long ll;

#define MAXK 200005
#define HASH 1048576   // 2^20

typedef struct{
    int i, j, k;
    ll area;
} Node;

Node heap[MAXK];
int heapSize = 0;

ll a[200005];
ll b[200005];
ll c[200005];

typedef struct State{
    int i, j, k;
    struct State *next;
} State;

State *table[HASH];

ll area(int i,int j,int k){
    return 2LL*a[i]*b[j]
         + 2LL*b[j]*c[k]
         + 2LL*c[k]*a[i];
}

int better(Node x, Node y){

    if(x.area != y.area)
        return x.area > y.area;

    if(x.i != y.i)
        return x.i < y.i;

    if(x.j != y.j)
        return x.j < y.j;

    return x.k < y.k;
}

void swap(Node *x, Node *y){
    Node t=*x;
    *x=*y;
    *y=t;
}

void push(Node x){

    heap[++heapSize] = x;

    int p = heapSize;

    while(p > 1){

        int pai = p / 2;

        if(better(heap[p], heap[pai])){

            swap(&heap[p], &heap[pai]);
            p = pai;

        }else
            break;
    }
}

Node pop(){

    Node ans = heap[1];

    heap[1] = heap[heapSize--];

    int p = 1;

    while(1){

        int l = 2*p;
        int r = l+1;
        int best = p;

        if(l <= heapSize && better(heap[l], heap[best]))
            best = l;

        if(r <= heapSize && better(heap[r], heap[best]))
            best = r;

        if(best == p)
            break;

        swap(&heap[p], &heap[best]);
        p = best;
    }

    return ans;
}

int empty(){
    return heapSize == 0;
}

unsigned int hashValue(int i, int j, int k){

    unsigned int h = (unsigned int)i * 73856093u;

    h ^= (unsigned int)j * 19349663u;

    h ^= (unsigned int)k * 83492791u;

    return h % HASH;
}

int exists(int i, int j, int k){

    unsigned int h = hashValue(i, j, k);

    State *p = table[h];

    while(p != NULL){

        if(p->i == i && p->j == j && p->k == k)
            return 1;

        p = p->next;
    }

    return 0;
}

void insertState(int i, int j, int k){

    unsigned int h = hashValue(i, j, k);

    State *novo = (State*)malloc(sizeof(State));

    novo->i = i;
    novo->j = j;
    novo->k = k;

    novo->next = table[h];

    table[h] = novo;
}

void addState(int i, int j, int k){

    if(i <= 0 || j <= 0 || k <= 0)
        return;

    if(exists(i, j, k))
        return;

    insertState(i, j, k);

    Node x;

    x.i = i;
    x.j = j;
    x.k = k;

    x.area = area(i, j, k);

    push(x);
}

int main(){

    int n;
    int K;

    scanf("%d %d", &n, &K);

    for(int i = 1; i <= n; i++)
        scanf("%lld", &a[i]);

    for(int i = 1; i <= n; i++)
        scanf("%lld", &b[i]);

    for(int i = 1; i <= n; i++)
        scanf("%lld", &c[i]);

    Node start;

    start.i = n;
    start.j = n;
    start.k = n;
    start.area = area(n, n, n);

    push(start);
    insertState(n, n, n);

    Node ans;

    while(K--){

        ans = pop();

        addState(ans.i - 1, ans.j, ans.k);

        addState(ans.i, ans.j - 1, ans.k);

        addState(ans.i, ans.j, ans.k - 1);
    }

    printf("%d %d %d\n", ans.i, ans.j, ans.k);

    return 0;
}
