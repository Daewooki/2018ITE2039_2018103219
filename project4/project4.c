#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h> 
#include <stdlib.h> 
#include <inttypes.h>
#include <limits.h>

//List
typedef struct ListNode{
    uint64_t nextNode;
    uint64_t weight;
    ListNode* next;
}ListNode;

typedef struct List{
    ListNode *head;
}List;
//graph
typedef struct Graph{
    uint64_t Vertex;
    List* list;
}Graph;
//heap
typedef struct HeapNode{
    uint64_t  num;
    uint64_t dist;
}HeapNode;

typedef struct Heap{
    uint64_t curSize;
    uint64_t vertexSize;
    uint64_t *pos;
    HeapNode **list;
}Heap;

void addEdge(Graph* graph, uint64_t src, uint64_t nextNode, uint64_t weight);
ListNode* addListNode(uint64_t Node, uint64_t weight);
HeapNode* newHeapNode(uint64_t num, uint64_t dist);
Graph* initGraph(uint64_t Node);
Heap* initHeap(uint64_t vertexSize);
void Heapify(Heap* heap, uint64_t index);
void swap(HeapNode** a, HeapNode** b);
bool existedHeap(Heap *heap, uint64_t num);
bool isEmpty(Heap* heap);
HeapNode* extractMinDist(Heap* heap);
void updateDist(Heap* heap, uint64_t num, uint64_t dist);
uint64_t dijkstra(Graph* graph, uint64_t src, uint64_t goal);

int main(int argc, char *argv[]){
    uint64_t Vertex,E,N,H;
    char buffer[255];
    FILE* fpR = NULL;
    fpR = fopen(argv[1],"r");
    FILE* fpW = NULL;
    fpW = fopen("output.txt","w");

    Vertex = atoi(fgets(buffer, sizeof(buffer), fpR));
    E = atoi(fgets(buffer, sizeof(buffer), fpR));
    //printf("%"PRIu64",%"PRIu64"\n",Vertex,E);
    
    Graph* graph = initGraph(Vertex);

    for(int i=0; i<E; i++){
        uint64_t v1, v2, w;
        fgets(buffer, sizeof(buffer), fpR);
        char *tok = strtok(buffer," ");
        v1 = atoi(tok);
        //printf("%"PRIu64",",v1);
        tok = strtok(NULL," ");
        v2 = atoi(tok);
        //printf("%"PRIu64",",v2);
        tok = strtok(NULL," ");
        w = atoi(tok);
        //printf("%"PRIu64"\n",w);
        addEdge(graph, v1, v2, w);
    }
    //graph complete.
    //read # of problem, read two lines - iterative
    N = atoi(fgets(buffer,sizeof(buffer), fpR));
    
    for(int i=0; i<N; i++){
        uint64_t min = UINT64_MAX;
        uint64_t H = atoi(fgets(buffer,sizeof(buffer),fpR));
        uint64_t *array = (uint64_t*)malloc(sizeof(uint64_t)*H);
        fgets(buffer,sizeof(buffer), fpR);
        char *tok = strtok(buffer," ");
        array[0] = atoi(tok);
        for(int j=1; j<H; j++){
            tok = strtok(NULL," ");
            array[j] = atoi(tok);
        }
        for(int k=0; k<H; k++){
            for(int l=k+1; l<H; l++){
                //printf("%d,%d,%"PRIu64",%"PRIu64"\n",k,l,array[k],array[l]);
                uint64_t dist = dijkstra(graph,array[k],array[l]);
                if(min>dist){
                    min = dist;
                }
            }
        }
        fprintf(fpW,"%"PRIu64"\n",min);
        free(array);
    }
    fclose(fpR);
    return 0;

}

Graph* initGraph(uint64_t Node){
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->Vertex = Node;
    graph->list = (List*)malloc(Node*sizeof(List));

    for (int i = 0; i < Node; ++i){
        graph->list[i].head = NULL;
    }
    return graph;
}

void addEdge(Graph* graph, uint64_t src, uint64_t nextNode, uint64_t weight){
    ListNode* newNode = addListNode(nextNode, weight);
    newNode->next = graph->list[src].head;
    graph->list[src].head = newNode;
    newNode = addListNode(src, weight);
    newNode->next = graph->list[nextNode].head;
    graph->list[nextNode].head = newNode;
}

