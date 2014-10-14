#include "liblmdb/lmdb.h"
#include "test.h"

#define MDB_RDWR 0

int main(void) {
	fprintf(stderr, "%s\n", __FILE__);

	unlink("./data.mdb");
	unlink("./data.mdb-lock");

	MDB_env *env;
	chk( mdb_env_create(&env) );
	chk( mdb_env_set_mapsize(env, MAP_SIZE) );
	chk( mdb_env_open(env, "./data.mdb", MDB_NOSUBDIR, 0600) );

	MDB_dbi dbi;
	{
		MDB_txn *txn;
		chk( mdb_txn_begin(env, NULL, MDB_RDWR, &txn) );
		chk( mdb_dbi_open(txn, NULL, 0, &dbi) );
		chk( mdb_txn_commit(txn) );
	}


	for(int i = 0; i < WRITES / TXN_SIZE; ++i) {
		MDB_txn *txn;
		chk( mdb_txn_begin(env, NULL, MDB_RDWR, &txn) );

		for(int j = 0; j < TXN_SIZE; ++j) {
			uint8_t k[KEY_SIZE];
			GENKEY(k);
			uint8_t d[] = DATA;

			MDB_val key = { sizeof(k), &k };
			MDB_val data = { sizeof(d), &d };
			chk( mdb_put(txn, dbi, &key, &data, PUT_FLAGS) );

		}


//		chk( lsmdb_autocompact(txn) );
		mdb_txn_commit(txn);
	}

	mdb_env_close(env);
	return 0;
}

