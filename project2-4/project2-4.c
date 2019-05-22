#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

uint64_t hash_func(uint64_t key, int64_t tablseize);
void find(struct node ht[], int64_t tablesize, uint64_t key,FILE *fpW);
uint64_t findempty(struct node ht[], int64_t tablesize, uint64_t key);
int64_t insert(struct node ht[], int64_t tablesize, uint64_t k, uint64_t v,FILE *fpW);
void remove(struct node ht[], int64_t tablesize, uint64_t key, FILE *fpW);
void printBitmap(struct node ht[], int64_t tablesize, int bitmapsize);
void printTable(struct node ht[], int64_t tablesize);
int BitSize = 32;
//int a = 3, b=42, p=101;
int a = 1, b=0, p=1;
typedef struct node{
    uint64_t key;
    uint64_t value;
    char status;
    struct hop *map;
}Node;

typedef struct hop{
    int bit;
}Hop;

int main(int argc, char *argv[]){
    int64_t N, i, tablesize;
    char buffer[255];

    FILE* fpR = NULL;
    fpR = fopen(argv[1],"r");

    N = atoi(fgets(buffer, sizeof(buffer), fpR));
    tablesize = N*3;

    Node *hash_table;
    hash_table = (Node *)malloc(sizeof(Node)*tablesize);
    for(i=0; i<tablesize; i++){
        hash_table[i].key=0;
        hash_table[i].value =0;
        hash_table[i].status = 'x';
        hash_table[i].map = (Hop *)malloc(sizeof(Hop)*BitSize);
        for(int j=0; j<BitSize; j++){
            hash_table[i].map[j].bit = 0;
        }
    }
    
    FILE *fpW = NULL;
    fpW=fopen("output.txt","w");

    uint64_t k, v;
    if(fpR!=NULL){
        int64_t temp;
        while((fgets(buffer, sizeof(buffer), fpR))!=NULL){
            char ch = buffer[0];
            char *tok = strtok(buffer," ");
            switch(ch){
                case 'I' : tok = strtok(NULL," ");
                           k = atoi(tok);
                           tok = strtok(NULL," ");
                           v = atoi(tok);
                           insert(hash_table, tablesize, k, v,fpW);
                           //printBitmap(hash_table, tablesize, BitSize);
                           //printTable(hash_table, tablesize);
                           break;

                case 'F' : tok = strtok(NULL," ");
                           k = atoi(tok);
                           find(hash_table, tablesize, k, fpW);
                           break;
                case 'D' : tok = strtok(NULL," ");
                           k = atoi(tok);
                           remove(hash_table, tablesize, k, fpW);
                           break;
                case 'Q' : break;
            }
        }
        fclose(fpR);
    }
    else{
        printf("FILE READ ERROR!\n");
    }

    fclose(fpW);

    printBitmap(hash_table, tablesize, BitSize);

    printTable(hash_table, tablesize);
    return 0;
}

uint64_t hash_func(uint64_t key, int64_t tablesize){
    uint64_t h1;
    //h1 = (((a * key) + b)%p)%tablesize;
    h1 = ((a*key)+b)%tablesize;
    return h1;
}

void find(struct node ht[], int64_t tablesize, uint64_t key, FILE *fpW){
    uint64_t h1,h2;
    h2 = h1 = hash_func(key, tablesize);
    while(ht[h1].status!='x'){
        if(ht[h1].key == key){
            fprintf(fpW,"%"PRIu64"\n",ht[h1].value);
            return;
        }else{
            h1 = (h1 +1)%tablesize;
            if(h2 ==h1){
                fprintf(fpW,"Not Found\n");
                return;
            }
        }
    }
    fprintf(fpW,"Not Found\n");
    return;
}


uint64_t findempty(struct node ht[], int64_t tablesize,uint64_t key){
    uint64_t h1;
    //h1 = (((a*key)+b)%p)%tablesize;
    h1 = ((a*key)+b)%tablesize;
    while(ht[h1].status == 'o'){
        if(ht[h1].key == key){
            return h1;
        }else{
        h1 = (h1+1)%tablesize;
        if(ht[h1].key != key && ht[h1].status == 'x'){
            return h1;
        }}
    }
    return h1;
}

