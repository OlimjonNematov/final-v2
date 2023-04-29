#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdint.h>

// Type def
typedef struct
{
    // ...
    __uint16_t i_mode;        // File type and mode
    __uint16_t i_uid;         // Owner user ID
    __uint32_t i_size;        // File size in bytes
    __uint32_t i_atime;       // Access time
    __uint32_t i_ctime;       // Creation time
    __uint32_t i_mtime;       // Modification time
    __uint32_t i_dtime;       // Deletion time
    __uint16_t i_gid;         // Owner group ID
    __uint16_t i_links_count; // Hard links count
    __uint32_t i_blocks;      // Number of 512-byte blocks allocated for the file
    __uint32_t i_flags;       // File flags
    // ...
    __uint32_t i_block[15]; // Block addresses (12 direct, 1 single indirect, 1 double indirect, 1 triple indirect)
    // ...
} ext3_inode;

typedef struct
{
    unsigned char *data;
    size_t size;
} data_blocks;

typedef struct
{
    __uint32_t bg_block_bitmap;
    __uint32_t bg_inode_bitmap;
    __uint32_t bg_inode_table;
    __uint16_t bg_free_blocks_count;
    __uint16_t bg_free_inodes_count;
    __uint16_t bg_used_dirs_count;
    __uint16_t bg_pad;
    __uint32_t bg_reserved[3];
} ext3_group_desc;
typedef struct
{
    __uint32_t s_inodes_count;
    __uint32_t s_blocks_count;
    __uint32_t s_r_blocks_count;
    __uint32_t s_free_blocks_count;
    __uint32_t s_free_inodes_count;
    __uint32_t s_first_data_block;
    __uint32_t s_log_block_size;
    __uint32_t s_log_frag_size;
    __uint32_t s_blocks_per_group;
    __uint32_t s_frags_per_group;
    __uint32_t s_inodes_per_group;
    __uint32_t s_mtime;
    __uint32_t s_wtime;
    __uint16_t s_mnt_count;
    __uint16_t s_max_mnt_count;
    __uint16_t s_magic;
    __uint16_t s_state;
    __uint16_t s_errors;
    __uint16_t s_minor_rev_level;
    __uint32_t s_lastcheck;
    __uint32_t s_checkinterval;
    __uint32_t s_creator_os;
    __uint32_t s_rev_level;
    __uint16_t s_def_resuid;
    __uint16_t s_def_resgid;
    __uint32_t s_first_ino;
    __uint16_t s_inode_size;
    __uint16_t s_block_group_nr;
    __uint32_t s_feature_compat;
    __uint32_t s_feature_incompat;
    __uint32_t s_feature_ro_compat;
    __uint8_t s_uuid[16];
    char s_volume_name[16];
    char s_last_mounted[64];
    __uint32_t s_algo_bitmap;
    __uint8_t s_prealloc_blocks;
    __uint8_t s_prealloc_dir_blocks;
    __uint16_t s_padding1;
    __uint8_t s_journal_uuid[16];
    __uint32_t s_journal_inum;
    __uint32_t s_journal_dev;
    __uint32_t s_last_orphan;
    __uint32_t s_hash_seed[4];
    __uint8_t s_def_hash_version;
    __uint8_t s_reserved_char_pad;
    __uint16_t s_reserved_word_pad;
    __uint32_t s_default_mount_opts;
    __uint32_t s_first_meta_bg;
    __uint32_t s_mkfs_time;
    __uint32_t s_jnl_blocks[17];
    // ... other fields
} ext3_superblock;

int read_superblock(const char *device, ext3_superblock *sb);
int find_inode_location(const char *device, int inode_number, const ext3_superblock *sb, ext3_group_desc *bgd, int *inode_block, int *inode_offset);
int read_inode(const char *device, int inode_block, int inode_offset, ext3_inode *inode);
int read_data_blocks(const char *device, const ext3_inode *inode, data_blocks *file_data, const ext3_superblock *sb);

int read_data_block(int fd, uint32_t block_address, uint8_t *buffer, int block_size);
int create_file_from_data(const char *file_name, const data_blocks *file_data);

int read_singly_indirect_blocks(int fd, uint32_t indirect_block_num, int block_size, uint8_t *file_data, uint32_t file_size, int direct_blocks_count);
int read_doubly_indirect_blocks(int fd, uint32_t doubly_indirect_block_num, uint8_t *buffer, int block_size);
int read_triply_indirect_blocks(int fd, uint32_t triply_indirect_block_num, uint8_t *buffer, int block_size);

#endif
