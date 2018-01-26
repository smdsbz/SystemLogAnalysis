#include <iostream>
using std::cout; using std::cin; using std::endl;

#include <string>
using std::string;

#include <stdexcept>


/****** LogRecord ******/

LogRecord::LogRecord(string str) {
  // NOTE: only set date, other should be done in Graph
  try {
    date = LogDate(str);
  } catch (const std::runtime_error &e) {
    throw std::runtime_error(string("LogRecord.cpp::LogRecord(str) failed ")
        + "to create LogRecord instance due to: " + e.what());
  }
  // date set
  message = nullptr; time_next = nullptr; sender_next = nullptr;
  return;
}


LogRecord::~LogRecord() {
  // HACK: actually there's nothing you can do here, for you don't
  //       know the Graph context, so you've got no way of knowing how to
  //       re-chain related `LogRecord`s
  /* pass */ ;
  return;
}


inline bool
LogRecord::operator<=(const LogRecord &other) {
  // NOTE: relation defined to be time relation
  return (this->date <= other.date);
}


inline bool
LogRecord::operator>=(const LogRecord &other) {
  return (this->date >= other.date);
}


inline LogRecord &
set_message(LogMessage &msg) {
  message = &msg;
  return this;
}


inline LogRecord &
set_time_successor(LogRecord &nxt) {
  time_suc = &nxt;
  return this;
}


inline LogRecord &
set_sender_successor(LogRecord &nxt) {
  sender_next = &nxt;
  return this;
}



