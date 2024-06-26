#include <stdlib.h>
#include <stdint.h>
#include <time.h>
//#include <x86intrin.h>
#include <inttypes.h>

#define ROTL32(x,y)	_rotl(x,y)

#define BIG_CONSTANT(x) (x)

uint32_t getblock32 ( const uint32_t * p, int i ){
  return p[i];
}

// uint32_t fmix32 ( uint32_t h ){
//     h ^= h >> 16;
//     h *= 0x85ebca6b;
//     h ^= h >> 13;
//     h *= 0xc2b2ae35;
//     h ^= h >> 16;
//     return h;
//

uint32_t MurmurHash3_x86_32 ( const void * key, int len, uint32_t seed){
    const uint8_t * data = (const uint8_t*)key;
    const int nblocks = len / 4;

    uint32_t h1 = seed;

    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    //----------
    // body

    const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);

    for(int i = -nblocks; i; i++)
    {
    uint32_t k1 = getblock32(blocks,i);

    k1 *= c1;
    k1 = ROTL32(k1,15);
    k1 *= c2;

    h1 ^= k1;
    h1 = ROTL32(h1,13); 
    h1 = h1*5+0xe6546b64;
    }

    //----------
    // tail

    const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

    uint32_t k1 = 0;

    switch(len & 3)
    {
    case 3: k1 ^= tail[2] << 16;
    case 2: k1 ^= tail[1] << 8;
    case 1: k1 ^= tail[0];
            k1 *= c1; k1 = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
    };

    //----------
    // finalization

    h1 ^= len;

    // h1 = fmix32(h1);

    return h1;
} 

uint32_t generate_seed() {
    // Use the current time as a seed
    time_t current_time;
    time(&current_time);

    // Use different offsets to generate two independent seeds
    uint32_t seed = (uint32_t)current_time;
    return seed;
}

uint32_t MurmurHash3 (const void* data, size_t length){
    uint32_t hashval = MurmurHash3_x86_32(data, length, generate_seed());
    // printf("MurmurHash:%" PRIu32 "\n", hashval);
    return hashval;
}
