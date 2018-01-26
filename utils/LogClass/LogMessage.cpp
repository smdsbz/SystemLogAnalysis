#include <iostream>
using std::cout; using std::cin; using std::endl;

#include <string>
using std::string;

#include <regex>
using std::regex; using std::regex_match;
using std::smatch; using std::ssub_match;

#include <stdexcept>

#include <cstdint>

#include "./LogClass.h"


/**** LogMessage ****/

void
LogMessage::_init_to_zero() {
  /* pass */
  return;
}


LogMessage::LogMessage() {
  _init_to_zero();
  return;
}


LogMessage::LogMessage(string str) {
  _init_to_zero();
  try {
    regex re(RE_WHOLE);
    smatch matches;
    regex_match(str, matches, re);
    if (matches.size() != 5) {  // match failed
      throw std::runtime_error(string("LogClass.cpp::LogMessage(str) `str` ")
          + "passed in was illegal: " + std::to_string(matches.size())
          + " found");
    }
    // match success
    host = matches[2].str();
    sender = matches[3].str();
    message = matches[4].str();
  } catch (const std::regex_error &e) {
    throw e;
  }
  return;
}


LogMessage::LogMessage(const LogMessage &copy) {
  this->host = copy.host;
  this->sender = copy.sender;
  this->message = copy.message;
  return;
}


LogMessage&
LogMessage::operator=(const LogMessage &other) {
  if (*this == other) { return *this; }
  this->host = other.host;
  this->sender = other.sender;
  this->message = other.message;
  return *this;
}


bool
LogMessage::operator!=(const LogMessage &other) {
  if (this->host != other.host) { return true; }
  if (this->sender != other.sender) { return true; }
  if (this->message != other.message) { return true; }
  return false;
}


bool
LogMessage::operator==(const LogMessage &other) {
  return (! (*this != other));
}


inline bool
LogMessage::notempty() {
  return (! (this->host.empty()));
}


void
LogMessage::append_msg(string more) {
  // WHY: some apps (like google-chrome) will generate logs that span across
  //      multiple lines (usually xml-like), thus you will need to append
  //      the messages in the new lines to a previous `LogMessage` instance,
  //      otherwise the log read into memory will be incomplete, lack unique
  //      informations and cause further problem reading following logs
  message.append(string("\n") + more);
  return;
}



/****** Test ******/

// int main(int argc, const char *argv[]) {
//   cout << "==== Test LogMessage ====" << endl;
// 
//   auto sample = string("Jan  8 12:07:06 zhuxiaoguangs-MacBook-Air com.apple.x"
//       "pc.launchd[1] (com.apple.preference.displays.MirrorDisplays): Service "
//       "only ran for 0 seconds. Pushing respawn out by 10 seconds.");
//   auto info = LogMessage();
//   cout << "empty init passed" << '\n';
//   info = LogMessage(sample);
//   cout << "arg init / operator= passed" << '\n';
//   auto info_cpy = LogMessage(info);
//   cout << "copy init passed" << '\n';
//   if (info == LogMessage(sample)) {
//     cout << "operator== / operator!= passed" << '\n';
//   } else { cout << "operator== / operator!= ERROR!" << '\n'; }
//   if (info.notempty() && LogMessage().notempty() == false) {
//     cout << "notempty() passed" << '\n';
//   }
//   info.append_msg("some other texts...");
//   cout << info.message << '\n';
// 
//   for (size_t timer = 0; timer != 1E+4; ++timer) {
//     cout << "hashed message ==> " << strhash(info.message) << '\n';
//   }
//   cout << "end of test" << endl;
//   return 0;
// }

