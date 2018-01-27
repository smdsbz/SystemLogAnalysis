#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "../LogClass/LogClass.h"

/****** Class Definitions ******/

extern const size_t HASH_SPACE;

class HashCell {
public:
  LogMessage    data;
  LogRecord    *entry   = nullptr;
  HashCell     *next    = nullptr;
  
  // TODO: Move iterator to LogRecord
  // const enum _AXIS_TYPE { TIME, SENDER };
  // class iterator {  // NOTE: default axis is time
  // public:
  //   LogRecord  *objcur  = nullptr;
  //   unsigned char axis  = TIME;
  // public:
  //   iterator(unsigned char axis=TIME) {
  //     objcur = nullptr; this->axis = axis; return;
  //   }
  //   inline iterator &operator= (const iterator &other) {
  //     this->objcur = other.objcur; this->axis = other.axis; return *this;
  //   }
  //   inline bool operator==(const iterator &other) {
  //     if (this->axis != other.axis) {
  //       throw "Not comparable!";
  //     }
  //     return this->objcur == other.objcur;
  //   }
  //   inline iterator &operator++() {
  //     switch (axis) {
  //       case _AXIS_TYPE::TIME  : objcur = objcur->time_suc;   break;
  //       case _AXIS_TYPE::SENDER: objcur = objcur->sender_suc; break;
  //       default: throw "Wrong axis!";
  //     }
  //     return *this;
  //   }
  //   inline LogRecord &operator* () { return *objcur; }
  //   inline LogRecord *operator->() { return  objcur; }
  // };

public:
  HashCell();
  ~HashCell();
  HashCell(const string log_string_whole);
  inline HashCell &operator= (const HashCell &other) {
    this->data  = other.data;
    this->entry = other.entry;
    this->next  = other.next;
    return *this;
  }
  inline bool occupied() { return entry == nullptr; }

  // inline iterator begin(const unsigned char axis=TIME) { // along time axis
  //   auto ret = iterator(TIME);
  //   ret.objcur = entry;
  //   return ret;
  // }
  // inline iterator end(const unsigned char axis=TIME, const size_t range=0) {
  //   auto ret = iterator(axis);
  //   if (range == 0) { ret.objcur = nullptr; }
  //   else {
  //     for (auto border = LogDate(
  //   }
  // }
};

#endif
