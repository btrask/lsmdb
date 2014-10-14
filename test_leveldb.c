#include <leveldb/c.h>
#include "test.h"

int main(void) {
	fprintf(stderr, "%s\n", __FILE__);

	char *err;

	leveldb_options_t *const opts = leveldb_options_create();
	assert(opts);
	leveldb_options_set_create_if_missing(opts, 1);
	leveldb_options_set_error_if_exists(opts, 1);
	leveldb_options_set_compression(opts, leveldb_no_compression);

	err = NULL;
	leveldb_destroy_db(opts, "./data.leveldb/", &err);
	if(err) fprintf(stderr, "%s\n", err);
	assert(!err);


	leveldb_writeoptions_t *const wopts = leveldb_writeoptions_create();
	assert(wopts);
	leveldb_writeoptions_set_sync(wopts, 1);

	err = NULL;
	leveldb_t *const db = leveldb_open(opts, "./data.leveldb/", &err);
	if(err) fprintf(stderr, "%s\n", err);
	assert(!err);
	assert(db);


	for(int i = 0; i < WRITES / TXN_SIZE; ++i) {
		leveldb_writebatch_t *const batch = leveldb_writebatch_create();
		assert(batch);

		for(int j = 0; j < TXN_SIZE; ++j) {
			uint8_t k[KEY_SIZE];
			GENKEY(k);
			uint8_t d[] = DATA;

			leveldb_writebatch_put(batch, (char *)k, sizeof(k), (char *)d, sizeof(d));

		}

		err = NULL;
		leveldb_write(db, wopts, batch, &err);
		if(err) fprintf(stderr, "%s\n", err);
		assert(!err);
	}

	leveldb_close(db);
	return 0;
}

