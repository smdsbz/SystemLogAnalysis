#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <iostream>
using std::cout; using std::cin; using std::endl;

#include <string>
using std::string;

#include <stdexcept>

#include "../LogClass/LogClass.h"
#include "./HashCell.h"


/****** HashTable : Class Declaration and Definitioin ******/

class MessageTable {
public:
  _HashCell_LogMessage *table = nullptr;
  size_t    space = 5000;
  HashFunc  hash  = HashFunc(30, 5000, 0);

  LogRecord *global_begin = nullptr;
  LogRecord *global_end   = nullptr;

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

  ~MessageTable() {
    // free cells in chain
    for (size_t cell_cur = 0; cell_cur != this->space; ++cell_cur) {
      auto *pcell = table + cell_cur;
      while (pcell->next != nullptr) {
        auto *to_free = pcell->next;
        pcell->next = to_free->next;
        // to_free->next = nullptr;
        delete to_free;
      }
    }
    // free chain heads in `this->table`
    delete [] this->table;
    return;
  }

  /* 函数名称：insert
   * 函数参数：要插入的 LogMessage，msg
   * 函数功能：将 msg 添加到查找表中
   * 返回值：  插入成功返回插入位置的引用，否则抛出对应错误
   */
  _HashCell_LogMessage &insert(const LogMessage &msgobj) {
    // get index
    size_t idx = this->hash(msgobj.get_message() + msgobj.get_sender());
    cout << "hash val recieved was: " << idx << " ==> "
         << msgobj.get_message().substr(0, 40) << '\n';
    if (idx >= this->space) {
      throw std::range_error("MessageTable::insert() HashFunc miscalculated!");
    }
    /* cout << idx << ": " << msgobj.get_message().substr(0, 40) << endl; */
    // find place to insert
    auto &msg_cell = this->table[idx];
    if (msg_cell.occupied()) {
      if (msg_cell.strict_equal(msgobj)) {  // check if exist
        /* cout << "Log \"" << msgobj.get_message() << "\" already in table!" */
        /*      << endl; */
        return msg_cell;    // HACK: It doesn't matter, just make sure
                            //       there *IS* one
      }
      auto pcell = &msg_cell;
      while (pcell->next != nullptr) {
        if (pcell->next->strict_equal(msgobj)) {  // check if exsit
          /* cout << "Log \"" << msgobj.get_message() << "\" already in table!" */
          /*      << endl; */
          return *(pcell->next);
        }
        // not this one ==> venture forth!
       pcell = pcell->next;
      } // end of while ==> pcell->next == nullptr
      try {
        // pcell->next = new _HashCell_LogMessage();
        pcell->set_next(new _HashCell_LogMessage());
      } catch (const std::bad_alloc &e) {
        cout << "Low Memory! Space allocation failed while inserting:\n"
             << msgobj.get_message() << endl;
        throw e;
      }
      // new `_HashCell_LogMessage` alloced at `pcell->next`
      pcell->next->reset_cell(msgobj);
      return *(pcell->next);
    } else {  // msg_cell.occupied() == false
      msg_cell.reset_cell(msgobj);
      return msg_cell;
    }
    throw std::logic_error("MessageTable::insert() Jumped out of if-else!");
  }

  inline MessageTable &join_rec_to_end(LogRecord *prec) {
    if (this->global_begin == nullptr) {
      this->global_begin = prec;
    }
    this->global_end = prec;
    return *this;
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
    auto pcell = this->table + hash(msg);
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

  inline void set_begin(LogRecord *beg) { this->global_begin = beg; }
  inline void set_end(LogRecord *end) { this->global_end = end; }
};



class SenderTable {
public:
  _HashCell_string     *table = nullptr;
  size_t    space = 200;
  HashFunc  hash = HashFunc(10, 200, 0);

public:
  
  inline SenderTable(size_t table_size = 200) {
    if (table_size == 0) {
      throw std::invalid_argument("SenderTable::SenderTable(...)::table_size "
          "Provide an appropriate size to init table!");
    }
    this->space = table_size;
    this->table = new _HashCell_string[this->space];
    this->hash = HashFunc(10, this->space, 0);
    return;
  }

  inline ~SenderTable() { delete [] this->table; }

  /* 函数名称：link
   * 函数参数：已存在的 LogRecord 的引用
   * 函数功能：将该 LogRecord 添加至 SenderTable 的关系网中
   * 返回值：  对应该 LogRecord 的发送者在 SenderTable 中的存储位置
   */
  _HashCell_string &link(LogRecord &rec) {
    // NOTE: There's actually no way of ensuring the `rec` isn't repeating!
    //       So here I pretend that my users know what they are doing...
    // get message address
    auto pmsg = rec.message;
    if (pmsg == nullptr) {
      throw std::runtime_error("SenderTable::link() Invalid `massage` field!");
    }
    // get copy of sender_name
    string sender_name = rec.get_sender();
    // get insert position
    size_t idx = this->hash(sender_name);
    if (idx >= this->space) {
      throw std::range_error("SenderTable::insert() HashFunc miscalculated!");
    }
    // do linking
    auto &sndr_cell = this->table[idx];
    if (sndr_cell.occupied()) {
      if (sndr_cell == sender_name) {   // search chain head
        sndr_cell.join_rec_to_end(&rec);
        return sndr_cell;
      } // not chain head, continue searching
      auto pcell = &sndr_cell;
      while (pcell->next != nullptr) {
        if (*(pcell->next) == sender_name) {    // sender found
          // no need for new cell
          pcell->next->join_rec_to_end(&rec);
          return *(pcell->next);
        }
        // sender not found *YET*, venture forth
        pcell = pcell->next;
      } // end of while
      // still not found? need new cell
      try {
        pcell->next = new _HashCell_string();
      } catch (const std::bad_alloc &e) {
        cout << "Low Memory! Space allocation failed while linking to "
             << sender_name << endl;
        throw e;
      }
      pcell->next->reset_cell(sender_name);
      pcell->next->join_rec_to_end(&rec);
      return *(pcell->next);
    } else {    // not occupied
      sndr_cell.reset_cell(sender_name);
      sndr_cell.join_rec_to_end(&rec);
      return sndr_cell;
    }
    throw std::logic_error("SenderTable::link() Jumped out of if-else!");
  }

  _HashCell_string &operator[](const string &sender) {
    auto pcell = this->table + hash(sender);
    while (pcell && (*pcell != sender)) { pcell = pcell->next; }
    if (pcell == nullptr) {
      throw std::overflow_error(string("SenderTable::operator[sender] ")
          + "No match found for: " + sender);
    }
    return *pcell;
  }

};

#endif

