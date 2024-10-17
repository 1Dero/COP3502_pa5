/*
Darel Gomez
COP3502
April 2, 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 25

typedef struct customer {
    char name[MAXLEN+1];
    int points;
} customer;

typedef struct bstnode {
    customer* custPtr;
    int size;
    struct bstnode* left;
    struct bstnode* right;
} bstnode;

//**Command Functions**//

// Precondition: A root for a binary search tree and a string of length 25 containing a customer's name, 
//               and the amount of points to be given.
// Postcondition: if the name already exists, adds the points to that customer.
//                else, creates a new customer structure with the given parameters, and inserts it into the tree.
//                Additionally, prints parameters
//                Returns 1 if a new node was added, else returns 0.
int add(bstnode** root, char* name, int points);

// Precondition: A root for a binary search tree and a string of length 25 containing the name of a customer that exists in the array, 
//               and the amount of points to be subtracted.
// Postcondition: Subtracts the given points from the customer's current points (can't go below 0).
//                Additionally, prints parameters
void sub(bstnode** root, char* name, int points);

// Precondition: A root for a binary search tree and a string of length 25 containing a customer's name, a flag variable set to 1 used to keep track of size
// Postcondition: if the customer exists in the tree, then it removes them and prints "<name> deleted".
//                else, it prints "<name> not found"
//                (if a node has two children, then it replaces with maximum node in left subtree)
bstnode* del(bstnode* root, char* name, int* flag);

// Precondition: A root for a binary search tree and a string of length 25 containing a customer's name
// Postcondition: if the customer exists in the tree, then it prints "<name> <points> <depth>"
//                else, it prints "<name> not found:"
void search(bstnode* root, char* name);

// Precondition: A root for a binary search tree and a string of length 25 containing a customer's name
// Postcondition: prints the number of names that come alphabetically before the given name
void count_smaller(bstnode* root, char* name);

//**Sort Functions**//

// Precondition: a pointer to an array, the left index, and the right index
// Postcondition: places the right index's element in its sorted index, and returns this index
int partition(customer* arr[], int left, int right);

// Precondition: a pointer to an array, the left index, and the right index
// Postconditon: a sorted array
void quickSort(customer* arr[], int left, int right);

//**Other Functions**//
// Precondition: A string of length 25 containing a customer's name, 
//               and the amount of points to be given.
// Postcondition: Returns a pointer to a customer initalized with the given parameters.
customer* initialize_customer(char* name, int points);

// Precondition: A string of length 25 containing a customer's name, 
//               and the amount of points to be given.
// Postcondition: Returns an initialized node containing a customer initialized with the given parameters.
bstnode* initialize_node(char* name, int points);

// Precondition: A pointer to an empty array, a tree root, and a pointer to the current index of the array.
// Postcondition: Returns an inorder array of the binary tree
void fillArray(customer** arr, bstnode* root, int* i);

// Precondition: A pointer to a node in a binary tree
// Postcondition: Returns the maximum node
bstnode* findMax(bstnode* node);

// Precondition: Two pointers to pointers storing a customer
// Postcondition: The two pointers are swapped
void swap(customer** a, customer** b) {
    customer* temp = *a;
    *a = *b;
    *b = temp;
}

/**Free Functions**/
// Precondition: root of a tree
// Postcondition: frees the memory allocated to the binary tree
void freeBinaryTree(bstnode* root) {
    if(root != NULL) {
        freeBinaryTree(root->left);
        freeBinaryTree(root->right);
        free(root->custPtr);
        free(root);
    }
}

int main() {
    int n; // number of commands
    scanf("%d", &n);

    bstnode* root = NULL;

    char command[MAXLEN+1];
    char name[MAXLEN+1];
    int points;
    for(int i = 0; i < n; i++) {
        scanf("%s", command);
        if(!strcmp(command, "add")) {
            scanf("%s %d", name, &points);
            add(&root, name, points);
        }
        else if(!strcmp(command, "sub")) {
            scanf("%s %d", name, &points);
            sub(&root, name, points);
        }
        else if(!strcmp(command, "del")) {
            scanf("%s", name);
            //printf("Size of root before del function: %d\n", root->size);
            int flag = 1;
            root = del(root, name, &flag);
            //printf("Size of root after del function:%d\n", root->size);
        }
        else if(!strcmp(command, "search")) {
            scanf("%s", name);
            search(root, name);
        }
        else if(!strcmp(command, "count_smaller")) {
            scanf("%s", name);
            count_smaller(root, name);
        }
        else {
            printf("Invalid command.\n");
            i--;
        }
    }
    if(root == NULL) return 0;
    //printf("Initializing an arr with size %d...\n", root->size);
    customer** arr = (customer**) malloc(sizeof(customer*) * (root->size));
    if(arr == NULL) {
        printf("Failed to allocate memory.\n");
        exit(-3);
    }
    int i = 0;
    //printf("------------\n");
    fillArray(arr, root, &i);
    //printf("------------\n");
    /*
    printf("------------\n");
    for(int i = 0; i < root->size; i++) {
        printf("%s %d\n", arr[i]->name, arr[i]->points);
    }
    printf("------------\n");
    */
    quickSort(arr, 0, root->size-1);
    for(int i = 0; i < root->size; i++) {
        printf("%s %d\n", arr[i]->name, arr[i]->points);
    }

    freeBinaryTree(root);
    free(arr);

    return 0;
}

void fillArray(customer** arr, bstnode* root, int* i) {
    if(root != NULL) {
        fillArray(arr, root->left, i);
        if(root->custPtr == NULL) {
            printf("Missing customer pointer!\n");
            exit(-4);
        }
        arr[*i] = root->custPtr;
        //printf("i=%d , arr[i] = %s with %d points\n", *i, arr[*i]->name, arr[*i]->points);
        *i += 1;
        fillArray(arr, root->right, i);
    }
}

