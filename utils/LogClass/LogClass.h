#ifndef LOGCLASS_H_
#define LOGCLASS_H_


/****** Regular Expressions ******/

const string months[12] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
    "Aug", "Sep", "Oct", "Nov", "Dec"
};
const string _re_month = "Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec";
// bsd format: syslog -F bsd > target_file
const string RE_DATE = string("(") + _re_month + ") ([1| ][0-9]) "
    + "([0-9]{2}):([0-9]{2}):([0-9]{2})";

const string RE_WHOLE = (string("((?:") + _re_month + ") [1| ][0-9] [0-9]{2}:"
    + "[0-9]{2}:[0-9]{2}) ")    // date
                                //   eg "Jan  1 19:43:19"
    + "([a-zA-Z\\-]+) "         // host name (lower/upper and short-dash)
                                //   eg "zhuxiaoguangs-MacBook-Air"
    + "([a-z\\.]+)\\[[0-9]+\\] "// sender (lower) and threadID
                                //   eg "steam_osx[584]"
    + "([^\n]+)$";              // message (anything hence to the eol)


/****** Class Definitions ******/

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
  LogDate(string re_mathed_date_string);
  inline bool operator==(const LogDate &other) {
    if (this->time.mon != other.time.mon) { return false; }
    if (this->time.dat != other.time.dat) { return false; }
    if (this->time.hor != other.time.hor) { return false; }
    if (this->time.min != other.time.min) { return false; }
    if (this->time.sec != other.time.sec) { return false; }
    return true;
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
  inline bool operator>=(const LogDate &other) {
    return !(*this < other);
  }
  void _init_to_zero();
  string str();
};


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
  LogMessage(string raw_input_str);
  LogMessage(const LogMessage &copy);
  inline LogMessage &operator= (const LogMessage &other) {
    if (*this == other) { return *this; }
    this->host = other.host;
    this->sender = other.sender;
    this->message = other.message;
    return *this;
  }
  inline bool operator!=(const LogMessage &other) {
    if (this->host != other.host) { return true; }
    if (this->sender != other.sender) { return true; }
    if (this->message != other.message) { return true; }
    return false;
  }  
  inline bool operator==(const LogMessage &other) { return !(*this != other); }  
  inline bool notempty() { return !(this->host.empty()); }
  void _init_to_zero();
  void append_msg(string more_message_str);
  // uint64_t hash();
};


class LogRecord {
public:
  LogMessage   *message     = nullptr;
  LogDate       date;
  LogRecord    *time_suc    = nullptr;
  LogRecord    *sender_suc  = nullptr;

public:
  LogRecord(string log_string);
  ~LogRecord(); // NOTE: should not break chain relations
  inline bool operator<=(const LogRecord &other) {
    // NOTE: relation defined as time relation
    return this->date <= other.date;
  }
  inline bool operator>=(const LogRecord &other) {
    return this->date >= other.date;
  }
  // called by Graph when inserting `LogRecord`s
  LogRecord &set_message(LogMessage &msg);
  LogRecord &set_time_successor(LogRecord &next_rec);
  LogRecord &set_sender_successor(LogRecord &next_rec_same_sender);
};


/****** Public Utilities ******/

constexpr size_t STRHASH_RANGE = 30;
constexpr size_t HASH_SPACE = 5000;

uint64_t strhash(string str);

#endif
