#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
int* QuickSort(int *,int,int);
void swap(int *, int *);

int num=0;
unsigned int i =0;
int main(void){
    int *input;

//read file - first integer is # of array.
//read through, put number to array.
    FILE *input_file=fopen("input.txt","r");
    if(input_file!=NULL){
        fscanf(input_file,"%d",&num);
        input=(int*)malloc(sizeof(int)*num);
        for(i =0; i<num; i++){
            fscanf(input_file,"%d", &input[i]);
            printf("%d@@@@@%d\n",i,input[i]);   
        }
    }
    fclose(input_file);

    QuickSort(input,0,num-1);

//file write. 
    FILE *output_file=fopen("output.txt","w");
    for(i=0; i<num; i++){
        fprintf(output_file,"%d ",input[i]);
    }
    fclose(output_file);
    return 0;
}

int* QuickSort(int *input, int start, int finish){
    printf("%d %d", start, finish);
    swap(&input[2],&input[4]);
    for(i =0; i<num; i++){
        printf("!!!%d\n",input[i]);
    }
    return input;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
