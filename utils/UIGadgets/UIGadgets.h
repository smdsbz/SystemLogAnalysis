#ifndef UIGADGETS_H_
#define UIGADGETS_H_


#include <iostream>
using std::cout; using std::cin; using std::endl;

#include <cstdio>
using std::putchar;

#include <cstdlib>
using std::system;

#include <iomanip>
using std::setw;

#include <string>
using std::string;

#include <stdexcept>


namespace UIGadgets {

  class ProgressBar {
  /* Illustration:
   * OK:  [===       ]  30%
   * ERR: [====!     ]  40%
   * DONE:[==========] DONE
   */
  public:
    unsigned short width     = 80;
    unsigned char  progress  = 0;

  public:

    inline ProgressBar(unsigned short wid=80,
                       unsigned char  start=0) {
      if (wid < 10U) {
        throw std::invalid_argument("UIGadgets::ProgressBar() Too small "
            "to draw!");
      }
      this->width    = wid;
      this->progress = start;
      return;
    }

    inline ProgressBar &_refresh_current_line() {
      putchar('\r'); putchar('[');
      unsigned char cur = 1;
      unsigned short bnd = this->width - 7;
      double nsp = static_cast<double>(this->progress) / 100.0 * bnd;
      for (; cur <= bnd; ++cur) {
        if (cur <= nsp) { putchar('='); }
        else { putchar(' '); }
      }
      cout << "] " << setw(3)
           << static_cast<unsigned short>(this->progress)
           << "%";
      cout.flush();
      return *this;
    }

    inline ProgressBar &draw_on_current_line(unsigned char rst_to=0) {
      this->progress = rst_to;
      if (rst_to > 100) {
        this->progress = 100;
      }
      _refresh_current_line();
      return *this;
    }

    inline ProgressBar &draw_done() {
      putchar('\r'); putchar('[');
      unsigned short cur = 1;
      unsigned short bnd = this->width - 7;
      for (; cur <= bnd; ++cur) { putchar('='); }
      cout << "] DONE" << endl;
      return *this;
    }

  };


  void show_pause() {
    cout << "Press <Enter> to continue..."; cout.flush();
    cin.get();
    return;
  }

  bool get_decision(const string &prompt="") {
    cout << prompt << " [Y/n] "; cout.flush();
    string raw;
    while (true) {
      cin >> raw; cin.clear(); cin.ignore(10000, '\n');
      if (raw == "Y" || raw == "y") { return true; }
      if (raw == "N" || raw == "n") { return false; }
      cout << "Choose again! [Y/n] "; cout.flush();
    }
    throw std::runtime_error("UIGadgets::get_decision() Unrecognized input!");
  }

}   // ns UIGadgets
#endif

