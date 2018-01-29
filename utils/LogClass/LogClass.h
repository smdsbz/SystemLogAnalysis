#ifndef LOGCLASS_H_
#define LOGCLASS_H_


#include <iostream>
using std::cout; using std::cin; using std::endl;

#include <string>
using std::string; using std::to_string;

#include <stdexcept>


/****** Regular Expressions ******/

const string months[12] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
    "Aug", "Sep", "Oct", "Nov", "Dec"
};
const string _re_month = "Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec";
// bsd format: syslog -F bsd > target_file
const string RE_DATE = string("(") + _re_month + ") ([1| ][0-9]) "
    + "([0-9]{2}):([0-9]{2}):([0-9]{2})";

const string RE_WHOLE = (string("((?:") + _re_month
    + ") [1| ][0-9] [0-9]{2}:[0-9]{2}:[0-9]{2}) ")      // date
                                //   eg "Jan  1 19:43:19"
    + "([a-zA-Z\\-]+) "         // host name (lower/upper and short-dash)
                                //   eg "zhuxiaoguangs-MacBook-Air"
    + "([a-z\\.]+)\\[[0-9]+\\][:]? "    // sender (lower) and threadID
                                        //   eg "steam_osx[584]"
    + "([^\n]+)$";              // message (anything hence to the eol)


/****** Class Declarations ******/

class LogDate {
public:
  struct {
    unsigned short mon : 4;   // 1 to 12
    unsigned short dat : 5;   // 1 to 31
    unsigned short hor : 5;   // 0 to 23
    unsigned short min : 6;   // 0 to 59
    unsigned short sec : 6;   // 0 to 59
    unsigned long  _   : 6;
  } time;

public:
  LogDate();
  LogDate(const string &re_mathed_date_string); // whole_log_string is fine
  inline bool operator==(const LogDate &other) {
    if (this->time.mon != other.time.mon) { return false; }
    if (this->time.dat != other.time.dat) { return false; }
    if (this->time.hor != other.time.hor) { return false; }
    if (this->time.min != other.time.min) { return false; }
    if (this->time.sec != other.time.sec) { return false; }
    return true;
  }
  inline bool operator!=(const LogDate &other) {
    return !(*this == other);
  }
  inline bool operator> (const LogDate &other) {
    if (this->time.mon <= other.time.mon) { return false; }
    if (this->time.dat <= other.time.dat) { return false; }
    if (this->time.hor <= other.time.hor) { return false; }
    if (this->time.min <= other.time.min) { return false; }
    if (this->time.sec <= other.time.sec) { return false; }
    return true;
  }
  inline bool operator<=(const LogDate &other) {
    return !(*this > other);
  }
  inline bool operator< (const LogDate &other) {
    if (this->time.mon >= other.time.mon) { return false; }
    if (this->time.dat >= other.time.dat) { return false; }
    if (this->time.hor >= other.time.hor) { return false; }
    if (this->time.min >= other.time.min) { return false; }
    if (this->time.sec >= other.time.sec) { return false; }
    return true;
  }
  inline bool operator>=(const LogDate &other) { return !(*this < other); }
  LogDate &operator+ (const size_t sec);
  void _init_to_zero();
  string str();
};


/**** LogMessage ****/

class LogMessage {
public:
  // LogDate   date;
  string    host;
  string    sender;
  // string    threadID;
  string    message;
  // uint64_t  hash;

public:
  LogMessage();
  LogMessage(const string &log_string_whole);
  LogMessage(const LogMessage &copy);
  inline LogMessage &operator= (const LogMessage &other) {
    if (*this == other) { return *this; }
    this->host    = other.host;
    this->sender  = other.sender;
    this->message = other.message;
    return *this;
  }
  inline bool operator!=(const LogMessage &other) {
    if (this->host    != other.host   ) { return true; }
    if (this->sender  != other.sender ) { return true; }
    if (this->message != other.message) { return true; }    // costs time!
    return false;
  }  
  inline bool operator==(const LogMessage &other) { return !(*this != other); }
  inline bool notempty() { return !(this->host.empty()); }
  void _init_to_zero();
  void append_msg(const string &more_message_str);
  inline string get_message() { return string(message); }
};



