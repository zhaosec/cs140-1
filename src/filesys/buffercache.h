#ifndef FILESYS_BUFFERCACHE_H
#define FILESYS_BUFFERCACHE_H

#include "devices/block.h"
#include "filesys/off_t.h"
#include "threads/synch.h"

/* Size of buffercache -- 64 blocks */
#define BUFFERCACHE_SIZE 64

/**
 * Denotes the type of sector held in the cache block
 */
enum sector_type
{
  METADATA,                     /* Metadata sector */
  REGULAR                       /* Regular sector */
};

/**
 * Flags to denote the state of a cache block
 */
enum cache_state
{
  READING = 0,
  WRITE_REQUESTED,
  WRITING,
  CLOCK,
  READY
};

/**
 * Names various states for buffer cache blocks for use with the clock
 * algorithm.
 */
enum cache_accessed
{
  CLEAN = 0x00,                 /* Untouched */
  ACCESSED = 0x01,              /* Accessed bit */
  DIRTY = 0x02,                 /* Dirty bit */
  META = 0x04,                  /* Metadata bit */
};

/**
 * A single entry in the buffer cache
 */
struct cache_entry
{
  void *kaddr;                  /* Address of cache block */
  int accessors;                /* Number of threads accessing buffer */
  block_sector_t sector;        /* Sector of block */
  block_sector_t next_sector;   /* Sector block will contain next */
  enum cache_state state;       /* Current state of block */
  enum cache_accessed accessed;	/* Accessed bits for block */
  enum sector_type type;        /* The type of sector */
  struct condition c;           /* To notify waiting threads */
};

bool buffercache_init (const size_t size);
int buffercache_read (const block_sector_t sector, enum sector_type type,
                      const int sector_ofs, const off_t size, void *buf,
                      const block_sector_t next_sector);
int buffercache_write (const block_sector_t sector, enum sector_type type,
                       const int sector_ofs, const off_t size, const void *buf,
                       const block_sector_t next_sector);
void buffercache_flush (const bool await);

#endif
