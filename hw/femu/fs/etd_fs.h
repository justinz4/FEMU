/* filesystem.h - Defines filesystem functions to be exported and used by other modules 
 * vim:ts=4 noexpandtab
 */

#include "etd_fs_structs.h"

#ifndef ETD_FS_H
#define ETD_FS_H

#define ETD_MAX_OPEN_FILES 8

/* File descriptors for STDIN, STDOUT */
#define ETD_STDIN 0
#define ETD_STDOUT 1

/* File type definitions */
#define ETD_FILE_TYPE_RTC 0
#define ETD_FILE_TYPE_DIR 1
#define ETD_FILE_TYPE_REGULAR 2
/* Note: these are used in check_valid_file_type */

//etd_inode_t* etd_inode_base;		/* Physical addresss of first inode */
//etd_data_block_t* data_base; 	/* Physical address of first data block */ // hahahahahahaha (name)
//etd_fd_t* fd_table;				/* File descriptor table for tracking open files, is of size MAX_OPEN_FILES */
//etd_fd_t kernel_fd_table[ETD_MAX_OPEN_FILES]; /* Block of memory allocated for file descriptors in the kernel 
//											NOTE: on filesystem init, MUST set fd_table to this */

/* Boot block, which also acts as our root directory */
//etd_boot_block_t root;

/* Bitmaps for filesystem contents */
//unsigned long long etd_inode_bitmap;
//uint8_t etd_data_block_bitmap[ETD_NUM_DATA_BLOCK_ADDR];

/* Initializes the filesystem using base_addr as the base 
 * 	physical address of the filesystem image in memory */
void filesystem_init(unsigned int base_addr);


/* Obtains file directory entry for a file in current directory 
 *	(TODO currently only root) given a file name */
etd_int32_t read_dentry_by_name(const uint8_t* fname, etd_dentry_t* dentry);

/* Obtains file directory entry for a file in current directory 
 *	(TODO currently only root) given an file d-entry index */
etd_int32_t read_dentry_by_index(etd_uint32_t index, etd_dentry_t* dentry);

/* Reads a number of bytes from a file given an inode and an offset */
etd_int32_t read_data(etd_uint32_t inode, etd_uint32_t offset, uint8_t* buf, etd_uint32_t length);

/*** File Operations Tables ***/

/* File operations for regular files */
etd_int32_t file_open(const uint8_t* filename);							/* Opens a file */
etd_int32_t file_close(etd_int32_t fd);										/* Closes a file */
etd_int32_t file_write(etd_int32_t fd, const void* buf, etd_int32_t nbytes);	/* Write a string to a file (TODO does nothing) */
etd_int32_t file_read(etd_int32_t fd, void* buf, etd_int32_t nbytes);			/* Reads a string from a file */

/* File operations table for regular files */
extern etd_file_ops_t file_ops_regular;

/* File operations for directories */
etd_int32_t directory_open(const uint8_t* filename);						/* Opens a directory */
etd_int32_t directory_close(etd_int32_t fd);									/* Closes a directory */
etd_int32_t directory_write(etd_int32_t fd, const void* buf, etd_int32_t nbytes);	/* Writes to a directory (TODO does nothing) */
etd_int32_t directory_read(etd_int32_t fd, void* buf, etd_int32_t nbytes);			/* Reads a file name from a directory */

etd_int32_t new_dentry(const uint8_t* fname); // write to directory
etd_int32_t remove_dentry(const uint8_t* fname); // write to directory

/* File operations table for directories */
extern etd_file_ops_t file_ops_dir;

#endif /* ETD_FS_H */
