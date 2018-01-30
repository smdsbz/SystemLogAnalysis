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

  HashTable<LogMessage> &insert(const LogMessage &msgobj) {
    // NOTE: Do *NOT* append_msg() here, do it in `StorageGraph`!
    // ALG:  Using sequential address policy.
    size_t idx = 0;
    try {   // NOTE: insert full log string *ONLY*!
      idx = static_cast<size_t>( hash(msgobj.message) );
    } catch (const std::runtime_error &e) {
      throw e;
    }
    auto &cell = table[idx];
    if (cell.occupied()) {
      if (cell == msgobj.message) {   // NOTE: auto string -> LogMessage
        std::cerr << "HashTable::HashTable(..) Tempting to insert log that "
                  << "already exists: " << msgobj.message << std::endl;
        return *this;   // do nothing
      }
      // ALG: sequential address
      try {
        cell.next = new HashCell<DataT>(msgobj);
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
      // cell.reset(msgobj);  // copy
      cell = msgobj;
    }
    return *this;
  }

  inline HashCell<string> &operator[](const string &sender_name) {
    auto *pcell = table + hash(sender_name);
    while (pcell && *pcell != sender_name) {
      pcell = pcell->next;
    }
    if (pcell == nullptr) {
      throw std::runtime_error("HashTable::operator[] No match found: "
          + sender_name);
    }
    return *pcell;
  }

  inline HashCell<LogMessage> &operator[](const LogMessage &log) {
    auto *pcell = table + hash(log.message);
    while (pcell && pcell->data.message != log.message) {
      std::cerr << "pcell->data.message ==> " << pcell->data.message << std::endl;
      std::cerr << "log.message ==> " << log.message << std::endl;
      pcell = pcell->next;
    }
    if (pcell == nullptr) {
      throw std::runtime_error("HashTable::operator[] No match found: "
          + log.message);
    }
    return *pcell;
  }


};

#endif
