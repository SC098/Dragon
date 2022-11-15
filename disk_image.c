#include "inode.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]){
    int n = argv[5];
    int m = argv[7];
    int d = argv[15];

    if (m > n)
        printf("%s \n", 
            "this program will not work correctly."
        );
    
    if (d > m)
        printf(
            "%s \n", "This program will not work correctly."
        );

    struct inode IMAGE_FILE[n];

    for (int i = 0; i < m; ++i){
        memset(&IMAGE_FILE[i], NULL, 1024);
    }
    struct inode new_node;
    IMAGE_FILE[d] = new_node;


}