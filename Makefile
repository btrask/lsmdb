CFLAGS += -Wall -Werror -Wno-unused -std=gnu99

.phony: all
all: test_lsmdb test_leveldb test_mdb

test_lsmdb: test_lsmdb.c test.h lsmdb.c lsmdb.h
	make -C ./deps/liblmdb
	$(CC) $(CFLAGS) test_lsmdb.c lsmdb.c ./deps/liblmdb/liblmdb.a -lpthread -lm -o $@

test_leveldb: test_leveldb.c test.h
	make -C ./deps/snappy
	make -C ./deps/leveldb
	$(CC) $(CFLAGS) -I./deps/leveldb/include test_leveldb.c ./deps/leveldb/libleveldb.a ./deps/snappy/.libs/libsnappy.a -lstdc++ -lpthread -o $@

test_mdb: test_mdb.c test.h
	make -C ./deps/liblmdb
	$(CC) $(CFLAGS) test_mdb.c ./deps/liblmdb/liblmdb.a -lpthread -lm -o $@

.phony: clean
clean:
	- make clean -C ./deps/liblmdb
	- rm test_lsmdb
	- make clean -C ./deps/snappy
	- make clean -C ./deps/leveldb
	- rm test_leveldb
	- rm test_mdb
