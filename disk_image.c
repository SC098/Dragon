#include "inode.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]){
    
    //char img_file[] = argv[3];
    int n = argv[5];
    int m = argv[7];
    //char fil[] = argv[9];
    int uid = argv[11];
    int gid = argv[13];
    int d = argv[15];
    int i = argv[17];

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