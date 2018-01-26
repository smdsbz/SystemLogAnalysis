#include <iostream>
using std::cout; using std::cin; using std::endl;

#include <string>
using std::string;

#include <stdexcept>

#include "./LogClass.h"


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
  message = nullptr; time_suc = nullptr; sender_suc = nullptr;
  return;
}


LogRecord::~LogRecord() {
  // HACK: Actually there's nothing you can do here, for you don't
  //       know the Graph context, so you've got no way of knowing how to
  //       re-chain related `LogRecord`s.
  /* pass */ ;
  return;
}


LogRecord &
LogRecord::set_message(LogMessage &msg) {
  message = &msg;
  return *this;
}


LogRecord &
LogRecord::set_time_successor(LogRecord &nxt) {
  time_suc = &nxt;
  return *this;
}


LogRecord &
LogRecord::set_sender_successor(LogRecord &nxt) {
  sender_suc = &nxt;
  return *this;
}



