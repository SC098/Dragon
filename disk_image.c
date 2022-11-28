#include "inode.h"
#include "fs_skeleton.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


int do_this(int argc, char *argv[]){
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
    int iv;
    for (iv = 0; iv < m; ++i){
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
    for (iv = 0; iv < m ; ++iv){
        struct inode *new_node;
        rawdata[iv] = new_node;
    }

    //see if we need to use memset to turn set all of the relevent data to 0. 

    //finding the start address of the inode
    uint64_t start_addr = (1024 * d) + &rawdata;
    //i think (i * 1024) is wrong. 
    struct inode * inode = &rawdata[start_addr];
    //finds the file size
    int size = inode->size;
    //finds the file we are trying to find
    uint64_t real_start = start_addr + (i *  size);


    return 0;

}