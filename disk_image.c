#include "inode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]){
    if(argc!=18){
        printf("%s \n", 
            "Incorrect number of, this program will not work correctly."
        );
        return 0;
    }
    int n = atoi(argv[5]);
    //fill 5 in with size of file
    //char (*img_file)[5] = argv[3];
    char *img_file = argv[3];
    int m = atoi(argv[7]);
    //fill in 5 with length of file
    //char (*fil)[5] = argv[9];
    char *fil = argv[9];
    int uid = atoi(argv[11]);
    int gid = atoi(argv[13]);
    int d = atoi(argv[15]);
    int i = atoi(argv[17]);

    if (m > n){
        printf("%s \n", 
            "this program will not work correctly."
        );
        return 0;
    }
    if (d > m){
        printf(
            "%s \n", "This program will not work correctly."
        );
        return 0;
    }
    //check if I will fit in one block



    // produce a disk image IMAGE_FILE of N total blocks of size 1024 bytes
    //(Disk image is an array of inodes (size N))
    struct inode IMAGE_FILE[n];
    //including the first M blocks which will be used for inodes, sets all the contents to zero
    //(size 1024 bytes)
    for (int iv = 0; iv < m; ++i){
        memset(&IMAGE_FILE[iv], 0, 1024);
    }
    //make a new node
    struct inode new_node;
    //set new node with specified uid and gid
    new_node.uid = uid;
    new_node.gid = gid;
    //placed in block D (counting from 0) within the disk image
    IMAGE_FILE[d] = new_node;
    //at position I (counting from 0) within that block

    

}