enum axis_type : unsigned char { TIME, SENDER, UNSPECIFIED };

class LogRecord {
public:
  LogMessage   *message     = nullptr;
  LogDate       date;
  LogRecord    *time_suc    = nullptr;
  LogRecord    *sender_suc  = nullptr;

  class iterator {
  public:
    LogRecord    *pRec    = nullptr;
    // LogRecord    *one_prev= nullptr;  // LogRecord.end() should *NOT* use this!
    axis_type     axis    = TIME;

  public:
    inline iterator(LogRecord &start,
                    const axis_type passed_axis=TIME) {
      this->pRec = &start;
      this->axis = passed_axis;
      return;
    }
    inline iterator(LogRecord *p,
                    const axis_type passed_axis=TIME) {
      this->pRec = p;
      this->axis = passed_axis;
      return;
    }
    iterator &operator++();
    inline bool operator==(const iterator &other) {
      if (this->axis != other.axis) {
        throw std::runtime_error("LogRecord::iterator::operator==() Cannot "
            "compare iterators on different axes!");
      }
      return this->pRec == other.pRec;
    }
    inline LogRecord &operator* () {
      if (this->pRec == nullptr) {
        throw std::runtime_error("LogRecord::iterator::operator*() Cannot get "
            "reference of `nullptr`!");
      }
      return *(this->pRec);
    }
    inline LogRecord *operator->() {
      if (this->pRec == nullptr) {
        throw std::runtime_error("LogRecord::iterator::operator->() Iterator "
            "pointing to `nullptr`!");
      }
      return this->pRec;
    }
  };

public:
  inline LogRecord() { return; }
  LogRecord(const string &re_mathed_date_string, const bool whole=false);
  ~LogRecord();
  inline bool operator<=(const LogRecord &other) {
    // NOTE: relation defined as time relation
    return this->date <= other.date;
 }
  inline bool operator>=(const LogRecord &other) {
    return this->date >= other.date;
  }
  inline LogRecord &operator= (const LogRecord &other) {
    this->message   = other.message;
    this->date      = other.date;
    this->time_suc  = other.time_suc;
    this->sender_suc= other.sender_suc;
    return *this;
  }
  // called by Graph when inserting `LogRecord`s
  inline LogRecord &set_message(LogMessage &msg) {
    message = &msg;
    return *this;
  }
  inline LogRecord &set_time_successor(LogRecord &nxt) {
    time_suc = &nxt;
    return *this;
  }
  inline LogRecord &set_sender_successor(LogRecord &nxt) {
    sender_suc = &nxt;
    return *this;
  }
  inline string get_message() { return message->message; }
  inline string get_date() { return date.str(); }
  /* iterator */
  inline iterator begin(axis_type axis=TIME) { return iterator(*this, axis); }
  inline iterator end(axis_type axis=TIME) { return iterator(nullptr, axis); }

};


/****** Public Utilities ******/

constexpr const size_t STRHASH_RANGE  = 30;
constexpr const size_t HASH_SPACE     = 5000;


class HashFunc {
public:
  size_t hash_range = STRHASH_RANGE;
  size_t pool_size  = HASH_SPACE;
  unsigned char selection = 0;

public:
  inline HashFunc(size_t hrange = STRHASH_RANGE,
                  size_t psize  = HASH_SPACE,
                  unsigned char slct = 0) {
    this->hash_range= hrange;
    this->pool_size = psize;
    this->selection = slct;
    return;
  }

  inline HashFunc &operator= (const HashFunc &other) {
    this->hash_range= other.hash_range;
    this->pool_size = other.pool_size;
    this->selection = other.selection;
    return *this;
  }

  uint64_t operator()(const string &str) {
    uint64_t ret = 0UL;
    uint8_t  multiplier = (this->pool_size / this->hash_range / 26U) % 10U;
    auto cur = str.end(); --cur;
    auto beg = str.begin();
    for (size_t max_range = this->hash_range;
         cur != beg && max_range != 0;
         --cur, --max_range) {
      ret += static_cast<uint8_t>(*cur) * multiplier;
    }
    return (ret - (this->hash_range
                   * static_cast<uint8_t>('0')) )
           % this->pool_size;
  }
};



#endif