ListNode* addListNode(uint64_t Node, uint64_t weight){
    ListNode* newNode=(ListNode*)malloc(sizeof(ListNode));
    newNode->nextNode = Node;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

HeapNode* newHeapNode(uint64_t num, uint64_t dist){
    HeapNode* heapNode=(HeapNode*) malloc(sizeof(HeapNode));
    heapNode->num = num;
    heapNode->dist = dist;
    return heapNode;
}
Heap* initHeap(uint64_t vertexSize){
    Heap* heap= (Heap*) malloc(sizeof(Heap));
    heap->pos = (uint64_t *)malloc(vertexSize * sizeof(uint64_t));
    heap->curSize = 0;
    heap->vertexSize = vertexSize;
    heap->list=(HeapNode**) malloc(vertexSize * sizeof(HeapNode*));
    return heap;
}

void Heapify(Heap* heap, uint64_t index){
    int left, right;
    int min = index;

    left = (2*index)+1;
    right = (2*index)+2;

    if (left<heap->curSize && heap->list[left]->dist < heap->list[min]->dist){
        min = left;
    }
    if (right<heap->curSize && heap->list[right]->dist < heap->list[min]->dist){
        min = right;
    }
    if (min != index){
        HeapNode *minNode = heap->list[min];
        HeapNode *indexNode = heap->list[index];

        heap->pos[minNode->num] = index;
        heap->pos[indexNode->num] = min;

        swap(&heap->list[min], &heap->list[index]);
        Heapify(heap, min);
    }
}

HeapNode* extractMinDist(Heap* heap){
    if (isEmpty(heap)==true){
        return NULL;
    }
    HeapNode* rootNode = heap->list[0];
    HeapNode* lastNode = heap->list[heap->curSize-1];
    heap->list[0] = lastNode;
    heap->pos[rootNode->num] = heap->curSize-1;
    heap->pos[lastNode->num] = 0;
    heap->curSize-=1;
    Heapify(heap, 0);
    return rootNode;
}

void updateDist(Heap* heap, uint64_t num, uint64_t dist){
    uint64_t curIndex = heap->pos[num];
    heap->list[curIndex]->dist = dist;

    while (curIndex && heap->list[curIndex]->dist < heap->list[(curIndex-1)/2]->dist){
        heap->pos[heap->list[curIndex]->num] = (curIndex-1)/2;
        heap->pos[heap->list[(curIndex-1)/2]->num] = curIndex;
        swap(&heap->list[curIndex], &heap->list[(curIndex-1)/2]);
        curIndex = (curIndex-1)/2;
    }
}

void swap(HeapNode** a, HeapNode** b){
    HeapNode* t = *a;
    *a = *b;
    *b = t;
}

bool existedHeap(Heap *heap, uint64_t num){
    if (heap->pos[num] < heap->curSize){
        return true;
    }
    return false;
}

bool isEmpty(Heap* heap){
    if(heap->curSize == 0){
        return true;
    }else{
        return false;
    }
}

uint64_t dijkstra(Graph* graph, uint64_t src, uint64_t goal){
    uint64_t vertex = graph->Vertex;
    uint64_t* dist = (uint64_t*)malloc(vertex * sizeof(uint64_t));
    Heap* heap = initHeap(vertex);

    for (int i = 0; i < vertex; i++){
        dist[i] = UINT64_MAX;
        heap->list[i] = newHeapNode(i, dist[i]);
        heap->pos[i] = i;
    }

    heap->list[src] = newHeapNode(src, dist[src]);
    heap->pos[src] = src;
    dist[src] = 0;
    updateDist(heap, src, dist[src]);
    heap->curSize = vertex;

    while (!isEmpty(heap)){
        HeapNode* heapNode = extractMinDist(heap);
        uint64_t u = heapNode->num;

        ListNode* curNode = graph->list[u].head;
        while (curNode != NULL)
        {
            uint64_t v = curNode->nextNode;
            if (existedHeap(heap, v) && dist[u] != UINT64_MAX && curNode->weight + dist[u] < dist[v]){
                dist[v] = dist[u] + curNode->weight;
                updateDist(heap, v, dist[v]);
            }
            curNode = curNode->next;
        }
    }
    return dist[goal];
}
