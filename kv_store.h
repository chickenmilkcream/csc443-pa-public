#ifndef KV_STORE_H
#define KV_STORE_H

#include <cstdint>
#include <string>

#include "memtable.h"

using namespace std;

class KeyValueStore
{
public:
  KeyValueStore(uint64_t memtable_size = 0);
  void open_db(string db);
  void close_db();
  db_val_t get(db_key_t key);
  void put(db_key_t key, db_val_t val);
  vector<pair<db_key_t, db_val_t> > scan(db_key_t min_key, db_key_t max_key);
  void read_from_file(const char *filename);
  void print();
  void set_get_method(string get_method);

private:
  Memtable memtable;
  int num_sst;
  int memtable_size;
  string get_method;
  void serialize();
  void write_to_file(vector<pair<db_key_t, db_val_t> > vector_mt);
  int binary_search_smallest(int file, db_key_t key);
  int binary_search_exact(int file, db_key_t key);
};

#endif
