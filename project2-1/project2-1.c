#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<inttypes.h>

typedef struct node{
    uint64_t key;
    uint64_t value;
    char status;
    uint64_t collision;
}Node;

uint64_t hash_func(uint64_t key, int tablesize);
int insert(struct node ht[], int tablesize, uint64_t key, uint64_t val);
int find(struct node ht[], uint64_t key, int tablesize);
int remove(struct node ht[], uint64_t key, int size);
void printTable(struct node ht[], int tablesize);

int main(int argc, char *argv[]){
    int N, i, temp, tablesize;
    char buffer[255];

    FILE* fpR = NULL;
    fpR = fopen(argv[1],"r");

    N = atoi(fgets(buffer,sizeof(buffer),fpR));
    tablesize = N*3;
    //printf("%d\n", tablesize);

    Node *hash_table;
    hash_table=(Node *)malloc(sizeof(Node)*tablesize);
    for(i=0; i<tablesize; i++){
        hash_table[i].key = 0;
        hash_table[i].value = 0;
        hash_table[i].status='x';
        hash_table[i].collision = 0;
    }

    FILE* fpW = NULL;
    fpW = fopen("output.txt","w");

    uint64_t k,v;
    if(fpR != NULL){
        while((fgets(buffer,sizeof(buffer),fpR))!=NULL){
            char ch = buffer[0];
            char *tok = strtok(buffer," ");
            switch(ch){
                case 'I' : tok = strtok(NULL," ");
                           k = atoi(tok);
                           tok = strtok(NULL," ");
                           v = atoi(tok);
                           temp = insert(hash_table, tablesize, k, v);
                           if(temp>-1){
                               fprintf(fpW,"Inserted\n");}
                           if(temp==-1){
                               fprintf(fpW,"Duplicated\n");}
                           if(temp==-2){
                               fprintf(fpW,"Table is Full!\n");}
                           break;
                case 'F' : tok = strtok(NULL," ");
                           k = atoi(tok);
                           temp = find(hash_table, k, tablesize);
                           if(temp>-1){
                               fprintf(fpW,"%"PRIu64"\n",hash_table[temp].value);}
                           else{
                               fprintf(fpW,"Not Found\n");}
                           break;
                case 'D' : //printf("%s\n",tok);
                           tok = strtok(NULL," ");
                           k = atoi(tok);
                           temp = remove(hash_table, k, tablesize); 
                           if(temp>-1){
                               fprintf(fpW,"Deleted\n");}
                           else{
                               fprintf(fpW,"Not Found\n");}
                           break;
                case 'Q' : break;
            }
        }
        fclose(fpR);
    }
    else{
        printf("file read error!");
    }
    printTable(hash_table, tablesize);
    
    fclose(fpW);
    free(hash_table);
    return 0;
}

uint64_t hash_func(uint64_t key, int tablesize){
    uint64_t h;
    h = key % tablesize;
    return h;
}

int insert(struct node ht[], int tablesize, uint64_t key, uint64_t val){
    uint64_t k1, k2;
    k2 = k1 = hash_func(key,tablesize);
    //printf("key index:%"PRIu64"value:%"PRIu64"\n", key, val);
    //if(ht[k1].status == 'x'){
    //    ht[k1].key = k1;
    //    ht[k1].value = val;
    //    ht[k1].status ='o';
    //    printf("Inserted\n");
    //    return k1;
    //}
    while(ht[k1].status != 'x'){
        if(ht[k1].key == key){
            //printf("Duplicated\n");
            ht[k1].value = val;
            return -1;
        }
        ht[k1].collision = key;
        k1 = (k1+1) % tablesize;
        if(k1 == k2){
            //printf("No area to insert!\n");
            return -2;
        }
    }
    ht[k1].key=key;
    ht[k1].value = val;
    ht[k1].status='o';
    //printf("Inserted\n");
    return k1;
}

int find(struct node ht[], uint64_t key, int tablesize){
    int k1, k2;
    k2 = k1 = hash_func(key, tablesize);
    while(ht[k1].status!='x'){
        if(ht[k1].key == key){
            //printf("Find index%d:%"PRIu64"\n",k1, ht[k1].value);
            return k1;
        }
        k1 = (k1+1) % tablesize;
        if(k1 == k2) {
            return -1;
        }
    }
    return -1;
}

int remove(struct node ht[], uint64_t key, int tablesize){
    int temp;
    temp = find(ht, key, tablesize);
    if(temp<0){
        //printf("remove : Not Found\n");
        return -1;
    }
    else{
        ht[temp].key = 0;
        ht[temp].value = 0;
        ht[temp].status = 'x';
        return temp;
    }
}

void printTable(struct node ht[], int tablesize){
    int i;
    printf("Key\tValue\tStatus\tCollision\n");
    for(i = 0; i<tablesize; i++){
        printf("%"PRIu64"\t%"PRIu64"\t%c\tWith %"PRIu64"\n",ht[i].key, ht[i].value,ht[i].status,ht[i].collision);
    }
}
