#include <stdio.h>
#include <stdlib.h>

typedef struct Node {    
    int val;
    struct Node *next; 
} Node;                    



int main()
{  
    int n;
    Node *root, *x;
    
    x=root=malloc(sizeof(Node));
    
    printf("n: ");
    scanf("%d", &n);
    
    if (n <= 0)
        return(0);
        
        
    printf("list: ");
    for(int i = 0; i<n; i++) {
        x->next = malloc(sizeof(Node));
        x=x->next;
        scanf("%d", &x->val);
    }
        
    
    int flag=0;
    for (x=root; x!= NULL; x=x->next) {
        if (!flag) {
            flag++;
            continue;
        }
            
            if ((x->val != 9) && (x->next->val != 9)) {
                printf("%d", x->val + 1);
            }
            else if ((x->val != 9) && (x->next->val=9)) {
                 printf(" %d", x->val + 2);
                }
            else if ((x->val = 9) && (x->next->val !=9)){
                printf("%d", 0);
                }
            else  if ((x->val = 9) && (x->next->val =9)) {
                printf(" %d", 1);
                }

            
            }
            while (root != NULL) {
            x = root;
            root = x->next;
            free(x);   
        }
      
  
    }


