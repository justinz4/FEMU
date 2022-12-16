/* filesystem_driver.c - Initializes the file system 
 * vim:ts=4 noexpandtab
 */

#include "filesystem.h"
#include "filesystem_structs.h"

/* void filesystem_init(unsigned int base_addr);
 * Inputs: base_addr - base address of physical memory address of filesystem image
 * Return Value: none
 * Function: Initializes base addresses used in file and directory operations */
void filesystem_init(unsigned int base_addr) {
    /* Load the boot block */
    root = *((etd_boot_block_t*)base_addr);

    /* Initialize where the inodes are found */
    etd_inode_base = (etd_inode_t*)(base_addr + ETD_BLOCK_SIZE);

	/* Create bitmaps to allow file creation */
	create_bitmaps();

	/* Initialize the base address for where to find data blocks */
    data_base = (etd_data_block_t*)(etd_inode_base + root.num_inodes);
    
    /* Set the current file descriptor table to one statically allocated in the kernel 
    	for file accesses while in the kernel */
    fd_table = (etd_fd_t*) kernel_fd_table;
}

/*
 * void create_bitmaps(void)
 * Inputs: None
 * Outputs: None
 * Return value: None
 * Side Effects: Initializes filesystem bitmaps
 */
void create_bitmaps() {
  etd_inode_t curr_inode;
  etd_uint32_t i, j;
  for(i = 0; i < root.num_dir_entries; ++i) {
	// Mark inodes as 'in use'
	etd_inode_bitmap |= (0x1 << root.dentries[i].inode_num);
	// printf("inode_bitmap %ll\n", inode_bitmap);
	// Get inode
	curr_inode = etd_inode_base[root.dentries[i].inode_num];
	for(j = 0; j < (curr_inode.length/ETD_FOUR_KB); ++j) {
	  // Mark data blocks as 'in use'
	  etd_data_block_bitmap[curr_inode.data_blocks[j]] = 0xFF;
	}
  }
}
