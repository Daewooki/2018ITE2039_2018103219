#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<inttypes.h>
#include<math.h>

#define A 3
#define B 42
#define P 101

uint64_t hash_func1(int, int, int, int64_t, uint64_t);
uint64_t hash_func2(int, int, int, int64_t, uint64_t);
int64_t insert(struct table ht[], int64_t, uint64_t, uint64_t);
int64_t find(struct table ht[],uint64_t, uint64_t, int64_t);
int64_t remove(struct table ht[], uint64_t, uint64_t,int64_t);
void printParam(struct table ht[],int64_t tablesize);
void printTable(struct table ht[],int64_t tablesize);

typedef struct table{
    int m;
    int x;
    int y;
    struct node *table2;
}Table;

typedef struct node{
    uint64_t key;
    uint64_t value;
    char status;
}Node;

int main(int argc, char*argv[]){
    int64_t tablesize,i;
    char buffer[255];

    FILE* fpR1 = NULL;
    fpR1 = fopen(argv[2],"r");
    int64_t temp;
    temp = atoi(fgets(buffer,sizeof(buffer),fpR1));
    tablesize=temp;
    //printf("%d\n",tablesize);
    
    Table *f_table;
    f_table = (Table *)malloc(sizeof(Table)*tablesize);
    //printf("%d %d %d\n",A,B,P);

    for(i=0; i<tablesize; i++){
        f_table[i].m=0;
        f_table[i].x=0;
        f_table[i].y=0;
    }

    for(i=0; i<tablesize;i++){
        uint64_t kz = atoi(fgets(buffer,sizeof(buffer),fpR1));
        uint64_t h = hash_func1(A,B,P,tablesize,kz);
        f_table[h].m+=1;
        //printf("K:%"PRIu64"\n",h);
    }

    for(i=0; i<tablesize; i++){
        f_table[i].m=f_table[i].m*f_table[i].m;
        if(f_table[i].m>1){
            f_table[i].x=rand()%30;
            f_table[i].y=rand()%30;
        }
        f_table[i].table2 = (Node *)malloc(sizeof(Node)*f_table[i].m);
        for(int j=0; j<f_table[i].m; j++){
            f_table[i].table2[j].key = 0;
            f_table[i].table2[j].value = 0;
            f_table[i].table2[j].status = 'x';
        }
    }
    
    fclose(fpR1);
    FILE* fpR2 = NULL;
    fpR2 = fopen(argv[1],"r");

    FILE* fpW = NULL;
    fpW = fopen("output.txt","w");
    uint64_t k,v;
    int64_t result=0;
    uint64_t t;
    if(fpR2!=NULL){
        while((fgets(buffer,sizeof(buffer),fpR2))!=NULL){
            char ch = buffer[0];
            char *tok = strtok(buffer," ");
            switch(ch){
                case 'I' : tok = strtok(NULL," ");
                           k = atoi(tok);
                           tok = strtok(NULL," ");
                           v = atoi(tok);
                           //printf("%"PRIu64" %"PRIu64"\n",k,v);
                           result = insert(f_table, tablesize, k, v);
                           if(result>0){
                               fprintf(fpW,"Inserted\n");
                           }
                           else if(result==-1){
                               fprintf(fpW,"Duplicated\n");
                           }
                           else if(result==-2){
                               fprintf(fpW,"Table is Full\n");
                           }
                           break;
                case 'F' : tok = strtok(NULL," ");
                           k = atoi(tok);
                           t = hash_func1(A, B, P, tablesize, k);
                           result = find(f_table, t, k, tablesize);
                           if(result>-1){
                               fprintf(fpW, "%"PRIu64"\n", f_table[t].table2[result].value);
                           }
                           else{
                               fprintf(fpW,"Not Found\n");
                           }
                           break;
                case 'D' : tok = strtok(NULL," ");
                           k = atoi(tok);
                           t = hash_func1(A, B, P, tablesize, k);
                           result = remove(f_table, t, k, tablesize);
                           if(result>-1){
                               fprintf(fpW, "Deleted\n");
                           }
                           else{
                               fprintf(fpW, "Not Found\n");
                           }
                           break;
               case 'Q' : break;
            }
        }
        fclose(fpR2);
    }
    else{
        printf("File Read Eroor!\n");
    }
    fclose(fpW);
    printParam(f_table, tablesize);
    printTable(f_table, tablesize);
    free(f_table->table2);
    free(f_table);
    return 0;
}

uint64_t hash_func1(int a, int b, int p, int64_t tablesize, uint64_t key){
    uint64_t h;
    h = ((a*key+b)%p)%tablesize;
    return h;
}

uint64_t hash_func2(int a, int b, int p, int64_t m, uint64_t key){
    uint64_t h;
    h = ((a*key+b)%p)%m;
    return h;
}

int64_t insert(struct table ht[], int64_t tablesize, uint64_t key, uint64_t val){
    uint64_t h1, h2, htemp;
    h1 = hash_func1(A,B,P,tablesize,key);
    htemp = h2 = hash_func2(ht[h1].x,ht[h1].y,P,ht[h1].m,key);
    double d = sqrt((double)ht[h1].m);
    int64_t temp = d;
    //printf("index:%"PRIu64"\t%.2f\n",h1,d);

    while(ht[h1].table2[h2].status!='x'){
        if(ht[h1].table2[h2].key == key){
            //printf("Duplicated & Changed Value!\n");
            ht[h1].table2[h2].value = val;
            return -1;
        }
        h2 = (h2+1) % ht[h1].m;
        if(h2 == htemp){
            //printf("No are to insert!\n");
            return -2;
        }
    }
    ht[h1].table2[h2].key = key;
    ht[h1].table2[h2].value= val;
    ht[h1].table2[h2].status='o';
    return 1;
}

int64_t find(struct table ht[], uint64_t t, uint64_t key, int64_t tablesize){
    uint64_t h1,h2;
    h2 = h1 = hash_func2(ht[t].x,ht[t].y,P,ht[t].m,key);
    while(ht[t].table2[h1].status!='x'){
        if(ht[t].table2[h1].key == key){
            return h1;
        }
        h1=hash_func2(ht[t].x,ht[t].y+1,P,ht[t].m,key);
        if(h1==h2){
            return -1;
        }
    }
    return -1;
}

int64_t remove(struct table ht[], uint64_t t, uint64_t key, int64_t tablesize){
    int64_t f1;
    f1 = find(ht,t,key,tablesize);
    if(f1<0){
        return -1;
    }
    else{
        ht[t].table2[f1].key = 0;
        ht[t].table2[f1].value=0;
        ht[t].table2[f1].status='x';
        return f1;
    }
}

void printParam(struct table ht[], int64_t tablesize){
    int64_t i;
    printf("hash1 Parameter:\n");
    printf("m\tx\ty\n");
    for(i=0; i<tablesize; i++){
        printf("%d\t%d\t%d\n",ht[i].m,ht[i].x,ht[i].y);
        //for(int j=0; j<ht[i].m;j++){
        //printf("%d\t%d\t%d\t%"PRIu64"\n",ht[i].m,ht[i].x,ht[i].y,ht[i].value[j]);
        //}
    }
}
void printTable(struct table ht[], int64_t tablesize){
    int64_t i;
    printf("index\tkey-Value-status: \n");
    for(i = 0; i<tablesize; i++){
        printf("%"PRId64"\t",i);
        for(int j=0; j<ht[i].m;j++){
            printf("%"PRIu64"-%"PRIu64"-%c\t",ht[i].table2[j].key,ht[i].table2[j].value,ht[i].table2[j].status);
        }
        printf("\n");
    }
}
