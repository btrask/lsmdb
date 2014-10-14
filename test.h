#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAP_SIZE (1024 * 1024 * 256)

#define WRITES (1000 * 1000)
#define TXN_SIZE 1000
#define KEY_SIZE sizeof(uint64_t)
#define DATA_SIZE 16

#ifdef SEQ
#define GENKEY genkey_seq
#define PUT_FLAGS MDB_APPEND
#else
#define GENKEY genkey_rnd
#define PUT_FLAGS 0
#endif

#define chk(x) ({ \
	int const __rc = (x); \
	if(MDB_SUCCESS != __rc) { \
		fprintf(stderr, "%s:%d %s: %s: %s\n", \
			__FILE__, __LINE__, __PRETTY_FUNCTION__, \
			#x, mdb_strerror(__rc)); \
		abort(); \
	} \
})

static uint64_t seed = 0;
static inline void genkey_rnd(uint8_t *const buf) {
	// Little endian, which essentially randomizes our insertions.
	buf[0] = 0xff & (seed >> 0x00);
	buf[1] = 0xff & (seed >> 0x08);
	buf[2] = 0xff & (seed >> 0x10);
	buf[3] = 0xff & (seed >> 0x18);
	buf[4] = 0xff & (seed >> 0x20);
	buf[5] = 0xff & (seed >> 0x28);
	buf[6] = 0xff & (seed >> 0x30);
	buf[7] = 0xff & (seed >> 0x38);
	seed++;
}
static inline void genkey_seq(uint8_t *const buf) {
	// Big endian.
	buf[7] = 0xff & (seed >> 0x00);
	buf[6] = 0xff & (seed >> 0x08);
	buf[5] = 0xff & (seed >> 0x10);
	buf[4] = 0xff & (seed >> 0x18);
	buf[3] = 0xff & (seed >> 0x20);
	buf[2] = 0xff & (seed >> 0x28);
	buf[1] = 0xff & (seed >> 0x30);
	buf[0] = 0xff & (seed >> 0x38);
	seed++;
}