void remove(struct node ht[], int64_t tablesize, uint64_t key, FILE *fpW){
    uint64_t h1, h2, h3;
    h3 = h2 = h1 = hash_func(key, tablesize);
    int64_t count = 0;
    printf("Delete : %"PRIu64"\n",h1);
    while(ht[h1].status!='x'){
        if(ht[h1].key == key){
            ht[h1].key = 0;
            ht[h1].value=0;
            ht[h1].status='x';
            ht[h3].map[count].bit=0;
            fprintf(fpW,"Removed\n");
            return;
        }
        h1 = (h1+1)%tablesize;
        if(h2==h1){
            fprintf(fpW,"Not Found\n");
            return;
        }
    }
    fprintf(fpW,"Not Found\n");
    return;
        
}

int64_t insert(struct node ht[], int64_t tablesize, uint64_t key, uint64_t val,FILE *fpW){
    uint64_t h1;
    int idx;
    printf("Input key : %"PRIu64"\n",key);
    h1 = hash_func(key, tablesize);
    printf("%"PRIu64"\n", h1);
    uint64_t empty = findempty(ht, tablesize, key);
    printf("%"PRIu64"\n",empty);
    int gap = empty-h1;
    int presite = empty-BitSize+1;
    while(gap>BitSize-1){
        for(int idx = 0; idx<BitSize; idx++){
            if(ht[presite].map[idx].bit=1){
                uint64_t tkey, tval;
                char tstatus;
                tkey = ht[presite+idx].key;
                tval = ht[presite+idx].value;
                tstatus = ht[presite+idx].status;
                ht[presite+idx].key = ht[empty].key;
                ht[presite+idx].value = ht[empty].value;
                ht[presite+idx].status = ht[empty].status;
                ht[presite].map[idx].bit=0;
                ht[empty].key = tkey;
                ht[empty].value = tval;
                ht[empty].status = tstatus;
                ht[presite].map[BitSize-1].bit=1;
                int down = BitSize-1-idx;
                gap = gap-down;
                fprintf(fpW, "Gap is bigger than Hop size, Swap! \n");
                if(gap<=BitSize-1){
                  //  ht[h1+gap].key=key;
                  //  ht[h1+gap].value = val;
                  //  ht[h1+gap].status='o';
                  //  ht[h1].map[gap].bit=1;
                    break;
                }
            }
        }

    }
    if(gap<=BitSize-1&&ht[h1].key == key){
        fprintf(fpW,"Duplicated! Value Changed.\n");
        ht[h1].value = val;
        return -1;
    }
    else if(gap<=BitSize-1 && ht[h1].key != key &&ht[h1].status=='o'){
        fprintf(fpW,"Collision! but gap is smaller than hop size. Inserted\n");
        ht[h1+gap].key=key;
        ht[h1+gap].value=val;
        ht[h1+gap].status='o';
        ht[h1].map[gap].bit = 1;
        return 1;
    }
    else{
        fprintf(fpW,"Inserted \n");
        ht[h1+gap].key=key;
        ht[h1+gap].value = val;
        ht[h1+gap].status='o';
        ht[h1].map[gap].bit=1;
        return 1;
    }
}

void printBitmap(struct node ht[], int64_t tablesize, int bitmapsize){
    printf("=============================== Hop Information =================================\n");
    for(int i=0; i<tablesize; i++){
        printf("Node index %d : ", i);
        for(int j=0; j<bitmapsize; j++){
            printf("%d ",ht[i].map[j].bit);
        }
        printf("\n");
    }
    printf("\n\n");
}
void printTable(struct node ht[], int64_t tablesize){
    int i=0;
    printf("+++++++++TABLE+++++++++\n");
    printf("|KEY\tVALUE\tSTATUS|\n");
    for(i = 0; i<tablesize; i++){
        printf("%"PRIu64"\t%"PRIu64"\t%c\n",ht[i].key, ht[i].value, ht[i].status);
    }
}
