#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <errno.h>
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
  for (i = 0; i < TOTAL_BLOCKS*BLOCK_SZ; ++i){
    if (~rawdata[i]){
      free_block = i;
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

  for (i = 0; i < N_DBLOCKS; i++) {
    int blockno = get_free_block();
    ip->dblocks[i] = blockno;
    // fill in here
  }

  // fill in here if IBLOCKS needed
  // if so, you will first need to get an empty block to use for your IBLOCK

  ip->size = nbytes;  // total number of data bytes written for file
  printf("successfully wrote %d bytes of file %s\n", nbytes, file);
}

void helper1(int argc, char *argv[]){
  
  if(argc!=18){
      printf("%s \n", 
          "Incorrect number of, this program will not work correctly."
      );
      return;
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
      return;
  }
  if (d > m){
      printf(
          "%s \n", "This program will not work correctly."
      );
      return;
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


  //return;
}

void main(int argc, char *argv[]) // add argument handling
{
  int i;
  FILE *outfile;

  outfile = fopen(output_filename, "wb");
  if (!outfile) {
    perror("datafile open");
    exit(-1);
  }

  // fill in here to place file 


  i = fwrite(rawdata, 1, TOTAL_BLOCKS*BLOCK_SZ, outfile);
  if (i != TOTAL_BLOCKS*BLOCK_SZ) {
    perror("fwrite");
    exit(-1);
  }

  i = fclose(outfile);
  if (i) {
    perror("datafile close");
    exit(-1);
  }

  printf("Done.\n");
  return;
}