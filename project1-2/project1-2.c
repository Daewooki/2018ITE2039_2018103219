#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
void QuickSort(int *number, int start, int finish);
int Partition(int *number, int start, int finish);
void swap(int *a, int *b);

//N:number of arrays, M: size of each array, K=k-th
int N, M, K;
char filename[100];
int main(void) {
    clock_t start, end;
    float time;
    start = clock();
//parameter file read
    FILE *param = fopen("input.txt", "r");
    fscanf(param, "%d %d %d", &N, &M, &K);
    fclose(param);
    if (N<1 || N>256) {
        printf("N must be a number between 1 and 256\n");
        return -1;
    }
    if (M<1 || M>1000000) {
        printf("M must be a number between 1 and 1000000\n");
        return -1;
    }
    if (K<1 || K>(N*M)) {
        printf("K must be a number between 1 and N*M\n");
        return -1;
    }
    //printf("N:%d, M:%d, K:%d\n", N, M, K);
    //scan parameter complete.
    int size = N*M;
    // create array
    int onenum[M];
    int number[size];
    for (int i = 0; i < size; i++) {
        number[i] = 0;
    }
    FILE *fp;
    for(int i=0; i<N; i++){
        sprintf(filename,"input%d.txt",i+1);
        fp = fopen(filename,"r");
        if(fp==NULL){
            printf("FILE OPEN ERROR!\n");
        }else{
            for(int j=0; j<M; j++){
                fscanf(fp,"%d",&onenum[j]);
            }
        }
        for(int k=0; k<M; k++){
            number[M*i+k]=onenum[k];
        }
        fclose(fp);
    }
    QuickSort(number,0,size-1);
   // int multiple = 1;
   // while(1){
   //     if(multiple*K>size)
   //         break;
   //     printf("%d ",numbesr[multiple*K-1]);
   //     multiple++;
   // }
   // printf("\n");

    FILE *output_file = fopen("output.txt","w");
    int multiple = 1;
    while(1){
        if(multiple*K>size)
            break;
        fprintf(output_file,"%d ",number[multiple*K-1]);
        multiple++;
    }


//free memory
    fclose(output_file);
    end=clock();
    time = (float)(end-start)/CLOCKS_PER_SEC;
    printf("시간: %.9f\n",time);
    return 0;
}
void QuickSort(int *number, int start, int finish){
    if(start<finish){
        int q = Partition(number, start, finish);
        QuickSort(number,start, q-1);
        QuickSort(number,q+1,finish);
    }
}

int Partition(int *number, int start, int finish){
    int x = number[finish];
    int y = start-1;
    for(int j = start; j<=finish-1; j++){
        if(number[j]<=x){
            y=y+1;
            swap(&number[y],&number[j]);
        }
    }
    swap(&number[y+1], &number[finish]);
    return y+1;
}

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}
