#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

#define BILLION 1000000000ULL

struct timespec start, end;
double elapsed_time;

struct Node* insert(struct Node* node, uint64_t key, uint64_t val,FILE *file);
struct Node* find(struct Node* node, uint64_t key,FILE *file);
uint64_t max(uint64_t a, uint64_t b);

struct Node{
    uint64_t key;
    uint64_t val;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
    int depth;
    const char* color;
};

struct Node* newNode(uint64_t key, uint64_t val){
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->key = key;
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->depth = 0;
    node->color = "red";
    printf("%s\n",node->color);
    return(node);
}

int depth(struct Node *N){
    if(N==NULL){
        return 0;
    }
    return N->depth;
}

uint64_t max(uint64_t a, uint64_t b){
    return (a>b)?a:b;
}

int getBalance(struct Node *N){
    if(N==NULL){
        return 0;
    }
    return depth(N->left) - depth(N->right);
}

struct Node *rightRotate(struct Node *N){
    struct Node *left = N->left;
    struct Node *left_right = left->right;

    left->right = N;
    N->left = left_right;

    N->depth = max(depth(N->left), depth(N->right))+1;
    left->depth = max(depth(left->left), depth(left->right))+1;

    return left;
}

struct Node *leftRotate(struct Node *N){
    struct Node *right = N->right;
    struct Node *right_left = right->left;

    right->left = N;
    N->right = right_left;

    N->depth = max(depth(N->left), depth(N->right))+1;
    right->depth = max(depth(right->left), depth(right->right))+1;

    return right;
}

struct Node* insert(struct Node* node, uint64_t key, uint64_t val,FILE *file){
    if(node == NULL){
        fprintf(file,"Inserted (%"PRIu64",%"PRIu64")\n",key, val);
        return(newNode(key,val));
    }
    if(key < node->key){
        node->left = insert(node->left, key, val,file);
        node->left->parent = node;
    }
    else if(key > node->key){
        node->right = insert(node->right, key, val,file);
        node->right->parent = node;
    }
    else if(key == node->key){
        fprintf(file,"Found (%"PRIu64",%"PRIu64") update v=%"PRIu64"\n",node->key,node->val, val);
        node->val = val;
    }
    else{
        fprintf(file,"Inserted (%"PRIu64",%"PRIu64")\n",key, val);
        return node;
    }

    node->depth = max(depth(node->left), depth(node->right)) + 1;

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
        if(node->left !=NULL){
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
        fprintf(file,"Found (%"PRIu64",%"PRIu64") on d=%d with c=%s\n",node->key,node->val,node->depth,node->color);
    }
}    

void print(struct Node *root,FILE *file){
    if(root != NULL){
        fprintf(file,"(%"PRIu64",%"PRIu64") ", root->key, root->val);
        print(root->left,file);
        print(root->right,file);
    }
}

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


