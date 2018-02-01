#ifndef STORAGEGRAPH_H_
#define STORAGEGRAPH_H_

#include <iostream>
using std::cout; using std::cin; using std::endl;

#include <string>
using std::string;

#include <stdexcept>

#include "../LogClass/LogClass.h"
#include "../HashTable/HashTable.h"


/****** StorageGraph : Class Declaration and Definitioin ******/

class Storage {
public:
  MessageTable     *messages = nullptr;
  SenderTable      *senders  = nullptr;

public:

  inline Storage(size_t msgtable_size = 5000,
                 size_t sdrtable_size = 200) {
    messages = new MessageTable(msgtable_size);
    senders = new SenderTable(sdrtable_size);
    return;
  }

  Storage &_time_sequence_promised_add(const string &log) {
    
  }

};


#endif
