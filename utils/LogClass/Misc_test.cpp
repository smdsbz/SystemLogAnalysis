#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

#include "./LogClass.h"


int
main(int argc, const char *argv[]) {
  string input_date;
  while (getline(cin, input_date)) {
    try {
      auto date = LogDate(input_date);
      cout << "matched LogDate: " << date.str() << endl;
    } catch (const std::runtime_error &e) {
      cout << e.what() << endl;
    }
  }
  cout << "<C-D> pressed" << endl;
  return 0;
}

