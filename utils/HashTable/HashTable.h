#ifndef HASHTABLE_H_
#define HASHTABLE_H_


#include <iostream>
using std::cout; using std::cin; using std::endl;

#include <string>
using std::string;

#include <stdexcept>


#include "../LogClass/LogClass.h"
#include "./HashCell.h"

extern const size_t HASH_SPACE;
extern uint64_t strhash(string);

template <class DataT>
class HashTable {
public:
  HashCell<DataT>  *table   = nullptr;
  size_t            space   = 0;
  axis_type         axis    = UNSPECIFIED;

  HashFunc          hash;

public:
  inline HashTable(size_t table_space   = HASH_SPACE,
                   axis_type iter_axis  = TIME) {
    if (table_space == 0) {
      throw std::invalid_argument("HashTable::HashTable(...) "
          "Provide the size to init the table!");
    }
    if (iter_axis != TIME && iter_axis != SENDER) {
      throw std::invalid_argument("HashTable::HashTable(...) "
          "Unkown type of iteration axis!");
    }
    this->space = table_space;
    this->axis  = iter_axis;
    this->table = new HashCell<DataT>[this->space];
    this->hash  = HashFunc(30, space, 0);
    return;
  }

  HashTable &insert(const string &str) {
    // NOTE: Do *NOT* append_msg() here, do it in `StorageGraph`!
    // ALG:  Using sequential address policy.
    auto idx = static_cast<size_t>( hash(str) );
    auto &cell = table[idx];
    if (cell.occupied()) {
      if (cell == str) {  // NOTE: auto string -> LogMessage
        std::cerr << "HashTable::HashTable(..) Tempting to insert log that "
                  << "already exists!" << std::endl;
        // std::cerr << cell.data.get_message() << str << std::endl;
        return *this;   // do nothing
      }
      // ALG: sequential address
      try {
        cell.next = new HashCell<DataT>(str);
      } catch (std::bad_alloc &e) {
        std::cerr << "HashTable::HashTable(...) Low memory!" << '\n';
        throw e;
      } catch (std::runtime_error &e) { // strange log string
        // before `new`
        cell.next = nullptr;
        throw e;
      }
    }   // end of `cell.occupied()`
    else {  // not occupied
      cell.reset(str);  // copy
    }
    return *this;
  }

  inline HashCell<DataT> &operator[](string log_whole) {
    auto *pcell = table + hash(log_whole);
    while (pcell && *pcell != log_whole) {
      pcell = pcell->next;
    }
    if (pcell == nullptr) {
      throw std::runtime_error("HashTable::operator[] No match found!");
    }
    return *pcell;
  }


};

#endif