int partition(customer* arr[], int left, int right) {
    int pivot = arr[right]->points;
    int i = left;
    int j = right - 1;
    while(j >= i) {
        if(arr[i]->points > pivot) {
            i++;
        }
        else if(arr[i]->points < pivot) {
            swap(&arr[i], &arr[j]);
            j--;
        }
        else {
            int cmp = strcmp(arr[i]->name, arr[right]->name);
            if(cmp < 0) {
                i++;
            }
            else {
                swap(&arr[i], &arr[j]);
                j--;
            }
        }
    }
    swap(&arr[i], &arr[right]);
    return i;
}

void quickSort(customer* arr[], int left, int right) {
    if(left < right) {
        int pi = partition(arr, left, right);

        quickSort(arr, left, pi-1);
        quickSort(arr, pi+1, right);
    }
}

customer* initialize_customer(char* name, int points) {
    customer* temp= (customer*) malloc(sizeof(customer));
    if(!temp) {
        printf("Unable to allocate memory.\n");
        exit(-1);
    }
    
    strcpy(temp->name, name);
    temp->points = points;

    return temp;
}

bstnode* initialize_node(char* name, int points) {
    bstnode* node = (bstnode*) malloc(sizeof(bstnode));
    if(!node) {
        printf("Unable to allocate memory.\n");
        exit(-1);
    }

    node->left = NULL;
    node->right = NULL;
    node->size = 1;
    node->custPtr = initialize_customer(name, points);

    return node;
}

int add(bstnode** root, char* name, int points) {
    if(*root == NULL) {
        bstnode* temp = initialize_node(name, points);
        printf("%s %d\n", name, points);
        *root = temp;
        return 1;
    }
    int cmp = strcmp(name, (*root)->custPtr->name);
    int res = 0;

    if(cmp > 0) res = add(&((*root)->right), name, points);
    else if(cmp < 0) res = add(&((*root)->left), name, points);
    else {
        (*root)->custPtr->points += points;
        printf("%s %d\n", name, (*root)->custPtr->points);
        return 0;
    }

    (*root)->size += res;
    return res;
}

void sub(bstnode** root, char* name, int points) {
    if(*root == NULL) {
        printf("%s not found\n", name);
        return;
    }
    int cmp = strcmp(name, (*root)->custPtr->name);
    if(cmp > 0) sub(&((*root)->right), name, points);
    else if(cmp < 0) sub(&((*root)->left), name, points);
    else {
        (*root)->custPtr->points -= points;
        if((*root)->custPtr->points < 0) (*root)->custPtr->points = 0;
        printf("%s %d\n", name, (*root)->custPtr->points);
    }
}

bstnode* findMax(bstnode* node) {
    if(node->right != NULL) return findMax(node->right);
    return node;
}

bstnode* del(bstnode* root, char* name, int* flag) {
    if(root == NULL) {
        printf("%s not found\n", name);
        *flag = 0;
        return root;
    }

    int cmp = strcmp(name, root->custPtr->name);
    int size = 0;
    if(cmp > 0) {
        if(root->right) size = root->right->size;
        root->right = del(root->right, name, flag);
        if(root->right != NULL && size != root->right->size) root->size -= *flag; 
        else if(root->right == NULL) root->size -= *flag;

        return root;
    }
    else if(cmp < 0) {
        if(root->left) size = root->left->size;
        root->left = del(root->left, name, flag);
        if(root->left != NULL && size != root->left->size) root->size -= *flag; 
        else if(root->left == NULL) root->size -= *flag;

        return root;
    }

    if(root->left == NULL) { // Only right child
        bstnode* temp = root->right;
        free(root->custPtr);
        free(root);
        printf("%s deleted\n", name);
        return temp;
    }
    else if(root->right == NULL) { // Only left child
        bstnode* temp = root->left;
        free(root->custPtr);
        free(root);
        printf("%s deleted\n", name);
        return temp;
    }
    else { // Two Children
        bstnode* parent = root;
        bstnode* replacement = root->left;
        while(replacement->right != NULL) {
            parent = replacement;
            parent->size -= *flag;
            replacement = replacement->right;
        }
        
        if(parent != root) {
            parent->right = replacement->left;
        }
        else {
            parent->left = replacement->left;
        }

        free(root->custPtr);
        root->custPtr = replacement->custPtr;
        root->size -= *flag;
        free(replacement);
        printf("%s deleted\n", name);
        return root;
    }
}

void search(bstnode* root, char* name) {
    if(root == NULL) {
        printf("%s not found\n", name);
        return;
    }
    bstnode* walker = root;
    int depth = 0;
    int cmp;
    do {
        cmp = strcmp(name, walker->custPtr->name);
        if(cmp > 0) {
            walker = walker->right;
            depth++;
        }
        else if(cmp < 0) {
            walker = walker->left;
            depth++;
        }
    } while(walker != NULL && cmp != 0);
    
    if(walker == NULL) {
        printf("%s not found\n", name);
    }
    else {
        printf("%s %d %d\n", name, walker->custPtr->points, depth);
    }
}

void count_smaller(bstnode* root, char* name) {
    if(root == NULL) {
        printf("0\n");
        return;
    }
    bstnode* walker = root;
    int count = 0;
    int cmp;
    do {
        cmp = strcmp(name, walker->custPtr->name);
        if(cmp > 0) {
            if(walker->left != NULL) count += walker->left->size;
            count += 1;
            walker = walker->right;
        }
        else if(cmp < 0) {
            walker = walker->left;
        }
    } while(walker != NULL && cmp != 0);
    if(walker != NULL && walker->left != NULL) count += walker->left->size;

    printf("%d\n", count);
}