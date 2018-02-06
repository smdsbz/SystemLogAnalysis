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


  /* class FrontEnd {*/
  /* public:*/
  /*   const char *Title_With_Line = \*/
  /*       "        SystemLogAnalysis (OS-X build) (by smdsbz)\n"*/
  /*       "  ======================================================\n";*/
  /*   const char *N1 = \  // before any log is loaded*/
  /*       "   1. Read from standard bsd-style syslog dump\n"*/
  /*       "\n   0. Exit program\n";*/
  /*   const char *N2 = \  // immediate after log loaded*/
  /*       "   1. Query on buffered log messages\n"*/
  /*       "   2. Insert new message (manually)\n"*/
  /*       "   3. Delete buffered message (manually)\n"*/
  /*       "\n   0. Exit program\n";*/
    
  /*   enum class LevelCode { N1, N2 };*/

  /*   LevelCode   current_level = LevelCode::N1;*/

  /* public:*/
    
  /*   FrontEnd &_render_current_level() {*/
  /*     switch (this->current_level) {*/
  /*       case LevelCode::N1: {*/
  /*         cout << this->N1;*/
  /*         break;*/
  /*       }*/
  /*       case LevelCode::N2: {*/
  /*         cout << this->N2;*/
  /*         break;*/
  /*       }*/
  /*       default: {*/
  /*         throw std::runtime_error("FrontEnd::_render_current_level() "*/
  /*             "Unknown LevelCode value!");*/
  /*         break;*/
  /*       }*/
  /*     } // end of switch*/
  /*     cout.flush();*/
  /*     return *this;*/
  /*   }*/

  /*   FrontEnd &render() {*/
  /*     cout << this->Title_With_Line; cout.flush();*/
  /*     this->_render_current_level();*/
  /*     return *this;*/
  /*   }*/

  /*   FrontEnd &change_level(LevelCode lc) {*/
  /*     this->current_level = lc;*/
  /*     return *this;*/
  /*   }*/

  /*   FrontEnd &pause() {*/
  /*     cout << "\nPress <Enter> to continue";*/
  /*     getchar();*/
  /*     cout.flush();*/
  /*     return *this;*/
  /*   }*/

    /* 函数名称：wait_for_instruction
     * 函数参数：无
     * 函数功能：输出操作提示，并从用户获取下一步操作指令
     * 返回值：  退出程序返回 false，否则返回 true
     */
  /*   bool wait_for_instruction(int &oper_code) {*/
  /*     system("clear");*/
  /*     this->render();*/
  /*     cout << "Please choose your operation: "; cout.flush();*/
  /*     cin >> oper_code;*/
  /*     if (this->current_level == LevelCode::N1oper_code == 0) { return false; }*/
  /*     return true;*/
  /*   }*/

  /* };    // class FrontEnd*/


  class Frame {
  public:
    string prompt;
    string message;
    /* string reserved; */

  public:

    Frame(const string &prm="",
          const string &msg="") {
      if (prm.empty()) {
        this->prompt = prm;
      }
      if (msg.empty()) {
        throw std::invalid_argument("UIGadgets::Frame() Operation list "
            "cannot be empty!");
      }
      return;
    }

    Frame &enter() {
      
      int op = 0;
    }

  } // class Frame

}   // ns UIGadgets
#endif

