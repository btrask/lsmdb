CFLAGS := -Wall -Werror -Wno-unused -std=gnu99

.phony: all
all: test_lsmdb

.phony: mdb
mdb:
	cd liblmdb && make

test_lsmdb: test_lsmdb.c test.h lsmdb.c lsmdb.h mdb
	$(CC) $(CFLAGS) test_lsmdb.c lsmdb.c liblmdb/liblmdb.a -pthread -lm -o $@

test_leveldb: test_leveldb.c test.h
	$(CC) $(CFLAGS) test_leveldb.c -lleveldb -lsnappy -o $@

.phony: clean
clean:
	cd liblmdb && make clean
	rm test_lsmdb
