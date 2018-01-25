#ifndef MISC_H_
#define MISC_H_

#include <iostream>
using std::cout; using std::cin; using std::endl;
#include <string>
using std::string;


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
    LogDate(string re_mathed_string);
    void _init_to_zero();
    string str();
};

#endif
