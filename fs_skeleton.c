#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <errno.h>
#include <stdint.h>
#include <assert.h>
#include "inode.h"

#define TOTAL_BLOCKS (10*1024)

//using this to add inodes and raw data
static unsigned char rawdata[TOTAL_BLOCKS*BLOCK_SZ];
static char bitmap[TOTAL_BLOCKS];

int get_free_block()
{
  int i;
  int free_block = -1;
  for (i = 0; i < TOTAL_BLOCKS; ++i){
    if (~bitmap[i]){
      free_block = i;
      bitmap[i] = 1;
      break;
    }
  }
  assert(free_block < TOTAL_BLOCKS);
  assert(bitmap[free_block]);
  return free_block;
}

void write_int(int pos, int val)
{
  int *ptr = (int *)&rawdata[pos];
  *ptr = val;
}

void place_file(char *file, int uid, int gid)
{
  int i, nbytes = 0;
  int i2block_index, i3block_index;
  struct inode *ip;
  FILE *fpr;
  unsigned char buf[BLOCK_SZ];

  ip->mode = 0;
  ip->nlink = 1;
  ip->uid = uid;
  ip->gid = gid;
  ip->ctime = random();
  ip->mtime = random();
  ip->atime = random();

  fpr = fopen(file, "rb");
  if (!fpr) {
    perror(file);
    exit(-1);
  }

  
  char c;
  int j;

  for (i = 0; i < N_DBLOCKS; i++) {
    int blockno = get_free_block();

    ip->dblocks[i] = blockno; //new

    for (j= (blockno * 1024); j < BLOCK_SZ + (blockno * 1024); ++j){
      if ((c = fgetc(fpr)) != '\0'){
        rawdata[j] = c;
      }
      else{
        break;
      }
    }
  }

  int k;
  
  for (i = 0; i < N_IBLOCKS; ++i){
    //creates the indirect block
    int blockno = get_free_block();
    ip->iblocks[i] = blockno;
    //fills the indirect block with  data blocks
    for (j = 0; j < BLOCK_SZ; ++j){
      //gets a data block
      //puts the data block into the indirect block;
      int datablock = get_free_block();

      ip->dblocks[i] = datablock; //new

      rawdata[blockno] = get_free_block();
      for (k= (blockno * 1024); k < BLOCK_SZ + (blockno * 1024); ++k){
        if ((c = fgetc(fpr)) != '\0'){
          rawdata[k] = c;
        }
        else{
          break;
        }
      }
    }
  }
    
    
  // fill in here
    


   
  //double_indirect_block[pointers to other data blocks][pointers to data blocks]
  int doubly_indirect_block[1024][1024];
  ip->i2block = doubly_indirect_block;  //warning?
  int a;
  for(a = 0;a < 1024; ++a){
    //create doubly indirect block
    int blockduo = get_free_block();
    
    for (i = 0; i < N_IBLOCKS; ++i){
      //creates the indirect block
      int blockno = get_free_block();
      ip->iblocks[i] = blockno;
      //fills the indirect block with  data blocks
      for (j = 0; j < BLOCK_SZ; ++j){
        //gets a data block
        //puts the data block into the indirect block;
        int datablock = get_free_block();

        ip->dblocks[i] = datablock; //new
        
        rawdata[blockno] = get_free_block();
        for (k= (blockno * 1024); k < BLOCK_SZ + (blockno * 1024); ++k){
          if ((c = fgetc(fpr)) != '\0'){
            rawdata[k] = c;
          }
          else{
            break;
          }
        }
      }
    }
  }


  int triply_indirect_block[1024][1024][1024];
  ip->i3block = triply_indirect_block; //warning?
  int b;
  for(b = 0; b < 1024; ++b){
    //create triply indirect block
    int blocktrio = get_free_block();


    for(a = 0;a < 1024; ++a){
      //create doubly indirect block
      int blockduo = get_free_block();
      
      for (i = 0; i < N_IBLOCKS; ++i){
        //creates the indirect block
        int blockno = get_free_block();
        ip->iblocks[i] = blockno;
        //fills the indirect block with  data blocks
        for (j = 0; j < BLOCK_SZ; ++j){
          //gets a data block
          //puts the data block into the indirect block;
          int datablock = get_free_block();

          ip->dblocks[i] = datablock; //new
          
          rawdata[blockno] = get_free_block();
          for (k= (blockno * 1024); k < BLOCK_SZ + (blockno * 1024); ++k){
            if ((c = fgetc(fpr)) != '\0'){
              rawdata[k] = c;
            }
            else{
              break;
            }
          }
        }
      }
    }
  }


  ip->size = nbytes;  // total number of data bytes written for file
  printf("successfully wrote %d bytes of file %s\n", nbytes, file);
}

struct inode * create(int argc, FILE *files, const char *argv[]){
  
  if(argc!=18){
      printf("%s \n", 
          "Incorrect number of, this program will not work correctly."
      );
      return;
  }
  int n = atoi(argv[5]);
  //fill 5 in with size of file
  //char (*img_file)[5] = argv[3];
  int m = atoi(argv[7]);
  //fill in 5 with length of file
  //char (*fil)[5] = argv[9];
  char *fil[100];
  memcpy(*fil, argv[9], 100);
  int uid = atoi(argv[11]);
  int gid = atoi(argv[13]);
  int d = atoi(argv[15]);
  int i = atoi(argv[17]);

  if (m > n){
      printf("%s \n", 
          "this program will not work correctly."
      );
      return -1;
  }
  if (d > m){
      printf(
          "%s \n", "This program will not work correctly."
      );
      return - 1;
  }

  //check if I will fit in one block
  int iv;
  FILE *outfile;
  



  // produce a disk image IMAGE_FILE of N total blocks of size 1024 bytes
  //(Disk image is an array of inodes (size N))

  //including the first M blocks which will be used for inodes, sets all the contents to zero
  //(size 1024 bytes)
  for (iv = 0; iv < m; ++iv){
      memset(&rawdata[iv], 0, 1024);
  }
  //make a new node
  struct inode new_node;
  //set new node with specified uid and gid
  new_node.uid = uid;
  new_node.gid = gid;
  
  //at position I (counting from 0) within that block
  for (iv = 0; iv < m ; ++iv){
      struct inode *new_node;
      rawdata[iv] = new_node;
  }
  //placed in block D (counting from 0) within the disk image


  //see if we need to use memset to turn set all of the relevent data to 0. 

  //finding the start address of the inode d
  uint64_t start_addr = (1024 * d) + &rawdata;
  //i think (i * 1024) is wrong. 
  struct inode * inode = &start_addr;

  inode->uid = uid;
  inode->gid = gid;

  return inode;
  //finds the file size
  

  

}

void main(int argc, char *argv[]) // add argument handling
{
  int i;
  FILE *outfile;
  

  outfile = fopen(argv[3], "wb");
  if (!outfile) {
    perror("datafile open");
    exit(-1);
  }

  // fill in here to place file 

  if (strcmp("-create", argv[1])){
    struct inode *file_location = create(argc, outfile, argv);
    int size = file_location->size;
  //finds the file we are trying to find
    uint64_t start_addr = (1024 * atoi(argv[15])) + &rawdata;
    char *file_to_write[] =argv[9];

    char c = &file_to_write[0];
    int byte; 

    //keeps track of the char we are currently at
    







  }


  

  i = fclose(outfile);
  if (i) {
    perror("datafile close");
    exit(-1);
  }

  printf("Done.\n");
}