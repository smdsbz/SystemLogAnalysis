#ifndef STORAGEGRAPH_H_
#define STORAGEGRAPH_H_

#include <iostream>
using std::cout; using std::cin; using std::endl;

#include <string>
using std::string;

#include <fstream>
using std::fstream; using std::getline;

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

  LogRecord &_time_sequence_promised_add(const string &log) {
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
    try {
      // NOTE: The following two lines may fail due to cell existing,
      //       but it actually won't make any difference! 'Cause it ensures you
      //       that the cells exist anyway, and it even finds the cell for
      //       you (returned as reference to cell)!
      //       Just pretend that every `insert()` or `link()` is new to
      //       that operand.
      auto &msg_cell = this->messages->insert(msg);  // may fail,
      // associate `LogRecord` and `LogMessage`
      //   or `link()` would fail!
      prec->set_message(msg_cell.data);
      auto &sndr_cell = this->senders->link(*prec);  // but finds cell for you!
      // add `LogRecord` to `Storage`
      // NOTE: The following tow lines requires that `log` passed hasn't
      //       appeared before.
      msg_cell.join_rec_to_end(prec);
      this->messages->join_rec_to_end(prec);
      sndr_cell.join_rec_to_end(prec);
    } catch (const std::bad_alloc &e) { throw e; }
    return *prec;
  }

  Storage &read_from_file(const string &filename) {
    fstream file;
    file.open(filename, std::ios::in);
    if (file.is_open()) {
      /* size_t cnt = 1; */
      // NOTE: Some log may span across multiple lines. So you have to store
      //       the previous line, in case you have to append to it later.
      string line;
      LogRecord *prec = nullptr;    // HACK: But instead, I'll store the
                                    //       `LogRecord` for convenience.
      LogMessage msgbuf;
      while (getline(file, line)) {
        /* cout << "line ==> " << line << endl; */
        try {
          msgbuf = LogMessage(line);
        } catch (const std::runtime_error &e) {
          // cannot match regex:
          //   1. false file
          //   2. last log spanned
          if (prec == nullptr) {    // case 1: false file
            throw std::runtime_error("Storage::read_from_file() Input file is "
                "not standard log text file!");
          } else {  // case 2: last log spanned
            prec->message->append_msg(line);
            /* cout << "appended \"" << line << "\"" << endl; */
            continue;
          }
        }
        // legit log! added to msgbuf
        prec = &(this->_time_sequence_promised_add(line));
      } // end of input while-loop
      file.close();
    } else {    // failed to open
      throw std::runtime_error(string("Storage::read_from_file() Failed to ")
          + "open file \"" + filename + "\"!");
    }
    return *this;
  }

};


#endif
