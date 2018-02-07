#include <iostream>
using std::cout; using std::cin;

#include <string>
using std::string;

#include <vector>
using std::vector;

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
  // NOTE: `storage` is promised mounted!
  cout << "Successfully loaded!" << endl;
  Misc::show_pause();


  int oper = -1;
  while (oper != 0) {
    system("clear");
    cout << endl;
    cout << "    SystemLogAnalysis (OS-X build) (by smdsbz)    \n"
         << "==================================================\n"
         << " 11. Query via message content\n"
         << " 12. Query via host\n"
         << " 13. Query via sender\n"
         << "\n"
         << " 21. Insert new log manually\n"
         << "\n"
         << " 31. Remove single log from buffered\n"
         << " 32. Remove message from buffered\n"
         << "- - - - - - - - - - - - - - - - - - - - - - - - -\n"
         << " 4 . Run analysis\n"
         << "- - - - - - - - - - - - - - - - - - - - - - - - -\n"
         << " 0 . Exit program\n"
         << "==================================================\n"
         << "Choose your operation: ";
    cout.flush();
    cin >> oper;    // get oper from user
    cin.clear(); cin.ignore(10000, '\n');   // get rid of illegal chars

    switch (oper) {
      case 11: {
        string in_msg; string fuzzy_ind; bool use_fuzzy;
        cout << "Use fuzzy find? [Y/n] "; cout.flush();
        cin >> fuzzy_ind; cin.clear(); cin.ignore(10000, '\n');
        if (fuzzy_ind == "Y" || fuzzy_ind == "y") { use_fuzzy = true; }
        else if (fuzzy_ind == "N" || fuzzy_ind == "n") { use_fuzzy = false; }
        else {  // unrecognized symbol - jmp out
          cout << "Unrecognized symbol!" << endl;
          Misc::show_pause(); break;
        }
        cout << "Message content: "; cout.flush();
        // cin >> in_msg; cin.clear(); cin.ignore(10000, '\n');
        getline(cin, in_msg);
        auto search_result = storage->query_via_message(in_msg, use_fuzzy);
        if (search_result.empty()) {
          cout << "No result!" << endl;
          Misc::show_pause(); break;
        }
        // TODO: Add human-readable interface for this.
        size_t cnt = 0;
        for (auto &rec : search_result) {
          cout << rec->get_message() << endl;
          if (cnt++ % 30 == 29) {
            cout << "---- MORE (press 'q' to break) ----";
            if (cin.get() == 'q') { break; };
            system("clear");
          }
        }
        cin.clear(); cin.ignore(10000, '\n');
        cout << cnt << " log(s) shown in total." << endl;
        Misc::show_pause(); break;
      }
      case 0: { break; }
      default: { break; }
    }

  }

  system("clear");
  cout << "Exiting..." << endl;
  delete storage;
  return 0;
}

