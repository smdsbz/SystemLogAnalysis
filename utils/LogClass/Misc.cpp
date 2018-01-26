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
    throw std::runtime_error("Misc.cpp::_month_str_to_ushort:: illegal "
        "month string");
  }
}


string
_month_ushort_to_str(unsigned short mon) {
  if (mon == 0 || mon > 12) {
    throw std::runtime_error("Misc.cpp::_month_ushort_to_str:: illegal "
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
    regex re(RE_DATE);
    smatch matches;
    regex_match(str, matches, re); // 匹配 str 中的模式，转换为日期
    if (matches.size() != 6) {  // 匹配失败
      // for (auto &each : matches) {
      //   cout << each << " ";
      // }
      // cout << endl;
      throw std::runtime_error(string("Misc.cpp::LogDate(str) `str` passed ")
          + "in was illegal: " + std::to_string(matches.size()) + " found");
    }
    // 匹配成功
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


/****** LogMessage ******/

uint64_t
strhash(string str) {
  // HACK: because it's highly possible that log messages have their first
  //       10-to-40-ish characters *IDENTICAL*, a better practice is that you
  //       start your hash from the end
  uint64_t ret = 0U;
  auto curr = str.end(); --curr;    // `str.end()` is actually one-off-end
  for (auto begin = str.begin(), size_t max_range = STRHASH_RANGE;
       curr != begin && max_range != 0; --curr, --max_range) {
    ret += static_cast<uint8_t>(*curr); // NOTE: it's okay if overflow
  }
  return ret;
}

