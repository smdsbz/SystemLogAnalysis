#ifndef HASHCELL_H_
#define HASHCELL_H_


#include "../LogClass/LogClass.h"

#include <type_traits>

template <bool B, class T=void>
using enable_if_t = typename std::enable_if<B, T>::type;
using std::is_same_v;


/****** HashCell<> : Class Declaration and Definition ******/

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

  inline ~HashCell() {
    if (entry != nullptr || next != nullptr) {
      string error_str = "HashCell::~HashCell() Dirty previous deletion!\n\t";
      error_str.append("LogRecord ");
      if (entry != nullptr) {
        error_str.append("NOT CLEAN!\n\t");
        cout << entry->get_message() << endl;
      }
      else { error_str.append("OK!\n\t"); }
      error_str.append("HashCell ");
      if (next != nullptr) { error_str.append("NOT CLEAN\n\t"); }
      else { error_str.append("OK!"); }
      throw std::runtime_error(error_str);    // NOTE: *CANNOT* be caught!
                                              //       Process got terminated!
    }
    return;
  }

  // template <class LogMessageT = DataT,
  //     typename = enable_if_t<is_same_v<LogMessageT, LogMessage> > >
  // HashCell(const string &str) {
  //   // NOTE: When you "refresh" a `HashCell` (ie `new` a `HashCell`), you might
  //   //       want to directly init it with a log string.
  //   //       You might tend to perform this kind of initalizations with some
  //   //       prerequisites in mind:
  //   //       - if you use the syntax:
  //   //               `hash_table[log_str] = HashCell(log_str);`
  //   //         you would suppose:
  //   //           - the `HashCell`'s space is already allocated via a
  //   //             `HashTable` init, and the space is static (ie *NOT* `new`-ed)
  //   //           - the `HashCell` is *NOT* occupied yet
  //   //       - if you use the syntax:
  //   //               `end_of_chain->next = new HashCell(new_log)`
  //   //         you would suppose:
  //   //           - along with the `new`-ed `HashCell`, there would also be a
  //   //             first `LogDate` under `this->entry` `new`-ed
  //   try {
  //     data  = LogMessageT(str);
  //     entry = new LogRecord(str, true);
  //     entry->set_message(data);
  //     next  = nullptr;
  //   } catch (const std::runtime_error &e) {     // illegal `str`
  //     throw e;
  //   } catch (const std::bad_alloc &e) {
  //     std::cerr << "HashCell::LogRecord allocation failed!" << std::endl;
  //     throw e;
  //   }
  //   return;
  // }

  HashCell(const LogMessage &msgobj) {
    data  = msgobj;
    entry = nullptr;
    next  = nullptr;
    return;
  }

  // template <class StringT = DataT,
  //     typename = enable_if_t<is_same_v<StringT, string> > >
  // HashCell(const string &str, const bool whole) {
  //   // NOTE: The string template `HashCell` is meant to recieve senders' names!
  //   //       An idea now I have, is that when passing a new log string to
  //   //       `StorageGraph`, `HashTable<LogMessage>` got init-ed first, then the
  //   //       `HashTable<string>`. So is to say that the space for `LogRecord`
  //   //       will be managed by `HashTable<LogMessage>`, *NOT* this one!
  //   //       You have to set the `entry` pointer field by yourself *AFTERWARDS*!
  //   if (whole == true) {
  //     data = LogMessage(str).sender; // don't bother with regex ANY MORE!
  //   } else {
  //     data = str;
  //   }
  //   entry   = nullptr;  // NOTE: Fix this *AFTERWARDS*!
  //   next    = nullptr;
  //   return;
  // }

  HashCell(const string &sender_name) {
    data  = sender_name;
    entry = nullptr;
    next  = nullptr;
    return;
  }

  // template <class LogMessageT = DataT,
  //     typename = enable_if_t<is_same_v<LogMessageT, LogMessage> > >
  // HashCell &reset(const string &log_whole) {
  //   try {
  //     data  = LogMessageT(log_whole);
  //     if (entry == nullptr) {
  //       entry = new LogRecord(log_whole, true);
  //     } else {
  //       throw std::runtime_error("HashCell<LogMessage>::reset() Wipe clean "
  //           "before reset!");
  //     }
  //     entry->set_message(data);
  //   } catch (const std::runtime_error &e) {
  //     throw e;
  //   } catch (const std::bad_alloc &e) {
  //     std::cerr << "HashCell::reset() LogRecord allocation failed!" << std::endl;
  //     throw e;
  //   }
  //   return *this;
  // }

  HashCell<LogMessage> &operator= (const LogMessage &msgobj) {
    if (entry != nullptr || next != nullptr) {
      throw std::runtime_error("HashCell::operator= Wipe clean before reset!");
    }
    data = msgobj;
    return *this;
  }

  // template <class StringT = DataT,
  //     typename = enable_if_t<is_same_v<StringT, string> > >
  // HashCell &reset(const string &log_whole, const bool whole) {
  //   if (whole == true) {
  //     data = LogMessage(log_whole).sender;
  //   } else {
  //     data = log_whole;
  //   }
  //   entry = nullptr;
  //   next = nullptr;
  //   return;
  // }

  HashCell<string> &operator= (const string &sender_name) {
    if (entry != nullptr || next != nullptr) {
      throw std::runtime_error("HashCell::operator= Wipe clean before reset!");
    }
    data = sender_name;
    return *this;
  }

  inline bool operator==(const LogMessage &objmsg) {
    return this->data == objmsg;
  }

  inline bool operator==(const string &sender_name) {
    return this->data == sender_name;   // NOTE: sender_name only
  }

  inline bool operator!=(const LogMessage &msgobj) { return !(*this == msgobj); }

  inline bool operator!=(const string &str) { return !(*this == str); }

  inline HashCell &set_entry(const LogRecord &other) {
    this->entry = &other; return *this;
  }

  inline bool occupied() { return !data.empty(); }
};

// template <> inline bool
// HashCell<LogMessage>::occupied() { return data.notempty(); }

// template <> inline bool
// HashCell<string>::occupied() { return !data.empty(); }



#endif
