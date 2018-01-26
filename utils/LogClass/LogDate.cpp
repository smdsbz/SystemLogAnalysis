#include <iostream>
using std::cout; using std::cin; using std::endl;

#include <string>
using std::string; using std::stoul;

#include <cstring>
using std::memset;

#include <regex>
using std::regex; using std::regex_match;
using std::smatch; using std::ssub_match;

#include <stdexcept>

#include "./LogClass.h"


/****** LogDate ******/

unsigned short
_month_str_to_ushort(string str) {
  if (str == "Jan") { return  1; }
  if (str == "Feb") { return  2; }
  if (str == "Mar") { return  3; }
  if (str == "Apr") { return  4; }
  if (str == "May") { return  5; }
  if (str == "Jun") { return  6; }
  if (str == "Jul") { return  7; }
  if (str == "Aug") { return  8; }
  if (str == "Sep") { return  9; }
  if (str == "Oct") { return 10; }
  if (str == "Nov") { return 11; }
  if (str == "Dec") { return 12; }
  else {
    throw std::runtime_error("LogDate.cpp::_month_str_to_ushort:: illegal "
        "month string");
  }
}


string
_month_ushort_to_str(unsigned short mon) {
  if (mon == 0 || mon > 12) {
    throw std::runtime_error("LogDate.cpp::_month_ushort_to_str:: illegal "
        "month number!");
  }
  return months[mon - 1];
}


string
_ushort_to_dualdigit(unsigned short us) {
  string ret;
  if (us < 10) { ret.push_back('0'); }
  else { ret.push_back(static_cast<char>('0' + us / 10)); }
  ret.push_back(static_cast<char>('0' + us % 10));
  return ret;
}


void
LogDate::_init_to_zero() {
  memset(&time, 0U, 4);
  return;
}

LogDate::LogDate() {
  _init_to_zero();
  return;
}

LogDate::LogDate(string str) {
  _init_to_zero();
  try {
    regex re(RE_DATE);  // regex
    smatch matches;     // store matched strings
    regex_match(str, matches, re); // do match
    if (matches.size() != 6) {  // match failed
      // for (auto &each : matches) {
      //   cout << each << " ";
      // }
      // cout << endl;
      throw std::runtime_error(string("LogDate.cpp::LogDate(str) `str` passed")
          + " in was illegal: " + std::to_string(matches.size()) + " found");
    }
    // match success
    time.mon = _month_str_to_ushort(matches[1].str());
    time.dat = stoul(matches[2].str());
    time.hor = stoul(matches[3].str());
    time.min = stoul(matches[4].str());
    time.sec = stoul(matches[5].str());
  } catch (const std::regex_error &e) {
    throw e;
  }
  return;
}


string
LogDate::str() {
  string ret;
  ret.append(_month_ushort_to_str(time.mon) + " ");
  ret.append(_ushort_to_dualdigit(time.dat) + " ");
  ret.append(_ushort_to_dualdigit(time.hor) + " ");
  ret.append(_ushort_to_dualdigit(time.min) + " ");
  ret.append(_ushort_to_dualdigit(time.sec));
  return ret;
}


/****** Test ******/

// int
// main(int argc, const char *argv[]) {
//   cout << "==== LogDate Test ====" << endl;
//   string input_date;
//   while (getline(cin, input_date)) {
//     try {
//       auto date = LogDate(input_date);
//       cout << "matched LogDate: " << date.str() << endl;
//     } catch (const std::runtime_error &e) {
//       cout << e.what() << endl;
//     }
//   }
//   cout << "<C-D> pressed" << endl;
//   return 0;
// }

