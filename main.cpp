#include <iostream>
using std::cout; using std::cin;

#include <string>
using std::string;

#include <filesystem>
using std::filesystem;

#include <stdexcept>

#include "./utils/LogClass/LogClass.h"
#include "./utils/HashTable/HashTable.h"
#include "./utils/StorageGraph/StorageGraph.h"
#include "./utils/UIGadgets.h"
using UIGadgets::FrontEnd;


int main(int argc, char **argv) {

  auto ui = FrontEnd();
  auto storage = nullptr;

  if (argc == 2) {  // program started with dumpfile's name
    if (filesystem::exists(argv[1]) {
      try {
        cout << "Loading from file " << argv[1] << ", please wait..." << endl;
        storage = new Storage(5000, 200);
        storage->read_from_file(argv[1]);
      } catch (const std::runtime_error &e) {
        cout << "Failed to load from file " << argv[1] << "!" << endl;
        delete storage; storage = nullptr;
        ui.pause();
      }
    } else {    // file does not exist
      cout << "Log dump with name " << argv[1] << " does not exist!" << endl;
      ui.pause();
    }
  } else if (argc == 1) {   // called without any arguments
    /* pass */ ;
  }


  delete storage;
  return 0;
}

