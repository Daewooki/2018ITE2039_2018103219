#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
void QuickSort(int *input,int start, int finish);
int Partition(int *input, int start, int finish);
void swap(int *a, int *b);

int num=0;
unsigned int i =0;
int *input;
int main(void){

//read file - first integer is # of array.
//read through, put number to array.
    FILE *input_file=fopen("input.txt","r");
    if(input_file!=NULL){
        fscanf(input_file,"%d",&num);
        if(num<0||num>100000){
            printf("Size must be in 1~100000\n");
            return -1;
        }
        input=(int*)malloc(sizeof(int)*num);
        for(i =0; i<num; i++){
            fscanf(input_file,"%d", &input[i]);   
        }
    }
    
    fclose(input_file);
    
    QuickSort(input,0,num-1);

//file write. 
    FILE *output_file=fopen("output.txt","w");
    for(i=0; i<num; i++){
        fprintf(output_file,"%d ",input[i]);
    }
    fprintf(output_file,"\n");
    fclose(output_file);
    free(input);
    return 0;
}

void QuickSort(int *input, int start, int finish){
    if(start<finish){
        int q = Partition(input, start, finish);
        QuickSort(input,start, q-1);
        QuickSort(input,q+1, finish);
    }
}

int Partition(int *input, int start, int finish){
    int x = input[finish];
    int y = start-1;
    for(int j = start; j<=finish-1; j++){
        if(input[j]<=x){
            y=y+1;
            swap(&input[y], &input[j]);
        }
    }
    swap(&input[y+1],&input[finish]);
    return y+1;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
