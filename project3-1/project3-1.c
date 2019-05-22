#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

#define BILLION 1000000000ULL

struct timespec start, end;
double elapsed_time;

void print(struct Node *root, FILE *file);
uint64_t max(uint64_t a, uint64_t b);
struct Node* insert(struct Node* node, uint64_t key, uint64_t val,FILE *file);
struct Node* find(struct Node* node, uint64_t key,FILE *file);


struct Node{
    uint64_t key;
    uint64_t val;
    struct Node *left;
    struct Node *right;
    int height;
    int depth;
};

int main(int argc, char *argv[]){
    struct Node *root = NULL;
    char buffer[255];
    FILE* fpR = NULL;
    fpR = fopen(argv[1],"r");

    FILE* fpW = NULL;
    fpW = fopen("output.txt","w");

    uint64_t key, val;
    clock_gettime(CLOCK_MONOTONIC, &start);
    if(fpR != NULL){
        while((fgets(buffer,sizeof(buffer),fpR))!=NULL){
            char ch = buffer[0];
            char *tok = strtok(buffer," ");
            switch(ch){
                case 'I' : tok = strtok(NULL," ");
                           key = atoi(tok);
                           tok = strtok(NULL," ");
                           val = atoi(tok);
                           root = insert(root,key,val,fpW);
                           break;
                case 'F' : tok = strtok(NULL," ");
                           key = atoi(tok);
                           find(root,key,fpW);
                           break;
                case 'P' : print(root,fpW);
                           fprintf(fpW,"\n");
                           break;
                case 'Q' : break;
            }
        }
        fclose(fpR);
    }
    else{
        printf("File Read Error!");
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed_time = (end.tv_nsec - start.tv_nsec) + (end.tv_sec - start.tv_sec) * BILLION;
    printf("ELAPSED TIME : %f\n",elapsed_time);
    fclose(fpW);
    return 0;
}


struct Node* newNode(uint64_t key, uint64_t val){
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->key = key;
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    node->depth = 0;
    return(node);
}

uint64_t max(uint64_t a, uint64_t b){
    return (a > b)? a : b;
}

int height(struct Node *N){
    if (N==NULL){
        return 0;
    }
    return N->height;
}
int depth(struct Node *N){
    if(N==NULL){
        return 0;
    }
    return N->depth;
}

struct Node *rightRotate(struct Node *N){
    struct Node *left = N->left;
    struct Node *left_right = left->right;

    left->right = N;
    N->left = left_right;

    N->height = max(height(N->left), height(N->right))+1;
    left->height = max(height(left->left), height(left->right))+1;

    return left;
}

struct Node *leftRotate(struct Node *N){
    struct Node *right = N->right;
    struct Node *right_left = right->left;

    right->left = N;
    N->right = right_left;

    N->height = max(height(N->left), height(N->right))+1;
    right->height = max(height(right->left), height(right->right))+1;
    return right;
}

int getBalance(struct Node *N){
    if(N==NULL){
        return 0;
    }
    return height(N->left) - height(N->right);
}

struct Node* insert(struct Node* node, uint64_t key, uint64_t val,FILE *file){
    if(node == NULL){
        fprintf(file,"Inserted (%"PRIu64",%"PRIu64")\n",key, val);
        return(newNode(key,val));
    }
    if(key < node->key){
        node->left = insert(node->left, key, val,file);
    }
    else if(key > node->key){
        node->right = insert(node->right, key, val,file);
    }
    else if(key == node->key){
        fprintf(file,"Found (%"PRIu64",%"PRIu64") update v=%"PRIu64"\n",node->key,node->val, val);
        node->val = val;
    }
    else{
        fprintf(file,"Inserted (%"PRIu64",%"PRIu64")\n",key, val);
        return node;
    }
    
    node->height = max(height(node->left), height(node->right)) + 1;

    int balance = getBalance(node);

    if(balance > 1 && key < node->left->key){
        return rightRotate(node);
    }
    if(balance < -1 && key > node->right->key){
        return leftRotate(node);
    }
    if(balance > 1 && key > node->left->key){
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if(balance < -1 && key < node->right->key){
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

struct Node* find(struct Node* node, uint64_t key,FILE *file){
    if(node == NULL)
    {
        fprintf(file,"Not Found\n");
        return node;
    }
    if(key < node->key){
        if(node->left != NULL){
            node->left->depth++;
        }
        find(node->left, key,file);
    }
    else if(key>node->key){
        if(node->right != NULL){
            node->right->depth++;
        }
        find(node->right, key,file);
    }
    else if (key==node->key){
        fprintf(file,"Found (%"PRIu64",%"PRIu64") on d=%d with h=%d\n",node->key,node->val,node->depth,node->height-1);
    }
}    

void print(struct Node *root,FILE *file){
    if(root != NULL){
        fprintf(file,"(%"PRIu64",%"PRIu64") ", root->key, root->val);
        print(root->left,file);
        print(root->right,file);
    }
}


