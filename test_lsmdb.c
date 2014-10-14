#include "lsmdb.h"
#include "test.h"

int main(void) {
	fprintf(stderr, "%s\n", __FILE__);

	unlink("./data.lsmdb");
	unlink("./data.lsmdb-lock");

	LSMDB_env *env;
	chk( lsmdb_env_create(&env) );
	chk( lsmdb_env_set_mapsize(env, MAP_SIZE) );
	chk( lsmdb_env_open(env, "./data.lsmdb", MDB_NOSUBDIR, 0600) );

/*	MDB_dbi dbi;
	{
		MDB_txn *txn;
		chk( mdb_txn_begin(env, NULL, MDB_RDWR, &txn) );
		chk( mdb_dbi_open(txn, NULL, 0, &dbi) );
		chk( mdb_txn_commit(txn) );
	}*/

	uint8_t k[KEY_SIZE];
	uint8_t d[DATA_SIZE] = {};

	for(int i = 0; i < WRITES / TXN_SIZE; ++i) {
		LSMDB_txn *txn;
		chk( lsmdb_txn_begin(env, NULL, MDB_RDWR, &txn) );

		for(int j = 0; j < TXN_SIZE; ++j) {
			GENKEY(k);

			MDB_val key = { sizeof(k), &k };
			MDB_val data = { sizeof(d), &d };
			chk( lsmdb_put(txn, &key, &data, PUT_FLAGS) );

		}


		chk( lsmdb_autocompact(txn) );
		lsmdb_txn_commit(txn);
	}

	lsmdb_env_close(env);
	return 0;
}

