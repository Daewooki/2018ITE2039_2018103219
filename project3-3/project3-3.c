#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include <time.h>

#define MAX_LEVEL 10
#define BILLION 1000000000ULL

struct timespec start, end;
double elapsed_time;


typedef struct Node{
    uint64_t key;
    uint64_t val;
    struct Node **nextList;
}Node;

typedef struct SkipList{
    int level;
    struct Node *start;
    //struct Node *end;
}SkipList;

SkipList *initialize(SkipList *list){
    Node *start = (Node *)malloc(sizeof(struct Node));
    //Node *end = (Node *)malloc(sizeof(struct Node));
    list->start = start;
    //list->end = end;
    start->key = UINT64_MAX;
    start->val = UINT64_MAX;
    //end->key = 0;
    //end->val = 0;
    list->level = 1; //most bottom level is 1.

    start->nextList = (Node **)malloc(sizeof(Node *)*MAX_LEVEL);
    for(int i = 0; i<=MAX_LEVEL; i++){
        start->nextList[i] = list->start;
        start->nextList[i]->key = list->start->key;
        start->nextList[i]->val = list->start->val;
    }
    return list;
}

int TossUp(){
    return rand() < RAND_MAX/2;
}

int random_level(){
    int level = 1;
    while(TossUp()==1 && level < MAX_LEVEL){
        level++;
    }
    return level;
}

void insert(SkipList *list, uint64_t key, uint64_t val, FILE* file){
    Node *newNodeList[MAX_LEVEL+1];
    Node *current = list->start;

    for(int i=0; i<MAX_LEVEL; i++){
        newNodeList[i] = NULL;
    }

    for(int i=list->level; i>0; i--){
        while(1){
            if(current->nextList[i]->key < key){
                current = current->nextList[i];
            }
            else{
                newNodeList[i] = current;
                break;
            }
        }
    }
    current = current->nextList[1];

    int level=1;
    if(key == current->key){
        fprintf(file,"Found (%"PRIu64",%"PRIu64") update v = %"PRIu64"\n",current->key, current->val, val);
        current->val = val;
    }
    else{
        level = random_level();
        if(level > list->level){
            for(int i=list->level+1; i<=level; i++){
                newNodeList[i] = list->start;
            }
            list->level = level;
        }

        current = (Node *)malloc(sizeof(Node));
        current->key = key;
        current->val = val;
        current->nextList=(Node **)malloc(sizeof(Node*)*(level+1));

        for(int i=1; i<=level; i++){
            current->nextList[i] = newNodeList[i]->nextList[i];
            newNodeList[i]->nextList[i] = current;
        }
        fprintf(file,"Inserted (%"PRIu64",%"PRIu64")\n",key, val);
    }
}
void find(SkipList *list, uint64_t key, FILE* file){
    Node *node = list->start;
    for(int i = list->level; i>=1; i--){
        //printf("%d\n",i);
        while(1){
            if(node->nextList[i]->key < key){
                node = node->nextList[i];
            }else{
                break;
            }
        }
    }
    if(node->nextList[1]->key == key){
        fprintf(file,"Found (%"PRIu64",%"PRIu64")\n",key, node->nextList[1]->val);
    }
    else{
        fprintf(file,"Not Found\n");
    }
}

int main(int argc, char *argv[]){
    struct SkipList list;
    initialize(&list);
    char buffer[255];
    FILE* fpR = NULL;
    fpR = fopen(argv[1],"r");

    FILE* fpW = NULL;
    fpW = fopen("output.txt","w");
    //printf("%"PRIu64"\n",UINT64_MAX);
    uint64_t key, val;
    clock_gettime(CLOCK_MONOTONIC, &start);
    if(fpR != NULL){
        while((fgets(buffer,sizeof(buffer),fpR))!=NULL){
            char ch = buffer[0];
            char *tok = strtok(buffer, " ");
            switch(ch){
                case 'I' : tok = strtok(NULL," ");
                           key = atoi(tok);
                           tok = strtok(NULL," ");
                           val = atoi(tok);
                           insert(&list,key, val,fpW);
                           break;
                case 'F' : tok = strtok(NULL," ");
                           key = atoi(tok);
                           find(&list,key,fpW);
                           break;
                case 'Q' : break;
            }
        }
        fclose(fpR);
    }
    else{
        printf("File Read Error!\n");
    }
    fclose(fpW);
    clock_gettime(CLOCK_MONOTONIC,&end);
    elapsed_time = (end.tv_nsec - start.tv_nsec) + (end.tv_sec - start.tv_sec)*BILLION;
    printf("%f\n",elapsed_time);
    return 0;
}
