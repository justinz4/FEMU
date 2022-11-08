/* filesystem_structs.h - Defines filesystem structs used in file and directory operations 
 * vim:ts=4 noexpandtab
 */

#include "etd_lib.h"

#ifndef ETD_FS_STRUCTS_H
#define ETD_FS_STRUCTS_H

#define ETD_NUM_DATA_BLOCK_ADDR 1023
#define ETD_DENTRY_SIZE 64
#define ETD_MAX_FILENAME_LENGTH 32

/* Disk block size */
#define ETD_BLOCK_SIZE ETD_FOUR_KB

/* Maximum files limited by boot block size */
#define ETD_MAX_FILES (ETD_BLOCK_SIZE / ETD_DENTRY_SIZE - 1)

/*** Block formats  ***/

/* directory structure */
// TODO for writable filesystem (just use boot block as directory until then)

/* directory entry structure */
typedef struct __attribute__((packed, aligned(ETD_DENTRY_SIZE))) dentry {
  etd_uint8_t filename[ETD_MAX_FILENAME_LENGTH];
  etd_uint32_t file_type;
  etd_uint32_t inode_num;
  etd_uint8_t reserved[24]; // not used
} etd_dentry_t;

/* data_block structure */
typedef struct __attribute__((packed, aligned(ETD_FOUR_KB))) data_block {
  etd_uint8_t data[ETD_FOUR_KB];
} etd_data_block_t;

/* inode structure */
typedef struct __attribute__((packed, aligned(ETD_FOUR_KB))) inode {
  etd_uint32_t length;
  etd_uint32_t data_blocks[ETD_NUM_DATA_BLOCK_ADDR];
} etd_inode_t;

/* boot block structure */
typedef struct __attribute__((packed, aligned(ETD_FOUR_KB))) boot_block {
  etd_uint32_t num_dir_entries;			/* number of directory entries in root */
  etd_uint32_t num_inodes;				/* number of inodes used by the system */
  etd_uint32_t num_data_blocks;			/* number of data blocks used by the filesystem */
  etd_uint8_t reserved[52]; 			/* not used, size is 64 - 3 * sizeof(uint32_t) */
  etd_dentry_t dentries[ETD_MAX_FILES]; 	/* maximum number of dentries that will fit into the block */
} etd_boot_block_t;

/* Checks that a given file type is a valid file type */
etd_uint8_t etd_check_valid_file_type(uint32_t file_type);


/*** Exposed file operation and descriptor types ***/

/* Function types for syscall file operation functions */
typedef etd_int32_t(*etd_read_t)(etd_int32_t, void*, etd_int32_t);			/* int32_t read (int32_t fd, void* buf, int32_t nbytes) */
typedef etd_int32_t(*etd_write_t)(etd_int32_t, const void*, etd_int32_t);	/* int32_t write (int32_t fd, const void* buf, int32_t nbytes) */
typedef etd_int32_t(*etd_open_t)(const etd_uint8_t*);					/* int32_t open (const uint8_t* filename) */
typedef etd_int32_t(*etd_close_t)(etd_int32_t);							/* int32_t close (int32_t fd) */

/* filesystem operations table */
typedef struct file_ops_t {
  etd_read_t read;
  etd_write_t write;
  etd_open_t open;
  etd_close_t close;
} etd_file_ops_t;

/* file descriptor structure */
typedef struct __attribute__((packed, aligned(16))) fd { // fd are 4B * 4 = 16B items
  etd_file_ops_t* fops_table;	/* file operations table for this file descriptor */
  etd_uint32_t inode_num;		/* inode number for regular file */
  etd_uint32_t file_pos; 		/* virutal addr within file (0 to length in bytes for regular files) */
  etd_uint32_t flags;			/* not used for regular files */
} etd_fd_t;

#endif /* ETD_FS_STRUCTS_H */
