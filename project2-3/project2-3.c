#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<inttypes.h>
#include<math.h>
#include<stdbool.h>

#define P 19

typedef struct node{
    uint64_t key;
    uint64_t value;
    char status;
}Node;

uint64_t hash_func(int function, uint64_t key, int tablesize); 
void rehash();
int64_t find(struct node ht1[], struct node ht2[],uint64_t key,int64_t tablesize);
void insert(struct node ht1[], struct node ht2[], int64_t tablesize, int64_t count, uint64_t key, uint64_t val,int flag,FILE *file);
int64_t remove(struct node ht1[], struct node ht2[], int64_t tablesize, uint64_t key);
void printTable(struct node ht1[],struct node ht2[], int64_t tablesize);

int param1=2, param2=29;
int64_t tablesize;
uint64_t pos[2];

int main(int argc, char *argv[]){
    int64_t N, i;
    char buffer[255];
    int flag = 1;
    FILE* fpR = NULL;
    fpR = fopen(argv[1],"r");

    N = atoi(fgets(buffer, sizeof(buffer), fpR));
    tablesize = N*N;
    //printf("tablesize : %d\n", tablesize);

    Node *table1;
    Node *table2;

    table1 = (Node *)malloc(sizeof(Node)*tablesize);
    table2 = (Node *)malloc(sizeof(Node)*tablesize);

    for(i=0; i<tablesize; i++){
            table1[i].key=0;
            table2[i].key=0;
            table1[i].value=0;
            table2[i].value=0;
            table1[i].status='x';
            table2[i].status='x';
    }

    FILE* fpW = NULL;
    fpW = fopen("output.txt","w");

    uint64_t k,v;
    int64_t result = 0;
    if(fpR != NULL){
        while((fgets(buffer,sizeof(buffer),fpR))!=NULL){
            char ch = buffer[0];
            char *tok = strtok(buffer," ");
            switch(ch){
                case 'I' : tok = strtok(NULL," ");
                           k = atoi(tok);
                           tok = strtok(NULL," ");
                           v = atoi(tok);
                           //printf("Inserted%"PRIu64": %"PRIu64"\n",k, v);
                           insert(table1, table2, tablesize,0, k, v,flag,fpW);
                           //printTable(table1, table2, tablesize);
                           //if(result==-2){
                           //    fprintf(fpW, "Duplicated\n");
                          // }
                           //else if(result>-1){
                           //    fprintf(fpW, "Inserted\n");
                           //}
                           break;
                case 'F' : tok = strtok(NULL," ");
                           k = atoi(tok);
                           result = find(table1, table2, k, tablesize);
                           if(result>0)
                               fprintf(fpW, "%"PRId64"\n", result);
                           else
                               fprintf(fpW, "Not Found\n");
                           break;
                case 'D' : tok = strtok(NULL," ");
                           k = atoi(tok);
                           result = remove(table1, table2, tablesize,k);
                           if(result>0)
                               fprintf(fpW, "Deleted\n");
                           else
                               fprintf(fpW, "Not Found\n");
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
    printTable(table1, table2, tablesize);
    free(table1);
    free(table2);
    //rehash();
    //printf("Param 1: %d,Param2 : %d,tablesize : %"PRId64"\n",param1, param2,tablesize);
    return 0;
}


uint64_t hash_func(int function, uint64_t key, int64_t tablesize){
    switch (function) {
        case 1: return ((param1*key)+param2) % tablesize;
        case 2: return (((param1*key)+param2)%P) % tablesize;
    }
}

void rehash(){
    tablesize = tablesize * tablesize;
    param1 = rand()%99;
    param2 = rand()%99;
}


int64_t find(struct node ht1[], struct node ht2[],uint64_t key,int64_t tablesize){
    uint64_t a,b;
    a = hash_func(1, key, tablesize);
    b = hash_func(2, key, tablesize);
    //file out 
    if(ht1[a].key == key){
       //printf("%"PRIu64"\n",ht1[a].value);
       return ht1[a].value;
    }else if(ht2[b].key==key){
       //printf("%"PRIu64"\n",ht2[b].value);
       return ht2[b].value;
    }
    else{
        return -1;
    }
}

void insert(struct node ht1[], struct node ht2[], int64_t tablesize, int64_t count, uint64_t key, uint64_t val,int flag,FILE *file){
    if(count == tablesize){
        printf("%"PRIu64"could not inserted\nRecycle existed.\nI will rehash.\n", key);
        rehash();
        return;
    } 
    //printf("%"PRIu64"\t%"PRIu64"\n",key,val);
    pos[0] = hash_func(1, key, tablesize);
    pos[1] = hash_func(2, key, tablesize);
    //printf("FLAG : %d\n",flag);
    //printf("%"PRIu64"\t%"PRIu64"\n",pos[0],pos[1]);
    if(ht1[pos[0]].key == key){
        ht1[pos[0]].value == val;
        fprintf(file,"Duplicated\n");
        return;
    }else if(ht2[pos[1]].key ==key){
        ht2[pos[1]].value == val;
        fprintf(file,"Duplicated\n");
        return;
    }

    if(flag == 1){
        if(ht1[pos[0]].status != 'x' && ht1[pos[0]].key != key){
            uint64_t tempkey, tempval;
            tempkey = ht1[pos[0]].key;
            tempval = ht1[pos[0]].value;
            //printf("%"PRIu64" ^ %"PRIu64"\n", tempkey, tempval);
            ht1[pos[0]].key=key;
            ht1[pos[0]].value = val;
            ht1[pos[0]].status = 'o';
            //printf("%"PRIu64" ^^ %"PRIu64"\n", ht1[pos[0]].key, ht1[pos[0]].value);
            //printf("FLAG@ : %d\n",flag);
            printf("Collision In table 1 : KEY %"PRIu64" with KEY %"PRIu64"\n" ,tempkey, key);
            insert(ht1,ht2,tablesize, count+1,tempkey, tempval,flag*-1,file);
        }
        else{
            ht1[pos[0]].key = key;
            ht1[pos[0]].value = val;
            ht1[pos[0]].status ='o';
            fprintf(file,"Inserted\n");
            //printf("FLAG@@ : %d\n",flag);
        }
    }
    else if(flag == -1){
        if(ht2[pos[1]].status != 'x'&&ht2[pos[1]].key!=key){
            uint64_t tempkey, tempval;
            tempkey = ht2[pos[1]].key;
            tempval = ht2[pos[1]].value;
            ht2[pos[1]].key=key;
            ht2[pos[1]].value = val;
            ht2[pos[1]].status = 'o';
            //printf("FLAG@@@ : %d\n",flag);
            printf("Collision In table 2 : KEY %"PRIu64" with KEY %"PRIu64"\n", tempkey, key);
            insert(ht1,ht2,tablesize, count+1,tempkey, tempval,flag*-1,file);
        }
        else{
            ht2[pos[1]].key = key;
            ht2[pos[1]].value = val;
            ht2[pos[1]].status ='o';
            fprintf(file,"Inserted\n");
            //printf("FLAG@@@@ : %d\n",flag);
            }
        }
}

int64_t remove(struct node ht1[], struct node ht2[], int64_t tablesize, uint64_t key){
    uint64_t a,b;
    a = hash_func(1, key, tablesize);
    b = hash_func(2, key, tablesize);
    if(ht1[a].key == key){
        ht1[a].key = 0;
        ht1[a].value = 0;
        ht1[a].status = 'x';
        //printf("Removed in 1\n");
        return 1;
    }else if(ht2[b].key==key){
        ht2[b].key = 0;
        ht2[b].value = 0;
        ht2[b].status = 'x';
        //printf("Removed in 2\n");
        return 1;
    }else{
        //printf("Not Found\n");
        return -1;
    }
}
    
    
void printTable(struct node ht1[], struct node ht2[],int64_t tablesize){
    int i;
    printf("Hash Table 1 \t\t Hash Table 2 \n");
    for(i=0; i<tablesize; i++){
        printf("%"PRIu64"-%"PRIu64"-%c---------------",ht1[i].key, ht1[i].value, ht1[i].status);
        printf("----- %"PRIu64"-%"PRIu64"-%c\t\t\t\n",ht2[i].key, ht2[i].value, ht2[i].status);
    }
}
