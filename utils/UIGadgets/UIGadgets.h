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

    inline ProgressBar &draw_on_next_line(bool reset=true,
                             unsigned char rst_to=0) {
      if (reset) {
        if (rst_to > 100) {
          throw std::invalid_argument("UIGadgets::ProgressBar::"
              "draw_on_next_line() Over 100%!");
        }
        this->progress = rst_to;
      }
      cout << endl;
      _refresh_current_line();
      return *this;
    }

    inline ProgressBar &draw_on_current_line(bool reset=true,
                                unsigned char rst_to=0) {
      if (reset) {
        if (rst_to > 100) {
          throw std::invalid_argument("UIGadgets::ProgressBar::"
              "draw_in_next_line() Over 100%!");
        }
        this->progress = rst_to;
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


  /* class TopFrame { */
  /* public: */
  /*   Storage */ 
  /* public: */

  /*   TopFrame &enter() { */
  /*     int op = 0; */
  /*     while (true) { */
  /*       system("clear"); */
  /*       cout << "        SystemLogAnalysis (OS-X build) - Nothing Loaded\n" */
  /*            << "  ======================================================\n" */
  /*            << "   1. Read from standard bsd-style syslog dump\n" */
  /*            << "\n" */
  /*            << "   0. Exit program\n" */
  /*            << "  ======================================================\n"; */
  /*       cout.flush(); */
  /*       cin >> op; */
  /*       switch (op) { */
  /*         case 1: { */
  /*           // TODO: Read file frame */
  /*           cout << "  Path to file: "; */
  /*           string filename; */
  /*           cin >> filename; */
  /*           try { */
              
  /*           } catch (const std::runtime_error &e) { */
  /*             cout << "" */
  /*           } */
  /*           break; */
  /*         } */
  /*         case 0: { return *this; } */
  /*       }   // end of switch */
  /*     } // input loop */
  /*   } */

  /* } // class TopFrame */




}   // ns UIGadgets
#endif

