#include <iostream>
using std::cout; using std::cin; using std::endl;

#include <string>
using std::string;

#include <stdexcept>
#include <new>

#include "./HashTable.h"


/****** HashCell ******/

HashCell::HashCell() {
  // NOTE: Handled by default initalizations already.
  /* pass */ ;
  return;
}


HashCell::HashCell(string str) {
  // NOTE: When you "refresh" a `HashCell` (ie `new` a `HashCell`), you might
  //       want to directly init it with a log string.
  //       You might tend to perform this kind of initalizations with some
  //       prerequisites in mind:
  //       - if you use the syntax:
  //               `hash_table[log_str] = HashCell(log_str);`
  //         you would suppose:
  //           - the `HashCell`'s space is already allocated via a 
  //             `HashTable` init, and the space is static (ie *NOT* `new`-ed)
  //           - the `HashCell` is *NOT* occupied yet
  //       - if you use the syntax:
  //               `end_of_chain->next = new HashCell(new_log)`
  //         you would suppose:
  //           - along with the `new`-ed `HashCell`, there would also be a
  //             first `LogDate` under `this->entry` `new`-ed
  try {
    data  = LogMessage(str);
    entry = new LogRecord(str.substr(0, 15));
    entry->set_message(data);
    next  = nullptr;
  } catch (const std::runtime_error &e) {     // illegal `str`
    throw e;
  } catch (const std::bad_alloc &e) {
    std::cerr << "HashCell::LogRecord allocation failed!" << std::endl;
    throw std::bad_alloc();
  }
  return;
}


HashCell::~HashCell() {
  // NOTE: The deletion of `HashCell` should perform `LogRecord` deletions
  //       as well. Since `sender_suc` relation is a superset of `time_suc`,
  //       luckily you only have to do deletion along the time axis!
  //       *BUT*, the `LogRecord` `this->entry` is pointing to, still might be
  //       other `LogRecord`'s `time_suc`, so you still *CAN'T* do the job!
  //       `LogRecord` deletions must be done in `HashTable`!
  if (entry != nullptr || next != nullptr) {
    string error_str = "HashCell::~HashCell() Dirty previous deletion!\n\t";
    error_str.append("LogRecord ");
    if (entry != nullptr) { error_str.append("NOT CLEAN!\n\t"); }
    else { error_str.append("OK!\n\t"); }
    error_str.append("HashCell ");
    if (next != nullptr) { error_str.append("NOT CLEAN\n\t"); }
    else { error_str.append("OK!"); }
    throw std::runtime_error(error_str);    // NOTE: *CANNOT* be caught!
                                            //       Process got terminated!
  }
  return;
}


/****** Test ******/

int main(int argc, const char *argv[]) {
  auto sample = string("Jan  8 12:07:06 zhuxiaoguangs-MacBook-Air com.apple.x"
      "pc.launchd[1] (com.apple.preference.displays.MirrorDisplays): Service "
      "only ran for 0 seconds. Pushing respawn out by 10 seconds.");
  cout << "- - - - Testing init method - - - -" << endl;
  auto phcell = new HashCell(sample);
  cout << "phcell->data  ==> " << phcell->data.message << endl;
  cout << "phcell->entry ==> (date) " << phcell->entry->get_date() << endl;
  // try {
  //   cout << "- - - - Trying dirty deletion - - - -" << endl;
  //   delete phcell;
  // } catch (const std::runtime_error &e) {
  //   std::cerr << "Caught ERROR!" << endl;
  //   cout << e.what() << endl;
  //   cout << "continue..." << "\n";
  // } catch (...) { std::cerr << "Default catch" << endl; }
  cout << "- - - - Trying legal deletion - - - -" << endl;
  delete phcell->entry; phcell->entry = nullptr;
  delete phcell;
  cout << "Test OK!" << endl;
  return 0;
}

