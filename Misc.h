#ifndef MISC_H_
#define MISC_H_

#include <iostream>
using std::cout; using std::cin; using std::endl;

#include <string>
using std::string;

#include <stdexcept>

#include "./utils/LogClass/LogClass.h"
#include "./utils/HashTable/HashTable.h"
#include "./utils/StorageGraph/StorageGraph.h"


namespace Misc {

  Storage *load_from_file(const string &filename) {
    Storage *storage = nullptr;
    if (true) {
      try {
        cout << "Loading from file " << filename << ", please wait..." << endl;
        storage = new Storage(5000, 200);
        storage->read_from_file(filename);
      } catch (const std::runtime_error &e) {
        cout << "Failed to load from file " << filename << "!" << endl;
        delete storage; storage = nullptr;
        return nullptr;
      }
    } else {    // file does not exist
      cout << "Log dump with name " << filename << " does not exist!" << endl;
      return nullptr;
    }
    return storage;
  }

  template <typename DataT>
  void show_query_result(vector<DataT> results) {
    if (results.empty()) {
      cout << "No result!" << endl;
      return;
    }
    size_t cnt = 0;
    for (auto &rec : results) {
      system("clear");
      cout << rec->_repr() << endl;   // HACK: Make sure the method
                                      //       `_repr()` is implemented
      cout << "---- MORE (h for help) ----"; cout.flush();
      char op = cin.get();
      switch (op) {
        case 'n': { break; }
        case 'q': { show_pause(); return; }
        default: { break; }
      } // end switch
    }
    cout << "---- END ----";
    cin.clear(); cin.ignore(10000, '\n');
    cout << cnt << " log(s) shown in total." << endl;
    return;
  }

}

#endif

