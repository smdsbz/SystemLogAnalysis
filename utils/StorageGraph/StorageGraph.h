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
    // prepare `LogMessage` content
    auto msg = LogMessage();
    try {
      msg = LogMessage(log, false);
    } catch (const std::runtime_error &e) {
      // log passed in can't match regex
      throw e;
    }
    // prepare `LogRecord` content
    LogRecord *prec = nullptr;
    try {
      prec = new LogRecord(log, true); // NOTE: `LogDate` included!
    } catch (const std::bad_alloc &e) {
      std::cerr << "Log Memory! Space allocation failed!" << endl;
      throw e;
    }
    // associate `LogRecord` and `LogMessage`
    //   or `link()` would fail!
    prec->set_message(msg);
    try {
      // NOTE: The following two lines may fail due to cell existing,
      //       but it actually won't make a difference! 'Cause it ensures you
      //       that the cells exist anyway, and it even finds the cell for
      //       you anyway (returned as reference to cell)!
      //       Just pretend that every `insert()` or `link()` is new to
      //       that operand.
      auto &msg_cell = this->messages->insert(msg);  // may fail,
      auto &sndr_cell = this->senders->link(*prec);  // but finds cell for you!
      // add `LogRecord` to `Storage`
      // NOTE: The following tow lines requires that `log` passed hasn't
      //       appeared before.
      msg_cell.join_rec_to_end(prec);
      this->messages->join_rec_to_end(prec);
      sndr_cell.join_rec_to_end(prec);
    } catch (const std::bad_alloc &e) { throw e; }
    return *this;
  }

};


#endif
