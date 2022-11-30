#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <assert.h>
#include "inode.h"

#define TOTAL_BLOCKS (10*1024)

//using this to add inodes and raw data
//static unsigned char rawdata[TOTAL_BLOCKS*BLOCK_SZ];
unsigned char *rawdata;
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

void place_file(char *file, int uid, int gid, struct inode *ip)
{
  int i, nbytes = 0;
  int i2block_index, i3block_index;
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
    //could use fread
    for (j= (blockno * BLOCK_SZ); j < BLOCK_SZ + (blockno * BLOCK_SZ); ++j){
      if ((c = fgetc(fpr)) != '\0'){
        //first m blocks reserved for inodes
        rawdata[j] = c;
      }
      else{
        break;
      }
    }
  }

  int k;
  if (c != '\0'){
    for (i = 0; i < N_IBLOCKS; ++i){
      //creates the indirect block
      int blockno = get_free_block();
      ip->iblocks[i] = blockno;
      //fills the indirect block with  data blocks
      for (j = 0; j < 1024; ++j){
        //gets a data block
        //puts the data block into the indirect block;
        int datablock = get_free_block();
        int addr = blockno*BLOCK_SZ+j*sizeof(int);

        rawdata[addr] = datablock;
        for (k= (datablock * BLOCK_SZ); k < BLOCK_SZ + (datablock * BLOCK_SZ); ++k){
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
    
  // fill in here
    


  if (c != '\0'){
    //double_indirect_block[pointers to other data blocks][pointers to data blocks]
    int double_indirect_block = get_free_block();
    ip->i2block = double_indirect_block; //warning?
    int a;
    for(a = 0; a < 1024; ++a){
      //create indirect block
      int blockduo = get_free_block();
      //address to write to = block size * block addr  + position *integer size
      int addr2 = double_indirect_block*BLOCK_SZ+a*sizeof(int);
      //call to write
      rawdata[addr2] = blockduo;
      for (j = 0; j < 1024; ++j){
        //gets a data block
        //puts the data block into the indirect block;
        int datablock = get_free_block();
        int addr = blockduo*BLOCK_SZ+j*sizeof(int);
        rawdata[addr] = datablock;
        for (k= (datablock * BLOCK_SZ); k < BLOCK_SZ + (datablock * BLOCK_SZ); ++k){
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

  if (c != '\0'){
    int triply_indirect_block = get_free_block();
    ip->i3block = triply_indirect_block; //warning?
    int b;
    for(b = 0; b < 1024; ++b){
      int double_indirect = get_free_block();

      //address to write to = block size * block addr  + position *integer size
      int addr = triply_indirect_block*BLOCK_SZ+b*sizeof(int);
      //call to write
      rawdata[addr] = double_indirect;

      int a;
      for(a = 0; a < 1024; ++a){
        //create doubly indirect block
        int blockduo = get_free_block();
        //address to write to = block size * block addr  + position *integer size
        int addr2 = double_indirect*BLOCK_SZ+a*sizeof(int);
        //call to write
        rawdata[addr2] = blockduo;
        for (j = 0; j < 1024; ++j){
          //gets a data block
          //puts the data block into the indirect block;
          int datablock = get_free_block();
          int addr = blockduo*BLOCK_SZ+j*sizeof(int);

          rawdata[addr] = datablock;
          for (k= (datablock * BLOCK_SZ); k < BLOCK_SZ + (datablock * BLOCK_SZ); ++k){
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
          "M > N, this program will not work correctly."
      );
      return NULL;
  }
  if (d >= m){
      printf(
          "D >= M, This program will not work correctly."
      );
      return NULL;
  }

  //check if I will fit the node list

  //figure out size of list using sizeof
  if (i >= (BLOCK_SZ/sizeof(struct inode))){
      //printf("%s \n", "This program will not work correctly.");
      printf("I > Block size, This program will not work correctly.");
      return NULL;
  }


  
  FILE *outfile;
  



  // produce a disk image IMAGE_FILE of N total blocks of size 1024 bytes
  //(Disk image is an array of inodes (size N))
  rawdata = malloc(n * BLOCK_SZ);
  //set all blocks to 0
  memset(&rawdata, 0, n * BLOCK_SZ);
  //make inode
  struct inode *ip;

  //call place file (reserve first m blocks for inodes)
  place_file(fil,uid,gid,ip);

  //set inode in dth position in ith inode in rawdata

  rawdata[d*BLOCK_SZ+i] = ip;
  

  return ip;

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
    char *file_to_write = argv[9];

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