#ifndef MISC_H_
#define MISC_H_

#include <iostream>
using std::cout; using std::cin; using std::endl;

#include <string>
using std::string;

/* #include <filesystem> */
/* using std::filesystem; */

#include <stdexcept>

#include "./utils/LogClass/LogClass.h"
#include "./utils/HashTable/HashTable.h"
#include "./utils/StorageGraph/StorageGraph.h"


namespace Misc {

  Storage *load_from_file(const string &filename) {
    Storage *storage = nullptr;
    /* if (filesystem::exists(filename) { */
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

  void show_pause() {
    cout << "Press <Enter> to continue..."; cout.flush();
    getchar();
    return;
  }

}

#endif

