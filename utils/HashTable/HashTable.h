#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <iostream>
using std::cout; using std::cin; using std::endl;

#include <string>
using std::string;

#include <stdexcept>

#include "../LogClass/LogClass.h"
#include "./HashCell.h"


extern uint64_t strhash(string);

/****** HashTable : Class Declaration and Definitioin ******/

class MessageTable {
public:
  _HashCell_LogMessage *table = nullptr;
  size_t      space = 5000;
  HashFunc    hash  = HashFunc(30, 5000, 0);

  LogRecord  *global_begin = nullptr;
  LogRecord  *global_end   = nullptr;

public:

  inline MessageTable(size_t table_size = 5000) {
    if (table_size == 0) {
      throw std::invalid_argument("MessageTable::MessageTable(...)::table_size "
          "Provide an appropriate size to init table!");
    }
    this->space = table_size;
    this->table = new _HashCell_LogMessage[this->space];
    this->hash = HashFunc(30, this->space, 0);
    this->global_begin = nullptr;
    this->global_end = nullptr;
    return;
  }

  /* 函数名称：insert
   * 函数参数：要插入的 LogMessage，msg
   * 函数功能：将 msg 添加到查找表中
   * 返回值：  插入成功返回插入位置的引用，否则抛出对应错误
   */
  _HashCell_LogMessage &insert(const LogMessage &msgobj) {
    size_t idx = this->hash(msgobj.get_message());
    if (idx >= this->space) {
      throw std::range_error("MessageTable::insert() HashFunc miscalculated!");
    }
    auto &msg_cell = this->table[idx];
    if (msg_cell.occupied()) {
      auto pcell = &msg_cell;
      while (pcell->next != nullptr) {
        if (pcell->next->strict_equal(msgobj)) {  // already inserted
          cout << "Log \"" << msgobj.get_message() << "\" already in table!"
               << endl;
          return *(pcell->next);
        }
        // not in table || value_equal ==> venture forth!
        pcell = pcell->next;
      } // end of while ==> pcell->next == nullptr
      try {
        pcell->next = new _HashCell_LogMessage();
      } catch (const std::bad_alloc &e) {
        cout << "Low Memory! Space allocation failed while inserting:\n"
             << msgobj.get_message() << endl;
      }
      pcell->next->reset_cell(msgobj);
      return *(pcell->next);
    } else {  // msg_cell.occupied() == false
      msg_cell.reset_cell(msgobj);
      return msg_cell;
    }
    throw std::logic_error("MessageTable::insert() Jumped out of if-else!");
  }

  _HashCell_LogMessage &operator[](const LogMessage &msgobj) {
    auto *pcell = this->table + hash( msgobj.get_message() );
    while ( pcell && (pcell->strict_equal(msgobj) == false) ) {
      pcell = pcell->next;
    }
    if (pcell == nullptr) {
      throw std::overflow_error(string("MessageTable::operator[LogMessage] "
          "No match found for: ") + msgobj.get_message());
    }
    // pcell != nullptr ==> match found!
    return *pcell;
  }

  _HashCell_LogMessage &operator[](const string &msg) {
    auto *pcell = this->table + hash(msg);
    while ( pcell && (pcell->value_equal(msg) == false) ) {
      pcell = pcell->next;
    }
    if (pcell == nullptr) {
      throw std::overflow_error(string("MessageTable::operator[LogMessage] ")
          + "No match found for: " + msg);
    }
    // pcell != nullptr ==> match found!
    return *pcell;
  }

  _HashCell_LogMessage &operator[](const size_t idx) {
    if (idx >= this->space) {
      throw std::range_error(string("MessageTable::operator[index] ")
          + "index(" + std::to_string(idx) + ") is out of range!");
    }
    return this->table[idx];
  }

};



#endif
