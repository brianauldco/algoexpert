#include <cstdio>
#include <cstdint>
#include <list>
#include <unordered_map>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <mutex>
#include <atomic>
// Write a C/C++ class (or pseudo code) that implements a fixed-size block
// memory allocator optimized for frequent allocation of blocks in a multi-
// threaded process.  Assume it must support up to 1 Million blocks with 64
// parallel threads fighting for allocation.

// The goal is to increase the efficiency of fixed-block size allocations and
// frees during runtime, since malloc/new tend to be slow in a highly parallel
// application.

  // data structures:
  //
  // void *buffer        --> the chunk of memory == BLOCK_SIZE * BLOCK_COUNT_TOTAL
  //
  // threadToArenaIndex  --> hash map where key is incoming thread id and value is index into an array of arenas
  //                         * each arena is associated with a thread
  //                         * first allocation by a given thread populate its arena and associated hashmap entry
  //                         * subsequent alloc/frees will look up arena in hashmap based on thread id
  //
  // arena_t arena[ BLOCK_COUNT_TOTAL / THREADS ];
  //                      --> array of arenas associated with each thread
  //                      --> 
    // Allocate big buffer of memory --> *buffer
    //
    //    +- sect0/buf0 +- sect0/buf2
    //    |             | 
    //    v             v
    //   ++------+------+ . . . +------+------+------+
    //   ||      |      |              |
    //   ||      |      | . . .        |
    //   ||      |      |              |
    //   ++------+------+ . . . +------+------+------+
    //           ^
    //           |
    //           +- sect0/buf1
    //
    // * buffer is logically split into THREAD_COUNT sections 0 --> THREAD_COUNT - 1
    // * each section has BLOCK_COUNT_TOTAL / THREAD_COUNT 'BLOCK_SIZE' BLOCKS in each section

using namespace std;

static const uint64_t c_blockSizeBytes  = 4096;
static const uint64_t c_blockCountTotal = 1 << 20;
static const uint64_t c_threadCount     = 64;
static const uint64_t c_sectionSize     = c_blockCountTotal / c_threadCount;

typedef struct _section_t {
  list<void*> freeList;
  mutex       freeListLock;
} section_t;

class BlockAllocator {

  section_t perThreadSection[c_threadCount];
  unordered_map<pid_t, uint64_t> threadToSectionIndexMap;
  void * buffer = NULL;
  pthread_rwlock_t MapRWLock = PTHREAD_RWLOCK_INITIALIZER;
  atomic<uint64_t> nextAvailableIndex{0};
  
  
public:  
  BlockAllocator( void ) {

    //buffer = posix_memalign( BLOCK_SIZE_BYTES, BLOCK_SIZE_BYTES * BLOCK_COUNT_TOTAL );
    if ( posix_memalign( &buffer, c_blockSizeBytes, c_blockSizeBytes * c_blockCountTotal ) != 0 ) {
      assert(!"Failed to allocated memory for BlockAllocator");
    }
    
    // Populate each section array with a full free list of buffers for its section
    for ( int i = 0; i < c_threadCount; i++ ) {
      
      uint64_t bufferSectionStartAddr = (uint64_t)buffer + ( i * c_sectionSize * c_blockSizeBytes );
      
      for ( int j = 0; j < c_sectionSize ; j++ ) {

	uint64_t blockBufferAddr =  bufferSectionStartAddr + ( j * c_blockSizeBytes );
	perThreadSection[i].freeList.push_back( (void*)blockBufferAddr );
	
      }
    }

  }

  ~BlockAllocator( void ) {
  }

  void * allocBuffer( void ) {

    void *buf = NULL;
    pid_t threadId = gettid();
    uint64_t sectionIndex;

    // get map read lock
    pthread_rwlock_rdlock(&MapRWLock);
    auto iter = threadToSectionIndexMap.find( threadId );
    if ( iter == threadToSectionIndexMap.end() ) {
      pthread_rwlock_unlock(&MapRWLock);
      sectionIndex = nextAvailableIndex.fetch_add(1);
      pthread_rwlock_wrlock(&MapRWLock);
      threadToSectionIndexMap[ threadId ] = sectionIndex;
      pthread_rwlock_unlock(&MapRWLock);
    } else {
      sectionIndex = iter->second;
      pthread_rwlock_unlock(&MapRWLock);
    }

    section_t *section = &perThreadSection[ threadId ];
    section->freeListLock.lock();
    if ( !section->freeList.empty() ) {
      void *buf = section->freeList.front();
      section->freeList.pop_front();
    }
    section->freeListLock.lock();
    
    return buf;
  }

  void freeBuffer( void * buf ) {

    // Derive thread index from buffer
    uint64_t sectionIndex = ((uint64_t)buf - (uint64_t)buffer) / ( c_sectionSize * c_blockSizeBytes );

    // atomically return to freeList for this section
    section_t *section = &perThreadSection[ sectionIndex ];
    section->freeListLock.lock();
    section->freeList.push_back(buf);
    section->freeListLock.lock();
    
   
    return;
  }
};

int main (int argc, char **argv) {

  BlockAllocator block_allocator;
  return 0;
}
