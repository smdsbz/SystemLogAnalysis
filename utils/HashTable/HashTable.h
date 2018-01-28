#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "../LogClass/LogClass.h"

#include <type_traits>

template <bool B, class T=void>
using enable_if_t = typename std::enable_if<B, T>::type;

// template <class L, class R>
// inline constexpr bool is_same_v = std::is_same_v<L, R>::value;
using std::is_same_v;


/****** Class Declarations ******/

template <class DataT>
class HashCell {
public:
  DataT         data;
  LogRecord    *entry   = nullptr;
  HashCell     *next    = nullptr;
  
public:

  inline HashCell() {
    // NOTE: Handled by default initalizations already.
    /* pass */ ;
    return;
  }  


  ~HashCell() {
    if (entry != nullptr || next != nullptr) {
      string error_str = "HashCell::~HashCell() Dirty previous deletion!\n\t";
      error_str.append("LogRecord ");
      if (entry != nullptr) { error_str.append("NOT CLEAN!\n\t"); }
      else { error_str.append("OK!\n\t"); }
      error_str.append("HashCell ");
      if (next != nullptr) { error_str.append("NOT CLEAN\n\t"); }
      else { error_str.append("OK!"); }
      throw std::runtime_error(error_str);    // NOTE: *CANNOT* be caught!
                                              //       Process got terminated!
    }
    return;
  }


  template <class LogMessageT = DataT,
      typename = enable_if_t<is_same_v<LogMessageT, LogMessage> > >
  HashCell(const string &str) {
    // NOTE: When you "refresh" a `HashCell` (ie `new` a `HashCell`), you might
    //       want to directly init it with a log string.
    //       You might tend to perform this kind of initalizations with some
    //       prerequisites in mind:
    //       - if you use the syntax:
    //               `hash_table[log_str] = HashCell(log_str);`
    //         you would suppose:
    //           - the `HashCell`'s space is already allocated via a 
    //             `HashTable` init, and the space is static (ie *NOT* `new`-ed)
    //           - the `HashCell` is *NOT* occupied yet
    //       - if you use the syntax:
    //               `end_of_chain->next = new HashCell(new_log)`
    //         you would suppose:
    //           - along with the `new`-ed `HashCell`, there would also be a
    //             first `LogDate` under `this->entry` `new`-ed
    try {
      data  = LogMessageT(str);
      entry = new LogRecord(str, true);
      entry->set_message(data);
      next  = nullptr;
    } catch (const std::runtime_error &e) {     // illegal `str`
      throw e;
    } catch (const std::bad_alloc &e) {
      std::cerr << "HashCell::LogRecord allocation failed!" << std::endl;
      throw e;
    }
    return;
  }


  template <class StringT = DataT,
      typename = enable_if_t<is_same_v<StringT, string> > >
  HashCell(const string &str, const bool whole) {
    // NOTE: The string template `HashCell` is meant to recieve senders' names!
    //       An idea now I have, is that when passing a new log string to
    //       `StorageGraph`, `HashTable<LogMessage>` got init-ed first, then the
    //       `HashTable<string>`. So is to say that the space for `LogRecord`
    //       will be managed by `HashTable<LogMessage>`, *NOT* this one!
    //       You have to set the `entry` pointer field by yourself *AFTERWARDS*!
    if (whole == true) {
      data = LogMessage(str).sender; // don't bother with regex ANY MORE!
    } else {
      data = str;
    }
    entry   = nullptr;  // NOTE: Fix this *AFTERWARDS*!
    next    = nullptr;
    return;
  }


  inline HashCell &operator= (const HashCell &other) {
    this->data  = other.data;
    this->entry = other.entry;
    this->next  = other.next;
    return *this;
  }

  inline bool occupied();
};

template <> inline bool
HashCell<LogMessage>::occupied() { return !data.notempty(); }

template <> inline bool
HashCell<string>::occupied() { return data.empty(); }



class LogRecord::iterator {
public:
  LogRecord    *prec    = nullptr;
  LogRecord    *one_prev= nullptr;
  enum axis_type : unsigned char { TIME, SENDER };
  unsigned char axis = TIME;

public:
  iterator(const axis_type=TIME);
  iterator &operator++();
  bool operator==(const iterator &other);
};


extern const size_t HASH_SPACE;

#endif
