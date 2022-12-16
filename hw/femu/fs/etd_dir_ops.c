/* etd_dir_ops.c - Implements file operations for directories 
 *						and operatiosn for obtaining directory entires 
 * vim:ts=4 noexpandtab
 */

#include "etd_fs.h"

/*** File Operations for Directories ***/

etd_file_ops_t file_ops_dir = {directory_read, directory_write, directory_open, directory_close};

/* etd_int32_t directory_open(const etd_uint8_t* path);
 * Inputs: path - name of directory to be opened
 * Return Value: 0 for success, -1 for failure
 * Function: Does nothing 
 */
etd_int32_t directory_open(const etd_uint8_t* path) {
    // TODO change current directory
    return 0;
}

/* etd_int32_t directory_close(etd_int32_t fd);
 * Inputs: fd - file descriptor of directory to close
 * Return Value: 0 for success, -1 for failure
 * Function: Does nothing 
 */
etd_int32_t directory_close(etd_int32_t fd) {
    return 0;
}

/* etd_int32_t directory_read(etd_int32_t fd, void* buf, etd_int32_t nbytes);
 * Inputs: fd - file descriptor of directory to read from
 *			buf - buffer for filename to be read 
 *			nbytes - number of bytes to read
 * Return Value: number of bytes read
 * Function: Reads the next filename from the directory 
 */
etd_int32_t directory_read(etd_int32_t fd, void* buf, etd_int32_t nbytes) {
  if (fd_table[fd].file_pos == root.num_inodes) return -1;
  if(nbytes > ETD_MAX_FILENAME_LENGTH) nbytes = ETD_MAX_FILENAME_LENGTH;
  etd_int32_t n = strlcpy(buf, (const etd_int8_t*)root.dentries[fd_table[fd].file_pos].filename, nbytes);
  memcpy((char*)buf, (char*)root.dentries[fd_table[fd].file_pos].filename, nbytes);

  fd_table[fd].file_pos += 0x01;

  return n; // return # of bytes read
	/* We can read at most MAX_FILENAME_LENGTH bytes* /
    if(nbytes > MAX_FILENAME_LENGTH) nbytes = MAX_FILENAME_LENGTH;
    
    strncpy((char*)buf, (char*)root.dentries[fd_table[fd].file_pos].filename, nbytes);
    fd_table[fd].file_pos += 1;

    return nbytes; // return # of bytes read*/
}

/* etd_int32_t directory_write(etd_int32_t fd, const void* buf, etd_int32_t nbytes);
 * Inputs: fd - file descriptor of directory to write to
 *			buf - buffer of filename to create in the directory
 *			nbytes - number of bytes in the filename to write
 * Return Value: 0 for success, -1 for failure
 * Function: Does nothing, always fails since it is not supported
 */
etd_int32_t directory_write(etd_int32_t fd, const void* buf, etd_int32_t nbytes) {
    return -1; 
}

/* etd_int32_t read_dentry_by_name(const etd_uint8_t* fname, dentry_t* dentry);
 * Inputs: fname - name of file whose dentry we want
 *			dentry - pointer to dir-entry that will be filled by 
 *					the dir-entry of the input file
 * Return Value: 0 for success, -1 for failure
 * Function: Loops through the root directory and search for file with 
 *				file name 'fname'
 */
etd_int32_t read_dentry_by_name(const etd_uint8_t* fname, dentry_t* dentry) {
    etd_int32_t i;
    // don't match empty filename
    if (fname[0]=='\0') return -1;

	/* Iterate through all dentries in root directory */
    for(i = 0; i < MAX_FILES; ++i) {
    	/* Directory entry does not exist */
        if(!check_valid_file_type(root.dentries[i].file_type)) 
        	continue;
        
        /* File name matches */
        if(!strncmp((char*)fname, (char*)root.dentries[i].filename, 32)) {
            memcpy(dentry, &(root.dentries[i]), ETD_DENTRY_SIZE);
            return 0;
        }
    }

    return -1;
}

/* etd_int32_t read_dentry_by_index(etd_int32_t index, dentry_t* dentry); 
 * Inputs: index - index of dir-entry in boot block
 *			dentry - pointer to dir-entry that will be filled by 
 *					the dir-entry of the input file
 * Return Value: 0 for success, -1 for failure
 * Function: Retrieves the index-th entry in the boot-block's 
 *				dir-entry list
 */
etd_int32_t read_dentry_by_index(etd_uint32_t index, dentry_t* dentry) {
    if(!check_valid_file_type(root.dentries[index].file_type)) {
        return -1;
    }
	if(root.dentries[index].filename[0] == 0) return -1;
    memcpy(dentry, &(root.dentries[index]), DENTRY_SIZE);
	return 0;
}


/* etd_uint8_t check_valid_file_type(etd_uint32_t file_type);
 * Inputs: file_type - a file type
 * Return Value: 1 for valid, 0 for invalid
 * Function: Returns 1 for a known file type, 0 for unknown
 */
etd_uint8_t check_valid_file_type(etd_uint32_t file_type) {
	switch(file_type) {
		case FILE_TYPE_RTC:
		case FILE_TYPE_DIR:
		case FILE_TYPE_REGULAR:
			return 1;
		default:
			break;
	}
	return 0;
}

etd_int32_t new_dentry(const etd_uint8_t* fname) {
  etd_int32_t i, j;
  for(i = 0; i < ETD_MAX_FILES; ++i) {
	if(!root.dentries[i].filename[0]) {
	  strcpy(&root.dentries[i].filename, fname);
	  root.dentries[i].file_type = ETD_FILE_TYPE_REGULAR;
	  // Find a free inode
	  for(j = 1; j < 30; ++j) {
		if(!etd_inode_base[j].length) break;
	  }

	  root.dentries[i].inode_num = j; // won't be len 0 for long
	  break;
	}
  }
  return 0;
}

etd_int32_t remove_dentry(const etd_uint8_t* fname) {
  etd_int32_t i, j;
  for(i = 0; i < ETD_MAX_FILES; ++i) {
	if(!strncmp((char*)fname, (char*)root.dentries[i].filename, 32)) {
	  // clear out inode
	  for(j = 0; j < (etd_inode_base[root.dentries[i].inode_num].length / ETD_FOUR_KB) + 1; ++j) {
		etd_data_block_bitmap[etd_inode_base[root.dentries[i].inode_num].data_blocks[j]] = 0;
	  }
	  etd_inode_base[root.dentries[i].inode_num].length = 0;

	  // clear dentry
	  memset(root.dentries[i].filename, 0, 32);
	  root.dentries[i].file_type = 0;
	  root.dentries[i].inode_num = 0;
	  break;
	}
  }

  // if this wasn't the last dentry
  dentry_t d;
  j = i+1; 
  for(; read_dentry_by_index(j, &d) != -1; ++j) continue;
  j -= 1;
  memcpy(&root.dentries[i], &root.dentries[j], sizeof(dentry_t));

  memset(root.dentries[j].filename, 0, 32);
  root.dentries[j].file_type = 0;
  root.dentries[j].inode_num = 0;
}
