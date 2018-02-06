#include <iostream>
using std::cout; using std::cin;

#include <string>
using std::string;

#include <stdexcept>

#include "./utils/LogClass/LogClass.h"
#include "./utils/HashTable/HashTable.h"
#include "./utils/StorageGraph/StorageGraph.h"
#include "./Misc.h"



int main(int argc, char **argv) {
  system("clear");

  Storage *storage = nullptr;

  if (argc == 2) {  // program started with dumpfile's name
    storage = Misc::load_from_file(argv[1]);
  } else if (argc == 1) {   // called without any arguments
    string filename;
    cout << "Specify path to log dump: ";
    cin >> filename;
    storage = Misc::load_from_file(filename);
  }

  if (storage == nullptr) { return -1; }
  cout << "Successfully loaded!" << endl;
  Misc::show_pause();

  cout << endl;
  delete storage;
  return 0;
}

